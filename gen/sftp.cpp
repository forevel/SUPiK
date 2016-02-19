#include <QApplication>
#include <QFile>
#include <QTcpSocket>

#include "sftp.h"

s_ftp sftp;

s_ftp::s_ftp(QObject *parent) : QObject(parent)
{
    Busy = false;
    Connected = false;
    ReadData.resize(65535);
//    ftp = 0;
}

s_ftp::~s_ftp()
{
    curl_global_cleanup();
    //    ftp->close();
//    FtpDisconnect();
}

// Установка связи и проверка соединения с FTP-сервером

bool s_ftp::CheckFtp()
{
    return ConnectToFtp();
}

static size_t CheckCallback(void *buffer, size_t size, size_t nmemb, void *stream)
{
    size_t realsize = size * nmemb;
    char *newdata = static_cast<char *>(stream);
    memcpy(newdata, buffer, realsize);
    newdata[realsize] = '\0';
    return realsize;
}

// установка соединения с FTP-сервером и определение необходимых связок сигнал-слот

bool s_ftp::ConnectToFtp()
{
    Busy = true;
    QString CheckUrl = QString(FTP_SERVER) + "/" + QString(CHECKFILE);
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK)
    {
        Busy = false;
        SFTPER("Невозможно открыть FTP-соединение");
        return false;
    }
    curl = curl_easy_init();
    if(curl)
    {
/*        struct MemoryStruct chunk;
        chunk.memory = static_cast<char *>(malloc(1));
        chunk.size = 0; */
        QByteArray buffer;
        buffer.resize(10);
        QString userpwd = "supik:wdbpy(WcTtTZzA_TEc-<";
        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd.toLocal8Bit().data());
        curl_easy_setopt(curl, CURLOPT_URL, CheckUrl.toLocal8Bit().data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CheckCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(buffer.data()[0]));
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            Busy = false;
            SFTPER("Ошибка приёма ключевого файла: "+QString::number(res));
            return false;
        }
        QString StringToCheck = QString::fromLocal8Bit(buffer.data());
        Busy = false;
        curl_easy_cleanup(curl);
        if (StringToCheck == "supik\n")
        {
            SFTPINFO("Файл получен успешно");
            Connected = true;
//            free(chunk.memory);
            return true;
        }
        else
        {
            SFTPER("Ключевой файл не найден на FTP-сервере");
            Connected = false;
//            free(chunk.memory);
            return false;
        }
    }
    else
    {
        SFTPER("Невозможно получить ссылку на curl");
        Busy = false;
        return false;
    }
/*    Tmr = new QTimer(this);
    Tmr->setInterval(5000); // 5 секунд, чтобы получить ответ от сервера
    connect(Tmr,SIGNAL(timeout()),this,SLOT(FtpTimeout()));
    Tmr->start();
    ftp = new QFtp(this);
    connect(ftp,SIGNAL(commandFinished(int,bool)),this,SLOT(TransferFinished(int, bool)));
    connect(ftp, SIGNAL(readyRead()), this, SLOT(ReadDataFromUrl()));
    connect(ftp,SIGNAL(dataTransferProgress(qint64,qint64)),this,SLOT(SetRangeAndValue(qint64,qint64)));
    ftp->connectToHost(QString(FTP_SERVER), 21);
    ftp->login("supik","wdbpy(WcTtTZzA_TEc-<");
    Tmr->stop(); */
}


// получить данные из удалённого файла по ссылке Url в буфер ReadData

void s_ftp::GetData(QString Url)
{
/*    Busy = true;
    RangeWasSent = false;
    ReadData.clear();
    RDptr=0;
    if (!Connected)
        ConnectToFtp();

    ftp->get(Url);*/
//    ftp->close();
}

// получить данные из удалённого файла по ссылке Url в локальный файл с именем filename

void s_ftp::GetFile(QString Url, QString filename)
{
/*    Busy = true;
    RangeWasSent = false;
    if (!Connected)
        ConnectToFtp();
    ReadFile = new QFile(filename, this);
    if (ReadFile->open(QIODevice::WriteOnly))
    {
        ftp->get(Url, ReadFile);
//        ftp->close();
    }
    else
    {
        SFTPER("Ошибка открытия файла "+filename);
        return;
    }*/
}

// отправить данные из локального файла с именем filename в удалённый файл по ссылке Url

void s_ftp::PutFile(QString Url, QString filename)
{
/*    Busy = true;
    RangeWasSent = false;

    if (!Connected)
        ConnectToFtp();
    QFile *data = new QFile(filename, this);
    if (data->open(QIODevice::ReadOnly))
    {
        CdFailed = false;
        ftp->cd(Url);
//        ftp->close();
        while (Busy)
            qApp->processEvents(QEventLoop::AllEvents);
        if (CdFailed) // нет такого каталога на сервере
        {
            ftp->mkdir(Url);
            ftp->close();
        }
        ftp->put(data, Url);
//        ftp->close();
    }
    else
    {
        SFTPER("Ошибка открытия файла "+filename);
        return;
    }*/
}

// создать каталог на удалённом сервере

void s_ftp::MakeDir(QString DirName)
{
/*    Busy = true;
    RangeWasSent = false;
    if (!Connected)
        ConnectToFtp();
    ftp->mkdir(DirName);*/
//    ftp->close();
}

// перейти в каталог на удалённом сервере

void s_ftp::ChangeDir(QString dir)
{
/*    DirToCD = dir;
    Busy = true;
    RangeWasSent = false;
    if (!Connected)
        ConnectToFtp();
    ftp->cd(dir);*/
//    ftp->close();
}

// прочитать содержимое текущего каталога в список FileList

void s_ftp::ListDir()
{
/*    Busy = true;
    FileList.clear();
    RangeWasSent = false;
    if (!Connected)
        ConnectToFtp();
    ftp->list();*/
//    ftp->close();
}

// слот, вызываемый при получении очередной порции информации о файлах в каталоге
/*
void s_ftp::AddToFileList(QUrlInfo FileInfo)
{
//    FileList.append(FileInfo);
}*/

// слот, вызываемый при получении каких-либо данных (readyRead)

void s_ftp::ReadDataFromUrl()
{
/*    qint64 RDSize = ReadData.size();
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
    emit NewDataAvailable();*/
}

// слот, вызываемый по окончании какой-либо команды

void s_ftp::TransferFinished(int, bool error)
{
/*    Error = error;
    if (ftp == 0)
        return;
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
        FtpListConnection = connect(ftp,SIGNAL(listInfo(QUrlInfo)),this,SLOT(AddToFileList(QUrlInfo)));
        CurrentDirectory.clear();
        CurrentDirectory.push("/");
        return;
    }
    if (ftp->currentCommand() == QFtp::Get)
    {
        if (error)
            SFTPER("Передача файла была отменена");
        else
            SFTPINFO("Файл получен успешно");
        if (ReadFile)
        {
            ReadFile->close();
            delete ReadFile;
        }
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
        else
        {
            if (DirToCD == "..")
            {
                if (CurrentDirectory.size()>2)
                    CurrentDirectory.pop();
                else
                {
                    SFTPWARN;
                }
            }
            else if (DirToCD == "/")
            {
                while (CurrentDirectory.size() > 1)
                    CurrentDirectory.pop();
            }
            else
                CurrentDirectory.push(DirToCD);
        }
        Busy = false;
    }
    if (ftp->currentCommand() == QFtp::Mkdir)
    {
        if (error)
        {
            SFTPER("Ошибка при создании каталога");
        }
        else
            SFTPINFO("Каталог создан успешно");
        Busy = false;
    }*/
}

// слот, вызываемый при скачивании/закачивании кусков файлов

void s_ftp::SetRangeAndValue(qint64 Value, qint64 Total)
{
/*    if (!RangeWasSent)
    {
        RangeWasSent = true;
        emit SetRange(0, Total);
    }
    emit SetValue(Value);*/
}

void s_ftp::FtpDisconnect()
{
/*    Connected = false;
    Busy = false;
    if (FtpListConnection)
        QObject::disconnect(FtpListConnection);
    if (ftp)
    {
//        ftp->abort();
        ftp->close();
        ftp->deleteLater();
        ftp = 0;
    }*/
}

void s_ftp::FtpTimeout()
{
/*    Busy = false;
    SFTPER("Превышено время ожидания установления соединения");
    FtpDisconnect();
    Tmr->stop();*/
}
