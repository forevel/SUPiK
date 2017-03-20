#include "clevertimer.h"

#include <algorithm>

CleverTimer::CleverTimer(QWidget *parent) : QLabel(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    TimerType = TIMER_SIMPLE;
    CountDirection = COUNT_UP;
    ThresholdActivated = false;
    Min.setHMS(0,0,0);
    Max.setHMS(0,0,1,0); // 1000 ms
}

CleverTimer::~CleverTimer()
{
    delete Tmr;
}

void CleverTimer::Init(int type)
{
    TimerType = type;
    Tmr = new QTimer;
    if ((type == TIMER_SIMPLE) || (type == TIMER_SIMPLIM))
        Tmr->setInterval(1000);
    else if (type == TIMER_SIMPMS)
        Tmr->setInterval(10);
    connect(Tmr,SIGNAL(timeout()),this,SLOT(TextUpdate()));
}

void CleverTimer::SetDirection(int dir)
{
    CountDirection = dir;
}

void CleverTimer::SetThreshold(quint64 threshold)
{
    ThresholdActivated = true;
    Thr.setHMS(0,0,0);
    Thr = Thr.addMSecs(threshold);
}

void CleverTimer::SetBoundaries(quint64 min, quint64 max)
{
    Min.setHMS(0,0,0);
    Min = Min.addMSecs(min);
    Max.setHMS(0,0,0);
    Max = Max.addMSecs(max);
    if (Min > Max) std::swap(Min, Max);
    if (Min == Max) TimerState = TIMER_STOPPED;
    else TimerState = TIMER_READY;
    Clear();
}

void CleverTimer::SetFont(int type, int size)
{
    QFont font;
    font.setBold(type&FONT_BOLD);
    font.setItalic(type&FONT_ITALIC);
    font.setUnderline(type&FONT_UNDERLINE);
    if (size < FONT_MAXPIXELSIZE)
        font.setPixelSize(size);
    setFont(font);
}

void CleverTimer::SetColor(int color)
{
    QColor tmpc;
    QPalette pal = this->palette();

    switch (color)
    {
    case COLOR_ALARM:
        tmpc = Qt::red;
        break;
    case COLOR_WARN:
        tmpc = Qt::yellow;
        break;
    case COLOR_NORMAL:
        tmpc = Qt::black;
        break;
    case COLOR_GREEN:
        tmpc = Qt::darkGreen;
        break;
    default:
        tmpc = Qt::darkBlue;
        break;
    }

    pal.setColor(this->foregroundRole(), tmpc);
    setPalette(pal);
}

void CleverTimer::Start()
{
    Tmr->start();
    TimerState = TIMER_RUNNING;
}

void CleverTimer::Stop()
{
    Tmr->stop();
    TimerState = TIMER_STOPPED;
}

void CleverTimer::Clear()
{
    if (CountDirection == COUNT_UP)
        Tme = Min;
    else
        Tme = Max; // clear time
}

int CleverTimer::Status()
{
    return TimerState;
}

void CleverTimer::TextUpdate()
{
    switch (TimerType)
    {
    case TIMER_SIMPLIM:
    {
        if ((CountDirection == COUNT_UP) && (Tme >= Max))
        {
            emit Finished();
            Stop();
            return;
        }
        if ((CountDirection == COUNT_DOWN) && (Tme <= Min))
        {
            emit Finished();
            Stop();
            return;
        }
    }
    case TIMER_SIMPLE:
    {
        if (CountDirection == COUNT_UP)
        {
            Tme = Tme.addSecs(1);
            if (Tme >= Thr)
                SetColor(COLOR_ALARM);
        }
        else
        {
            Tme = Tme.addSecs(-1);
            if (Tme <= Thr)
                SetColor(COLOR_ALARM);
        }
        setText(Tme.toString("hh:mm:ss"));
        break;
    }
    case TIMER_SIMPMS:
    {
        if (CountDirection == COUNT_UP)
            Tme = Tme.addMSecs(10);
        else
            Tme = Tme.addMSecs(-10);
        setText(Tme.toString("hh:mm:ss:zzz"));
        break;
    }
    }
}
