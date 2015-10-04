#include "s_tqwidget.h"
#include <QPainter>
#include <QStyleOption>

s_tqWidget::s_tqWidget(QString pic, QWidget *parent) :
    QWidget(parent)
{
//    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QWidget {background-color: rgba(0,0,0,0);}");
    this->pic = pic;
}

void s_tqWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    if (!pic.isEmpty())
        p.drawPixmap(rect(), QPixmap(pic));
}

void s_tqWidget::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqWidget::getAData()
{
    return this->adata;
}
