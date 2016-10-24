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
    ResultType = RESULT_NONE;
    CmdMap.insert(S_GVBFS, {"S_GVBFS", 7, "S5", RESULT_VECTOR, true, true});
    CmdMap.insert(S_GVSBFS, {"S_GVSBFS", 7, "S1", RESULT_MATRIX, true, true});
    CmdMap.insert(S_GVSBC, {"S_GVSBC", 3, "S2", RESULT_VECTOR, false, false});
    CmdMap.insert(S_GVSBCF, {"S_GVSBCF", 6, "S3", RESULT_VECTOR, false, true});
    CmdMap.insert(S_GCS, {"S_GCS", 2, "S4", RESULT_VECTOR, false, false});
    CmdMap.insert(S_TC, {"S_TC", 3, "S6", RESULT_NONE, false, false});
    CmdMap.insert(S_TA, {"S_TA", 5, "S7", RESULT_NONE, false, false});
    CmdMap.insert(S_TD, {"S_TD", 2, "S8", RESULT_NONE, false, false});
    CmdMap.insert(S_INS, {"S_INS", 2, "S9", RESULT_STRING, false, false});
    CmdMap.insert(S_UPD, {"S_UPD", 6, "S:", RESULT_NONE, false, false});
    CmdMap.insert(S_DEL, {"S_DEL", 4, "S;", RESULT_NONE, false, false});
    CmdMap.insert(S_RDEL, {"S_RDEL", 4, "S<", RESULT_NONE, false, false});
    CmdMap.insert(S_SRCH, {"S_SRCH", 6, "S=", RESULT_MATRIX, true, false});
    CmdMap.insert(S_GID, {"S_GID", 2, "S>", RESULT_STRING, false, false});
    CmdMap.insert(T_GVSBFS, {"T_GVSBFS", 6, "T1", RESULT_MATRIX, true, true});
    CmdMap.insert(T_GVSBC, {"T_GVSBC", 2, "T2", RESULT_VECTOR, false, false});
    CmdMap.insert(T_GVSBCF, {"T_GVSBCF", 4, "T3", RESULT_VECTOR, false, false});
    CmdMap.insert(T_GFT, {"T_GFT", 1, "T7", RESULT_MATRIX, false, false});
    CmdMap.insert(T_TV, {"T_TV", 3, "T:", RESULT_STRING, false, false});
    CmdMap.insert(T_IDTV, {"T_IDTV", 2, "T9", RESULT_VECTOR, false, false});
    CmdMap.insert(T_INS, {"T_INS", 1, "T>", RESULT_STRING, false, false});
    CmdMap.insert(T_UPD, {"T_UPD", 3, "T?", RESULT_NONE, false, false});
    CmdMap.insert(T_DEL, {"T_DEL", 2, "T5", RESULT_NONE, false, false});
    CmdMap.insert(T_RDEL, {"T_RDEL", 2, "T6", RESULT_NONE, false, false});
    CmdMap.insert(T_IDTVL, {"T_IDTVL", 1, "T;", RESULT_VECTOR, false, false});
    CmdMap.insert(T_GID, {"T_GID", 1, "T8", RESULT_STRING, false, false});
    CmdMap.insert(T_VTID, {"T_VTID", 3, "T=", RESULT_STRING, false, false});
    CmdMap.insert(T_TID, {"T_TID", 3, "T<", RESULT_STRING, false, false});
    CmdMap.insert(T_C, {"T_C", 3, "T4", RESULT_STRING, false, false});
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

    if ((Command >= S_GVSBFS) && (Command <= T_TL))
    {
        CmdStruct st = CmdMap[Command];
        FieldsNum = 0;
        if (CheckArgs(st.CmdString, Args, st.ArgsNum, st.CheckForFieldsNum, st.CheckForPairsNum) != 0)
            return;
        if (FieldsNum == 0)
            FieldsNum = 1; // если не выставлено значение поля в функции CheckArgs, выставить его принудительно в 1 (одно поле на запись)
        QStringList sl;
        sl << st.Prefix;
        sl.append(Args);
        QString tmps = Join(sl);
        CommandString = tmps + "\n";
        ResultType = st.ResultType;
    }
    switch (Command)
    {
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
    default:
        break;
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
    case T_DEL:
    case T_RDEL:
    case T_UPD:
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
    case T_GID:
    case T_INS:
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
    case T_C:
    case T_IDTV:
    case T_TV:
    case T_IDTVL:
    case T_GFT:
    case T_TID:
    case T_VTID:
    case T_TF:
    case T_TH:
    case T_TL:
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

int Client::CheckArgs(QString cmd, QStringList &Args, int argsnum, bool fieldscheck, bool pairscheck)
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
        bool ok;
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
