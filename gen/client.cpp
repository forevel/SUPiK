#include <QThread>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QDateTime>
#include <QDir>

#include "client.h"
#include "log.h"

Client *Cli = 0;
Log *CliLog;

Client::Client(QObject *parent) : QObject(parent)
{
    Connected = false;
    FirstReplyPass = FirstComPass = true;
    ComReplyTimeoutIsSet = false;
    fp = 0;
}

Client::~Client()
{
}

int Client::Connect(QString Host, QString Port)
{
    CliLog = new Log;
    CliLog->Init(pc.HomeDir+"/cli.log");
    CliLog->info("=== Log started ===");
    Busy = Connected = CmdOk = false;
    TimeoutTimer = new QTimer;
    TimeoutTimer->setInterval(5000);
    connect(TimeoutTimer,SIGNAL(timeout()),this,SLOT(Timeout()));
    GetComReplyTimer = new QTimer;
    GetComReplyTimer->setInterval(1000); // таймер на получение данных, если за 1 сек ничего не принято, считаем, что посылка закончена, и можно её обрабатывать
    connect(GetComReplyTimer,SIGNAL(timeout()),this,SLOT(ComReplyTimeout()));
    GetFileTimer = new QTimer;
    GetFileTimer->setInterval(500); // таймер на получение файлов, если за 500 мс ничего не принято, считаем, что файл окончен
    connect(GetFileTimer,SIGNAL(timeout()),this,SLOT(GetFileTimerTimeout()));
    MainEthernet = new Ethernet(Host, Port.toInt(), Ethernet::ETH_SSL);
    QThread *thr = new QThread;
    MainEthernet->moveToThread(thr);
    MainEthernet->sslsock->moveToThread(thr);
    connect(thr,SIGNAL(finished()),MainEthernet,SLOT(deleteLater()));
    connect(thr,SIGNAL(finished()),thr,SLOT(deleteLater()));
    connect(thr,SIGNAL(started()),MainEthernet,SLOT(Run()));
    connect(MainEthernet,SIGNAL(connected()),this,SLOT(ClientConnected()));
    connect(MainEthernet,SIGNAL(disconnected()),this,SLOT(ClientDisconnected()));
    connect(MainEthernet,SIGNAL(newdataarrived(QByteArray *)),this,SLOT(ParseReply(QByteArray *)));
    connect(this,SIGNAL(ClientSend(QByteArray *)),MainEthernet,SLOT(InitiateWriteDataToPort(QByteArray *)));
    TimeoutTimer->start();
    DetectedError = CLIER_NOERROR;
    LoginOk = false;
    CurrentCommand = CMD_LOGINREQ;
    thr->start();
    Busy = true;
    while (!LoginOk && (DetectedError == CLIER_NOERROR))
    {
        QThread::msleep(50);
        qApp->processEvents(QEventLoop::AllEvents);
    }
    if (!Connected)
    {
        CliLog->warning("Entering autonomous mode...");
        return CLIER_TIMEOUT;
    }
    if (!LoginOk)
    {
        CliLog->warning("Wrong login or password");
        return DetectedError;
    }
    return CLIER_NOERROR;
}

void Client::StopThreads()
{
    Disconnect();
}

void Client::Disconnect()
{
    SendCmd(CMD_QUIT);
    MainEthernet->Busy = false;
    MainEthernet->Stop();
}

void Client::SendCmd(int Command, QStringList &Args)
{
    CurrentCommand = Command;
    Busy = true;
    TimeoutTimer->start();
    QString CommandString;
    switch (Command)
    {
    // запрос обработки и выдачи данных для tablefields. В Args - описание того, что хотим получить:
    // 1. Взять из таблицы Args(2)=table все значения по полям Args(3...n+3)=headers, для которых поле Args(n+4)=cond_header_0 равно Args(n+5)=value_0, поле
    //    Args(n+6)=cond_header_1 равно Args(n+7)=value_1, ... поле Args(n+m*2+2)=cond_header_m равно Args(n+m*2+3)=value_m
    //    Args(0) = n, Args(1) = m
    //      TF <numbytes_starting_from_GVBFS> GVSBFS n m table headers_0 headers_1 ... headers_n cond_header_0 value_0 cond_header_1 value_1 ... cond_header_m value_m [order_header] [ASC|DESC]
    case CMD_TF_GVSBFS:
    {
        if (Args.size() < 2)
        {
            CliLog->error("CMD_TF_GVSBFS: Number of arguments is less than 2");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        bool ok;
        QString fieldsnum = Args.takeAt(0);
        int fnum = fieldsnum.toInt(&ok);
        if (!ok)
        {
            CliLog->error("CMD_TF_GVSBFS: argument is not a number");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString pairsnum = Args.takeAt(0);
        int pnum = fieldsnum.toInt(&ok);
        if (!ok)
        {
            CliLog->error("CMD_TF_GVSBFS: argument is not a number");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        if (Args.size() < fnum+2*pnum+1) // +1 - table
        {
            CliLog->error("CMD_TF_GVSBFS: Number of fields is less than mentioned in header");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString tmps = Args.join(" ");
        CommandString = "TF " + QString::number(tmps.size()+fieldsnum.size()+pairsnum.size()+9) + " GVSBFS " + fieldsnum + " " + pairsnum + " " + tmps + "\n";
        // 9 = три пробела + sizeof("GVSBFS")
        break;
    }
    // запрос выдачи данных из БД SQL напрямую. В Args находится:
    // 0 - количество запрашиваемых полей n;
    // 1 - количество пар сравнения m;
    // 2 - имя БД
    // 3 - имя таблицы
    // 4...Args(0)+3 - наименования полей запроса
    // Args(0)+4...Args(0)+Args(1)*2+3 - пары сравнения, сначала поле сравнения, затем значение
    // Args(0)+Args(1)*2+4 - поле, по которому проводить сортировку
    // Args(0)+Args(1)*2+5 - "ASC" или "DESC" - в каком порядке проводить сортировку
    // Формат: GVSBFS <numbytes_starting_from_n> n m db table headers_0 headers_1 ... headers_n cond_header_0 value_0 cond_header_1 value_1 ... cond_header_m value_m [order_header] [ASC|DESC]
    case CMD_GVSBFS:
    {
        if (Args.size() < 2)
        {
            CliLog->error("CMD_GVSBFS: Number of arguments is less than 2");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString fieldsnum = Args.takeAt(0);
        bool ok;
        int fnum = fieldsnum.toInt(&ok);
        if (!ok)
        {
            CliLog->error("CMD_TF_GVSBFS: argument is not a number");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString pairsnum = Args.takeAt(0);
        int pnum = fieldsnum.toInt(&ok);
        if (!ok)
        {
            CliLog->error("CMD_TF_GVSBFS: argument is not a number");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        if (Args.size() < fnum+2*pnum+1) // +1 - table
        {
            CliLog->error("CMD_GVSBFS: Number of fields is less than mentioned in header");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString tmps = Args.join(" ");
        CommandString = "GVSBFS " + QString::number(tmps.size()+fieldsnum.size()+pairsnum.size()+2) + " " + fieldsnum + " " + pairsnum + " " + tmps + "\n";
        // 2 = два пробела
        break;
    }
    case CMD_MESSAGES:
    {
        break;
    }
    case CMD_CHATMSGS:
    {
        break;
    }
        // GETF получить файл с сервера
        // 0 - относительный корневой папки .supik путь на сервере
        // 1 - имя файла
        // Формат: GETF <путь> <имя_файла>\n
        // далее ожидание прихода "RDY <длина_в_байтах>\n", затем приём содержимого файла блоками по 16384 байт, после каждого отсылка RDY\n
        // по окончании контроль прихода IDLE\n
    case CMD_GETFILE:
    {
        if (Args.size() < 2)
        {
            CliLog->error("CMD_GETF: Number of arguments is less than 3");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString path = Args.takeAt(0);
        QString filename = Args.takeAt(0);
        CommandString = "GETF \"" + path + "\" \"" +filename + "\"\n";
        QString fullfilename = pc.HomeDir+"Incoming/"+filename;
        fp = fopen(fullfilename.toStdString().c_str(), "wb");
        break;
    }
        // ANS_GETFILE - второй и последующие ответы на принятую информацию (с записью блока в файл)
    case ANS_GETFILE:
    {
        CommandString = "RDY\n";
        break;
    }
        // PUTF отправить файл на сервер
        // 0 - относительный корневой папки .supik путь на сервере
        // 1 - имя файла
        // 2 - длина файла
        // Формат: PUTF <путь> <имя_файла> <длина_в_байтах>\n
        // далее ожидание прихода RDY\n, затем отсылка содержимого файла блоками по 16384 байт, после каждого контроль приёма RDY\n
        // по окончании контроль прихода IDLE\n
    case CMD_PUTFILE:
    {
//        CommandString = "STOR " + Args + "\n";
        break;
    }
    case CMD_QUIT:
    {
        CommandString = "QUIT\n";
        TimeoutTimer->setInterval(1000);
        TimeoutTimer->start();
        break;
    }
    case ANS_LOGIN:
    {
        CommandString = pc.PersLogin+"\n";
        break;
    }
    case ANS_PSW:
    {
        CommandString = pc.PersPsw+"\n";
        break;
    }
    }
    if (Command == ANS_PSW)
        CliLog->info(">********");
    else
        CliLog->info(">"+CommandString.toLocal8Bit());
    QByteArray *ba = new QByteArray(CommandString.toUtf8());
    emit ClientSend(ba);
}

void Client::ParseReply(QByteArray *ba)
{
    if (!ComReplyTimeoutIsSet)
        GetComReplyTimer->start(); // если не было таймаута, рестартуем таймер
    QString ServerResponse, IncomingString;
    QStringList ArgList;
    CmdOk = false;
    if (CurrentCommand != ANS_GETFILE) // приём файла обрабатывается по-другому
    {
        switch (FirstReplyPass)
        {
        case true:
        {
            RcvData.clear();
            RcvData.append(*ba);
            if (RcvData.right(1) == "\n") // очередная посылка закончена, надо передать её на обработку
                break;
            FirstReplyPass = false;
            return;
        }
        case false:
        {
            RcvData.append(*ba);
            if (RcvData.right(1) == "\n") // очередная посылка закончена, надо передать её на обработку
                break;
            return;
        }
        }
        FirstReplyPass = true;
        IncomingString = QString::fromLocal8Bit(RcvData);
        CliLog->info("<"+IncomingString);
        ArgList = IncomingString.split(" ");
        if (ArgList.isEmpty()) // ничего толкового не получено
            return;
        ServerResponse = ArgList.takeFirst(); // в ArgList останутся только аргументы
        if (ServerResponse == SERVERRSTR)
        {
            CliLog->error("Server error response");
            DetectedError = CLIER_SERVER;
            Busy = false;
            TimeoutTimer->stop();
            return;
        }
    }
    switch (CurrentCommand)
    {
    case CMD_LOGINREQ: // по сути установление соединения, должны получить запрос LOGIN
    {
        if (ServerResponse == "LOGIN\n")
        {
            CliLog->info("<"+ServerResponse);
            CmdOk = true;
            SendCmd(ANS_LOGIN);
            return;
        }
        break;
    }
    case ANS_LOGIN:
    {
        if (ServerResponse == "PSW\n")
        {
            CliLog->info("<"+ServerResponse);
            CmdOk = true;
            SendCmd(ANS_PSW);
            return;
        }
        else
            DetectedError = CLIER_LOGIN;
        break;
    }
    case ANS_PSW:
    {
        // если получили в ответ "GROUP <access>", значит, всё в порядке, иначе ошибка пароля
        if (ServerResponse == "GROUP")
        {
            LoginOk = true;
            bool ok;
            pc.access = ArgList.at(0).toLong(&ok,16);
            if (!ok)
            {
                CliLog->warning("Group access undefined: "+ArgList.at(0));
                pc.access = 0x0; // нет доступа никуда
                DetectedError = CLIER_GROUP;
                return;
            }
            CliLog->info("Group access: "+QString::number(pc.access));
            CmdOk = true;
            CurrentCommand = CMD_IDLE;
        }
        else
            DetectedError = CLIER_PSW;
        break;
    }
    case CMD_QUIT:
    {
        if (ServerResponse == "BYE\n")
            CmdOk = true;
        break;
    }
    case CMD_TF_GVSBFS:
    {
        // Формат ответа на запрос из таблицы tablefields:
        // 1. Ответ на GVSBFS:
        //      TF <number_of_bytes_starting_from_number_of_records> <number_of_records>\n
        //      value[0][0] value[1][0] ... value[n][0]\n
        //      value[0][1] value[1][1] ... value[n][1]\n
        //      ...
        //      value[0][k] value[1][k] ... value[n][k]\n

        break;
    }
    case CMD_GVSBFS:
    {
        // Формат ответа на запрос GVSBFS:
        //      GVSBFS <number_of_bytes_starting_from_number_of_records> <number_of_records>\n
        //      value[0][0] value[1][0] ... value[n][0]\n
        //      value[0][1] value[1][1] ... value[n][1]\n
        //      ...
        //      value[0][k] value[1][k] ... value[n][k]\n

        if (ServerResponse == "GVSBFS")
        {
            if (FirstComPass) // первая порция данных
            {
                CliLog->info("<"+ServerResponse);
                if (ArgList.size()<2) // нет то ли длины, то ли количества записей
                {
                    WriteErrorAndBreakReceiving("Некорректное количество аргументов");
                    return;
                }
                bool ok, ok2;
                ReadBytes = ArgList.at(0).toInt(&ok);
                MsgNum = ArgList.at(1).toInt(&ok2);
                ReadBytes -= ArgList.at(0).size() + ArgList.at(1).size() + 2; // 2 - один пробел и один символ перевода строки
                if ((!ok) || (!ok2) || (MsgNum <= 0) || (ReadBytes <= 0))
                {
                    WriteErrorAndBreakReceiving("Некорректные длина или количество посылок");
                    return;
                }
                FirstComPass = false;
                TimeoutTimer->start();
                return;
            }
            else
            {
                emit DataReady(ArgList);
                for (int i=0; i<ArgList.size(); i++)
                    ReadBytes -= ArgList.at(i).size()+1; // один пробел между аргументами
                ReadBytes -= 1; // \n
                MsgNum--;
                if (MsgNum == 0)
                {
                    FirstComPass = true;
                    if (ReadBytes != 0)
                    {
                        WriteErrorAndBreakReceiving("Неверно подсчитана длина посылки");
                        return;
                    }
                    CmdOk = true;
                }
                TimeoutTimer->start();
                return;
            }
        }
        else
        {
            WriteErrorAndBreakReceiving("Некорректный ответ сервера");
            return;
        }
        break;
    }
    // первый приём после команды
    case CMD_GETFILE:
    {
        if (ServerResponse == "RDY")
        {
            if (ArgList.size()>0)
            {
                CmdOk = true;
                bool ok;
                filesize = ArgList.at(0).toLong(&ok,10);
                filepos = 0;
                if (!ok)
                {
                    CliLog->warning("Not a decimal value detected");
                    DetectedError = CLIER_GETFER;
                    return;
                }
                emit BytesOverall(filesize);
                SendCmd(ANS_GETFILE);
                return;
            }
            else
            {
                CliLog->warning("File size not detected");
                DetectedError = CLIER_GETFER;
                return;
            }
        }
        else
            DetectedError = CLIER_LOGIN;
        break;
    }
        // последующие приёмы файла
    case ANS_GETFILE:
    {
        if (fp == 0)
        {
            CliLog->error("File error");
            CurrentCommand = CMD_IDLE;
            DetectedError = CLIER_GETFER;
            break;
        }
/*        if (ComReplyTimeoutIsSet)
        {
            if (fp)
                fclose(fp);
            fp = 0;
            emit TransferComplete();
            CmdOk = true;
            break;
        }*/
//        QString IncomeData = QString::fromLocal8Bit(*ba);
        CliLog->info("< ...binary data "+QString::number(ba->size())+" size...");
/*        if (IncomeData == SERVERRSTR)
        {
            CliLog->error("Server error response");
            DetectedError = CLIER_SERVER;
            Busy = false;
            TimeoutTimer->stop();
            return;
        }
        if (IncomeData == "IDLE\n") // закончили передачу
        {
            fclose(fp);
            fp = 0;
            emit TransferComplete();
            CmdOk = true;
            break;
        }  */
        size_t WrittenBytes = fwrite(ba->data(),1,ba->size(),fp);
        emit BytesRead(WrittenBytes);
        CliLog->info(QString::number(WrittenBytes)+" bytes written to file");
        filepos += WrittenBytes;
        if (filepos >= filesize)
        {
            if (fp)
                fclose(fp);
            fp = 0;
            emit TransferComplete();
            CmdOk = true;
            break;
        }
        SendCmd(ANS_GETFILE);
        break;
    }
    default:
        break;
    }
    if (CmdOk)
        TimeoutTimer->stop();
    Busy = false;
}

void Client::WriteErrorAndBreakReceiving(QString ErMsg)
{
    CliLog->warning(ErMsg);
    TimeoutTimer->stop();
    Busy = false;
    CurrentCommand = CMD_IDLE;
}

void Client::SetBytesWritten(qint64 bytes)
{
    WrittenBytes += bytes;
    emit BytesWritten(WrittenBytes);
}


void Client::GetFileTimerTimeout()
{
    FileBusy = false;
}

void Client::ClientConnected()
{
    Connected = true;
    TimeoutTimer->start(); // рестарт таймера для получения запроса от сервера
}

void Client::ClientDisconnected()
{
    Busy = false;
    Connected = false;
    DetectedError = CLIER_CLOSED;
}

void Client::ClientErr(int error)
{
    CLIER(Ethernet::EthernetErrors()[error]);
    Busy = false;
    DetectedError = CLIER_GENERAL;
    TimeoutTimer->stop();
}

void Client::Timeout()
{
    CLIER("Произошло превышение времени ожидания");
    Busy = false;
    FileBusy = false;
    DetectedError = CLIER_TIMEOUT;
    TimeoutTimer->stop();
}

void Client::ComReplyTimeout()
{
    ComReplyTimeoutIsSet = true;
    QByteArray *ba = new QByteArray;
    ParseReply(ba); // принудительная обработка принятой посылки
    delete ba;
}
