#define _CRT_SECURE_NO_WARNINGS

#include <QThread>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QDateTime>
#include <QTextCodec>
#include <QDir>

#include "client.h"
#include "supik.h"

Client *Cli = 0;

Client::Client(QObject *parent) : QObject(parent)
{
    PingIsDisabled = false;
    NextActive = false;
    ServRetryActive = false;
    WaitActive = false;
    ServRetryCount = 0;
    EthStatus.setStatus(STAT_CLOSED);
    isAboutToFinish = false;
    isThereSomethingToSend = false;
    ResultType = RESULT_NONE;
    CurRetrPeriod = 0;
    LastArgs.clear();
    LastCommand = M_IDLE;
    RetryTimePeriods[0] = CL_RETR1;
    RetryTimePeriods[1] = CL_RETR2;
    RetryTimePeriods[2] = CL_RETR3;
    RetryTimePeriods[3] = CL_RETR4;
    RetryTimePeriods[4] = CL_RETR5;
    RetryTimePeriods[5] = CL_RETR6;
    RetryTimePeriods[6] = CL_RETR7;
    CmdMap.insert(S_GVBFS, {"S_GVBFS", 7, "S5", RESULT_VECTOR, true, true});
    CmdMap.insert(S_GVSBFS, {"S_GVSBFS", 7, "S1", RESULT_MATRIX, true, true});
    CmdMap.insert(S_GVSBC, {"S_GVSBC", 3, "S2", RESULT_VECTOR, false, false});
    CmdMap.insert(S_GVSBCF, {"S_GVSBCF", 6, "S3", RESULT_VECTOR, false, true});
    CmdMap.insert(S_GCS, {"S_GCS", 2, "S4", RESULT_VECTOR, false, false});
    CmdMap.insert(S_TC, {"S_TC", 3, "S6", RESULT_NONE, false, false});
    CmdMap.insert(S_TA, {"S_TA", 5, "S7", RESULT_NONE, false, false});
    CmdMap.insert(S_TD, {"S_TD", 2, "S8", RESULT_NONE, false, false});
    CmdMap.insert(S_INS, {"S_INS", 2, "S9", RESULT_INT, false, false});
    CmdMap.insert(S_UPD, {"S_UPD", 6, "S:", RESULT_NONE, false, false});
    CmdMap.insert(S_DEL, {"S_DEL", 4, "S;", RESULT_NONE, false, false});
    CmdMap.insert(S_RDEL, {"S_RDEL", 4, "S<", RESULT_NONE, false, false});
    CmdMap.insert(S_SRCH, {"S_SRCH", 6, "S=", RESULT_MATRIX, true, false});
    CmdMap.insert(S_GID, {"S_GID", 2, "S>", RESULT_INT, false, false});
    CmdMap.insert(T_GVSBFS, {"T_GVSBFS", 6, "T1", RESULT_MATRIX, true, true});
    CmdMap.insert(T_GVSBC, {"T_GVSBC", 2, "T2", RESULT_VECTOR, false, false});
    CmdMap.insert(T_GVSBCF, {"T_GVSBCF", 5, "T3", RESULT_VECTOR, false, false});
    CmdMap.insert(T_GFT, {"T_GFT", 1, "T7", RESULT_MATRIX, false, false});
    CmdMap.insert(T_TV, {"T_TV", 3, "T:", RESULT_STRING, false, false});
    CmdMap.insert(T_IDTV, {"T_IDTV", 2, "T9", RESULT_VECTOR, false, false});
    CmdMap.insert(T_INS, {"T_INS", 1, "T>", RESULT_INT, false, false});
    CmdMap.insert(T_UPD, {"T_UPD", 3, "T?", RESULT_NONE, false, false});
    CmdMap.insert(T_DEL, {"T_DEL", 2, "T5", RESULT_NONE, false, false});
    CmdMap.insert(T_RDEL, {"T_RDEL", 2, "T6", RESULT_NONE, false, false});
    CmdMap.insert(T_IDTVL, {"T_IDTVL", 1, "T;", RESULT_VECTOR, false, false});
    CmdMap.insert(T_GID, {"T_GID", 1, "T8", RESULT_INT, false, false});
    CmdMap.insert(T_VTID, {"T_VTID", 2, "T=", RESULT_STRING, false, false});
    CmdMap.insert(T_TID, {"T_TID", 3, "T<", RESULT_STRING, false, false});
    CmdMap.insert(T_C, {"T_C", 3, "T4", RESULT_STRING, false, false});
    CmdMap.insert(T_UPDV, {"T_UPDV", 3, "TC", RESULT_NONE, false, false});
    CmdMap.insert(M_PUTFILE, {"M_PUTFILE", 4, "M7", RESULT_NONE, false, false}); // 0 - local filename, 1 - type, 2 - subtype, 3 - filename, [4] - filesize, added in SendCmd
    CmdMap.insert(M_GETFILE, {"M_GETFILE", 3, "M8", RESULT_NONE, false, false}); // 0 - type, 1 - subtype, 2 - filename
    CmdMap.insert(M_ACTIVATE, {"M_ACTIVATE", 2, "M9", RESULT_STRING, false, false}); // 0 - code, 1 - newpass
    CmdMap.insert(M_STATUS, {"M_STATUS", 0, "M3", RESULT_STRING, false, false});
    CmdMap.insert(M_PING, {"M_PING", 0, "M4", RESULT_STRING, false, false});
    InitiateTimers();
}

Client::~Client()
{
}

void Client::Run()
{
    QTime retrytime;
    MainEthernet = new Ethernet;
    MainEthernet->SetEthernet(Host, Port.toInt(), Ethernet::ETH_SSL);
    connect(MainEthernet,SIGNAL(connected()),this,SLOT(ClientConnected()));
    connect(MainEthernet,SIGNAL(disconnected()),this,SLOT(ClientDisconnected()));
    connect(MainEthernet,SIGNAL(NewDataArrived(QByteArray)),this,SLOT(ParseReply(QByteArray)));
    connect(MainEthernet,SIGNAL(byteswritten(qint64)),this,SLOT(SendNext(qint64)));
    connect(this,SIGNAL(CallSend(int,QStringList&)),this,SLOT(Send(int,QStringList&)));
    DetectedError = CLIER_NOERROR;
    while (!isAboutToFinish)
    {
        if (isThereSomethingToSend)
        {
            EthStatus.setCommandActive();
#ifndef TIMERSOFF
            TimeoutTimer->start();
#endif
            Send(CurrentCommand, CurrentArgs);
            while (EthStatus.isCommandActive())
            {
                QTime tme;
                tme.start();
                while (tme.elapsed() < MAINSLEEP)
                    qApp->processEvents(QEventLoop::AllEvents);
            }
            if (DetectedError == CLIER_TIMEOUT)
            {
                // try to reconnect
                MainEthernet->Disconnect();
                EthStatus.setStatus(STAT_CLOSED);
                retrytime.start();
            }
        }
    }
    emit finished();
}

void Client::StartLog()
{
    CliLog = new Log;
    CliLog->Init("cli.log");
    CliLog->info("=== Log started ===");
}

void Client::FinishThread()
{
    isAboutToFinish = true;
}

int Client::Connect(QString host, QString port, int clientmode)
{
    ClientMode = clientmode;
    Host = host;
    Port = port;
    if (EthStatus.isConnected()) // если уже подсоединены, не надо по второму разу
        return CLIER_NOERROR;
    EthStatus.setMode(clientmode);
    if (EthStatus.isTestMode())
    {
        Pers = "test";
        Pass = "test";
    }
    else
    {
        Pers = pc.PersLogin;
        Pass = pc.PersPsw;
    }
    CmdOk = false;
    PrevLastBA.clear();
    LoginOk = false;

    while ((EthStatus.isntConnected()) && (DetectedError == CLIER_NOERROR))
    {
        QTime tme;
        tme.start();
        while (tme.elapsed() < MAINSLEEP)
            qApp->processEvents(QEventLoop::AllEvents);
    }
    if (EthStatus.isntConnected())
    {
        CliLog->warning("Entering autonomous mode...");
        return CLIER_TIMEOUT;
    }
    DetectedError = CLIER_NOERROR;
    if (!PrevLastBA.isEmpty()) // if there was some data in canal while we were connecting
    {
        QByteArray ba = PrevLastBA;
        PrevLastBA.clear();
        ParseReply(ba); // parse it
    }
    while (!LoginOk && (DetectedError == CLIER_NOERROR))
    {
        QTime tme;
        tme.start();
        while (tme.elapsed() < MAINSLEEP)
            qApp->processEvents(QEventLoop::AllEvents);
    }
    if (!LoginOk)
    {
        CliLog->warning("Wrong login or password");
        return DetectedError;
    }
    return DetectedError;
}

void Client::InitiateTimers()
{
    TimeoutTimer = new QTimer;
    TimeoutTimer->setInterval(MAINTIMEOUT);
    connect(TimeoutTimer,SIGNAL(timeout()),this,SLOT(Timeout()));
    EthStateChangeTimer = new QTimer;
    EthStateChangeTimer->setInterval(ETHTIMEOUT); // таймер на получение файлов, если за 2 с ничего не принято, считаем, что файл окончен
    EthStateChangeTimer->setSingleShot(true);
    connect(EthStateChangeTimer,SIGNAL(timeout()),this,SLOT(EthStateChangeTimerTimeout()));
    RetrTimer = new QTimer;
    RetrTimer->setInterval(RetryTimePeriods[CurRetrPeriod]);
    connect(RetrTimer,SIGNAL(timeout()),this,SLOT(RetrTimeout()));
}

void Client::SendNext(qint64 bytes)
{
    switch(CurrentCommand)
    {
    case M_APUTFILE:
    {
        CliLog->info(QString::number(bytes)+" bytes written to file");
        WrittenBytes += bytes;
        break;
    }
    default:
        break;
    }
}

void Client::Disconnect()
{
    if (EthStatus.isAboutToClose())
    {
        if (MainEthernet != 0)
            delete MainEthernet;
        EthStatus.setStatus(STAT_CLOSED);
    }
}

void Client::SendCmd(int command, QStringList &args)
{
    emit CallSend(command, args);
}

void Client::Send(int command, QStringList &args)
{
    if ((EthStatus.isCommandActive()) && (command == M_PING))
    {
        CliLog->error("Command active already: "+QString::number(CurrentCommand));
        return;
    }
    if ((EthStatus.isntConnected()) || (TimeoutCounter > 3)) // if we're disconnected
    {
        CliLog->error("Ethernet disconnected");
        TimeoutCounter = 0;
        PingIsDisabled = true;
        DetectedError = CLIER_CLOSED;
        if (!EthStatus.isAboutToClose())
            ClientDisconnected();
        RetrTimeout();
        return;
    }
    CurrentCommand = command;
    NextActive = false;
    PrevLastBA.clear();
    if ((command != M_LOGIN) && (command != M_ANSLOGIN) && (command != M_AGETFILE) && (command != M_NEXT)) // not "RDY"
    {
        LastCommand = command;
        LastArgs = args; // store command arguments for retrying
    }
    if (!ServRetryActive) // if this send is not a retry
        ServRetryCount = 0;
    if (EthStatus.isTestMode() && (command != M_ACTIVATE) && (command != M_ANSLOGIN) && (command != M_QUIT))
    {
        CliLog->warning("illegal test command");
        DetectedError = CLIER_CMDER;
        return;
    }
    if (command != M_NEXT)
    {
        Result.clear(); // очищаем результаты
        ResultStr.clear();
    }
    DetectedError = CLIER_NOERROR;
//    Busy = true;
#ifndef TIMERSOFF
    TimeoutTimer->start();
#endif
    QString CommandString;
    if (CmdMap.keys().contains(command))
    {
        CmdStruct st = CmdMap[command];
        FieldsNum = 0;
        if (!CheckArgs(st.CmdString, args, st.ArgsNum, st.CheckForFieldsNum, st.CheckForPairsNum))
            return;
        if (FieldsNum == 0)
            FieldsNum = 1; // если не выставлено значение поля в функции CheckArgs, выставить его принудительно в 1 (одно поле на запись)
        QStringList sl;
        sl << st.Prefix;
        sl.append(args);
        QString tmps = sl.join(TOKEN);
        CommandString = tmps;
        ResultType = st.ResultType;
    }
    switch (command)
    {
    case M_PING:
        if (PingIsDisabled)
            return;
    case M_STATUS:
    case M_ACTIVATE:
        break; // no operands or all is already included in CommandString
/*    case CMD_MESSAGES:
    {
        break;
    }
    case CMD_CHATMSGS:
    {
        break;
    } */
    // GETF получить файл с сервера
    // 0 - тип файла
    // 1 - подтип файла
    // 2 - имя файла
    // Формат: M8 <тип> <подтип> <имя_файла>\n
    // далее ожидание прихода "<длина_в_байтах>\n", затем приём содержимого файла блоками по 16384 байт
    case M_GETFILE:
    {
        bool ok;
        int fltype, flsubtype;
        QString path = pc.HomeDir + "/";
        fltype = args.at(0).toInt(&ok);
        if ((ok) && (fltype < PathPrefixes.size()))
            path += PathPrefixes.at(fltype);
        flsubtype = args.at(1).toInt(&ok);
        if ((ok) && (flsubtype < PathSuffixes.size()))
            path += PathSuffixes.at(flsubtype);
        QDir *dr = new QDir;
        dr->mkpath(path);
        delete dr;
        fp.setFileName(path + args.at(2));
        if (!fp.open(QIODevice::WriteOnly))
        {
            ERMSG("Невозможно создать файл" + args.at(2));
            DetectedError = CLIER_GETFER;
            return;
        }
        ReadBytes = 0;
        RcvDataSize = 0;
        break;
    }
    // PUTFILE - отправить файл на сервер
    // 0 - имя файла для отправки на клиенте (на сервере игнорируется)
    // 1 - тип файла
    // 2 - подтип файла
    // 3 - имя файла на сервере
    case M_PUTFILE:
    {
        fp.setFileName(args.at(0));
        if (!fp.open(QIODevice::ReadOnly))
        {
            ERMSG("Невозможно открыть файл "+args.at(0));
            DetectedError = CLIER_PUTFER;
            return;
        }
        WrittenBytes = 0;
        XmitDataSize = fp.size();
        emit BytesOverall(XmitDataSize);
        CommandString.push_back(TOKEN);
        CommandString += QString::number(XmitDataSize); // file length added
        break;
    }
    case M_APUTFILE:
    {
        while ((WrittenBytes < XmitDataSize) && (DetectedError == CLIER_NOERROR))
        {
            quint64 BytesToSend = XmitDataSize - WrittenBytes;
            quint64 NextThr;
            if (BytesToSend > READBUFMAX)
                BytesToSend = READBUFMAX;
            if (fp.isOpen())
            {
                WrData = QByteArray(fp.read(BytesToSend));
                if (WrData.isEmpty())
                {
                    Error("Error while reading file", CLIER_PUTFER);
                    return;
                }
#ifndef TIMERSOFF
                TimeoutTimer->start();
#endif
                CliLog->info("> ...binary data " + QString::number(WrData.size()) + " size...");
                NextThr = WrittenBytes + WrData.size();
                MainEthernet->WriteData(WrData);
                emit BytesWritten(WrData.size());
            }
            else
            {
                Error("File error", CLIER_PUTFER);
                EthStatus.clearCommandActive();
                return;
            }
            while (WrittenBytes < NextThr)
            {
                qApp->processEvents(QEventLoop::AllEvents, 100);
            }
        }
        if (fp.isOpen())
            fp.close();
        emit TransferComplete();
        CmdOk = true;
        EthStatus.clearCommandActive();
        TimeoutTimer->stop();
        CurrentCommand = M_IDLE;
        return;
    }
    case M_AGETFILE:
    case M_NEXT:
    {
        CommandString = "RDY";
        break;
    }
    case M_QUIT:
    {
        CommandString = "M1";
        break;
    }
    case M_ANSLOGIN:
    {
        CommandString = Pers + TOKEN + Pass + TOKEN + PROGVER;
        break;
    }
    default:
        break;
    }
    EthStatus.setCommandActive();
    if (command == M_ANSLOGIN)
        CliLog->info(">"+Pers);
    else
        CliLog->info(">"+CommandString); //+codec->fromUnicode(CommandString));
    QByteArray ba = CommandString.toUtf8();//codec->fromUnicode(CommandString));
    MainEthernet->WriteData(ba);
    emit BytesWritten(ba.size());
}

// ############################################ ОБРАБОТКА ОТВЕТА ##############################################

void Client::ParseReply(QByteArray ba)
{
    if (ba.isEmpty())
        return;
    if (EthStatus.isntConnected()) // if there's a real connection but the flag wasn't set (at the start)
    {
        PrevLastBA += ba;
        return;
    }
    emit BytesRead(ba.size());
    QString RcvDataString;
    CmdOk = false;
    ServRetryActive = false; // if there should be a retry, set it at SERVRETSTR processing
    DetectedError = CLIER_NOERROR;
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    if (CurrentCommand != M_AGETFILE) // приём файла обрабатывается по-другому
    {
        RcvDataString = QString::fromLocal8Bit(codec->fromUnicode(ba));
        if (RcvDataString == SERVERRSTR)
        {
            if (fp.isOpen())
                fp.close();
            Error("Server error response", CLIER_SERVER);
            TimeoutTimer->stop();
            SetWaitEnded();
            EthStatus.clearCommandActive();
            return;
        }
        if (RcvDataString == SERVEMPSTR)
        {
            if (fp.isOpen())
                fp.remove(); // remove empty created in files.cpp file
            Error("Server empty response", CLIER_EMPTY);
            TimeoutTimer->stop();
            SetWaitEnded();
            EthStatus.clearCommandActive();
            return;
        }
        if (RcvDataString == SERVRETSTR) // for file operations
        {
            CliLog->info("Server wants retry, trying retry");
            ++ServRetryCount;
            if (ServRetryCount > CL_MAXRETRCOUNT)
            {
                if (fp.isOpen())
                    fp.close();
                Error("Server timeout", CLIER_SERVER);
            }
            else
            {
                ServRetryActive = true;
                Busy = false;
                CurrentCommand = M_IDLE;
                SendCmd(LastCommand, LastArgs);
            }
            TimeoutTimer->stop();
            SetWaitEnded();
            EthStatus.clearCommandActive();
            return;
        }
        if (RcvDataString == SERVIDLSTR)
        {
            CurrentCommand = M_IDLE;
            SetWaitEnded();
            EthStatus.clearCommandActive();
            return;
        }
        if (RcvDataString == SERVWAIT)
        {
            TimeoutTimer->start();
            if (!WaitActive)
            {
                emit WaitStarted();
                WaitActive = true;
            }
            return;
        }
        QString cmdstr = QString::number(CurrentCommand);
        CliLog->info("<["+cmdstr+"] "+RcvDataString);
    }
    SetWaitEnded();
    if (RcvDataString == "M5") // M_LOGIN
    {
        CmdOk = true;
        Busy = false;
        EthStatus.setCommandActive();
        SendCmd(M_ANSLOGIN);
        return;
    }
    switch (CurrentCommand)
    {
    case M_STATUS:
        ResultStr = RcvDataString;
        CmdOk = true;
        break;
    case M_PING:
        if (ResultStr == SERVEROK)
            CmdOk = true;
        break;
    case M_ANSLOGIN:
    {
        // если получили в ответ "GROUP <access>", значит, всё в порядке, иначе ошибка пароля
        if ((ClientMode == STAT_MODETEST) && (RcvDataString == SERVEROK))
        {
            LoginOk = true;
            CmdOk = true;
        }
        else
        {
            QStringList sl = QString::fromLocal8Bit(ba).split(TOKEN); // разделение буфера на подстроки по пробелу. Значения в кавычках берутся целиком
            if (sl.size() < 2) // GROUP <access>
            {
                Error("Bad group answer", CLIER_PSW);
                break;
            }
            if (sl.at(0) == "M2")
            {
                LoginOk = true;
                bool ok;
                pc.access = sl.at(1).toLong(&ok,16);
                if (!ok)
                {
                    pc.access = 0x0; // нет доступа никуда
                    Error("Group access undefined: "+sl.at(1), CLIER_GROUP);
                    EthStatus.clearCommandActive();
                    return;
                }
                CliLog->info("Group access: "+QString::number(pc.access));
                CmdOk = true;
            }
            else
                Error("Bad password", CLIER_PSW);
        }
        break;
    }
    case M_QUIT:
    {
        if (RcvDataString == "BYE")
            CmdOk = true;
        break;
    }
    // commands without any reply
    case S_TC:
    case S_TA:
    case S_TD:
    case S_UPD:
    case S_DEL:
    case S_RDEL:
    case T_DEL:
    case T_RDEL:
    case T_UPD:
    case T_UPDV:
    {
        if (RcvDataString == "OK")
            CmdOk = true;
        else
        {
            Error("Error while command processing", CLIER_CMDER);
            EthStatus.clearCommandActive();
            return;
        }
        break;
    }
    // commands with int reply
    case S_GID:
    case S_INS:
    case T_GID:
    case T_INS:
    // commands with vector reply
    case S_GVSBC:
    case S_GVSBCF:
    case S_GVSBFS:
    case S_GVBFS:
    case S_GCS:
    case S_SRCH:
    case T_GVSBC:
    case T_GVSBCF:
    case T_GVSBFS:
    case T_IDTV:
//    case T_IDTVL:
    case T_GFT:
    case T_TF:
    case T_TH:
    case T_TL:
    // commands with string reply
    case M_ACTIVATE:
    case T_TV:
    case T_TID:
    case T_VTID:
    case T_C:
    {
        // Формат ответа на запрос:
        //      <number_of_records><0x7F>value[0]<0x7F>value[1]...
        bool ok;
        QList<QByteArray> RcvList = ba.split(TOKEN);
        if (RcvList.isEmpty()) // нет ничего в принятой посылке (0 байт)
        {
            Error("Wrong answer", CLIER_WRANSW);
            EthStatus.clearCommandActive();
            return;
        }
        if (!NextActive)
        {
            QString tmps = QString::fromUtf8(RcvList.takeFirst());
            RcvDataSize = tmps.toInt(&ok); // количество байт
            if ((!ok) || (RcvDataSize < 0))
            {
                Error("Incorrect number of bytes", CLIER_WRANSW);
                EthStatus.clearCommandActive();
                return;
            }
            if (RcvDataSize == 0)
            {
                Error("Empty answer", CLIER_EMPTY);
                break;
            }
            RcvDataSize += tmps.size() + 1; // to substract the size with token at the following line
            NextActive = true;
        }
        if (RcvList.isEmpty()) // нет ничего после размера
        {
            Error("Wrong answer", CLIER_WRANSW);
            EthStatus.clearCommandActive();
            return;
        }
        RcvDataSize -= ba.size();
        QStringList sl;

        switch (ResultType)
        {
        case RESULT_STRING:
        {
            ResultStr += QString::fromUtf8(RcvList.at(0));
            break;
        }
        case RESULT_INT:
        {
            bool ok;
            ResultInt = QString::fromUtf8(RcvList.at(0)).toInt(&ok);
            if (!ok)
            {
                Error("It's not possible to convert to integer: "+RcvDataString, CLIER_WRANSW);
                EthStatus.clearCommandActive();
                return;
            }
            CmdOk = true;
            TimeoutTimer->stop();
            CurrentCommand = M_IDLE;
            EthStatus.clearCommandActive();
            return;
        }
        case RESULT_MATRIX:
        {
            if (!Result.isEmpty()) // дополняем последний элемент
            {
                PrevLastBA += RcvList.takeFirst();
                sl = Result.takeLast(); // берём предыдущий считанный кусок
                sl.replace(sl.size()-1, QString::fromUtf8(PrevLastBA));
            }
            PrevLastBA = RcvList.last(); // prepare for the next chunk
            while ((sl.size() < FieldsNum) && !(RcvList.isEmpty()))
                sl.append(QString::fromUtf8(RcvList.takeFirst()));
                Result.append(sl);
            while (RcvList.size())
            {
                Result.append(QStringList());
                sl.clear();
                while ((sl.size() < FieldsNum) && !(RcvList.isEmpty()))
                    sl.append(QString::fromUtf8(RcvList.takeFirst()));
                Result.replace(Result.size()-1, sl);
            }
            break;
        }
        case RESULT_VECTOR:
        {
            if (Result.isEmpty())
                Result.append(QStringList());
            sl = Result.at(0);
            if (!sl.isEmpty())
            {
                PrevLastBA += RcvList.takeFirst();
                sl.replace(sl.size()-1, QString::fromUtf8(PrevLastBA));
            }
            PrevLastBA = RcvList.last();
            while (!RcvList.isEmpty())
                sl.append(QString::fromUtf8(RcvList.takeFirst()));
            Result.replace(0, sl);
            break;
        }
        default:
            Error("Bad result type", CLIER_WRANSW);
            EthStatus.clearCommandActive();
            return;
        }
        if (RcvDataSize == 0) // последний фрагмент обработан
        {
            CmdOk = true;
            TimeoutTimer->stop();
            EthStatus.clearCommandActive();
            return;
        }
#ifndef TIMERSOFF
        TimeoutTimer->start();
#endif
        return;
    }
    case M_PUTFILE:
    {
        if (RcvDataString != "OK")
        {
            Error("Not ok answer", CLIER_PUTFER);
            EthStatus.clearCommandActive();
            return;
        }
        SendCmd(M_APUTFILE);
        return;
        break;
    }
    // первый приём после команды
    case M_GETFILE:
    {
        CmdOk = true;
        bool ok;
        RcvDataSize = RcvDataString.toLong(&ok,10);
        if (!ok)
        {
            if (fp.isOpen())
                fp.remove();
            Error("Not a decimal value detected", CLIER_GETFER);
            EthStatus.clearCommandActive();
            return;
        }
        emit BytesOverall(RcvDataSize);
        EthStatus.clearCommandActive();
        SendCmd(M_AGETFILE);
        return;
    }
    // последующие приёмы файла
    case M_AGETFILE:
    {
        if (!fp.isOpen())
        {
            Error("File error", CLIER_GETFER);
            return;
        }
        CliLog->info("< ...binary data "+QString::number(ba.size())+" size...");
        if (ba.data() == SERVERRSTR)
        {
            if (fp.isOpen())
                fp.remove();
            Error("Server error response", CLIER_SERVER);
            return;
        }
        ReadBytes += ba.size();
        qint64 rb = fp.write(ba);
//        emit BytesRead(ReadBytes);
        CliLog->info(QString::number(rb)+" bytes written to file");
        CliLog->info(QString::number(ReadBytes)+" bytes written overall");
        quint64 ReadRemains = RcvDataSize - ReadBytes;
        CliLog->info(QString::number(ReadRemains)+" bytes least");
        if (ReadBytes >= RcvDataSize)
        {
            if (fp.isOpen())
                fp.close();
            emit TransferComplete();
            CmdOk = true;
        }
        else
        {
#ifndef TIMERSOFF
            TimeoutTimer->start();
#endif
            return;
        }
        break;
    }
    default:
        break;
    }
    TimeoutTimer->stop();
    EthStatus.clearCommandActive();
}

void Client::Error(QString ErMsg, int ErrorInt)
{
    CliLog->warning(ErMsg);
    DetectedError = ErrorInt;
    TimeoutTimer->stop();
    EthStatus.clearCommandActive();
    CurrentCommand = M_IDLE;
}

void Client::EthStateChangeTimerTimeout()
{
    Disconnect();
}

void Client::ClientConnected()
{
    EthStatus.setStatus(STAT_CONNECTED);
#ifndef TIMERSOFF
    TimeoutTimer->start(); // рестарт таймера для получения запроса от сервера
#endif
    emit RetrEnded();
    PingIsDisabled = false;
}

void Client::ClientDisconnected()
{
    EthStatus.setStatus(STAT_ABOUTTOCLOSE); // delete MainEthernet to try connect later if needed
    EthStateChangeTimer->start();
}

void Client::Timeout()
{
    ERMSG("Timeout detected");
    EthStatus.clearCommandActive();
    DetectedError = CLIER_TIMEOUT;
    TimeoutTimer->stop();
}

// проверка аргументов

bool Client::CheckArgs(QString cmd, QStringList &args, int argsnum, bool fieldscheck, bool pairscheck)
{
    int pnum;
    if (args.size() < argsnum)
    {
        CliLog->error(cmd + ": Number of arguments is less than " + QString::number(argsnum));
        DetectedError = CLIER_WRARGS;
        return false;
    }
    if (fieldscheck)
    {
        if (args.size()<1)
        {
            ERMSG("DBG: Fieldscheck");
            return false;
        }
        QString fieldsnum = args.at(0);
        bool ok;
        FieldsNum = fieldsnum.toInt(&ok);
        if (!ok)
        {
            CliLog->error(cmd + ": argument is not a number");
            DetectedError = CLIER_WRARGS;
            return false;
        }
    }
    if (pairscheck)
    {
        int pidx = (fieldscheck) ? 1 : 0;
        if (args.size()<(pidx+1))
        {
            ERMSG("DBG: Pairscheck");
            return false;
        }
        QString pairsnum = args.at(pidx);
        bool ok;
        pnum = pairsnum.toInt(&ok);
        if (!ok)
        {
            CliLog->error(cmd + ": argument is not a number");
            DetectedError = CLIER_WRARGS;
            return false;
        }
    }
    if (fieldscheck && pairscheck)
    {
        if (args.size() < FieldsNum+2*pnum+2) // +1 - db, table
        {
            CliLog->error(cmd + ": Number of fields is less than mentioned in header: "+QString::number(args.size())+" "+QString::number(FieldsNum+2*pnum+1));
            DetectedError = CLIER_WRARGS;
            return false;
        }
    }
    return true;
}

int Client::GetFile(const QString &type, const QString &subtype, const QString &filename)
{
    QStringList sl;
    sl.clear();
    sl << type << subtype << filename;
    SendCmd(M_GETFILE, sl);
    while (EthStatus.isCommandActive())
    {
        QThread::msleep(10);
        qApp->processEvents(QEventLoop::AllEvents);
    }
    return DetectedError;
}

int Client::PutFile(const QString &localfilename, const QString &type, const QString &subtype, const QString &filename)
{
    QStringList sl;
    sl << localfilename << type << subtype << filename;
    SendCmd(M_PUTFILE, sl);
    while (EthStatus.isCommandActive())
    {
        QThread::msleep(10);
        qApp->processEvents(QEventLoop::AllEvents);
    }
    return DetectedError;
}

int Client::SendAndGetResult(int command, QStringList &args)
{
    Cli->SendCmd(command, args);
    while (EthStatus.isCommandActive())
    {
        QThread::msleep(10);
        qApp->processEvents(QEventLoop::AllEvents);
    }
    return DetectedError;
}

bool Client::isConnected()
{
    return EthStatus.isConnected();
}

void Client::RetrTimeout()
{
    RetrTimer->stop();
    if (Connect(Host, Port, ClientMode) == CLIER_NOERROR) // if the connection was successful send previous command with previous arguments
    {
        PingIsDisabled = false;
        EthStatus.clearCommandActive();
        SendCmd(LastCommand, LastArgs);
        emit RetrEnded();
        CurRetrPeriod = 0;
        return;
    }
    // else try again later
    if (CurRetrPeriod < CL_MAXRETR)
        ++CurRetrPeriod;
    RetrTimer->setInterval(RetryTimePeriods[CurRetrPeriod]);
    RetrTimer->start();
    emit RetrStarted(RetrTimer->interval()/1000);
}

void Client::SetWaitEnded()
{
    if (WaitActive)
    {
        WaitActive = false;
        emit WaitEnded();
    }
}
