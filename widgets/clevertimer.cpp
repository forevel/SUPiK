#include "clevertimer.h"

#include <algorithm>

CleverTimer::CleverTimer(QWidget *parent) : QLabel(parent)
{
    TimerType = TIMER_SIMPLE;
    CountDirection = COUNT_UP;
    Min = 0;
    Max = 1000;
}

CleverTimer::~CleverTimer()
{
    delete tmr;
}

void CleverTimer::Init(int type)
{
    TimerType = type;
    tmr = new QTimer;
    if (type == TIMER_SIMPLE)
        tmr->setInterval(1000);
    else if (type == TIMER_SIMPMS)
        tmr->setInterval(10);
    connect(tmr,SIGNAL(timeout()),this,SLOT(TextUpdate()));
}

void CleverTimer::SetDirection(int dir)
{
    CountDirection = dir;
}

void CleverTimer::SetBoundaries(quint64 min, quint64 max)
{
    Min = min;
    Max = max;
    if (Min > Max) std::swap(Min, Max);
    if (Min == Max) TimerState = TIMER_STOPPED;
    else TimerState = TIMER_READY;
}
