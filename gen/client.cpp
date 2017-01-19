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
    Connected = false;
    FirstReplyPass = true;
    ComReplyTimeoutIsSet = false;
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
    CmdMap.insert(T_GVSBCF, {"T_GVSBCF", 5, "T3", RESULT_VECTOR, false, false});
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
    CmdMap.insert(T_UPDV, {"T_UPDV", 3, "TC", RESULT_NONE, false, false});
    CmdMap.insert(M_PUTFILE, {"M_PUTFILE", 4, "M7", RESULT_NONE, false, false}); // 0 - local filename, 1 - type, 2 - subtype, 3 - filename, [4] - filesize, added in SendCmd
    CmdMap.insert(M_GETFILE, {"M_GETFILE", 3, "M8", RESULT_NONE, false, false}); // 0 - type, 1 - subtype, 2 - filename
    CmdMap.insert(M_ACTIVATE, {"M_ACTIVATE", 2, "M9", RESULT_STRING, false, false}); // 0 - code, 1 - newpass
    CliLog = new Log;
    CliLog->Init(pc.HomeDir+"/cli.log");
    CliLog->info("=== Log started ===");
}

Client::~Client()
{
}

int Client::Connect(QString Host, QString Port, int ClientMode)
{
    if (Connected) // если уже подсоединены, не надо по второму разу
        return CLIER_NOERROR;
    CliMode = ClientMode;
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
    TimeoutTimer = new QTimer;
    TimeoutTimer->setInterval(MAINTIMEOUT);
    connect(TimeoutTimer,SIGNAL(timeout()),this,SLOT(Timeout()));
    GetComReplyTimer = new QTimer;
    GetComReplyTimer->setInterval(DATATIMEOUT); // таймер на получение данных, если за 3 сек ничего не принято, считаем, что посылка закончена, и можно её обрабатывать
    connect(GetComReplyTimer,SIGNAL(timeout()),this,SLOT(ComReplyTimeout()));
    GetFileTimer = new QTimer;
    GetFileTimer->setInterval(GETTIMEOUT); // таймер на получение файлов, если за 2 с ничего не принято, считаем, что файл окончен
    connect(GetFileTimer,SIGNAL(timeout()),this,SLOT(GetFileTimerTimeout()));
    MainEthernet = new Ethernet(Host, Port.toInt(), Ethernet::ETH_SSL);
    QThread *thr = new QThread;
    MainEthernet->moveToThread(thr);
    MainEthernet->sslsock->moveToThread(thr);
    connect(thr,SIGNAL(finished()),MainEthernet,SLOT(deleteLater()));
    connect(thr,SIGNAL(finished()),thr,SLOT(deleteLater()));
    connect(thr,SIGNAL(started()),MainEthernet,SLOT(Run()));
    connect(MainEthernet, SIGNAL(finished()),thr, SLOT(quit()));
    connect(MainEthernet,SIGNAL(connected()),this,SLOT(ClientConnected()));
    connect(MainEthernet,SIGNAL(disconnected()),this,SLOT(ClientDisconnected()));
    connect(MainEthernet,SIGNAL(newdataarrived(QByteArray)),this,SLOT(ParseReply(QByteArray)));
    connect(this,SIGNAL(ClientSend(QByteArray)),MainEthernet,SLOT(InitiateWriteDataToPort(QByteArray)));
#ifndef TIMERSOFF
    TimeoutTimer->start();
#endif
    DetectedError = CLIER_NOERROR;
    LoginOk = false;
    CurrentCommand = M_LOGIN;
    thr->start();
    while (!LoginOk && (DetectedError == CLIER_NOERROR))
    {
        QThread::msleep(MAINSLEEP);
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
    if (Connected)
    {
        SendCmd(M_QUIT);
        while ((Busy == true) && (DetectedError == CLIER_NOERROR))
        {
            QThread::msleep(MAINSLEEP);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        MainEthernet->Busy = false;
        MainEthernet->Stop();
        Connected = false;
    }
    // wait 1 second to let the ethernet thread finish
    QThread::sleep(1);
}

void Client::SendCmd(int Command, QStringList &Args)
{
    if (Busy)
    {
        DetectedError = CLIER_BUSY;
        return;
    }
    if ((CliMode == CLIMODE_TEST) && (Command != M_ACTIVATE) && (Command != M_ANSLOGIN) && (Command != M_ANSPSW) && (Command != M_QUIT))
    {
        CliLog->warning("illegal test command");
        DetectedError = CLIER_CMDER;
        return;
    }
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

    if (CmdMap.keys().contains(Command))
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
        fltype = Args.at(0).toInt(&ok);
        if (ok)
            flsubtype = Args.at(1).toInt(&ok);
        if ((!ok) || (fltype >= PathPrefixes.size()) || (flsubtype >= PathSuffixes.size()))
        {
            ERMSG("GETFILE: Ошибка в параметрах");
            DetectedError = CLIER_GETFER;
            return;
        }
        QDir *dr = new QDir;
        dr->mkpath(pc.HomeDir+PathPrefixes.at(fltype)+PathSuffixes.at(flsubtype));
        delete dr;
        fp.setFileName(pc.HomeDir + PathPrefixes.at(fltype) + PathSuffixes.at(flsubtype) + Args.at(2));
        if (!fp.open(QIODevice::WriteOnly))
        {
            ERMSG("Невозможно создать файл" + Args.at(2));
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
        fp.setFileName(Args.at(0));
        if (!fp.open(QIODevice::ReadOnly))
        {
            ERMSG("Невозможно открыть файл "+Args.at(0));
            DetectedError = CLIER_PUTFER;
            return;
        }
        WrittenBytes = 0;
        XmitDataSize = fp.size();
        emit BytesOverall(XmitDataSize);
        CommandString.chop(1); // deletes CRLF character
        CommandString.push_back(TOKEN);
        CommandString += QString::number(XmitDataSize) + "\n"; // file length added
        break;
    }
    case M_APUTFILE:
    {
        CliLog->info("> ...binary data...");
        MainEthernet->Busy = true;
        emit ClientSend(WrData);
        while (MainEthernet->Busy)
            qApp->processEvents(QEventLoop::AllEvents, 10);
//        delete WrData;
        return;
    }
        // ANS_GETFILE - второй и последующие ответы на принятую информацию (с записью блока в файл)
    case M_AGETFILE:
    case M_NEXT:
    {
        CommandString = "RDY\n";
        break;
    }
    case M_QUIT:
    {
        CommandString = "M1\n";
        break;
    }
    case M_ANSLOGIN:
    {
        CommandString = Pers+"\n";
        break;
    }
    case M_ANSPSW:
    {
        CommandString = Pass+"\n";
        break;
    }
    default:
        break;
    }
    if (Command == M_ANSPSW)
        CliLog->info(">********");
    else
        CliLog->info(">"+CommandString); //+codec->fromUnicode(CommandString));
    QByteArray ba = CommandString.toUtf8();//codec->fromUnicode(CommandString));
    ComReplyTimeoutIsSet = false;
    MainEthernet->Busy = true;
    emit ClientSend(ba);
    while ((MainEthernet->Busy) && (DetectedError == CLIER_NOERROR))
        qApp->processEvents(QEventLoop::AllEvents, 10);
//    delete ba;
}

// ############################################ ОБРАБОТКА ОТВЕТА ##############################################

void Client::ParseReply(QByteArray ba)
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
    if (CurrentCommand != M_AGETFILE) // приём файла обрабатывается по-другому
    {
        switch (FirstReplyPass)
        {
        case true:
        {
            RcvData.clear();
            RcvData.append(codec->fromUnicode(ba));
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
            RcvData.append(codec->fromUnicode(ba));
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
        if (ArgList.isEmpty()) // ничего толкового не получено
            return;
        ServerResponse = ArgList.first();
        if (ServerResponse == SERVERRSTR)
        {
            CliLog->error("Server error response");
            DetectedError = CLIER_SERVER;
            Busy = false;
            TimeoutTimer->stop();
            if (fp.isOpen())
                fp.close();
            return;
        }
        if (ServerResponse == SERVEMPSTR)
        {
            CliLog->info("Server empty response");
            DetectedError = CLIER_EMPTY;
            Busy = false;
            TimeoutTimer->stop();
            if (fp.isOpen())
                fp.remove();
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
            Busy = false;
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
            Busy = false;
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
        if ((CliMode == CLIMODE_TEST) && (ServerResponse == SERVEROK))
        {
            LoginOk = true;
            CmdOk = true;
            CurrentCommand = M_IDLE;
        }
        else
        {
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
                    Busy = false;
                    return;
                }
                CliLog->info("Group access: "+QString::number(pc.access));
                CmdOk = true;
                CurrentCommand = M_IDLE;
            }
            else
                DetectedError = CLIER_PSW;
        }
        break;
    }
    case M_QUIT:
    {
        if (ServerResponse == "BYE")
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
        if (ServerResponse == "OK")
            CmdOk = true;
        else
        {
            DetectedError = CLIER_CMDER;
            return;
        }
        break;
    // commands with int reply
    case S_GID:
    case S_INS:
    case T_GID:
    case T_INS:
    {
        bool ok;
        if (ArgList.size()<1) // нет количества записей
        {
            WriteErrorAndBreakReceiving("Некорректное количество аргументов");
            DetectedError = CLIER_WRANSW;
            Busy = false;
            return;
        }
        ResultInt = ArgList.at(0).toInt(&ok);
        if (!ok)
        {
            CliLog->warning("It's not possible to convert to integer: "+ArgList.at(0));
            DetectedError = CLIER_WRANSW;
            Busy = false;
            return;
        }
        CmdOk = true;
        break;
    }
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
    {
        // Формат ответа на запрос:
        //      <number_of_records>\n
        //      value[0][0] value[1][0] ... value[n][0] value[0][1] value[1][1] ... value[n][1] ... value[0][k] value[1][k] ... value[n][k]\n

        if (ArgList.size()<1) // нет количества записей
        {
            WriteErrorAndBreakReceiving("Некорректное количество аргументов");
            DetectedError = CLIER_WRANSW;
            Busy = false;
            return;
        }
        bool ok;
        MsgNum = ArgList.at(0).toInt(&ok);
        if ((!ok) || (MsgNum < 0))
        {
            WriteErrorAndBreakReceiving("Некорректное количество посылок");
            DetectedError = CLIER_WRANSW;
            Busy = false;
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
        Busy = false;
        SendCmd(M_NEXT);
        return;
        break;
    }
    // commands with string reply
    case M_ACTIVATE:
    case T_TV:
    case T_TID:
    case T_VTID:
    case T_C:
    {
        if (ArgList.size()<1) // нет количества записей
        {
            WriteErrorAndBreakReceiving("Некорректное количество аргументов");
            DetectedError = CLIER_WRANSW;
            return;
        }
        ResultStr = ArgList.at(0);
        CmdOk = true;
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
        Busy = false;
        SendCmd(M_NEXT);
        return;
        break;
    }
    case M_PUTFILE:
    case M_APUTFILE:
    {
        if (ServerResponse == "OK")
        {
            emit BytesWritten(WrittenBytes);
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
//                        delete WrData;
                        DetectedError = CLIER_PUTFER;
                        Busy = false;
                        return;
                    }
                    WrittenBytes += WrData.size();
                    Busy = false;
                    SendCmd(M_APUTFILE);
                    return;
                }
                else
                {
                    DetectedError = CLIER_PUTFER;
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
        }
        else
        {
            DetectedError = CLIER_PUTFER;
            Busy = false;
            return;
        }
        break;
    }
        // первый приём после команды
    case M_GETFILE:
    {
        CmdOk = true;
        bool ok;
        RcvDataSize = ServerResponse.toLong(&ok,10);
        if (!ok)
        {
            CliLog->warning("Not a decimal value detected");
            DetectedError = CLIER_GETFER;
            if (fp.isOpen())
                fp.remove();
            Busy = false;
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
            CliLog->error("File error");
            DetectedError = CLIER_GETFER;
            Busy = false;
            return;
        }
        if (ComReplyTimeoutIsSet)
        {
            emit TransferComplete();
            CliLog->error("GetFile read timeout");
            QString tmpString = "ReadBytes = " + QString::number(ReadBytes);
            CliLog->info(tmpString);
            tmpString = "RcvDataSize = " + QString::number(RcvDataSize);
            CliLog->info(tmpString);
            DetectedError = CLIER_GETFTOUT;
            Busy = false;
            TimeoutTimer->stop();
            if (fp.isOpen())
                fp.remove();
            break;
        }
        CliLog->info("< ...binary data "+QString::number(ba.size())+" size...");
        if (ba.data() == SERVERRSTR)
        {
            CliLog->error("Server error response");
            DetectedError = CLIER_SERVER;
            Busy = false;
            TimeoutTimer->stop();
            if (fp.isOpen())
                fp.remove();
            Busy = false;
            return;
        }
        if (ba.data() == "IDLE\n") // закончили передачу
        {
            if (fp.isOpen())
                fp.close();
            emit TransferComplete();
            CmdOk = true;
            break;
        }
        ReadBytes += ba.size();
        qint64 rb = fp.write(ba);
        emit BytesRead(ReadBytes);
        CliLog->info(QString::number(rb)+" bytes written to file");
        if (ReadBytes >= RcvDataSize)
        {
            if (fp.isOpen())
                fp.close();
            emit TransferComplete();
            CmdOk = true;
            Busy = false;
            SendCmd(M_AGETFILE);
            break;
        }
        Busy = false;
        SendCmd(M_AGETFILE);
        return;
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
}

void Client::ComReplyTimeout()
{
    ComReplyTimeoutIsSet = true;
    QByteArray ba;
    ParseReply(ba); // принудительная обработка принятой посылки
    GetComReplyTimer->stop();
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
            ERMSG("DBG: Fieldscheck");
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
            ERMSG("DBG: Pairscheck");
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

int Client::GetFile(const QString &type, const QString &subtype, const QString &filename)
{
    QStringList sl;
    sl.clear();
    sl << type << subtype << filename;
    Cli->SendCmd(M_GETFILE, sl);
    while (Cli->Busy)
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
    Cli->SendCmd(M_PUTFILE, sl);
    while (Cli->Busy)
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
