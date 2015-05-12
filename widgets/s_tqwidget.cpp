#include "s_tqwidget.h"
#include <QPainter>
#include <QStyleOption>

s_tqwidget::s_tqwidget(QWidget *parent) :
    QWidget(parent)
{
//    setAttribute(Qt::WA_TranslucentBackground);
}

void s_tqwidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void s_tqwidget::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqwidget::getAData()
{
    return this->adata;
}
