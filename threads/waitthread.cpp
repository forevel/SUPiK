#include "waitthread.h"

#include <QTimer>

void WaitThread::run()
{
    QTimer *tmr = new QTimer;
    tmr->setInterval(10);
    connect(tmr,SIGNAL(timeout()),this,SIGNAL(TenMsPassed()));
    tmr->setSingleShot(false);
    tmr->start();
    exec();
}
