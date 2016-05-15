#include "portactivity.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <QLabel>

PortActivity::PortActivity(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
//    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QVBoxLayout *vlyout = new QVBoxLayout;
    QPixmap pmap (":/res/2off.png");
    QLabel *lbl = new QLabel;
    lbl->setPixmap(pmap);
    lbl->setObjectName("portactivitylabel");
    vlyout->addWidget(lbl);
    setLayout(vlyout);
}

PortActivity::~PortActivity()
{

}

void PortActivity::Start()
{
    UTmr = new QTimer;
    UTmr->setInterval(1000);
    connect(UTmr,SIGNAL(timeout()),this,SLOT(ClearUpload()));
    DTmr = new QTimer;
    DTmr->setInterval(1000);
    connect(DTmr,SIGNAL(timeout()),this,SLOT(ClearDownload()));
}

void PortActivity::UploadActive()
{
    if (UploadIsActive)
        return;
    UploadIsActive = true;
    QLabel *lbl = this->findChild<QLabel *>("portactivitylabel");
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

void PortActivity::DownloadActive()
{
    if (DownloadIsActive)
        return;
    DownloadIsActive = true;
    QLabel *lbl = this->findChild<QLabel *>("portactivitylabel");
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

void PortActivity::ClearUpload()
{
    UTmr->stop();
    UploadIsActive = false;
    QLabel *lbl = this->findChild<QLabel *>("portactivitylabel");
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

void PortActivity::ClearDownload()
{
    DTmr->stop();
    DownloadIsActive = false;
    QLabel *lbl = this->findChild<QLabel *>("portactivitylabel");
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
