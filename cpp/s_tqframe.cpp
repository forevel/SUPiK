#include "../inc/s_tqframe.h"

s_tqFrame::s_tqFrame(QWidget *parent) :
    QFrame(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

void s_tqFrame::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqFrame::getAData()
{
    return this->adata;
}
