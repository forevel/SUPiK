#ifndef CHECKTHREAD_H
#define CHECKTHREAD_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QSettings;
class QSqlDatabase;
class QColor;
QT_END_NAMESPACE

#include "../gen/publicclass.h"

class CheckThread : public QObject
{
    Q_OBJECT
public:
    explicit CheckThread(QObject *parent=0);
    ~CheckThread();

    QTimer *MainTimer;
    int notify; // активные уведомления (см. PR_-defines выше)
    bool NewNotifyHasArrived; // признак поступления новых с момента последнего опроса событий
    bool Acknowledged; // признак того, что сообщение о поступлении новых событий мы уже выдавали
    bool UpdateInProgress; // признак того, что список сообщений обновляется
    bool IsAboutToFinish;

    void MainCheck();

public slots:
    void Start();
    void Finish();

signals:
    void finished();

private:
    void AddProblemToList(PublicClass::ProblemStruct prob);
    void Wait(int msec);
    QSqlDatabase OpenDB(QString dbid, QString dbname);

private slots:
    void MinuteTest();

};

#endif // CHECKTHREAD_H
