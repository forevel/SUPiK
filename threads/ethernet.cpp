#include <QThread>
#include <QCoreApplication>

#include "Ethernet.h"
#include "../gen/publicclass.h"
#include "../gen/log.h"

Log *EthLog;

Ethernet::Ethernet(QString Host, int Port, int Type, QObject *parent) :
    QObject(parent)
{
    EthLog = new Log;
    EthLog->Init(pc.HomeDir+"/eth.log");
    EthLog->info("Log started");
    sock = 0;
    sslsock = 0;
    EthType = Type;
    switch (EthType)
    {
    case ETH_PLAIN:
        sock = new QTcpSocket(this);
        connect(sock,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(SockError(QAbstractSocket::SocketError)));
        connect(sock,SIGNAL(connected()),this,SIGNAL(connected()));
        connect(sock,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
        connect(sock,SIGNAL(readyRead()),this,SLOT(CheckForData()));
        connect(sock,SIGNAL(bytesWritten(qint64)),this,SIGNAL(byteswritten(qint64)));
        EthLog->info("Connecting to host "+Host+":"+QString::number(Port)+"...");
        sock->connectToHost(Host,Port,QIODevice::ReadWrite,QAbstractSocket::IPv4Protocol);
        break;
    case ETH_SSL:
        sslsock = new QSslSocket(this);
        sslsock->setProtocol(QSsl::AnyProtocol);
        connect(sslsock, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(SslError(QAbstractSocket::SocketError)));
        connect(sslsock,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
        connect(sslsock,SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(SslErrors(QList<QSslError>)));
        connect(sslsock,SIGNAL(encrypted()),this,SLOT(SslSocketEncrypted()));
        connect(sslsock,SIGNAL(encrypted()),this,SIGNAL(connected()));
        connect(sslsock, SIGNAL(stateChanged(QAbstractSocket::SocketState)),this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));
        EthLog->info("SSL: Connecting to host "+Host+":"+QString::number(Port)+"...");
        sslsock->connectToHost(Host, Port);
        break;
    default:
        break;
    }
    OutDataBuf.clear();
    ClosePortAndFinishThread = false;
    Busy = false;
}

void Ethernet::Run()
{
    Busy = true;
    switch (EthType)
    {
    case ETH_PLAIN:
    {
        if (sock)
        {
            while (sock->state() == QAbstractSocket::ConnectedState)
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
        break;
    }
    case ETH_SSL:
    {
        while (1)
        {
            OutDataBufMtx.lock();
            if (!OutDataBuf.isEmpty()) // что-то пришло в выходной буфер для записи
                SendData();
            OutDataBufMtx.unlock();
            if (ClosePortAndFinishThread)
            {
                if (sslsock->isOpen())
                {
                    sslsock->close();
                    sslsock->disconnect();
                    delete sslsock;
                }
                emit finished();
                Busy = false;
                return;
            }
            QThread::msleep(10);
            qApp->processEvents();
        }
        break;
    }
    default:
        break;
    }
}

void Ethernet::SslSocketEncrypted()
{
    EthLog->info("SSL: Secure connection established!");
    connect(sslsock, SIGNAL(readyRead()),this, SLOT(CheckForData()));
    connect(sslsock,SIGNAL(encryptedBytesWritten(qint64)),this,SIGNAL(byteswritten(qint64)));
}

void Ethernet::SocketStateChanged(QAbstractSocket::SocketState state)
{
     if ((state == QAbstractSocket::UnconnectedState) || (state == QAbstractSocket::ClosingState))
     {
         ClosePortAndFinishThread = true;
     }
     switch (state)
     {
     case QAbstractSocket::UnconnectedState:
         EthLog->info("SSL: Socket in unconnected state");
         emit disconnected();
         break;
     case QAbstractSocket::HostLookupState:
         EthLog->info("SSL: Socket in host lookup state");
         break;
     case QAbstractSocket::ConnectingState:
         EthLog->info("SSL: Socket in connecting state");
         break;
     case QAbstractSocket::ConnectedState:
         EthLog->info("SSL: Socket in connected state");
         sslsock->setPrivateKey(pc.HomeDir+"cakey.pem", QSsl::Rsa, QSsl::Pem, "Seneles2");
         sslsock->setLocalCertificate(pc.HomeDir+"cacert.pem");
         sslsock->startClientEncryption();
         break;
     case QAbstractSocket::BoundState:
         EthLog->info("SSL: Socket in bound state");
         break;
     case QAbstractSocket::ClosingState:
         EthLog->info("SSL: Socket in closing state");
         break;
     case QAbstractSocket::ListeningState:
         EthLog->info("SSL: Socket in listening state");
         break;
     default:
         break;
     }
}

void Ethernet::Stop()
{
    EthLog->info("Socket close pending");
    ClosePortAndFinishThread = true;
}

void Ethernet::SslError(QAbstractSocket::SocketError err)
{
    if (err == QAbstractSocket::RemoteHostClosedError)
        ClosePortAndFinishThread = true;
    EthLog->warning("SSL Error: "+sslsock->errorString());
    ClosePortAndFinishThread = true;
//    emit error(err+2); // до 1 другие ошибки, err от -1
}

void Ethernet::SslErrors(QList<QSslError> errlist)
{
    for (int i=0; i<errlist.size(); i++)
        EthLog->warning("SSL Errors: "+errlist.at(i).errorString());
    sslsock->ignoreSslErrors();
}

void Ethernet::SockError(QAbstractSocket::SocketError err)
{
    EthLog->error("SockError: "+QString::number(err));
    ERMSG(sock->errorString());
}

void Ethernet::SendData()
{
    qint64 res;
    if (EthType == ETH_PLAIN)
        res = sock->write(OutDataBuf);
    else if (EthType == ETH_SSL)
        res = sslsock->write(OutDataBuf);
    QCoreApplication::processEvents(); // по рекомендации отсюда: http://www.prog.org.ru/topic_25254_0.html
    if (res == -1)
        emit error(SKT_SENDDATAER); // ошибка
    OutDataBuf.clear();
}

void Ethernet::InitiateWriteDataToPort(QByteArray *ba)
{
    OutDataBufMtx.lock();
    OutDataBuf = *ba;
    OutDataBufMtx.unlock();
    delete ba;
}

void Ethernet::CheckForData()
{
    QByteArray *ba;
    if (EthType == ETH_PLAIN)
        ba = new QByteArray(sock->readAll());
    else if (EthType == ETH_SSL)
        ba = new QByteArray(sslsock->readAll());
//    EthLog->info("Bytes received: ");
//    EthLog->info(ba->data());
    emit newdataarrived(ba);
}
