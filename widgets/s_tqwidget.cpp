#include "s_tqwidget.h"
#include <QPainter>
#include <QStyleOption>

s_tqWidget::s_tqWidget(QWidget *parent) :
    QWidget(parent)
{
//    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QWidget {background-color: rgba(0,0,0,0);}");
}

void s_tqWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void s_tqWidget::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqWidget::getAData()
{
    return this->adata;
}
