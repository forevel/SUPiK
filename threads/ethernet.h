#ifndef Ethernet_H
#define Ethernet_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>
#include <QByteArray>
#include <QSslSocket>

#define ETHER(a)   ERMSG(PublicClass::ER_ETH,__LINE__,a)
#define ETHWARN    WARNMSG(PublicClass::ER_ETH,__LINE__)
#define ETHINFO(a) INFOMSG(PublicClass::ER_ETH,__LINE__,a)

// ошибки IP-сокета
#define SKT_UNKNOWNER       1 // неизвестная ошибка
#define SKT_REFUSEDER       2 // тайм-аут при открытии сокета
#define SKT_REMOTECLOSEDER  3 // удалённый узел разорвал соединение
#define SKT_NOTFOUNDER      4 // узел не найден
#define SKT_ACCESSER        5 // ошибка доступа к сокету
#define SKT_RESER           6 // недостаточно памяти для сокета
#define SKT_TOER            7 // тайм-аут при операции с сокетом
#define SKT_LARGEER         8 // слишком большой размер датаграммы
#define SKT_NETWORKER       9 // обрыв связи
#define SKT_ADDRESSER       10 // адрес уже используется
#define SKT_BINDER          11 // адрес привязки не принадлежит данному ПК
#define SKT_UNSUPER         12 // неподдерживаемая операция
#define SKT_PROXYAUTHER     13 // прокси-сервер требует авторизацию
#define SKT_SSLER           14 // ошибка при установлении защищённого соединения
#define SKT_UNFINISHER      15 // есть незавершённая операция
#define SKT_PROXYREFER      16 // прокси-сервер отклонил соединение
#define SKT_PROXYCLOSER     17 // прокси-сервер закрыл соединение
#define SKT_PROXYTOER       18 // произошёл тайм-аут при работе с прокси-сервером
#define SKT_PROXYNOTFOUNDER 19 // прокси-сервер не найден
#define SKT_PROXYPROTER     20 // ответ прокси-сервера не распознан
#define SKT_OPERER          21 // сокет занят
#define SKT_SSLINTER        22 // ошибка SSL на стороне ПК
#define SKT_SSLINVDATAER    23 // некорректные данные SSL
#define SKT_TEMPER          24 // повторите операцию позже
#define SKT_SENDDATAER      25 // ошибка при отправке данных

class Ethernet : public QObject
{
    Q_OBJECT

public:
    enum EthernetTypes
    {
        ETH_PLAIN,
        ETH_SSL
    };

    QTcpSocket *sock;
    QSslSocket *sslsock;
    Ethernet(QString Host, int Port, int Type=ETH_PLAIN, QObject *parent = 0);
    bool ClosePortAndFinishThread, Busy;
    static QMap<int,QString> EthernetErrors()
    {
        QMap<int,QString> map;
        map.insert(1, QString::fromLocal8Bit("Неизвестная ошибка"));
        map.insert(2, QString::fromLocal8Bit("Тайм-аут при открытии сокета"));
        map.insert(3, QString::fromLocal8Bit("Удалённый узел разорвал соединение"));
        map.insert(4, QString::fromLocal8Bit("Узел не найден"));
        map.insert(5, QString::fromLocal8Bit("Ошибка доступа к сокету"));
        map.insert(6, QString::fromLocal8Bit("Недостаточно памяти для сокета"));
        map.insert(7, QString::fromLocal8Bit("Тайм-аут при операции с сокетом"));
        map.insert(8, QString::fromLocal8Bit("Слишком большой размер датаграммы"));
        map.insert(9, QString::fromLocal8Bit("Обрыв связи"));
        map.insert(10, QString::fromLocal8Bit("Адрес уже используется"));
        map.insert(11, QString::fromLocal8Bit("Адрес привязки не принадлежит данному ПК"));
        map.insert(12, QString::fromLocal8Bit("Неподдерживаемая операция"));
        map.insert(13, QString::fromLocal8Bit("Прокси-сервер требует авторизацию"));
        map.insert(14, QString::fromLocal8Bit("Ошибка при установлении защищённого соединения"));
        map.insert(15, QString::fromLocal8Bit("Есть незавершённая операция"));
        map.insert(16, QString::fromLocal8Bit("Прокси-сервер отклонил соединение"));
        map.insert(17, QString::fromLocal8Bit("Прокси-сервер закрыл соединение"));
        map.insert(18, QString::fromLocal8Bit("Произошёл тайм-аут при работе с прокси-сервером"));
        map.insert(19, QString::fromLocal8Bit("Прокси-сервер не найден"));
        map.insert(20, QString::fromLocal8Bit("Ответ прокси-сервера не распознан"));
        map.insert(21, QString::fromLocal8Bit("Сокет занят"));
        map.insert(22, QString::fromLocal8Bit("Ошибка SSL на стороне ПК"));
        map.insert(23, QString::fromLocal8Bit("Некорректные данные SSL"));
        map.insert(24, QString::fromLocal8Bit("Повторите операцию позже"));
        map.insert(25, QString::fromLocal8Bit("Ошибка при отправке данных"));
        return map;
    }

//    bool SetIpAndPort(QString Host, quint16 Port);

public slots:
    void Run();
    void Stop();
    void InitiateWriteDataToPort(QByteArray *);

signals:
    void error(int);
    void connected();
    void disconnected();
    void finished();
    void newdataarrived(QByteArray *);
    void byteswritten(qint64 bytes);

private slots:
    void CheckForData();
    void SockError(QAbstractSocket::SocketError);
    void SslError(QAbstractSocket::SocketError);
    void SslErrors(QList<QSslError> errlist);
    void SocketStateChanged(QAbstractSocket::SocketState state);
    void SslSocketEncrypted();

private:
    QMutex OutDataBufMtx;
    QByteArray *DataToSend;
    QByteArray OutDataBuf;
    QString Host;
    quint16 Port;
    void SendData();
    int EthType;

protected:
};

#endif // Ethernet_H
