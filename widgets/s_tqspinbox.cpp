#include "s_tqspinbox.h"

s_tqSpinBox::s_tqSpinBox(QWidget *parent) :
    QDoubleSpinBox(parent)
{
    setStyleSheet("QDoubleSpinBox {background-color: transparent; border: 1px solid gray; border-radius: 5px;}");
}

void s_tqSpinBox::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqSpinBox::getAData()
{
    return this->adata;
}
