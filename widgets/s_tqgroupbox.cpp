#include "s_tqgroupbox.h"

s_tqGroupBox::s_tqGroupBox(int r, int g, int b, int a, QWidget *parent) :
    QGroupBox(parent)
{
    setStyleSheet("QGroupBox {background-color: rgba("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+","+QString::number(a)+");"
                  "border: 2px solid gray; border-radius: 10px; margin-top: 5px;}"
                  "QGroupBox::title {top: -6px; left: 15px; padding-left: 5px; padding-right: 5px;}");
    setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}

void s_tqGroupBox::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqGroupBox::getAData()
{
    return this->adata;
}
