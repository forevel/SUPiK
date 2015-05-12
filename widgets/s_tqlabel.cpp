#include "s_tqlabel.h"
#include <QPainter>
#include <QPaintEvent>

s_tqLabel::s_tqLabel(QWidget *parent) :
    QLabel(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

s_tqLabel::s_tqLabel(QString txt, QWidget *parent) : QLabel(parent)
{
    setText(txt);
    setAttribute(Qt::WA_TranslucentBackground);
}

void s_tqLabel::paintEvent(QPaintEvent *event)
{
    /*QPainter paint;
    paint.begin(this);
    paint.setBrush (QBrush (QColor (255, 255, 255, 0)));
    paint.setPen (Qt::NoPen);
    paint.drawRect (0, 0, width(), height());
    paint.end();*/
    //event->accept();
    QLabel::paintEvent(event);
}

void s_tqLabel::setData(QVariant dat)
{
    this->data = dat;
}

QVariant s_tqLabel::getData()
{
    return this->data;
}
