#include "../inc/s_tqspinbox.h"

s_tqspinbox::s_tqspinbox(QWidget *parent) :
    QSpinBox(parent)
{
    setStyleSheet("QSpinBox {background-color: transparent;}");
}

void s_tqspinbox::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqspinbox::getAData()
{
    return this->adata;
}
