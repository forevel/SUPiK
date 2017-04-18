#include "s_statusbar.h"
#include <QPixmap>
#include <QHBoxLayout>
#include <QStyleOptionFrameV2>
#include "s_tqlabel.h"
#include "wd_func.h"

s_StatusBar::s_StatusBar(QWidget *parent) : QWidget(parent)
{
//    setStyleSheet("QWidget {border: 1px black; background-color: rgb(234, 234, 214); \
//                                  font: bold;}");
    setAttribute(Qt::WA_DeleteOnClose);
    QHBoxLayout *hlyout = new QHBoxLayout;
    hlyout->addStretch(600);
    s_tqLabel *lbl = new s_tqLabel;
    lbl->setObjectName("incbytes");
    lbl->setStyleSheet("QLabel {color: green;}");
    hlyout->addWidget(lbl);
    lbl = new s_tqLabel("/");
    hlyout->addWidget(lbl);
    lbl = new s_tqLabel;
    lbl->setObjectName("outbytes");
    lbl->setStyleSheet("QLabel {color: #999933;}");
    hlyout->addWidget(lbl);
    setLayout(hlyout);
    IncBytes = OutBytes = 0;
}

s_StatusBar::~s_StatusBar()
{

}

void s_StatusBar::paintEvent(QPaintEvent *e)
{
    e->accept();
}

void s_StatusBar::UpdateIncomeBytes(quint64 bytes)
{
    IncBytes += bytes;
    WDFunc::SetLBLText(this, "incbytes", "Х: " + QString::number(IncBytes));
}

void s_StatusBar::UpdateOutgoingBytes(quint64 bytes)
{
    OutBytes += bytes;
    WDFunc::SetLBLText(this, "outbytes", "Ы: " + QString::number(OutBytes));
}

void s_StatusBar::SetRetrCounter(int value)
{

}

void s_StatusBar::DisableRetrCounter()
{

}

void s_StatusBar::SetServerStatusOffline()
{

}

void s_StatusBar::SetServerStatusOnline()
{

}
