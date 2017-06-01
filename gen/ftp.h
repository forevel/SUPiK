#ifndef FTP_H
#define FTP_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include "../threads/ethernet.h"
#include "publicclass.h"

#define PORTFTP     21 // порт связи по протоколу FTP

#define FTP_INITIALIZED 100 // Запрошенное действие инициировано, дождитесь следующего ответа прежде, чем выполнять новую команду.
#define FTP_COMMENT     110	// Комментарий
#define FTP_DELAYED     120	// Функция будет реализована через nnn минут
#define FTP_CHANOPENED  125 // Канал открыт, обмен данными начат
#define FTP_FILESTATOK  150 // Статус файла правилен, подготавливается открытие канала
#define FTP_CMDOK       200	// Команда корректна
#define FTP_CMD_NIMPLEM 202 // Команда не поддерживается
#define FTP_SYS_STATUS  211 // Системный статус или отклик на справочный запрос
#define FTP_CTG_STATUS  212 // Состояние каталога
#define FTP_FILE_STATUS 213 // Состояние файла
#define FTP_INFO_MSG    214 // Справочное поясняющее сообщение
#define FTP_SYSINFO     215 // Выводится вместе с информацией о системе по команде SYST
#define FTP_FTPREADY    220 // Служба готова для нового пользователя.
#define FTP_QUITOK      221 // Благополучное завершение по команде quit
#define FTP_CHANOK      225 // Канал сформирован, но информационный обмен отсутствует
#define FTP_CHANCLOSED  226 // Закрытие канала, обмен завершен успешно
#define FTP_PASV_ENAB   227 // Переход в пассивный режим (h1,h2,h3,h4,p1,p2).
#define FTP_LONG_PASV   228 // переход в длинный пассивный режим (длинный адрес, порт).
#define FTP_EXT_PASV    229 // Переход в расширенный пассивный режим (|||port|).
#define FTP_USEROK      230 // Пользователь идентифицирован, продолжайте
#define FTP_SESS_CLOSED 231 // Пользовательский сеанс окончен; Обслуживание прекращено.
#define FTP_CLS_PEND    232 // Команда о завершении сеанса принята, она будет завершена по завершении передачи файла.
#define FTP_QUERYOK     250 // Запрос прошёл успешно
#define FTP_PATH_CREAT  257 // «ПУТЬ» создан.
#define FTP_USEROK_PSW  331 // Имя пользователя корректно, нужен пароль
#define FTP_AUTH_NEED   332 // Для входа в систему необходима аутентификация
#define FTP_MOREINFO_ND 350 // Запрошенное действие над файлом требует большей информации
#define FTP_NOTFOUND    404 // Данный удалённый сервер не найден
#define FTP_PROC_INCOR  421 // Процедура невозможна, канал закрывается
#define FTP_INFOCH_ER   425 // Открытие информационного канала не возможно
#define FTP_CH_CLOSED   426 // Канал закрыт, обмен прерван
#define FTP_HOSTNFOUND  434 // Запрашиваемый хост недоступен
#define FTP_FILE_DENIED 450 // Запрошенная функция не реализована, файл не доступен, например, занят
#define FTP_LOCALERROR  451 // Локальная ошибка, операция прервана
#define FTP_WRITEFILEER 452 // Ошибка при записи файла (недостаточно места)
#define FTP_CMDSYNTAXER 500 // Синтаксическая ошибка, команда не может быть интерпретирована (возможно она слишком длинна)
#define FTP_ARGSYNTAXER 501 // Синтаксическая ошибка (неверный параметр или аргумент)
#define FTP_CMD_NOTUSED 502 // Команда не используется (нелегальный тип MODE)
#define FTP_CMD_SEQER   503 // Неудачная последовательность команд
#define FTP_CMD_ER      504 // Команда не применима для такого параметра
#define FTP_LOGINER     530 // Вход не выполнен! Требуется авторизация (not logged in)
#define FTP_FILEAUTH    532 // Необходима аутентификация для запоминания файла
#define FTP_FILENFOUND  550 // Запрошенная функция не реализована, файл недоступен, например, не найден
#define FTP_OPER_CANCEL 551 // Запрошенная операция прервана. Неизвестный тип страницы.
#define FTP_OUT_MEMORY  552 // Запрошенная операция прервана. Выделено недостаточно памяти
#define FTP_FILENAMEER  553 // Запрошенная операция не принята. Недопустимое имя файла.

class Ftp : public QObject
{
    Q_OBJECT
public:
    explicit Ftp(QObject *parent=0);
    ~Ftp();

    bool Connect(QString Host);
    bool Login(QString User, QString Password);
    bool ChDir(QString Dir);
    bool MkDir(QString Dir);
    bool List();
    bool SendFile(QString Filename, QByteArray *ba, int size = 0);
    bool GetFile(QString Filename, QByteArray *ba, int size = 0);
    void Disconnect();

public slots:
    void StopThreads();

signals:
    void FtpSend(QByteArray *);
    void FileSend(QByteArray *);
    void BytesOverall(qint64 bytes);
    void BytesRead(qint64 bytes);
    void BytesWritten(qint64 bytes);

private:
    QByteArray *BufData, *RcvData, *XmitData;
    Ethernet *MainEthernet, *FileEthernet;
    QTimer *TimeoutTimer, *GetComReplyTimer, *GetFileTimer;
    bool Busy, FileBusy, Connected, FileConnected, CmdOk, CanLog;
    QFile *LogFile;
    QTextStream *LogStream;
    QString FileHost;
    quint16 FilePort;
    qint64 WrittenBytes, ReadBytes;
    int CurrentCommand, RcvDataSize, XmitDataSize;

    enum Commands
    {
        CMD_USER, CMD_PASS, CMD_CDUP, CMD_CWD, CMD_DELE, CMD_LIST, CMD_PASV, CMD_MKD, \
        CMD_PWD, CMD_QUIT, CMD_REIN, CMD_RETR, CMD_RMD, CMD_STOR, CMD_TYPE, CMD_START
    };

    void StartFtp(QString Host, quint16 Port);
    bool StartPASV(int Command, QString Filename = "", QByteArray *ba = 0, int size = 0);
    bool SendCmd(int Command, QString Args="");

private slots:
    void FtpGet(QByteArray *);
    void FileGet(QByteArray *);
    void FtpConnected();
    void FtpFileConnected();
    void Timeout();
    void FtpErr(int error);
    void SetBytesWritten(qint64 bytes);
    void ParseReply();
    void GetFileTimerTimeout();
};

extern Ftp *Ftps;

#endif // FTP_H
