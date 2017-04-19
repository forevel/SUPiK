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
    Sf = new QPixmap(":/res/server_offline.png");
    Sna = new QPixmap(":/res/server_online_active.png");
    Snp = new QPixmap(":/res/server_online_passive.png");
    QHBoxLayout *hlyout = new QHBoxLayout;
    hlyout->addStretch(600);
    s_tqLabel *lbl = new s_tqLabel;
    lbl->setObjectName("retrcounter");
    lbl->setText("---");
    lbl->setStyleSheet("QLabel {color: red;}");
//    lbl->setEnabled(false);
    hlyout->addWidget(lbl);
    lbl = new s_tqLabel;
    lbl->setObjectName("serverstatus");
    lbl->setPixmap(*Sf);
    hlyout->addWidget(lbl);
    lbl = new s_tqLabel;
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
    ActiveTimer = new QTimer;
    ActiveTimer->setInterval(500);
    ActiveTimer->setSingleShot(true);
    connect(ActiveTimer,SIGNAL(timeout()),this,SLOT(ClearActiveServerState()));
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
    WDFunc::SetLBLImage(this, "serverstatus", Sna);
    ActiveTimer->start();
}

void s_StatusBar::UpdateOutgoingBytes(quint64 bytes)
{
    OutBytes += bytes;
    WDFunc::SetLBLText(this, "outbytes", "Ы: " + QString::number(OutBytes));
    WDFunc::SetLBLImage(this, "serverstatus", Sna);
    ActiveTimer->start();
}

void s_StatusBar::ClearActiveServerState()
{
    WDFunc::SetLBLImage(this, "serverstatus", Snp);
}

void s_StatusBar::SetRetrCounter(int value)
{
    SetServerStatusOffline();
    WDFunc::SetLEData(this, "retrcounter", QString::number(value));
}

void s_StatusBar::DisableRetrCounter()
{
    SetServerStatusOnline();
    WDFunc::SetLEData(this, "retrcounter", "---");
}

void s_StatusBar::SetServerStatusOffline()
{
    WDFunc::SetLBLImage(this, "serverstatus", Sf);
}

void s_StatusBar::SetServerStatusOnline()
{
    WDFunc::SetLBLImage(this, "serverstatus", Snp);
}
