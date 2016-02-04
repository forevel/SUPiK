#ifndef CHECKTHREAD_H
#define CHECKTHREAD_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QSettings;
class QSqlDatabase;
class QColor;
QT_END_NAMESPACE

#include "../gen/publicclass.h"

// Макросы для выдачи сообщений
#define CTHREADER(a)    ERMSG(PublicClass::ER_CTHREAD, __LINE__, a)
#define CTHREADDBG      DBGMSG(PublicClass::ER_CTHREAD,__LINE__)
#define CTHREADINFO(a)  INFOMSG(PublicClass::ER_CTHREAD, __LINE__, a)
#define CTHREADWARN     WARNMSG(PublicClass::ER_CTHREAD,__LINE__)

class CheckThread : public QObject
{
    Q_OBJECT
public:
    explicit CheckThread(QObject *parent=0);
    ~CheckThread();

    QTimer *MainTimer;
    QStringList sysMessages;
    QStringList whMessages;
    QStringList altMessages;
    QStringList tbMessages;
    QStringList sadmMessages;
//    bool probsdetected;
//    int probpos;
    int notify; // активные уведомления (см. PR_-defines выше)
    bool NewNotifyHasArrived; // признак поступления новых с момента последнего опроса событий
    bool Acknowledged; // признак того, что сообщение о поступлении новых событий мы уже выдавали
    bool UpdateInProgress; // признак того, что список сообщений обновляется

    void MainCheck();

public slots:
    void Start();

signals:
    void finished();
    void NewProblemsDetected(PublicClass::ProblemStruct &);

private:

private slots:
    void MinuteTest();

};

#endif // CHECKTHREAD_H
