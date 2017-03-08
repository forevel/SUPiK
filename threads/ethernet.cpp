#include <QThread>
#include <QCoreApplication>

#include "Ethernet.h"
#include "../gen/publicclass.h"
#include "../gen/log.h"

Log *EthLog;

Ethernet::Ethernet(QObject *parent) : QObject(parent)
{
    ReadData.clear();
    Level = 0;
}

void Ethernet::SetEthernet(const QString &Host, int Port, int Type)
{
    EthLog = new Log;
    EthLog->Init("eth "+Host+"_"+QString::number(Port)+".log");
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
        connect(sock,SIGNAL(disconnected()),this, SLOT(OkToDelete()));
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
        connect(sslsock,SIGNAL(disconnected()),this,SLOT(OkToDelete()));
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
}

void Ethernet::Disconnect()
{
    try
    {
        switch (EthType)
        {
        case ETH_PLAIN:
        {
            if (sock)
            {
                if (sock->isOpen())
                {
                    sock->disconnectFromHost();
                    QThread::msleep(10);
                }
            }
            break;
        }
        case ETH_SSL:
        {
            if (sslsock->isOpen())
            {
                sslsock->disconnectFromHost();
                QThread::msleep(10);
            }
        }
        default:
            break;
        }
        emit disconnected();
    }
    catch (...)
    {
        EthLog->error("Error while disconnecting");
    }
}

void Ethernet::OkToDelete()
{
    try
    {
        if (EthType == ETH_PLAIN)
            sock->deleteLater();
        else
            sslsock->deleteLater();
    }
    catch(...)
    {
        EthLog->error("Error while deleting socket");
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
/*     if ((state == QAbstractSocket::UnconnectedState) || (state == QAbstractSocket::ClosingState))
     {
         Disconnect();
     } */
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
         sslsock->setPrivateKey(pc.HomeDir+"/certs/cakey.pem", QSsl::Rsa, QSsl::Pem, "Seneles2");
         sslsock->setLocalCertificate(pc.HomeDir+"/certs/cacert.pem");
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

void Ethernet::SslError(QAbstractSocket::SocketError err)
{
    EthLog->warning("SSL Error: "+sslsock->errorString());
    if (err == QAbstractSocket::RemoteHostClosedError)
        return;
    Disconnect();
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

void Ethernet::WriteData(QByteArray &ba)
{
    qint64 res;
    if (EthType == ETH_PLAIN)
        res = sock->write(ba);
    else if (EthType == ETH_SSL)
        res = sslsock->write(ba);
    QCoreApplication::processEvents(); // по рекомендации отсюда: http://www.prog.org.ru/topic_25254_0.html
    if (res == -1)
        emit error(SKT_SENDDATAER); // ошибка
}

void Ethernet::CheckForData()
{
    if (Level > 0) // уже что-то получаем
        return;
    ++Level;
    if (EthType == ETH_PLAIN)
    {
        while (sock->bytesAvailable())
            ReadData += sock->readAll();
    }
    else if (EthType == ETH_SSL)
    {
        while (sslsock->bytesAvailable())
            ReadData += sslsock->readAll();
    }
/*    EthLog->info("Level: ");
    EthLog->info(QString::number(Level)); */
    emit NewDataArrived(ReadData);
    ReadData.clear();
    --Level;
}
