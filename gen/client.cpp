#define _CRT_SECURE_NO_WARNINGS

#include <QThread>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QDateTime>
#include <QTextCodec>
#include <QDir>

#include "client.h"

Client *Cli = 0;

Client::Client(QObject *parent) : QObject(parent)
{
    NextActive = false;
    RetryActive = false;
    RetryCount = 0;
    Connected = false;
    ComReplyTimeoutIsSet = false;
    ResultType = RESULT_NONE;
    TimeoutCounter = 0;
    CurRetrPeriod = 0;
    LastArgs.clear();
    LastCommand = M_IDLE;
    MainEthernet = 0;
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
    CmdMap.insert(T_VTID, {"T_VTID", 3, "T=", RESULT_STRING, false, false});
    CmdMap.insert(T_TID, {"T_TID", 3, "T<", RESULT_STRING, false, false});
    CmdMap.insert(T_C, {"T_C", 3, "T4", RESULT_STRING, false, false});
    CmdMap.insert(T_UPDV, {"T_UPDV", 3, "TC", RESULT_NONE, false, false});
    CmdMap.insert(M_PUTFILE, {"M_PUTFILE", 4, "M7", RESULT_NONE, false, false}); // 0 - local filename, 1 - type, 2 - subtype, 3 - filename, [4] - filesize, added in SendCmd
    CmdMap.insert(M_GETFILE, {"M_GETFILE", 3, "M8", RESULT_NONE, false, false}); // 0 - type, 1 - subtype, 2 - filename
    CmdMap.insert(M_ACTIVATE, {"M_ACTIVATE", 2, "M9", RESULT_STRING, false, false}); // 0 - code, 1 - newpass
}

Client::~Client()
{
}

void Client::StartLog()
{
    CliLog = new Log;
    CliLog->Init("cli.log");
    CliLog->info("=== Log started ===");
}

int Client::Connect(QString host, QString port, int clientmode)
{
    Host = host;
    Port = port;
    ClientMode = clientmode;
    if (Connected) // если уже подсоединены, не надо по второму разу
        return CLIER_NOERROR;
    if (ClientMode == CLIMODE_TEST)
    {
        Pers = "test";
        Pass = "test";
    }
    else
    {
        Pers = pc.PersLogin;
        Pass = pc.PersPsw;
    }
    Busy = Connected = CmdOk = false;
    InitiateTimers();

    MainEthernet = new Ethernet;
    MainEthernet->SetEthernet(Host, Port.toInt(), Ethernet::ETH_SSL);
    connect(MainEthernet,SIGNAL(connected()),this,SLOT(ClientConnected()));
    connect(MainEthernet,SIGNAL(disconnected()),this,SLOT(ClientDisconnected()));
    connect(MainEthernet,SIGNAL(NewDataArrived(QByteArray)),this,SLOT(ParseReply(QByteArray)));
#ifndef TIMERSOFF
    TimeoutTimer->start();
#endif
    DetectedError = CLIER_NOERROR;
    LoginOk = false;
    CurrentCommand = M_LOGIN;
    while (!LoginOk && (DetectedError == CLIER_NOERROR))
    {
        QTime tme;
        tme.start();
        while (tme.elapsed() < MAINSLEEP)
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

void Client::InitiateTimers()
{
    TimeoutTimer = new QTimer;
    TimeoutTimer->setInterval(MAINTIMEOUT);
    connect(TimeoutTimer,SIGNAL(timeout()),this,SLOT(Timeout()));
    GetComReplyTimer = new QTimer;
    GetComReplyTimer->setInterval(DATATIMEOUT); // таймер на получение данных, если за 3 сек ничего не принято, считаем, что посылка закончена, и можно её обрабатывать
    connect(GetComReplyTimer,SIGNAL(timeout()),this,SLOT(ComReplyTimeout()));
    GetFileTimer = new QTimer;
    GetFileTimer->setInterval(GETTIMEOUT); // таймер на получение файлов, если за 2 с ничего не принято, считаем, что файл окончен
    connect(GetFileTimer,SIGNAL(timeout()),this,SLOT(GetFileTimerTimeout()));
    RetrTimer = new QTimer;
    RetrTimer->setInterval(RetryTimePeriods[CurRetrPeriod]);
    connect(RetrTimer,SIGNAL(timeout()),this,SLOT(RetrTimeout()));
}

void Client::Disconnect()
{
    try
    {
        if (Connected)
        {
            SendCmd(M_QUIT);
            while ((Busy == true) && (DetectedError == CLIER_NOERROR))
            {
                QThread::msleep(MAINSLEEP);
                qApp->processEvents(QEventLoop::AllEvents);
            }
            MainEthernet->Disconnect();
            Connected = false;
        }
        if (MainEthernet != 0)
        {
            delete MainEthernet;
            MainEthernet = 0;
        }
    }
    catch(...)
    {
        ERMSG("Exception in Client::Disconnect()");
    }
}

void Client::SendCmd(int command, QStringList &args)
{
    CurrentCommand = command;
    NextActive = false;
    PrevLastBA.clear();
    if ((!Connected) || (TimeoutCounter > CL_MAXRETRCOUNT)) // if we're disconnected or there was max timouts count try to restart connection
    {
        Disconnect();
        LastArgs = args; // store command arguments for retrying
        LastCommand = command;
        RetrTimer->start();
        DetectedError = CLIER_CLOSED;
        return;
    }
    if (!RetryActive) // if this send is not a retry
        RetryCount = 0;
    if (Busy)
    {
        DetectedError = CLIER_BUSY;
        return;
    }
    if ((ClientMode == CLIMODE_TEST) && (command != M_ACTIVATE) && (command != M_ANSLOGIN) && (command != M_ANSPSW) && (command != M_QUIT))
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
    Busy = true;
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
        CommandString = tmps;// + "\n";
        ResultType = st.ResultType;
    }
    switch (command)
    {
/*    case CMD_MESSAGES:
    {
        break;
    }
    case CMD_CHATMSGS:
    {
        break;
    } */
    // ACTIVATE активировать учётную запись пользователя
    // 0 - код активации
    // 1 - новый пароль
    case M_ACTIVATE:
        break; // all is already included in CommandString
    // GETF получить файл с сервера
    // 0 - тип файла
    // 1 - подтип файла
    // 2 - имя файла
    // Формат: M8 <тип> <подтип> <имя_файла>\n
    // далее ожидание прихода "<длина_в_байтах>\n", затем приём содержимого файла блоками по 16384 байт, после каждого отсылка RDY\n
    case M_GETFILE:
    {
        bool ok;
        int fltype, flsubtype;
        fltype = args.at(0).toInt(&ok);
        if (ok)
            flsubtype = args.at(1).toInt(&ok);
        if ((!ok) || (fltype >= PathPrefixes.size()) || (flsubtype >= PathSuffixes.size()))
        {
            ERMSG("GETFILE: Ошибка в параметрах");
            DetectedError = CLIER_GETFER;
            return;
        }
        QDir *dr = new QDir;
        QString path = pc.HomeDir + "/" + PathPrefixes.at(fltype) + PathSuffixes.at(flsubtype);
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
        CliLog->info("> ...binary data " + QString::number(WrData.size()) + "size...");
        MainEthernet->WriteData(WrData);
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
        CommandString = Pers + TOKEN + Pass;
        break;
    }
    default:
        break;
    }
    if (command == M_ANSLOGIN)
        CliLog->info(">"+Pers);
    else
        CliLog->info(">"+CommandString); //+codec->fromUnicode(CommandString));
    QByteArray ba = CommandString.toUtf8();//codec->fromUnicode(CommandString));
    ComReplyTimeoutIsSet = false;
    MainEthernet->WriteData(ba);
    emit BytesWritten(ba.size());
}

// ############################################ ОБРАБОТКА ОТВЕТА ##############################################

void Client::ParseReply(QByteArray ba)
{
    emit BytesRead(ba.size());
#ifndef TIMERSOFF
    if (!ComReplyTimeoutIsSet)
        GetComReplyTimer->start(); // если не было таймаута, рестартуем таймер
#endif
    QString RcvDataString;
    CmdOk = false;
    RetryActive = false; // if there should be a retry, set it at SERVRETSTR processing
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
            return;
        }
        if (RcvDataString == SERVEMPSTR)
        {
            if (fp.isOpen())
                fp.remove(); // remove empty created in files.cpp file
            Error("Server empty response", CLIER_EMPTY);
            return;
        }
        if (RcvDataString == SERVRETSTR) // for file operations
        {
            CliLog->info("Server wants retry, trying retry");
            ++RetryCount;
            if (RetryCount > CL_MAXRETRCOUNT)
            {
                if (fp.isOpen())
                    fp.close();
                Error("Server timeout", CLIER_SERVER);
            }
            else
            {
                RetryActive = true;
                SendCmd(CurrentCommand, LastArgs);
            }
            return;
        }
        if (RcvDataString == SERVIDLSTR)
            CurrentCommand = M_IDLE;
        CliLog->info("<"+RcvDataString);
    }
    switch (CurrentCommand)
    {
    case M_LOGIN: // по сути установление соединения, должны получить запрос LOGIN
    {
        if (RcvDataString == "M5")
        {
            CmdOk = true;
            Busy = false;
            SendCmd(M_ANSLOGIN);
            return;
        }
        break;
    }
    case M_ANSLOGIN:
    {
        // если получили в ответ "GROUP <access>", значит, всё в порядке, иначе ошибка пароля
        if ((ClientMode == CLIMODE_TEST) && (RcvDataString == SERVEROK))
        {
            LoginOk = true;
            CmdOk = true;
            CurrentCommand = M_IDLE;
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
                    return;
                }
                CliLog->info("Group access: "+QString::number(pc.access));
                CmdOk = true;
                CurrentCommand = M_IDLE;
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
            return;
        }
        if (!NextActive)
        {
            QString tmps = QString::fromUtf8(RcvList.takeFirst());
            RcvDataSize = tmps.toInt(&ok); // количество байт
            if ((!ok) || (RcvDataSize < 0))
            {
                Error("Некорректное количество байт", CLIER_WRANSW);
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
                return;
            }
            CmdOk = true;
            TimeoutTimer->stop();
            CurrentCommand = M_IDLE;
            Busy = false;
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
            return;
            break;
        }
        if (RcvDataSize == 0) // последний фрагмент обработан
        {
            CmdOk = true;
            TimeoutTimer->stop();
            CurrentCommand = M_IDLE;
            Busy = false;
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
            return;
        }
    }
    case M_APUTFILE:
    {
//        emit BytesWritten(WrittenBytes);
        CliLog->info(QString::number(WrittenBytes)+" bytes written to file");
        qint64 BytesToSend = XmitDataSize - WrittenBytes;
        if (BytesToSend > READBUFMAX)
            BytesToSend = READBUFMAX;
        if (BytesToSend > 0)
        {
            if (fp.isOpen())
            {
                WrData = QByteArray(fp.read(BytesToSend));
                if (WrData.isEmpty())
                {
                    Error("Error while reading file", CLIER_PUTFER);
                    return;
                }
                WrittenBytes += WrData.size();
                Busy = false;
                SendCmd(M_APUTFILE);
#ifndef TIMERSOFF
                TimeoutTimer->start();
#endif
                return;
            }
            else
            {
                Error("File error", CLIER_PUTFER);
                return;
            }
        }
        else
        {
            if (fp.isOpen())
                fp.close();
            emit TransferComplete();
            CmdOk = true;
            break;
        }
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
            return;
        }
        emit BytesOverall(RcvDataSize);
        Busy = false;
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
        if (ComReplyTimeoutIsSet)
        {
            emit TransferComplete();
            QString tmpString = "ReadBytes = " + QString::number(ReadBytes);
            CliLog->info(tmpString);
            tmpString = "RcvDataSize = " + QString::number(RcvDataSize);
            CliLog->info(tmpString);
            if (fp.isOpen())
                fp.remove();
            Error("GetFile read timeout", CLIER_GETFTOUT);
            break;
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
            Busy = true;
            return;
        }
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

void Client::Error(QString ErMsg, int ErrorInt)
{
    CliLog->warning(ErMsg);
    DetectedError = ErrorInt;
    TimeoutTimer->stop();
    Busy = false;
    CurrentCommand = M_IDLE;
}

void Client::GetFileTimerTimeout()
{
    FileBusy = false;
    ++TimeoutCounter;
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
//    Disconnect(); // delete MainEthernet to try connect later if needed
}

void Client::ClientErr(int error)
{
    ERMSG(Ethernet::EthernetErrors()[error]);
    Busy = false;
    DetectedError = CLIER_GENERAL;
    TimeoutTimer->stop();
}

void Client::Timeout()
{
    ERMSG("Произошло превышение времени ожидания");
    Busy = false;
    FileBusy = false;
    DetectedError = CLIER_TIMEOUT;
    TimeoutTimer->stop();
    ++TimeoutCounter;
}

void Client::ComReplyTimeout()
{
    ComReplyTimeoutIsSet = true;
    QByteArray ba;
    ParseReply(ba); // принудительная обработка принятой посылки
    GetComReplyTimer->stop();
    ++TimeoutCounter;
}

// проверка аргументов

bool Client::CheckArgs(QString cmd, QStringList &args, int argsnum, bool fieldscheck, bool pairscheck)
{
    int pnum;
    if (args.size() < argsnum)
    {
        CliLog->error(cmd + ": Number of arguments is less than " + QString::number(argsnum));
        DetectedError = CLIER_WRARGS;
        Busy = false;
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
            Busy = false;
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
            Busy = false;
            return false;
        }
    }
    if (fieldscheck && pairscheck)
    {
        if (args.size() < FieldsNum+2*pnum+2) // +1 - db, table
        {
            CliLog->error(cmd + ": Number of fields is less than mentioned in header: "+QString::number(args.size())+" "+QString::number(FieldsNum+2*pnum+1));
            DetectedError = CLIER_WRARGS;
            Busy = false;
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
    while (Busy)
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
    while (Busy)
    {
        QThread::msleep(10);
        qApp->processEvents(QEventLoop::AllEvents);
    }
    return DetectedError;
}

int Client::SendAndGetResult(int command, QStringList &args)
{
    Cli->SendCmd(command, args);
    while (Cli->Busy)
    {
        QThread::msleep(10);
        qApp->processEvents(QEventLoop::AllEvents);
    }
    return DetectedError;
}

bool Client::isConnected()
{
    return Connected;
}

void Client::RetrTimeout()
{
    if (CurRetrPeriod < CL_MAXRETR)
        ++CurRetrPeriod;
    RetrTimer->setInterval(RetryTimePeriods[CurRetrPeriod]);
    RetrTimer->stop();
//    Disconnect();
    if (Connect(Host, Port, ClientMode) == CLIER_NOERROR) // if the connection was successful send previous command with previous arguments
        SendCmd(LastCommand, LastArgs);
    else // else try again later
        RetrTimer->start();
}
