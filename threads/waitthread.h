#ifndef WAITTHREAD_H
#define WAITTHREAD_H

#include <QThread>

class WaitThread : public QThread
{
    Q_OBJECT

public:
    void run();

signals:
    void TenMsPassed();
};

#endif // WAITTHREAD_H
