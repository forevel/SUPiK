#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include "../threads/ethernet.h"
#include "publicclass.h"

#define CLIER(a)   ERMSG(PublicClass::ER_CLI,__LINE__,a)
#define CLIWARN    WARNMSG(PublicClass::ER_CLI,__LINE__)
#define CLIINFO(a) INFOMSG(PublicClass::ER_CLI,__LINE__,a)

#define SERVERRSTR  "ERROR\n"
#define TIMERSOFF // если не закомментировано, таймауты отключены

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent=0);
    ~Client();

    enum ClientErrors
    {
        CLIER_NOERROR,
        CLIER_GENERAL,
        CLIER_CLOSED,
        CLIER_TIMEOUT,
        CLIER_SERVER,
        CLIER_LOGIN,     // некорректный логин (нет такого пользователя)
        CLIER_PSW,       // пароли введённый и в БД не совпадают
        CLIER_GROUP,     // не найдена группа доступа
        CLIER_WRARGS,    // неправильные аргументы
        CLIER_GETFTOUT,  // таймаут во время приёма файла
        CLIER_GETFER    // ошибка во время приёма файла
    };

    enum Messages
    {
        // ServerToClient
        CMD_LOGINREQ, // запрос имени пользователя
        CMD_PSWREQ,	// запрос пароля (зарез.)
        ANS_GROUP,    // группа доступа
        ANS_SQLRESULT, // результат обработки sql-запроса
        ANS_MESSAGES, // текущие сообщения для пользователя
        ANS_CHATMSGS, // сообщения из чата для пользователя
        ANS_CHATSTATUS, // состояние чата в ответ на CMD_CHATREQ
        ANS_OKTORCV, // подтверждение готовности к приёму файла
        ANS_GETFILE, // подтверждение приёма файла
        ANS_OKTOXMT, // подтверждение готовности к отправке файла
        ANS_PUTFILE, // подтверждение отправки файла
        ANS_DIRLIST, // выдача содержимого каталога
        ANS_QUIT, // подтверждение завершения сеанса связи
        ANS_GVSBFS, // подтверждение приёма очередной порции sql-ответа
        ANS_GVSBC, // GetValuesByColumn
        // ClientToServer
        ANS_LOGIN, // имя пользователя
        ANS_PSW, // пароль
        CMD_TF_GVSBFS, // запросы sql для tablefields
        CMD_GVSBFS, // запрос sql простой: Get Values From Table By Fields - возвращаются все записи
        CMD_GVBFS, // запрос sql простой: Get Value From Table By Fields - возвращается одна запись
        CMD_GVSBC, // GetValuesByColumn - возвращаются все записи
        CMD_MESSAGES, // запрос текущих сообщений для пользователя
        CMD_CHATMSGS, // запрос сообщений из чата
        CMD_CHATREQ, // запрос состояния чата (пользователи)
        CMD_GETFILE, // запрос файла из хранилища
        CMD_PUTFILE, // запрос на отсылку файла в хранилище
        CMD_DIRLIST, // запрос списка файлов в каталоге
        CMD_QUIT, // завершение сеанса связи
        CMD_IDLE, // состояние ожидания команды
        CMD_STATUS  // запрос статуса от сервера
    };

    bool Busy;
    QList<QStringList> Result;

    int Connect(QString Host, QString Port);
    void Disconnect();
    void SendCmd(int Command, QStringList &Args=QStringList());

public slots:
    void StopThreads();

signals:
    void ClientSend(QByteArray *);
    void FileSend(QByteArray *);
    void BytesOverall(qint64 bytes);
    void BytesRead(qint64 bytes);
    void BytesWritten(qint64 bytes);
    void DataReady(QStringList &); // очередная порция данных готова
    void TransferComplete(); // окончание приёма/передачи файла

private:

    QByteArray RcvData;
    Ethernet *MainEthernet, *FileEthernet;
    QTimer *TimeoutTimer, *GetComReplyTimer, *GetFileTimer;
    bool FileBusy, Connected, FileConnected, CmdOk, LoginOk, FirstReplyPass, ComReplyTimeoutIsSet;
    QTextStream *LogStream;
    QString FileHost;
    quint16 FilePort;
    qint64 WrittenBytes, ReadBytes, MsgNum;
    int CurrentCommand, RcvDataSize, XmitDataSize, DetectedError;
    FILE *fp;
    size_t filesize;
    size_t filepos;
    int GVSBFS_FieldsNum;

    QString RemoveSpaces(QString str);
    void WriteErrorAndBreakReceiving(QString ErMsg);
    QStringList SeparateBuf(QByteArray &buf);

private slots:
    void ClientConnected();
    void ClientDisconnected();
    void Timeout();
    void ClientErr(int error);
    void SetBytesWritten(qint64 bytes);
    void ParseReply(QByteArray *ba);
    void GetFileTimerTimeout();
    void ComReplyTimeout();
};

extern Client *Cli;

#endif // CLIENT_H
