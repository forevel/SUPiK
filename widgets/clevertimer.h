/* Class CleverTimer derived from QLabel
 * It's an implementation of label presenting the value of timer with such functionality as two count direction,
 * boundaries, signal emitting when reaching the boundary, several output format of timer value and much more.
 * Usage:
 *          CleverTimer *Tmr = new CleverTimer;
 *          Tmr->SetDirection(COUNT_UP);
 *          Tmr->Init(TIMER_SIMPLE);
 *          Tmr->SetBoundaries(0, 1000);
 *          QHBoxLayout *hlyout = new QHBoxLayout;
 *          hlyout->addWidget(Tmr);
 *          setLayout(hlyout);
 *          Tmr->Start();
 * */

#ifndef CLEVERTIMER_H
#define CLEVERTIMER_H

#include <QLabel>
#include <QTimer>
#include <QTime>

#define FONT_NORMAL 0
#define FONT_BOLD   1
#define FONT_ITALIC 2
#define FONT_UNDERLINE  4

#define FONT_MAXPIXELSIZE   72

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

    enum TimerColors
    {
        COLOR_NORMAL, // normal black color
        COLOR_WARN, // yellow warning color
        COLOR_ALARM, // red warning color
        COLOR_GREEN // green color
    };

    void SetDirection(int dir);
    void Init(int type);
    void SetBoundaries(quint64 min, quint64 max); // min & max in milliseconds
    void SetFont(int type, int size); // set font of timer output
    void SetColor(int color); // set color of timer output foreground
    void SetThreshold(quint64 threshold);
    int Status(); // returns timer status

public slots:
    void Start();
    void Stop();
    void Clear(); // reset TimerCounter to initial value

signals:
    void Finished(); // timer finished counting

private:
    int CountDirection;
    int TimerType;
    QTime Min, Max, Thr;
    QTimer *Tmr;
    int TimerState;
    QTime Tme;
    bool ThresholdActivated;

private slots:
    void TextUpdate();
};

#endif // CLEVERTIMER_H
