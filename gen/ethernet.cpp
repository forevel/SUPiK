#include <QThread>
#include <QCoreApplication>

#include "Ethernet.h"

Ethernet::Ethernet(QObject *parent) :
    QObject(parent)
{
    OutDataBuf.clear();
    ClosePortAndFinishThread = false;
    Busy = false;
}

void Ethernet::SetIpAndPort(QString Host, quint16 Port)
{
    this->Host = Host;
    this->Port = Port;
}

void Ethernet::Run()
{
    Busy = true;
    sock = new QTcpSocket(this);
    connect(sock,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(seterr(QAbstractSocket::SocketError)));
    connect(sock,SIGNAL(connected()),this,SIGNAL(connected()));
    connect(sock,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
    sock->connectToHost(Host,Port,QIODevice::ReadWrite,QAbstractSocket::IPv4Protocol);
    connect(sock,SIGNAL(readyRead()),this,SLOT(CheckForData()));
    connect(sock,SIGNAL(bytesWritten(qint64)),this,SIGNAL(byteswritten(qint64)));
    while (1)
    {
        OutDataBufMtx.lock();
        if (!OutDataBuf.isEmpty()) // что-то пришло в выходной буфер для записи
            SendData();
        OutDataBufMtx.unlock();
        if (ClosePortAndFinishThread)
        {
            if (sock->isOpen())
            {
                sock->close();
                sock->disconnect();
                delete sock;
            }
            emit finished();
            break;
        }
        QThread::msleep(10);
        qApp->processEvents();
    }
}

void Ethernet::Stop()
{
    ClosePortAndFinishThread = true;
}

void Ethernet::seterr(QAbstractSocket::SocketError err)
{
    emit error(err+2); // до 1 другие ошибки, err от -1
}

void Ethernet::SendData()
{
    qint64 res = sock->write(OutDataBuf);
    if (res == -1)
        emit error(SKT_SENDDATAER); // ошибка
    OutDataBuf.clear();
}

void Ethernet::InitiateWriteDataToPort(QByteArray *ba)
{
    OutDataBufMtx.lock();
    OutDataBuf = *ba;
    OutDataBufMtx.unlock();
}

void Ethernet::CheckForData()
{
    QByteArray *ba = new QByteArray(sock->readAll());
    emit newdataarrived(ba);
}
