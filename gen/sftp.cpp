#include <QApplication>
#include <QFile>
#include <QNetworkConfiguration>
#include <QNetworkSession>

#include "sftp.h"

s_ftp sftp;

s_ftp::s_ftp(QObject *parent) : QObject(parent)
{
    Busy = false;
    Connected = false;
    ReadData.resize(65535);
    ftp = 0;
    Tmr = new QTimer;
    Tmr->setInterval(5000); // 5 секунд, чтобы получить ответ от сервера
    connect(Tmr,SIGNAL(timeout()),this,SLOT(FtpTimeout()));
}

s_ftp::~s_ftp()
{
    FtpDisconnect();
}

// Установка связи и проверка соединения с FTP-сервером

bool s_ftp::CheckFtp()
{
    QString CheckUrl = QString(CHECKFILE);
    GetData(CheckUrl);
    while (Busy)
        qApp->processEvents(QEventLoop::AllEvents);
    if (QString(ReadData.data()) == "supik\n")
        return true;
    return false;
}

// установка соединения с FTP-сервером и определение необходимых связок сигнал-слот

void s_ftp::ConnectToFtp()
{
    Tmr->start();
    ftp = new QFtp(this);
    connect(ftp,SIGNAL(commandFinished(int,bool)),this,SLOT(TransferFinished(int, bool)));
    connect(ftp, SIGNAL(readyRead()), this, SLOT(ReadDataFromUrl()));
    connect(ftp,SIGNAL(dataTransferProgress(qint64,qint64)),this,SLOT(SetRangeAndValue(qint64,qint64)));
    ftp->connectToHost(QString(FTP_SERVER), 21);
    ftp->login("supik","wdbpy(WcTtTZzA_TEc-<");
    Tmr->stop();
}

// получить данные из удалённого файла по ссылке Url в буфер ReadData

void s_ftp::GetData(QString Url)
{
    Busy = true;
    RangeWasSent = false;
    ReadData.clear();
    RDptr=0;
    if (!Connected)
        ConnectToFtp();

    ftp->get(Url);
    ftp->close();
}

// получить данные из удалённого файла по ссылке Url в локальный файл с именем filename

void s_ftp::GetFile(QString Url, QString filename)
{
    Busy = true;
    RangeWasSent = false;
    if (!Connected)
        ConnectToFtp();
    QFile *data = new QFile(filename, this);
    if (data->open(QIODevice::WriteOnly))
    {
        ftp->get(Url, data);
        ftp->close();
    }
    else
    {
        SFTPER("Ошибка открытия файла "+filename);
        return;
    }
}

// отправить данные из локального файла с именем filename в удалённый файл по ссылке Url

void s_ftp::PutFile(QString Url, QString filename)
{
    Busy = true;
    RangeWasSent = false;

    if (!Connected)
        ConnectToFtp();
    QFile *data = new QFile(filename, this);
    if (data->open(QIODevice::ReadOnly))
    {
        CdFailed = false;
        ftp->cd(Url);
        ftp->close();
        while (Busy)
            qApp->processEvents(QEventLoop::AllEvents);
        if (CdFailed) // нет такого каталога на сервере
        {
            ftp->mkdir(Url);
            ftp->close();
        }
        ftp->put(data, Url);
        ftp->close();
    }
    else
    {
        SFTPER("Ошибка открытия файла "+filename);
        return;
    }
}

// создать каталог на удалённом сервере

void s_ftp::MakeDir(QString DirName)
{
    Busy = true;
    RangeWasSent = false;
    if (!Connected)
        ConnectToFtp();
    ftp->mkdir(DirName);
    ftp->close();
}

// перейти в каталог на удалённом сервере

void s_ftp::ChangeDir(QString dir)
{
    Busy = true;
    RangeWasSent = false;
    if (!Connected)
        ConnectToFtp();
    ftp->cd(dir);
    ftp->close();
}

// слот, вызываемый при получении каких-либо данных (readyRead)

void s_ftp::ReadDataFromUrl()
{
    qint64 RDSize = ReadData.size();
    if (RDSize > 4000000L)
    {
        emit ReadBufferFull();
        return;
    }
    QFtp *ftp = static_cast<QFtp *>(sender());
    quint64 BytesRead = ftp->bytesAvailable();
    ReadData.resize(RDSize+BytesRead);
    BytesRead = ftp->read(&(ReadData.data()[RDptr]), BytesRead); // BytesRead справа - сколько хочется скачать, BytesRead слева - сколько реально удалось скачать
    RDptr += BytesRead;
    emit NewDataAvailable();
}

// слот, вызываемый по окончании какой-либо команды

void s_ftp::TransferFinished(int, bool error)
{
    if (ftp->currentCommand() == QFtp::ConnectToHost)
    {
        if (error)
        {
            SFTPER("Невозможно соединение с FTP-сервером");
            FtpDisconnect();
            return;
        }
        Connected = true;
        SFTPINFO("Создано подключение к FTP-серверу");
        return;
    }
    if (ftp->currentCommand() == QFtp::Get)
    {
        if (error)
            SFTPER("Передача файла была отменена");
        else
            SFTPINFO("Файл получен успешно");
        Busy = false;
    }
    if (ftp->currentCommand() == QFtp::Put)
    {
        if (error)
            SFTPER("Ошибка при отправке файла");
        else
            SFTPINFO("Файл отправлен успешно");
        Busy = false;
    }
    if (ftp->currentCommand() == QFtp::Cd)
    {
        if (error)
            CdFailed = true;
    }
    if (ftp->currentCommand() == QFtp::Mkdir)
    {
        if (error)
        {
            SFTPER("Ошибка при создании каталога");
        }
        else
            SFTPER("Каталог создан успешно");
        Busy = false;
    }
}

// слот, вызываемый при скачивании/закачивании кусков файлов

void s_ftp::SetRangeAndValue(qint64 Value, qint64 Total)
{
    if (!RangeWasSent)
    {
        RangeWasSent = true;
        emit SetRange(0, Total);
    }
    emit SetValue(Value);
}

void s_ftp::FtpDisconnect()
{
    Connected = false;
    if (ftp)
    {
        ftp->abort();
        ftp->deleteLater();
        ftp = 0;
    }
}

void s_ftp::FtpTimeout()
{
    FtpDisconnect();
    Tmr->stop();
}
