#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include "../threads/waitthread.h"

class WaitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WaitWidget(QWidget *parent = 0);
    ~WaitWidget();
    bool Finished;

    void Start();
    void Stop();
    void SetMessage(QString msg);

signals:
    void finished();

public slots:

private slots:
    void Rotate();

private:
    WaitThread *thr;
    QString Message;
    float gamma;

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // WAITWIDGET_H
