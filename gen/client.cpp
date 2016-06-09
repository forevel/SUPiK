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
#ifndef TIMERSOFF
    TimeoutTimer->start();
#endif
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
        Result.clear();
        if (Args.size() < 2)
        {
            CliLog->error("CMD_TF_GVSBFS: Number of arguments is less than 2");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        bool ok;
        QString fieldsnum = Args.takeAt(0);
        GVSBFS_FieldsNum = fieldsnum.toInt(&ok);
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
        if (Args.size() < GVSBFS_FieldsNum+2*pnum+1) // +1 - table
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
    case CMD_GVSBFS:
    case CMD_GVBFS:
    {
        Result.clear(); // очищаем результаты
        if (Args.size() < 2)
        {
            CliLog->error("CMD_GVSBFS: Number of arguments is less than 2");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString fieldsnum = Args.takeAt(0);
        bool ok;
        GVSBFS_FieldsNum = fieldsnum.toInt(&ok);
        if (!ok)
        {
            CliLog->error("CMD_GVSBFS: argument is not a number");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString pairsnum = Args.takeAt(0);
        int pnum = pairsnum.toInt(&ok);
        if (!ok)
        {
            CliLog->error("CMD_GVSBFS: argument is not a number");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        if (Args.size() < GVSBFS_FieldsNum+2*pnum+2) // +1 - db, table
        {
            CliLog->error("CMD_GVSBFS: Number of fields is less than mentioned in header: "+QString::number(Args.size())+" "+QString::number(GVSBFS_FieldsNum+2*pnum+1));
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString tmps = Args.join(" ");
        if (Command == CMD_GVBFS)
            CommandString = "GVBFS ";
        else
            CommandString = "GVSBFS ";
        CommandString.append(fieldsnum + " " + pairsnum + " " + tmps + "\n");
        // 2 = два пробела
        break;
    }
        // запрос всех значений по одному полю
        // формат запроса: GVSBC db tble column [order_header] [ASC|DESC]\n
        // формат ответа: <value1> <value2> ... <valuen>\n
    case CMD_GVSBC:
    {
        Result.clear(); // очищаем результаты
        if (Args.size() < 3)
        {
            CliLog->error("CMD_GVSBC: Number of arguments is less than 3");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        GVSBFS_FieldsNum = 1; // одна колонка - одно поле в каждой записи
        QString tmps = Args.join(" ");
        CommandString = "GVSBC ";
        CommandString.append(tmps + "\n");
        break;
    }
    case CMD_GVSBCF:
    {
        Result.clear();
        if (Args.size() < 5)
        {
            CliLog->error("CMD_GVSBCF: Number of arguments is less than 5");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        GVSBFS_FieldsNum = 1; // одна колонка - одно поле в каждой записи
        QString tmps = Args.join(" ");
        CommandString = "GVSBCF ";
        CommandString.append(tmps + "\n");
        break;
    }
        // запрос колонок (полей) из таблицы
        // формат запроса: GCS db tble\n
        // формат ответа: <column1> <column2> ... <columnn>\n
    case CMD_GCS:
    {
        Result.clear(); // очищаем результаты
        if (Args.size() < 2)
        {
            CliLog->error("CMD_GCS: Number of arguments is less than 2");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString tmps = Args.join(" ");
        CommandString = "GCS ";
        CommandString.append(tmps + "\n");
        break;
    }
    case CMD_SQLTC:
    {
        if (Args.size() < 3)
        {
            CliLog->error("CMD_SQLTC: Number of arguments is less than 3");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString tmps = Args.join(" ");
        CommandString = "SQLTC ";
        CommandString.append(tmps + "\n");
        break;
    }
    case CMD_SQLTA:
    {
        QString fieldsnum = Args.takeAt(0);
        bool ok;
        GVSBFS_FieldsNum = fieldsnum.toInt(&ok);
        if (!ok)
        {
            CliLog->error("CMD_SQLTA: argument is not a number");
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        if (Args.size() < GVSBFS_FieldsNum+2) // +2 - db,table
        {
            CliLog->error("CMD_SQLTA: Number of fields is less than mentioned in header: "+QString::number(Args.size())+" "+QString::number(GVSBFS_FieldsNum+2));
            DetectedError = CLIER_WRARGS;
            Busy = false;
            return;
        }
        QString tmps = Args.join(" ");
        CommandString = "SQLTA ";
        CommandString.append(fieldsnum + " " + tmps + "\n");
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
    case ANS_GVSBFS:
    case ANS_GVSBC:
    case ANS_GVSBCF:
    case ANS_GCS:
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
        CliLog->info(">"+CommandString); //+codec->fromUnicode(CommandString));
    QByteArray *ba = new QByteArray(CommandString.toUtf8());//codec->fromUnicode(CommandString));
    emit ClientSend(ba);
}

void Client::ParseReply(QByteArray *ba)
{
#ifndef TIMERSOFF
    if (!ComReplyTimeoutIsSet)
        GetComReplyTimer->start(); // если не было таймаута, рестартуем таймер
#endif
    QString ServerResponse, IncomingString;
    QStringList ArgList;
    CmdOk = false;
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
                break;
            FirstReplyPass = false;
            return;
        }
        case false:
        {
            RcvData.append(codec->fromUnicode(*ba));
            if (RcvData.right(1) == "\n") // очередная посылка закончена, надо передать её на обработку
                break;
            return;
        }
        }
        FirstReplyPass = true;
        IncomingString = QString::fromLocal8Bit(RcvData);
        CliLog->info("<"+IncomingString);
        ArgList = SeparateBuf(RcvData);
        if (ArgList.isEmpty()) // ничего толкового не получено
            return;
        QString tmps = ArgList.last(); // убираем последний перевод строки
        if (tmps.right(1) == "\n")
            tmps.chop(1);
        ArgList.replace(ArgList.size()-1, tmps);
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
            CurrentCommand = CMD_IDLE;
    }
    switch (CurrentCommand)
    {
    case CMD_LOGINREQ: // по сути установление соединения, должны получить запрос LOGIN
    {
        if (ServerResponse == "LOGIN")
        {
            CmdOk = true;
            SendCmd(ANS_LOGIN);
            return;
        }
        break;
    }
    case ANS_LOGIN:
    {
        if (ServerResponse == "PSW")
        {
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
            CurrentCommand = CMD_IDLE;
        }
        else
            DetectedError = CLIER_PSW;
        break;
    }
    case CMD_QUIT:
    {
        if (ServerResponse == "BYE")
            CmdOk = true;
        break;
    }
    case CMD_SQLTC:
    case CMD_SQLTA:
    case CMD_SQLTD:
        if (ServerResponse == "OK")
            CmdOk = true;
        break;
    case CMD_TF_GVSBFS:
    {
        // Формат ответа на запрос из таблицы tablefields:
        // 1. Ответ на GVSBFS:
        //      TF <number_of_records>\n
        //      value[0][0] value[1][0] ... value[n][0]\n
        //      value[0][1] value[1][1] ... value[n][1]\n
        //      ...
        //      value[0][k] value[1][k] ... value[n][k]\n

        break;
    }
    case CMD_GVSBC:
    case CMD_GVSBCF:
    case CMD_GVSBFS:
    {
        // Формат ответа на запрос GVSBFS:
        //      GVSBFS <number_of_records>\n
        //      value[0][0] value[1][0] ... value[n][0]\n
        //      value[0][1] value[1][1] ... value[n][1]\n
        //      ...
        //      value[0][k] value[1][k] ... value[n][k]\n

        if ((ServerResponse == "GVSBFS") || (ServerResponse == "GVSBC") || (ServerResponse == "GVSBCF"))
        {
            if (ArgList.size()<2) // нет количества записей
            {
                WriteErrorAndBreakReceiving("Некорректное количество аргументов");
                return;
            }
            bool ok;
            MsgNum = ArgList.at(1).toInt(&ok);
            if ((!ok) || (MsgNum < 0))
            {
                WriteErrorAndBreakReceiving("Некорректное количество посылок");
                return;
            }
            if (MsgNum == 0)
                CliLog->info("Пустой ответ");
#ifndef TIMERSOFF
            TimeoutTimer->start();
#endif
            if (CurrentCommand == CMD_GVSBFS)
                SendCmd(ANS_GVSBFS);
            else if (CurrentCommand == CMD_GVSBC)
                SendCmd(ANS_GVSBC);
            else if (CurrentCommand == CMD_GVSBCF)
                SendCmd(ANS_GVSBCF);
            return;
        }
        else
        {
            WriteErrorAndBreakReceiving("Некорректный ответ сервера");
            return;
        }
        break;
    }
    case CMD_GCS:
    {
        // Формат ответа на запрос GCS:
        //      GCS <number_of_fields>\n
        //      column[0] colun[1] ... colun[n]\n
        if ((ServerResponse == "GCS"))
        {
            if (ArgList.size()<2) // нет количества записей
            {
                WriteErrorAndBreakReceiving("Некорректное количество аргументов");
                return;
            }
            bool ok;
            GVSBFS_FieldsNum = ArgList.at(1).toInt(&ok);
            if ((!ok) || (GVSBFS_FieldsNum < 0))
            {
                WriteErrorAndBreakReceiving("Некорректное количество посылок");
                return;
            }
            if (GVSBFS_FieldsNum == 0)
                CliLog->info("Пустой ответ");
#ifndef TIMERSOFF
            TimeoutTimer->start();
#endif
            SendCmd(ANS_GCS);
            return;
        }
        else
        {
            WriteErrorAndBreakReceiving("Некорректный ответ сервера");
            return;
        }
        break;
    }
    case CMD_GVBFS:
        MsgNum = 1; // планируется приём одной записи
    case ANS_GVSBFS:
    {
        if (MsgNum == 0) // конец передачи, пришёл IDLE
        {
            CmdOk = true;
            break;
        }
        while ((MsgNum) && (ArgList.size()))
        {
            if (ArgList.last() == "\n")
                ArgList.removeLast();
            if (ArgList.size() < GVSBFS_FieldsNum)
            {
                   CliLog->warning("Некратное число записей в SQL-ответе");
                   MsgNum = 0;
                   ArgList.clear();
                   CmdOk = true;
                   break;
            }
            QStringList sl;
            for (int i=0; i<GVSBFS_FieldsNum; i++)
                sl.append(ArgList.takeFirst());
            Result.append(sl);
            MsgNum--;
        }
        if (MsgNum == 0) // кончились ответы, можно выходить
        {
            CmdOk = true;
            break;
        }
#ifndef TIMERSOFF
        TimeoutTimer->start();
#endif
        SendCmd(ANS_GVSBFS); // для GVSBC ответ тоже будет RDY
        return;
        break;
    }
        // Get Values By Column
    case ANS_GVSBC:
    case ANS_GVSBCF:
    {
        if (MsgNum == 0) // конец передачи, пришёл IDLE
        {
            CmdOk = true;
            break;
        }
        const int SLNum = GVSBFS_FieldsNum;
        if (SLNum > SLNUMMAX)
        {
            WriteErrorAndBreakReceiving("Too many columns to get: "+QString::number(SLNum));
            break;
        }
        QStringList sl[SLNUMMAX];
        if (!Result.isEmpty())
        {
            for (int i=0; i<SLNum; i++)
                sl[i] = Result.at(i);
        }
        while ((MsgNum) && (ArgList.size()))
        {
            if (ArgList.last() == "\n")
                ArgList.removeLast();
            if (ArgList.size() < SLNum)
            {
                   CliLog->warning("Некратное число записей в SQL-ответе");
                   MsgNum = 0;
                   ArgList.clear();
                   CmdOk = true;
                   break;
            }
            for (int i=0; i<SLNum; i++)
                sl[i].append(ArgList.takeFirst());
            MsgNum--;
        }
        Result.clear();
        for (int i=0; i<SLNum; i++)
            Result.append(sl[i]);
        if (MsgNum == 0) // кончились ответы, можно выходить
        {
            CmdOk = true;
            break;
        }
#ifndef TIMERSOFF
        TimeoutTimer->start();
#endif
        SendCmd(CurrentCommand); // для GVSBC ответ тоже будет RDY
        return;
        break;
    }
    case ANS_GCS:
    {
        if (GVSBFS_FieldsNum == 0) // конец передачи, пришёл IDLE
        {
            CmdOk = true;
            break;
        }
        QStringList sl;
        if (!Result.isEmpty())
            sl = Result.takeFirst();
        if (ArgList.last() == "\n")
            ArgList.removeLast();
        while ((GVSBFS_FieldsNum) && (ArgList.size()))
        {
            sl.append(ArgList.takeFirst());
            GVSBFS_FieldsNum--;
        }
        Result.clear();
        Result.append(sl);
        if (GVSBFS_FieldsNum == 0) // кончились ответы, можно выходить
        {
            CmdOk = true;
            break;
        }
#ifndef TIMERSOFF
        TimeoutTimer->start();
#endif
        SendCmd(CurrentCommand);
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
    CurrentCommand = CMD_IDLE;
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
    QStringList retvect, tmpsl;
    QString tmps,tmps2;
    int i;
    bool QuotesEnabled = false;

    tmpsl = QString::fromLocal8Bit(buf).split(" ");
    tmps2.clear();
    for (i=0; i<tmpsl.size(); i++)
    {
        tmps2 = tmpsl.at(i);
        if (tmps2.left(1) == "\"") // начало блока в кавычках
        {
            tmps2.remove(0,1); // убираем первую кавычку
            QuotesEnabled = true;
        }
        if (tmps2.right(1) == "\"") // конец блока в кавычках
        {
            tmps2.chop(1); // убираем последнюю кавычку
            QuotesEnabled = false;
        }
        tmps.append(tmps2);
        if (!QuotesEnabled)
        {
            retvect.append(tmps);
            tmps.clear();
        }
        else
          tmps.append(" ");
    }
    return retvect;
}
