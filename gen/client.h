#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include "../threads/ethernet.h"
#include "publicclass.h"

#define CLIER(a)   ERMSG(PublicClass::ER_CLI,__LINE__,a)
#define CLIWARN    WARNMSG(PublicClass::ER_CLI,__LINE__)
#define CLIINFO(a) INFOMSG(PublicClass::ER_CLI,__LINE__,a)

#define SERVERRSTR  "ERROR"
#define SERVEMPSTR  "EMPTY"
//#define TIMERSOFF // если не закомментировано, таймауты отключены
#define SLNUMMAX    10 // максимальное число полей в запросе по столбцам
#define TOKEN       0x7F // разделитель

// M-commands (main)
#define M_QUIT		1001 // подтверждение завершения сеанса связи
#define M_GROUP		1002 // группа доступа
#define M_STATUS	1003
#define M_STATS		1004
#define M_LOGIN		1005 // запрос имени пользователя
#define M_PSW		1006 // запрос пароля (зарез.)
// M-statuses
#define M_IDLE		1000
#define M_RDY		1051
#define M_ERROR		1052
#define M_NEXT		1053 // подтверждение готовности приёма следующей порции данных
#define M_ANSPSW	1094
#define M_ANSLOGIN	1095
#define M_BYE		1099

// S-commands (sql)
#define S_GVSBFS	1101 // simple sql-query by several fields
#define S_GVSBC		1102 // field data query
#define S_GVSBCF	1103 // field data query with additional condition
#define S_GCS		1104 // GetColumnsFromTable - get fields from table
#define S_GVBFS		1105 // simple sql-query for one field
#define S_TC		1106 // create table
#define S_TA		1107 // alter table
#define S_TD		1108 // delete table
#define S_INS		1109 // insert into table
#define S_UPD		1110 // update fields in table
#define S_DEL		1111 // "delete" from table (set deleted=1)
#define S_RDEL		1112 // real delete from table
#define S_SRCH		1113 // search in table like
#define S_GID		1114 // get new id from table

// TF-commands (tablefields)
#define T_GVSBFS	1201 // ValuesByFields
#define T_GVSBC		1202 // htovl
#define T_GVSBCF	1203 // htovlc
#define T_C         1204 // TFCheck
#define T_DEL		1205
#define T_RDEL		1206
#define T_GFT		1207 // GetFullTable (tbvll)
#define T_GID		1208
#define T_IDTV		1209
#define T_TV		1210
#define T_IDTVL		1211
#define T_TID		1212
#define T_VTID		1213
#define T_INS		1214
#define T_UPD		1215
#define T_TF        1216
#define T_TH        1217
#define T_TL        1218

// C-commands (components)
#define C_CRALT		1301

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent=0);
    ~Client();

    enum ResultTypes
    {
        RESULT_MATRIX,  // результат - таблица строк
        RESULT_VECTOR,  // результат - вектор значений
        RESULT_STRING,  // результат - одна строка
        RESULT_NONE     // без результата
    };

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
        CLIER_GETFER,    // ошибка во время приёма файла
        CLIER_WRANSW,    // кривой ответ от сервера
        CLIER_CMDER,     // ошибка обработки команды
        CLIER_EMPTY     // пустой ответ
    };

    enum Messages
    {
        ANS_MESSAGES, // текущие сообщения для пользователя
        ANS_CHATMSGS, // сообщения из чата для пользователя
        ANS_CHATSTATUS, // состояние чата в ответ на CMD_CHATREQ
        ANS_OKTORCV, // подтверждение готовности к приёму файла
        ANS_GETFILE, // подтверждение приёма файла
        ANS_OKTOXMT, // подтверждение готовности к отправке файла
        ANS_PUTFILE, // подтверждение отправки файла
        ANS_DIRLIST, // выдача содержимого каталога
        CMD_MESSAGES, // запрос текущих сообщений для пользователя
        CMD_CHATMSGS, // запрос сообщений из чата
        CMD_CHATREQ, // запрос состояния чата (пользователи)
        CMD_GETFILE, // запрос файла из хранилища
        CMD_PUTFILE, // запрос на отсылку файла в хранилище
        CMD_DIRLIST, // запрос списка файлов в каталоге
        CMD_STATUS  // запрос статуса от сервера
    };

    struct CmdStruct
    {
        QString CmdString;
        int ArgsNum;
        QString Prefix;
        int ResultType;
        bool CheckForFieldsNum;
        bool CheckForPairsNum;
    };

    QMap<int, CmdStruct> CmdMap;
    bool Busy;
    QList<QStringList> Result;
    QString ResultStr;
    int ResultInt, DetectedError;

    int Connect(QString Host, QString Port);
    void Disconnect();
    void SendCmd(int Command, QStringList &Args=QStringList());
    QString Join(QStringList &sl);

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
    bool FileBusy, Connected, FileConnected, CmdOk, LoginOk, FirstReplyPass, ComReplyTimeoutIsSet, FieldsLeast;
    QTextStream *LogStream;
    QString FileHost;
    quint16 FilePort;
    qint64 WrittenBytes, ReadBytes, MsgNum;
    int CurrentCommand, RcvDataSize, XmitDataSize;
    FILE *fp;
    size_t filesize;
    size_t filepos;
    int FieldsNum;
    int ResultType;
    int FieldsLeastToAdd;

    QString RemoveSpaces(QString str);
    void WriteErrorAndBreakReceiving(QString ErMsg);
    QStringList SeparateBuf(QByteArray &buf);
    int CheckArgs(QString cmd, QStringList &Args, int argsnum, bool fieldscheck=false, bool pairscheck=false);

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
