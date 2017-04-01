#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QMap>
#include "../threads/ethernet.h"
#include "publicclass.h"
#include "log.h"

#define SERVERRSTR  "ERROR"
#define SERVEMPSTR  "EMPTY"
#define SERVRETSTR  "RETRY"
#define SERVIDLSTR  "IDLE"
#define SERVEROK    "OK"

#define TIMERSOFF // если не закомментировано, таймауты отключены
#define SLNUMMAX    10 // максимальное число полей в запросе по столбцам
#define TOKEN       0x7F // разделитель

// M-commands (main)
#define M_COMMAND   1000
#define M_QUIT		1001 // подтверждение завершения сеанса связи
#define M_GROUP		1002 // группа доступа
#define M_STATUS	1003
#define M_STATS		1004
#define M_LOGIN		1005 // запрос имени пользователя
#define M_PSW		1006 // запрос пароля (зарез.)
#define M_PUTFILE   1007 // отправка файла на сервер
#define M_GETFILE   1008 // прием файла с сервера
#define M_ACTIVATE  1009 // активация пользователя
// M-statuses
#define M_IDLE		1000
#define M_ANEXT     1047
#define M_RDY		1051
#define M_ERROR		1052
#define M_NEXT		1053 // подтверждение готовности приёма следующей порции данных
#define M_AGETFILE  1092
#define M_APUTFILE  1093
#define M_ANSPSW	1094
#define M_ANSLOGIN	1095
#define M_BYE		1099

// S-commands (sql)
#define S_COMMAND   1100
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
#define T_COMMAND   1200
#define T_GVSBFS	1201 // ValuesByFields
#define T_GVSBC		1202 // GetValuesByColumn
#define T_GVSBCF	1203 // GetValuesByColumnAndField
#define T_C         1204 // TFCheck
#define T_DEL		1205
#define T_RDEL		1206
#define T_GFT		1207 // GetFullTable (GetAllValuesFromTable)
#define T_GID		1208
#define T_IDTV		1209
#define T_TV		1210
#define T_IDTVL		1211
#define T_TID		1212
#define T_VTID		1213
#define T_INS		1214
#define T_UPD		1215 // "?"
#define T_TF        1216 // "@"
#define T_TH        1217
#define T_TL        1218
#define T_UPDV      1219 // "C"

#define T_END       1299
// C-commands (components)
#define C_CRALT		1301

// описания типов файлов
#define FLT_TB       "0"
#define FLT_DOC      "1"
#define FLT_ALT      "2"
#define FLT_PERS     "3"

// описания подтипов файлов
#define FLST_PROT     "0" // протоколы
#define FLST_TECHDOC  "1" // техдокументация
#define FLST_LIBS     "2" // библиотеки
#define FLST_LIBSYM   "3" // библиотеки Altium - SchLib
#define FLST_LIBFOOT  "4" // библиотеки Altium - PcbLib
#define FLST_PHOTO    "5" // фотографии

#define READBUFMAX  16384

// incoming sizes
#define SZ_RDY  4 // "RDY\n"
#define SZ_PUTFILE  3 // "OK\n"
#define SZ_DUMMY    1 // dummy size when we're in concern that the incoming packet size will be less than 4096 bytes long

#define GETTIMEOUT  3000 // таймаут на приём файла - 3 секунды
#define MAINTIMEOUT 5000 // таймаут на ответ от сервера - 5 секунд
#define DATATIMEOUT 10000 // таймаут на приём - 3 секунды

#define MAINSLEEP   50  // количество мс сна в процессах
#define CL_MAXRETRCOUNT    3 // максимальное количество попыток повторить команду
#define CL_RETR1       1
#define CL_RETR2       2
#define CL_RETR3       4
#define CL_RETR4       8
#define CL_RETR5       16
#define CL_RETR6       32
#define CL_RETR7       64 // 64 seconds is the maximum time period between two connection retries
#define CL_MAXRETR     7 // maximum number of retry time periods

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
        RESULT_NONE,    // без результата
        RESULT_INT      // результат - целое число
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
        CLIER_PUTFER,    // ошибка отправки файла
        CLIER_WRANSW,    // кривой ответ от сервера
        CLIER_CMDER,     // ошибка обработки команды
        CLIER_EMPTY,     // пустой ответ
        CLIER_BUSY      // предыдущий запрос ещё не обработан
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
        CMD_DIRLIST, // запрос списка файлов в каталоге
        CMD_STATUS  // запрос статуса от сервера
    };

    enum ClientModes
    {
        CLIMODE_TEST,   // тестовый режим с тестовыми логином и паролем - нужно для активации
        CLIMODE_WORK    // основной режим работы
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

    QList<QStringList> Result;
    QString ResultStr;
    int ResultInt;

    int Connect(QString host, QString port, int clientmode);
    bool isConnected();
    void Disconnect();
    void SendCmd(int command, QStringList &args=QStringList());
    int GetFile(const QString &type, const QString &subtype, const QString &filename);
    int PutFile(const QString &localfilename, const QString &type, const QString &subtype, const QString &filename);
    int SendAndGetResult(int command, QStringList &args=QStringList()); // send command with arguments and wait for result
    void StartLog();

public slots:

signals:
    void ClientSend(QByteArray);
    void BytesOverall(quint64 bytes);
    void BytesRead(quint64 bytes);
    void BytesWritten(quint64 bytes);
    void DataReady(QStringList &); // очередная порция данных готова
    void TransferComplete(); // окончание приёма/передачи файла

private:
    QMap<int, CmdStruct> CmdMap;
    const QStringList PathPrefixes = QStringList() << "tb/" << "doc/" << "alt/" << "pers/";
    const QStringList PathSuffixes = QStringList() << "prot/" << "dsheet/" << "libs/" << "symbols/" << "footprints/" << "photo/";
    int RetryTimePeriods[CL_MAXRETR];
    QByteArray RcvData, WrData;
    Ethernet *MainEthernet, *FileEthernet;
    QTimer *TimeoutTimer, *GetComReplyTimer, *GetFileTimer, *RetrTimer; // general timeout, timer for server reply, getfile timer and timer to retry connection
    bool FileBusy, Connected, CmdOk, LoginOk, ComReplyTimeoutIsSet;
    QString Host, Port, FileHost;
    quint16 FilePort;
    quint64 WrittenBytes, ReadBytes, RcvDataSize, XmitDataSize;
    int CurrentCommand, CliMode, LastCommand;
    QFile fp;
    int ClientMode; // mode = CLIMODE_TEST or CLIMODE_WORK
    int FieldsNum;
    int ResultType;
    QString Pers, Pass;
    QByteArray PrevLastBA; // last string in previous ethernet received chunk to be concatenated with the first string from the next chunk
    Log *CliLog;
    int TimeoutCounter; // counter of timeouts, when it equals 3 the connection is forced reconnected
    int RetryCount; // counter of retry reply from server to make disconnection
    int CurRetrPeriod; // index of current retry time period from RetryTimePeriods array
    QStringList LastArgs; // Args vector that was last used in SendCmd (for proper retrying)
    bool RetryActive; // flag indicates that retrying active
    bool NextActive; // flag indicates that we have already a result size and this is a 2,3... chunks
    bool Busy;
    int DetectedError;

    QString RemoveSpaces(QString str);
    void Error(QString ErMsg, int ErrorInt=CLIER_GENERAL);
    bool CheckArgs(QString cmd, QStringList &args, int argsnum, bool fieldscheck=false, bool pairscheck=false);
    void InitiateTimers();

private slots:
    void ClientConnected();
    void ClientDisconnected();
    void Timeout();
    void ClientErr(int error);
    void ParseReply(QByteArray ba);
    void GetFileTimerTimeout();
    void ComReplyTimeout();
    void RetrTimeout();
};

extern Client *Cli;

#endif // CLIENT_H
