#include <QThread>
#include <QCoreApplication>

#include "Ethernet.h"
#include "../gen/publicclass.h"

Ethernet::Ethernet(QObject *parent) : QObject(parent)
{
    ReadData.clear();
    ReadDataSize = 0;
    EthLog = new Log;
    Receiving = false;
}

Ethernet::~Ethernet()
{
    Disconnect();
}

void Ethernet::Connect(int Type)
{
    EthLog->Init("eth "+pc.SupikServer+"_"+QString::number(pc.SupikPort)+".log");
    EthLog->info("Log started");
    sock = 0;
    sslsock = 0;
    EthType = Type;
    switch (EthType)
    {
    case ETH_PLAIN:
        sock = new QTcpSocket(this);
        connect(sock,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(SockError(QAbstractSocket::SocketError)));
        connect(sock,SIGNAL(connected()),this,SIGNAL(Connected()));
        connect(sock,SIGNAL(disconnected()),this, SIGNAL(Disconnected()));
        connect(sock,SIGNAL(readyRead()),this,SLOT(CheckForData()));
        connect(sock,SIGNAL(bytesWritten(qint64)),this,SIGNAL(byteswritten(qint64)));
        EthLog->info("Connecting to host "+pc.SupikServer+":"+QString::number(pc.SupikPort)+"...");
        sock->connectToHost(pc.SupikServer,pc.SupikPort,QIODevice::ReadWrite,QAbstractSocket::IPv4Protocol);
        break;
    case ETH_SSL:
        sslsock = new QSslSocket(this);
        sslsock->setProtocol(QSsl::AnyProtocol);
        connect(sslsock, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(SslError(QAbstractSocket::SocketError)));
        connect(sslsock,SIGNAL(disconnected()),this,SIGNAL(Disconnected()));
        connect(sslsock,SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(SslErrors(QList<QSslError>)));
        connect(sslsock,SIGNAL(encrypted()),this,SLOT(SslSocketEncrypted()));
        connect(sslsock,SIGNAL(encrypted()),this,SIGNAL(Connected()));
        connect(sslsock, SIGNAL(stateChanged(QAbstractSocket::SocketState)),this, SLOT(SocketStateChanged(QAbstractSocket::SocketState)));
        EthLog->info("SSL: Connecting to host "+pc.SupikServer+":"+QString::number(pc.SupikPort)+"...");
        sslsock->connectToHost(pc.SupikServer, pc.SupikPort);
        break;
    default:
        break;
    }
}

void Ethernet::Disconnect()
{
    if (EthType == ETH_PLAIN)
        sock->close();
    else
        sslsock->close();
}

/*void Ethernet::Run()
{
    TimeoutTimer = new QTimer;
    TimeoutTimer->setInterval(MAINTIMEOUT);
    connect(TimeoutTimer,SIGNAL(timeout()),this,SLOT(Timeout()));
    FinishThread = TimeoutDetected = false;
    Result.clear();
    ResultCode = CLIER_NOERROR;
    SendCmd();
    while (!TimeoutDetected && Busy) // ждём, пока либо сервер не отработает, либо не наступит таймаут
        pc.Wait(TIME_GENERAL);
    while (!FinishThread) // ждём, пока вышестоящий поток не заберёт результаты
        pc.Wait(TIME_GENERAL);
}*/

void Ethernet::Finish()
{
    FinishThread = true;
}

void Ethernet::SslSocketEncrypted()
{
    EthLog->info("SSL: Secure connection established!");
    connect(sslsock, SIGNAL(readyRead()),this, SLOT(CheckForData()));
    connect(sslsock,SIGNAL(bytesWritten(qint64)),this,SIGNAL(byteswritten(qint64)));
}

void Ethernet::SocketStateChanged(QAbstractSocket::SocketState state)
{
     switch (state)
     {
     case QAbstractSocket::UnconnectedState:
         EthLog->info("SSL: Socket in unconnected state");
         emit Disconnected();
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
//    Disconnect();
    emit Disconnected();
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
        emit Error(SKT_SENDDATAER); // ошибка
}

void Ethernet::CheckForData()
{
    if (Receiving) // уже что-то получаем
        return;
    Receiving = true;
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
    emit NewDataArrived(ReadData);
    ReadData.clear();
    Receiving = false;
}
