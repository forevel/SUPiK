#include "../inc/s_transparentqlabel.h"

s_transparentQLabel::s_transparentQLabel(QWidget *parent) :
    QLabel(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

void s_transparentQLabel::paintEvent(QPaintEvent *event)
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
