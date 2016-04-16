#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include "ethernet.h"
#include "publicclass.h"

#define CLIER(a)   ERMSG(PublicClass::ER_CLI,__LINE__,a)
#define CLIWARN    WARNMSG(PublicClass::ER_CLI,__LINE__)
#define CLIINFO(a) INFOMSG(PublicClass::ER_CLI,__LINE__,a)

#define SERVERRSTR  "ERROR"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent=0);
    ~Client();

    bool Connect(QString Host, QString Port);
    bool ChDir(QString Dir);
    bool MkDir(QString Dir);
    bool List();
    bool SendFile(QString Dir, QString Filename, QByteArray *ba, int size = 0);
    bool GetFile(QString Dir, QString Filename, QByteArray *ba, int size = 0);
    void Disconnect();

public slots:
    void StopThreads();

signals:
    void ClientSend(QByteArray *);
    void FileSend(QByteArray *);
    void BytesOverall(qint64 bytes);
    void BytesRead(qint64 bytes);
    void BytesWritten(qint64 bytes);

private:
    enum Messages
    {
      // ServerToClient
      CMD_LOGINREQ, // запрос имени пользователя
      CMD_PSWREQ,	// запрос пароля (зарез.)
      ANS_SQLRESULT, // результат обработки sql-запроса
      ANS_MESSAGES, // текущие сообщения для пользователя
      ANS_CHATMSGS, // сообщения из чата для пользователя
      ANS_CHATSTATUS, // состояние чата в ответ на CMD_CHATREQ
      ANS_OKTORCV, // подтверждение готовности к приёму файла
      ANS_GETFILE, // подтверждение окончания приёма файла
      ANS_OKTOXMT, // подтверждение готовности к отправке файла
      ANS_PUTFILE, // подтверждение окончания отправки файла
      ANS_DIRLIST, // выдача содержимого каталога
      ANS_QUIT, // подтверждение завершения сеанса связи
      // ClientToServer
      ANS_LOGIN, // имя пользователя
      ANS_PSW, // пароль
      CMD_SQL, // запрос sql
      CMD_MESSAGES, // запрос текущих сообщений для пользователя
      CMD_CHATMSGS, // запрос сообщений из чата
      CMD_CHATREQ, // запрос состояния чата (пользователи)
      CMD_GETFILE, // запрос файла из хранилища
      CMD_PUTFILE, // запрос на отсылку файла в хранилище
      CMD_DIRLIST, // запрос списка файлов в каталоге
      CMD_QUIT, // завершение сеанса связи
      CMD_IDLE // состояние ожидания команды
    };

    enum ClientErrors
    {
        CLIER_NOERROR,
        CLIER_GENERAL,
        CLIER_CLOSED,
        CLIER_TIMEOUT,
        CLIER_SERVER
    };

    QByteArray *BufData, *RcvData, *XmitData;
    Ethernet *MainEthernet, *FileEthernet;
    QTimer *TimeoutTimer, *GetComReplyTimer, *GetFileTimer;
    bool Busy, FileBusy, Connected, FileConnected, CmdOk, LoginOk;
    QFile *LogFile;
    QTextStream *LogStream;
    QString FileHost;
    quint16 FilePort;
    qint64 WrittenBytes, ReadBytes;
    int CurrentCommand, RcvDataSize, XmitDataSize, DetectedError;

    bool SendCmd(int Command, QStringList Args=QStringList());
    QString RemoveSpaces(QString str);

private slots:
    void ClientGet(QByteArray *);
    void FileGet(QByteArray *);
    void ClientConnected();
    void ClientDisconnected();
    void ClientFileConnected();
    void Timeout();
    void ClientErr(int error);
    void SetBytesWritten(qint64 bytes);
    void ParseReply();
    void GetFileTimerTimeout();
};

extern Client *Cli;

#endif // CLIENT_H
