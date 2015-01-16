#include "../inc/s_tqcheckbox.h"

s_tqCheckBox::s_tqCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

void s_tqCheckBox::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqCheckBox::getAData()
{
    return this->adata;
}

