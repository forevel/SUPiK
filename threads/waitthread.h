#ifndef WAITTHREAD_H
#define WAITTHREAD_H

#include <QObject>

class WaitThread : public QObject
{
    Q_OBJECT

public:
    explicit WaitThread(QObject *parent = 0);

    bool FinishQuery;

signals:
    void TenMsPassed();
    void Finished();

public slots:
    void Run();
    void Stop();

};

#endif // WAITTHREAD_H
