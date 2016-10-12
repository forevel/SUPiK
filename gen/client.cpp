#define _CRT_SECURE_NO_WARNINGS

#include <QThread>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QDateTime>
#include <QTextCodec>
#include <QDir>

#include "client.h"
#include "log.h"

#define _CRT_SECURE_NO_WARNINGS

Client *Cli = 0;
Log *CliLog;

Client::Client(QObject *parent) : QObject(parent)
{
    Connected = false;
    FirstReplyPass = true;
    ComReplyTimeoutIsSet = false;
    fp = 0;
    ResultType = RESULT_MATRIX;
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
#ifndef TIMERSOFF
    TimeoutTimer->start();
#endif
    DetectedError = CLIER_NOERROR;
    LoginOk = false;
    CurrentCommand = M_LOGIN;
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
    return DetectedError;
}

void Client::StopThreads()
{
    Disconnect();
}

void Client::Disconnect()
{
    SendCmd(M_QUIT);
    MainEthernet->Busy = false;
    MainEthernet->Stop();
}

void Client::SendCmd(int Command, QStringList &Args)
{
    Result.clear(); // очищаем результаты
    FieldsLeast = false;
    FieldsLeastToAdd = 0;
    DetectedError = CLIER_NOERROR;
    CurrentCommand = Command;
    Busy = true;
#ifndef TIMERSOFF
    TimeoutTimer->start();
#endif
    QString CommandString;
    switch (Command)
    {
        // запросы выдачи данных из БД SQL напрямую. В Args находится:
        // 0 - количество запрашиваемых полей n;
        // 1 - количество пар сравнения m;
        // 2 - имя БД
        // 3 - имя таблицы
        // 4...Args(0)+3 - наименования полей запроса
        // Args(0)+4...Args(0)+Args(1)*2+3 - пары сравнения, сначала поле сравнения, затем значение
        // Args(0)+Args(1)*2+4 - поле, по которому проводить сортировку
        // Args(0)+Args(1)*2+5 - "ASC" или "DESC" - в каком порядке проводить сортировку
        // Формат: GVSBFS n m db table headers_0 headers_1 ... headers_n cond_header_0 value_0 cond_header_1 value_1 ... cond_header_m value_m [order_header] [ASC|DESC]
    case S_GVSBFS:
    case S_GVBFS:
    {
        if (CheckArgs("S_GV(S)BFS", Args, 7, true, true) != 0)
            return;
        QStringList sl;
        if (Command == S_GVBFS)
            sl << "S5";
        else
            sl << "S1";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        ResultType = RESULT_MATRIX;
        break;
    }
        // запрос всех значений по одному полю
        // формат запроса: GVSBC db tble column [order_header] [ASC|DESC]\n
        // формат ответа: <value1> <value2> ... <valuen>\n
    case S_GVSBC:
    {
        if (CheckArgs("S_GVSBC", Args, 3) != 0)
            return;
        FieldsNum = 1; // одна колонка - одно поле в каждой записи
        QStringList sl;
        sl << "S2";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        ResultType = RESULT_VECTOR;
        break;
    }
        // запрос всех значений по одному полю
        // формат запроса: GVSBCF <pairs_num> db tble column <field1> <value1> ... <fieldn> <valuen> [order_header] [ASC|DESC]\n
        // формат ответа: <value1> <value2> ... <valuen>\n
    case S_GVSBCF:
    {
        if (CheckArgs("S_GVSBCF", Args, 6, false, true) != 0)
            return;
        FieldsNum = 1; // одна колонка - одно поле в каждой записи
        QStringList sl;
        sl << "S3";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        ResultType = RESULT_VECTOR;
        break;
    }
        // запрос колонок (полей) из таблицы
        // формат запроса: GCS db tble\n
        // формат ответа: <column1> <column2> ... <columnn>\n
    case S_GCS:
    {
        if (CheckArgs("S_GCS", Args, 2) != 0)
            return;
        FieldsNum = 1; // одно поле в каждой записи
        QStringList sl;
        sl << "S4";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        ResultType = RESULT_VECTOR;
        break;
    }
        // поиск записей, в которых поле field похоже на выражение value
        // формат запроса: SQLSRCH db tble field value\n
        // формат ответа: SQLSRCH <num>\n
        // <value1> <value2> ... <valuenum>
    case S_SRCH:
    {
        if (CheckArgs("S_SRCH", Args, 5, true) != 0)
            return;
        QStringList sl;
        sl << "S=" << fieldsnum;
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        ResultType = RESULT_MATRIX;
        break;
    }
        // создание таблицы
        // формат запроса: SQLTC db tble <column1> <column2> ... <columnn>\n
        // формат ответа: OK или ERROR
    case S_TC:
    {
        if (CheckArgs("S_TC", Args, 3) != 0)
            return;
        QStringList sl;
        sl << "S6";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        break;
    }
        // удаление таблицы
        // формат запроса: SQLTD db tble\n
        // формат ответа: OK или ERROR
    case S_TD:
    {
        if (CheckArgs("S_TD", Args, 2) != 0)
            return;
        QStringList sl;
        sl << "S8";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        break;
    }
        // изменение таблицы
        // формат запроса: SQLTA n m db tble <add_column1> <add_column2> ... <add_columnn> <del_column1> <del_column2> ... <del_columnm>\n
        // формат ответа: OK или ERROR
    case S_TA:
    {
        if (CheckArgs("S_TA", Args, 5, true) != 0)
            return;
        QStringList sl;
        sl << "S7";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        break;
    }
        // получение первого свободного ИД из таблицы
        // формат запроса: SQLGID db tble\n
        // формат ответа: SQLGID <id> или ERROR
    case S_GID:
    {
        if (CheckArgs("S_GID", Args, 2) != 0)
            return;
        QStringList sl;
        sl << "S>";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        break;
    }
        // вставка записи в таблицу
        // формат запроса: SQLINS db tble [<field1> <value1>] [<field2> <value2>] ... [<fieldn> <valuen>]\n
        // формат ответа: SQLGID <id> или ERROR
    case S_INS:
    {
        if (CheckArgs("S_INS", Args, 2) != 0)
            return;
        QStringList sl;
        sl << "S9";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        break;
    }
        // обновление записей в таблице
        // формат запроса: SQLUPD db tble <field1> <value1> <field2> <value2> ... <fieldn> <valuen> <field> <value>\n
        // формат ответа: OK или ERROR
    case S_UPD:
    {
        if (CheckArgs("S_UPD", Args, 6) != 0)
            return;
        QStringList sl;
        sl << "S:";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        break;
    }
        // удаление записей из таблицы
        // формат запроса: SQLDEL db tble <field> <value>\n
        // формат ответа: OK или ERROR
    case S_DEL:
    {
        if (CheckArgs("S_DEL", Args, 4) != 0)
            return;
        QStringList sl;
        sl << "S;";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        break;
    }
        // реальное удаление записей из таблицы
        // формат запроса: SQLRDEL db tble <field1> <value1> <field2> <value2> ... <fieldn> <valuen>\n
        // формат ответа: OK или ERROR
    case S_RDEL:
    {
        if (CheckArgs("S_RDEL", Args, 4) != 0)
            return;
        QStringList sl;
        sl << "S<";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        break;
    }
    case T_GVSBC:
    {
        FieldsNum = 1; // одна колонка - одно поле в каждой записи
        if (CheckArgs("T_GVSBC", Args, 2) != 0)
            return;
        QStringList sl = QStringList() << "T2";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        ResultType = RESULT_VECTOR;
        break;
    }
    case T_GVSBCF:
    {
        FieldsNum = 1; // одна колонка - одно поле в каждой записи
        if (CheckArgs("T_GVSBCF", Args, 4) != 0)
            return;
        QStringList sl = QStringList() << "T3";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        ResultType = RESULT_VECTOR;
        break;
    }
    case T_GVSBFS:
    {
        if (CheckArgs("T_GVSBFS", Args, 6, true, true) != 0)
            return;
        QStringList sl;
        sl << "T1";
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        ResultType = RESULT_MATRIX;
        break;
    }
    case T_C:
    {
        break;
    }
/*    case CMD_MESSAGES:
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
        QStringList sl;
        sl << "GETF" << path << filename;
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        QString fullfilename = pc.HomeDir+"Incoming/"+filename;
        fp = fopen(fullfilename.toStdString().c_str(), "wb");
        break;
    }
        // ANS_GETFILE - второй и последующие ответы на принятую информацию (с записью блока в файл)
    case ANS_GETFILE: */
    case M_NEXT:
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
/*    case CMD_PUTFILE:
    {
        break;
    } */
    case M_QUIT:
    {
        CommandString = "M1\n";
        TimeoutTimer->setInterval(1000);
        TimeoutTimer->start();
        break;
    }
    case M_ANSLOGIN:
    {
        CommandString = pc.PersLogin+"\n";
        break;
    }
    case M_ANSPSW:
    {
        CommandString = pc.PersPsw+"\n";
        break;
    }
    }
/*    if (Command == ANS_PSW)
        CliLog->info(">********");
    else */
        CliLog->info(">"+CommandString); //+codec->fromUnicode(CommandString));
    QByteArray *ba = new QByteArray(CommandString.toUtf8());//codec->fromUnicode(CommandString));
    emit ClientSend(ba);
}

// ############################################ ОБРАБОТКА ОТВЕТА ##############################################

void Client::ParseReply(QByteArray *ba)
{
#ifndef TIMERSOFF
    if (!ComReplyTimeoutIsSet)
        GetComReplyTimer->start(); // если не было таймаута, рестартуем таймер
#endif
    QString ServerResponse, IncomingString;
    QStringList ArgList;
    CmdOk = false;
    DetectedError = CLIER_NOERROR;
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    if (CurrentCommand != ANS_GETFILE) // приём файла обрабатывается по-другому
    {
        switch (FirstReplyPass)
        {
        case true:
        {
            RcvData.clear();
            RcvData.append(codec->fromUnicode(*ba));
            if (RcvData.right(1) == "\n") // очередная посылка закончена, надо передать её на обработку
            {
                while (RcvData.right(1) == "\n") // убираем все переводы строки
                    RcvData.chop(1);
                break;
            }
            FirstReplyPass = false;
            return;
        }
        case false:
        {
            RcvData.append(codec->fromUnicode(*ba));
            if (RcvData.right(1) == "\n") // очередная посылка закончена, надо передать её на обработку
            {
                while (RcvData.right(1) == "\n") // убираем все переводы строки
                    RcvData.chop(1);
                break;
            }
            return;
        }
        }
        FirstReplyPass = true;
        IncomingString = QString::fromLocal8Bit(RcvData);
        CliLog->info("<"+IncomingString);
        ArgList = SeparateBuf(RcvData);
//        while (ArgList.last().isEmpty())
//            ArgList.removeLast();
        if (ArgList.isEmpty()) // ничего толкового не получено
            return;
        ServerResponse = ArgList.first();
        if (ServerResponse == SERVERRSTR)
        {
            CliLog->error("Server error response");
            DetectedError = CLIER_SERVER;
            Busy = false;
            TimeoutTimer->stop();
            return;
        }
        if (ServerResponse == "IDLE")
            CurrentCommand = M_IDLE;
    }
    switch (CurrentCommand)
    {
    case M_LOGIN: // по сути установление соединения, должны получить запрос LOGIN
    {
        if (ServerResponse == "M5")
        {
            CmdOk = true;
            SendCmd(M_ANSLOGIN);
            return;
        }
        break;
    }
    case M_ANSLOGIN:
    {
        if (ServerResponse == "M6")
        {
            CmdOk = true;
            SendCmd(M_ANSPSW);
            return;
        }
        else
            DetectedError = CLIER_LOGIN;
        break;
    }
    case M_ANSPSW:
    {
        // если получили в ответ "GROUP <access>", значит, всё в порядке, иначе ошибка пароля
        if (ServerResponse == "M2")
        {
            LoginOk = true;
            bool ok;
            pc.access = ArgList.at(1).toLong(&ok,16);
            if (!ok)
            {
                CliLog->warning("Group access undefined: "+ArgList.at(1));
                pc.access = 0x0; // нет доступа никуда
                DetectedError = CLIER_GROUP;
                return;
            }
            CliLog->info("Group access: "+QString::number(pc.access));
            CmdOk = true;
            CurrentCommand = M_IDLE;
        }
        else
            DetectedError = CLIER_PSW;
        break;
    }
    case M_QUIT:
    {
        if (ServerResponse == "BYE")
            CmdOk = true;
        break;
    }
    case S_TC:
    case S_TA:
    case S_TD:
    case S_UPD:
    case S_DEL:
    case S_RDEL:
        if (ServerResponse == "OK")
            CmdOk = true;
        else
        {
            DetectedError = CLIER_CMDER;
            return;
        }
        break;
    case S_GID:
    case S_INS:
    {
        bool ok;
        ResultInt = ArgList.at(0).toInt(&ok);
        if (!ok)
        {
            CliLog->warning("It's not possible to convert to integer: "+ArgList.at(0));
            DetectedError = CLIER_WRANSW;
            return;
        }
        CmdOk = true;
        break;
    }
    case S_GVSBC:
    case S_GVSBCF:
    case S_GVSBFS:
    case S_GVBFS:
    case S_GCS:
    case S_SRCH:
    case T_GVSBC:
    case T_GVSBCF:
    case T_GVSBFS:
    {
        // Формат ответа на запрос:
        //      <number_of_records>\n
        //      value[0][0] value[1][0] ... value[n][0] value[0][1] value[1][1] ... value[n][1] ... value[0][k] value[1][k] ... value[n][k]\n

        if (ArgList.size()<1) // нет количества записей
        {
            WriteErrorAndBreakReceiving("Некорректное количество аргументов");
            DetectedError = CLIER_WRANSW;
            return;
        }
        bool ok;
        MsgNum = ArgList.at(0).toInt(&ok);
        if ((!ok) || (MsgNum < 0))
        {
            WriteErrorAndBreakReceiving("Некорректное количество посылок");
            DetectedError = CLIER_WRANSW;
            return;
        }
        if (MsgNum == 0)
        {
            CliLog->info("Пустой ответ");
            Busy = false;
            DetectedError = CLIER_EMPTY;
            break;
        }
#ifndef TIMERSOFF
        TimeoutTimer->start();
#endif
        SendCmd(M_NEXT);
        return;
        break;
    }
    case M_NEXT:
    {
        if (MsgNum == 0) // конец передачи, пришёл IDLE
        {
            CmdOk = true;
            break;
        }
        QStringList sl;
        if (ResultType == RESULT_VECTOR)
        {
            if (Result.size() > 0)
                sl = Result.takeFirst();
            Result.clear();
        }
        while ((MsgNum) && (ArgList.size()))
        {
            if (ArgList.size() < FieldsNum)
            {
                if (MsgNum == 1)
                {
                   CliLog->warning("Некратное число записей в SQL-ответе");
                   MsgNum = 0;
                   ArgList.clear();
                   DetectedError = CLIER_EMPTY;
                   break;
                }
                else // блок из READBUFMAX символов был считан, будет следующий
                    FieldsLeastToAdd = FieldsNum - ArgList.size();
            }
            if (ResultType == RESULT_MATRIX)
            {
                sl.clear();
                if (FieldsLeast) // осталось с предыдущего раза
                {
                    sl = Result.takeLast(); // берём предыдущий считанный кусок
                    for (int i=0; i<FieldsLeastToAdd; i++)
                        sl.append(ArgList.takeFirst());
                    FieldsLeast = false;
                    FieldsLeastToAdd = 0;
                    MsgNum--;
                }
                else if (FieldsLeastToAdd != 0)
                {
                    FieldsLeast = true;
                    for (int i=0; i<ArgList.size(); i++)
                        sl.append(ArgList.takeFirst());
                }
                else
                {
                    for (int i=0; i<FieldsNum; i++)
                        sl.append(ArgList.takeFirst());
                    MsgNum--;
                }
                Result.append(sl);
            }
            else // ResultType == RESULT_VECTOR
            {
                for (int i=0; i<FieldsNum; i++)
                    sl.append(ArgList.takeFirst());
                MsgNum--;
            }
        }
        if (DetectedError != CLIER_NOERROR)
            break;
        if (MsgNum == 0) // кончились ответы, можно выходить
        {
            if (ResultType == RESULT_VECTOR)
                Result.append(sl);
            CmdOk = true;
            break;
        }
#ifndef TIMERSOFF
        TimeoutTimer->start();
#endif
        SendCmd(M_NEXT);
        return;
        break;
    }
    // первый приём после команды
    case CMD_GETFILE:
    {
        if (ServerResponse == "RDY")
        {
            if (ArgList.size()>1)
            {
                CmdOk = true;
                bool ok;
                filesize = ArgList.at(1).toLong(&ok,10);
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
            CurrentCommand = M_IDLE;
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
    CurrentCommand = M_IDLE;
    Busy = false;
}

void Client::WriteErrorAndBreakReceiving(QString ErMsg)
{
    CliLog->warning(ErMsg);
    TimeoutTimer->stop();
    Busy = false;
    CurrentCommand = M_IDLE;
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
#ifndef TIMERSOFF
    TimeoutTimer->start(); // рестарт таймера для получения запроса от сервера
#endif
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

// разделение буфера на подстроки по пробелу. Значения в кавычках берутся целиком

QStringList Client::SeparateBuf(QByteArray &buf)
{
    return QString::fromLocal8Bit(buf).split(0x7F);
}

// объединение списка строк в одну строку через разделитель

QString Client::Join(QStringList &sl)
{
    return sl.join(TOKEN);
}

// проверка аргументов

int Client::CheckArgs(QString &cmd, QStringList &Args, int argsnum, bool fieldscheck, bool pairscheck)
{
    int pnum;
    if (Args.size() < argsnum)
    {
        CliLog->error(cmd + ": Number of arguments is less than " + QString::number(argsnum));
        DetectedError = CLIER_WRARGS;
        Busy = false;
        return 1;
    }
    if (fieldscheck)
    {
        if (Args.size()<1)
        {
            CLIER("DBG: Fieldscheck");
            return 5;
        }
        QString fieldsnum = Args.at(0);
        bool ok;
        FieldsNum = fieldsnum.toInt(&ok);
        if (!ok)
        {
            CliLog->error(cmd + ": argument is not a number");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return 2;
        }
    }
    if (pairscheck)
    {
        int pidx = (fieldscheck) ? 1 : 0;
        if (Args.size()<(pidx+1))
        {
            CLIER("DBG: Pairscheck");
            return 5;
        }
        QString pairsnum = Args.at(pidx);
        pnum = pairsnum.toInt(&ok);
        if (!ok)
        {
            CliLog->error(cmd + ": argument is not a number");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return 3;
        }
    }
    if (fieldscheck && pairscheck)
    {
        if (Args.size() < FieldsNum+2*pnum+2) // +1 - db, table
        {
            CliLog->error(cmd + ": Number of fields is less than mentioned in header: "+QString::number(Args.size())+" "+QString::number(FieldsNum+2*pnum+1));
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return 4;
        }
    }
    return 0;
}
