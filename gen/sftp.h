#ifndef S_FTP_H
#define S_FTP_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "publicclass.h"

#define FTP_SERVER  "ftp://ftp.asu-vei.ru"

#define SFTPER(a)   ERMSG(PublicClass::ER_SFTP,__LINE__,a)
#define SFTPWARN    WARNMSG(PublicClass::ER_SFTP,__LINE__)

class s_ftp : public QObject
{
    Q_OBJECT
public:
    s_ftp(QObject *parent = 0);

    QByteArray ReadData;
    bool Busy;

    bool IsFtpAvailable();

signals:
    void NewDataAvailable();
    void ReadBufferFull();
    void ReadFinished();

private:
    void GetFile(QString Url);
    bool CheckForFtp();

private slots:
    void SetError(QNetworkReply::NetworkError err);
    void ReadDataFromUrl();
    void FinishRead();
};

extern s_ftp sftp;

#endif // S_FTP_H
