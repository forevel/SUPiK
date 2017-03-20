#include "s_statusbar.h"
#include <QPixmap>
#include <QHBoxLayout>
#include "s_tqlabel.h"
#include "wd_func.h"

s_StatusBar::s_StatusBar(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
//    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QHBoxLayout *hlyout = new QHBoxLayout;
    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel;
    lbl->setObjectName("incbytes");
    lbl->SetColor(Qt::darkGreen);
    hlyout->addWidget(lbl, 10);
    lbl = new s_tqLabel(" / ");
    hlyout->addWidget(lbl, 10);
    lbl = new s_tqLabel;
    lbl->setObjectName("outbytes");
    lbl->SetColor(Qt::darkYellow);
    hlyout->addWidget(lbl, 10);
    setLayout(hlyout);
    IncBytes = OutBytes = 0;
}

s_StatusBar::~s_StatusBar()
{

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

/*
void s_StatusBar::Start()
{
    UTmr = new QTimer;
    UTmr->setInterval(1000);
    connect(UTmr,SIGNAL(timeout()),this,SLOT(ClearUpload()));
    DTmr = new QTimer;
    DTmr->setInterval(1000);
    connect(DTmr,SIGNAL(timeout()),this,SLOT(ClearDownload()));
}

void s_StatusBar::UploadActive()
{
    if (UploadIsActive)
        return;
    UploadIsActive = true;
    QLabel *lbl = this->findChild<QLabel *>("s_StatusBarlabel");
    if (lbl == 0)
        return;
    if (DownloadIsActive)
    {
        QPixmap OnPmap(":/res/2on.png");
        lbl->setPixmap(OnPmap);
    }
    else
    {
        QPixmap UpOnPmap(":/res/1ud.png");
        lbl->setPixmap(UpOnPmap);
    }
    UTmr->start();
}

void s_StatusBar::DownloadActive()
{
    if (DownloadIsActive)
        return;
    DownloadIsActive = true;
    QLabel *lbl = this->findChild<QLabel *>("s_StatusBarlabel");
    if (lbl == 0)
        return;
    if (UploadIsActive)
    {
        QPixmap OnPmap(":/res/2on.png");
        lbl->setPixmap(OnPmap);
    }
    else
    {
        QPixmap UpOnPmap(":/res/1dd.png");
        lbl->setPixmap(UpOnPmap);
    }
    DTmr->start();
}

void s_StatusBar::ClearUpload()
{
    UTmr->stop();
    UploadIsActive = false;
    QLabel *lbl = this->findChild<QLabel *>("s_StatusBarlabel");
    if (lbl == 0)
        return;
    if (DownloadIsActive)
    {
        QPixmap OnPmap(":/res/1dd.png");
        lbl->setPixmap(OnPmap);
    }
    else
    {
        QPixmap UpOnPmap(":/res/2off.png");
        lbl->setPixmap(UpOnPmap);
    }
}

void s_StatusBar::ClearDownload()
{
    DTmr->stop();
    DownloadIsActive = false;
    QLabel *lbl = this->findChild<QLabel *>("s_StatusBarlabel");
    if (lbl == 0)
        return;
    if (UploadIsActive)
    {
        QPixmap OnPmap(":/res/1ud.png");
        lbl->setPixmap(OnPmap);
    }
    else
    {
        QPixmap UpOnPmap(":/res/2off.png");
        lbl->setPixmap(UpOnPmap);
    }
}
*/
