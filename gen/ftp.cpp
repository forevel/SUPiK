#include <QThread>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QDateTime>

#include "ftp.h"

Ftp *Ftps;

Ftp::Ftp(QObject *parent) : QObject(parent)
{
}

Ftp::~Ftp()
{
}

bool Ftp::Connect(QString Host)
{
    LogFile = new QFile("ftp.log");
    if (!LogFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        FTPER("Невозможно создать log-файл");
        CanLog = false;
    }
    else
        CanLog = true;
    QString tmps = "Log started at " + QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
    if (CanLog)
        LogFile->write(tmps.toLocal8Bit());
    Busy = Connected = CmdOk = false;
    BufData = new QByteArray;
    TimeoutTimer = new QTimer;
    TimeoutTimer->setInterval(10000);
    connect(TimeoutTimer,SIGNAL(timeout()),this,SLOT(Timeout()));
    GetComReplyTimer = new QTimer;
    GetComReplyTimer->setInterval(200); // таймер на получение данных, если за 200 мс ничего не принято, считаем, что посылка закончена, и можно её обрабатывать
    connect(GetComReplyTimer,SIGNAL(timeout()),this,SLOT(ParseReply()));
    GetFileTimer = new QTimer;
    GetFileTimer->setInterval(500); // таймер на получение файлов, если за 500 мс ничего не принято, считаем, что файл окончен
    connect(GetFileTimer,SIGNAL(timeout()),this,SLOT(GetFileTimerTimeout()));
    MainEthernet = new Ethernet;
    QThread *thr = new QThread;
    MainEthernet->moveToThread(thr);
    connect(thr,SIGNAL(finished()),MainEthernet,SLOT(deleteLater()));
    connect(thr,SIGNAL(finished()),thr,SLOT(deleteLater()));
    connect(thr,SIGNAL(started()),MainEthernet,SLOT(Run()));
    connect(MainEthernet,SIGNAL(connected()),this,SLOT(FtpConnected()));
    connect(MainEthernet,SIGNAL(newdataarrived(QByteArray *)),this,SLOT(FtpGet(QByteArray *)));
    connect(this,SIGNAL(FtpSend(QByteArray *)),MainEthernet,SLOT(InitiateWriteDataToPort(QByteArray *)));
    MainEthernet->SetIpAndPort(Host, PORTFTP);
    TimeoutTimer->start();
    CurrentCommand = CMD_START;
    thr->start();
    Busy = true;
    while (Busy)
        qApp->processEvents(QEventLoop::AllEvents);
    if ((!Connected) || (!CmdOk))
        return false;
    return true;
}

void Ftp::StopThreads()
{
    Disconnect();
}

void Ftp::Disconnect()
{
    SendCmd(CMD_QUIT);
    MainEthernet->Stop();
    LogFile->close();
}

bool Ftp::SendCmd(int Command, QString Args)
{
    CurrentCommand = Command;
    Busy = true;
    TimeoutTimer->start();
    QString CommandString;
    switch (Command)
    {
    case CMD_USER:
    {
        CommandString = "USER " + Args + "\n";
        break;
    }
    case CMD_PASS:
    {
        CommandString = "PASS " + Args + "\n";
        break;
    }
    case CMD_PASV:
    {
        CommandString = "PASV \n";
        break;
    }
    case CMD_RETR:
    {
        CommandString = "RETR " + Args + "\n";
        break;
    }
    case CMD_STOR:
    {
        CommandString = "STOR " + Args + "\n";
        break;
    }
    case CMD_QUIT:
    {
        CommandString = "QUIT \n";
        break;
    }
    case CMD_CWD:
    {
        CommandString = "CWD " + Args + "\n";
        break;
    }
    case CMD_MKD:
    {
        CommandString = "MKD " + Args + "\n";
        break;
    }
    case CMD_LIST:
    {
        CommandString = "LIST \n";
        break;
    }
    }
    if (CanLog)
    {
        LogFile->write(QString("---CLIENT---\n").toLocal8Bit());
        LogFile->write(CommandString.toLocal8Bit());
    }
    QByteArray ba;
    ba.append(CommandString.toUtf8());
    emit FtpSend(&ba);
    while (Busy)
        qApp->processEvents(QEventLoop::AllEvents);
    TimeoutTimer->stop();
    if (!CmdOk)
        return false;
    return true;
}

bool Ftp::Login(QString User, QString Password)
{
    if (!SendCmd(CMD_USER, User))
    {
        FTPER("Неверное имя");
        return false;
    }
    if (!SendCmd(CMD_PASS, Password))
    {
        FTPER("Неверный пароль");
        return false;
    }
    return true;
}

bool Ftp::ChDir(QString Dir)
{
    if (!SendCmd(CMD_CWD, Dir))
    {
        FTPER("Что-то не так с именем каталога");
        return false;
    }
    return true;
}

bool Ftp::List()
{
    if (!StartPASV(CMD_LIST))
    {
        FTPER("Команда LIST не прошла");
        return false;
    }
    return true;
}

bool Ftp::MkDir(QString Dir)
{
    if (!SendCmd(CMD_MKD, Dir))
    {
        FTPER("Невозможно создать каталог "+Dir);
        return false;
    }
    return true;
}

bool Ftp::StartPASV(int Command, QString Filename, QByteArray *ba, int size)
{
    switch (Command)
    {
    case CMD_RETR:
    case CMD_LIST:
    {
        RcvData = ba;
        RcvDataSize = size;
        ReadBytes = 0;
        break;
    }
    case CMD_STOR:
    {
        XmitData = ba;
        XmitDataSize = size;
        WrittenBytes = 0;
        break;
    }
    default:
        break;
    }
    if (!SendCmd(CMD_PASV))
        return false;
    FileConnected = false;
    Ethernet *eth = new Ethernet;
    QThread *thr = new QThread;
    eth->moveToThread(thr);
    connect(thr,SIGNAL(finished()),eth,SLOT(deleteLater()));
    connect(thr,SIGNAL(finished()),thr,SLOT(deleteLater()));
    connect(thr,SIGNAL(started()),eth,SLOT(Run()));
    connect(eth,SIGNAL(connected()),this,SLOT(FtpFileConnected()));
    if (Command == CMD_STOR)
    {
        connect(eth,SIGNAL(byteswritten(qint64)),this,SLOT(SetBytesWritten(qint64)));
        connect(this,SIGNAL(FileSend(QByteArray *)),eth,SLOT(InitiateWriteDataToPort(QByteArray *)));
    }
    else
        connect(eth,SIGNAL(newdataarrived(QByteArray *)),this,SLOT(FileGet(QByteArray *)));
    eth->SetIpAndPort(FileHost, FilePort);
    TimeoutTimer->start();
    thr->start();
    FileBusy = true;
    while (FileBusy)
        qApp->processEvents(QEventLoop::AllEvents);
    if (!FileConnected)
    {
        eth->Stop();
        return false;
    }
    FileBusy = true;
    TimeoutTimer->start();
    if (!SendCmd(Command, Filename))
    {
        eth->Stop();
        return false;
    }
    if (Command == CMD_STOR)
    {
        QFile *file = new QFile(Filename);
        if (!file->open(QIODevice::WriteOnly))
        {
            FTPER("Ошибка открытия файла: "+Filename);
            return false;
        }
        QByteArray *tmpba = new QByteArray(file->readAll());
        emit FileSend(tmpba);
    }
    while (FileBusy)
        qApp->processEvents(QEventLoop::AllEvents);
    eth->Stop();
    return true;
}

bool Ftp::GetFile(QString Filename, QByteArray *ba, int size)
{
    QByteArray *tmpba = new QByteArray;
    if (!StartPASV(CMD_LIST, Filename, tmpba, size))
    {
        FTPER("Не прошла команда LIST");
        return false;
    }

    // в tmpba содержится вывод команды LIST в виде:
    // -rwxrwxr-x    1 1004       supik         6811190 Feb 22 22:00 [Cold_Cut]-Autumn_Leaves_(Irresistable_Force_Mix_Trip_2).mp3\n
    // drwxr-xr-x    2 1004       supik             512 Dec 18 22:45 incoming\n

    QStringList tmpsl = (QString::fromUtf8(tmpba->data())).split("\n");
    qint64 fsize;
    while (!tmpsl.isEmpty())
    {
        QString tmps = tmpsl.takeFirst();
        QStringList tmpsl = tmps.split(QRegularExpression("\\s+")); // используем любые разделители
        if (tmpsl.size() > 8) // подразумеваем вывод в Unix-стиле
        {
            tmps.clear();
            for (int i=8; i<tmpsl.size(); i++)
                tmps += tmpsl.at(i) + " ";
            tmps.remove(QRegularExpression("\\s+"));
            if (tmps == Filename)
            {
                fsize = tmpsl.at(4).toInt();
                break;
            }
        }
    }
    emit BytesOverall(fsize);
    return StartPASV(CMD_RETR, Filename, ba, size);
}

void Ftp::FtpFileConnected()
{
    FileBusy = false;
    FileConnected = true;
    TimeoutTimer->stop();
}

bool Ftp::SendFile(QString Filename, QByteArray *ba, int size)
{
    return StartPASV(CMD_STOR, Filename, ba, size);
}

void Ftp::SetBytesWritten(qint64 bytes)
{
    WrittenBytes += bytes;
    emit BytesWritten(WrittenBytes);
}

void Ftp::FtpGet(QByteArray *ba)
{
    BufData->append(*ba);
    if (CanLog)
    {
        LogFile->write(QString("---SERVER---\n").toLocal8Bit());
        LogFile->write(*BufData);
    }
    GetComReplyTimer->start();
}

void Ftp::FileGet(QByteArray *ba)
{
    int basize = ba->size();
    ReadBytes += basize;
    QString tmps = *ba;
    if (CanLog)
    {
        LogFile->write(QString("---FILE---\n").toLocal8Bit());
        LogFile->write(QString(tmps).toLocal8Bit());
    }
    emit BytesRead(ReadBytes);
    GetFileTimer->start();
    TimeoutTimer->stop();
    if (RcvData != 0)
    {
        int rcvsize = RcvData->size() + basize;
        if (rcvsize < RcvDataSize)
            RcvData->append(*ba);
    }
}

void Ftp::GetFileTimerTimeout()
{
    FileBusy = false;
}

void Ftp::ParseReply()
{
    CmdOk = false;
    GetComReplyTimer->stop();
    QString FtpResultString = QString::fromLocal8Bit(*BufData);
    BufData->clear();
    QStringList FtpResultStringList = FtpResultString.split("\n");
    if (FtpResultStringList.size() > 1)
        FtpResultString = FtpResultStringList.at(FtpResultStringList.size()-2);
    else
        FtpResultString = FtpResultStringList.last();
    QString FtpResultS = FtpResultString.left(3);
    bool ok;
    int FtpResult = FtpResultS.toInt(&ok);
    if (!ok);
    else
    {
        switch (CurrentCommand)
        {
        case CMD_START: // по сути установление соединения и анализ приветствия сервера
        {
            if (FtpResult == FTP_FTPREADY)
                CmdOk = true;
            break;
        }
        case CMD_USER:
        {
            if ((FtpResult == FTP_USEROK) || (FtpResult == FTP_AUTH_NEED) || (FtpResult == FTP_USEROK_PSW))
                CmdOk = true;
            break;
        }
        case CMD_PASS:
        {
            if (FtpResult == FTP_USEROK)
                CmdOk = true;
            break;
        }
        case CMD_PASV:
        {
            if (FtpResult == FTP_PASV_ENAB)
            {
                CmdOk = true;
                int tmpi1 = FtpResultString.indexOf("(");
                if (tmpi1 == -1)
                    CmdOk = false;
                else
                {
                    int tmpi2 = FtpResultString.indexOf(")");
                    if ((tmpi2 == -1) || (tmpi2 <= tmpi1))
                        CmdOk = false;
                    else
                    {
                        QStringList tmpsl = FtpResultString.mid(tmpi1+1, (tmpi2-tmpi1-1)).split(","); // вытаскиваем host и port
                        if (tmpsl.size() < 6)
                            CmdOk = false;
                        else
                        {
                            FileHost = tmpsl.at(0)+"."+tmpsl.at(1)+"."+tmpsl.at(2)+"."+tmpsl.at(3);
                            QString tmps = tmpsl.at(4);
                            quint16 tmpqi = tmps.toInt(&ok);
                            if (!ok)
                                CmdOk = false;
                            else
                            {
                                FilePort = tmpqi * 256;
                                tmps = tmpsl.at(5);
                                tmpqi = tmps.toInt(&ok);
                                if (!ok)
                                    CmdOk = false;
                                else
                                    FilePort += tmpqi;
                            }
                        }
                    }
                }
            }
            break;
        }
        case CMD_RETR:
        case CMD_STOR:
        {
            if (FtpResult == FTP_FILESTATOK)
                return;
            if (FtpResult == FTP_CHANCLOSED)
                CmdOk = true;
            FileBusy = false;
            break;
        }
        case CMD_CWD:
        {
            if (FtpResult == FTP_QUERYOK)
                CmdOk = true;
            break;
        }
        case CMD_MKD:
        {
            if (FtpResult == FTP_PATH_CREAT)
                CmdOk = true;
            break;
        }
        case CMD_QUIT:
        {
            if (FtpResult == FTP_QUITOK)
                CmdOk = true;
            break;
        }
        case CMD_LIST:
        {
            if (FtpResult == FTP_CHANCLOSED)
                CmdOk = true;
            else
                CmdOk = false;
            break;
        }
        }
    }
    Busy = false;
}

void Ftp::FtpConnected()
{
    Connected = true;
    TimeoutTimer->stop();
}

void Ftp::FtpErr(int error)
{
    FTPER(Ethernet::EthernetErrors()[error]);
    Busy = false;
    TimeoutTimer->stop();
}

void Ftp::Timeout()
{
    FTPER("Произошло превышение времени ожидания");
    Busy = false;
    FileBusy = false;
    TimeoutTimer->stop();
}
