#include <QHBoxLayout>

#include "s_tqlabel.h"
#include "s_tqprogressbar.h"
#include "wd_func.h"

s_tqProgressBar::s_tqProgressBar(const QString &txt, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *hlyout = new QHBoxLayout;
    hlyout->addWidget(WDFunc::NewLBL(this, "prbtext", txt), 0);
    PRB = new QProgressBar;
    PRB->setFormat("%v out of %m");
    hlyout->addWidget(PRB);
    setLayout(hlyout);
    setAttribute(Qt::WA_TranslucentBackground);
}

void s_tqProgressBar::setText(const QString &txt)
{
    WDFunc::SetLBLText(this, "prbtext", txt);
}

void s_tqProgressBar::setValue(quint64 value)
{
    PRB->setValue(value);
}

void s_tqProgressBar::setRange(quint64 min, quint64 max)
{
    PRB->setRange(min, max);
}
