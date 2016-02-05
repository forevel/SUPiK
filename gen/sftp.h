#ifndef S_FTP_H
#define S_FTP_H

#include <QObject>
#include <QtFtp/QtFtp>
#include <QIODevice>
#include <QTimer>
#include <QtFtp/QUrlInfo>
#include <QStack>

#include "publicclass.h"

#define FTP_SERVER  "ftp.asu-vei.ru"
#define CHECKFILE   "xmHXP_FW~h"

#define SFTPER(a)   ERMSG(PublicClass::ER_SFTP,__LINE__,a)
#define SFTPWARN    WARNMSG(PublicClass::ER_SFTP,__LINE__)
#define SFTPINFO(a) INFOMSG(PublicClass::ER_SFTP,__LINE__,a)

class s_ftp : public QObject
{
    Q_OBJECT
public:
    explicit s_ftp(QObject *parent=0);
    ~s_ftp();

    QByteArray ReadData;
    QList<QUrlInfo> FileList;
    QStack<QString> CurrentDirectory;
    QFile *ReadFile;

    bool CheckFtp();
    void PutFile(QString Url, QString filename);
    void GetData(QString Url);
    void GetFile(QString Url, QString filename);
    void ChangeDir(QString dir);
    void ListDir(); // прочитать текущую директорию, список файлов - в FileList

signals:
    void NewDataAvailable();
    void ReadBufferFull();
    void ReadFinished();
    void WriteFinished();
    void SetRange(int, int);
    void SetValue(int);

private:
    bool Busy, RangeWasSent, Connected, CdFailed;
    quint32 RDptr;
    QFtp *ftp;
    QTimer *Tmr;
    QMetaObject::Connection FtpListConnection;
    QString DirToCD;

    void ConnectToFtp();
    void FtpDisconnect();
    void MakeDir(QString DirName);

private slots:
    void ReadDataFromUrl();
    void TransferFinished(int, bool error);
    void SetRangeAndValue(qint64 Value, qint64 Total);
    void FtpTimeout();
    void AddToFileList(QUrlInfo FileInfo);
};

extern s_ftp sftp;

#endif // S_FTP_H
