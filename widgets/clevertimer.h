/* Class CleverTimer derived from QLabel
 * It's an implementation of label presenting the value of timer with such functionality as two count direction,
 * boundaries, signal emitting when reaching the boundary, several output format of timer value and much more.
 * Usage:
 *          CleverTimer *tmr = new CleverTimer;
 *          tmr->Init(TIMER_SIMPLE);
 *          tmr->SetDirection(COUNT_UP);
 *          tmr->SetBoundaries(0, 1000);
 *          QHBoxLayout *hlyout = new QHBoxLayout;
 *          hlyout->addWidget(tmr);
 *          setLayout(hlyout);
 *          tmr->Start();
 * */

#ifndef CLEVERTIMER_H
#define CLEVERTIMER_H

#include <QLabel>
#include <QTimer>

class CleverTimer : public QLabel
{
    Q_OBJECT
public:
    explicit CleverTimer(QWidget *parent=0);
    ~CleverTimer();

    enum CountDirections
    {
        COUNT_UP,
        COUNT_DOWN
    };

    enum TimerTypes
    {
        TIMER_SIMPLE, // simple timer counts from Min to endless or from Max to zero depends of CountDirection, output is like hh:mm:ss
        TIMER_SIMPMS, // same as the above but the format is like mm:ss:ms
        TIMER_SIMPLIM // same as TIMER_SIMPLE but with limit: either from Min to Max or from Max to Min
    };

    enum TimerStates
    {
        TIMER_READY, // timer is ready to start
        TIMER_STOPPED, // timer is reached the boundary
        TIMER_RUNNING // timer is running and counting
    };

    void SetDirection(int dir);
    void Init(int type);
    void SetBoundaries(quint64 min, quint64 max); // min & max in milliseconds
    void Start();
    void Stop();

private:
    int CountDirection;
    int TimerType;
    quint64 Min, Max;
    QTimer *tmr;
    int TimerState;

private slots:
    void TextUpdate();
};

#endif // CLEVERTIMER_H
