#include "waitwidget.h"
#include "s_tqlabel.h"
#include <QPixmap>
#include <QBitmap>
#include <QImage>
#include <QPainter>
#include <QVBoxLayout>

WaitWidget::WaitWidget(QWidget *parent) : QWidget(parent)
{
    Finished = false;
    setAttribute(Qt::WA_TranslucentBackground);
    gamma = 0.0;
    Message = "";
    setMaximumSize(200, 220);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

WaitWidget::~WaitWidget()
{

}

void WaitWidget::Start()
{
    thr = new WaitThread;
    connect(thr,SIGNAL(TenMsPassed()),this,SLOT(Rotate()));
    connect(thr,SIGNAL(finished()),thr,SLOT(deleteLater()));
    thr->start();
}

void WaitWidget::Stop()
{
    if (thr)
    {
        thr->quit();
        thr->wait();
        delete thr;
        thr = 0;
    }
    Finished = true;
    emit finished();
    this->close();
}

void WaitWidget::Rotate()
{
    gamma += 0.5;
    update();
}

void WaitWidget::paintEvent(QPaintEvent *e)
{
    QPixmap rotatedPixmap(":/res/Object1-0.png");
//    rotatedPixmap.setMask(QBitmap(rotatedPixmap));
    QSize size = rotatedPixmap.size();
    QPainter p1(this);
//    p1.setBackgroundMode(Qt::TransparentMode);
    p1.setRenderHint(QPainter::Antialiasing);
    p1.translate(size.height()/2,size.height()/2);
    p1.rotate(gamma);
    p1.translate(-size.height()/2,-size.height()/2);
    p1.drawPixmap(0, 0, rotatedPixmap);
    p1.end();
    QPixmap rotatedPixmap2(":/res/Object2.png");
    QPainter p2(this);
    p2.setRenderHint(QPainter::Antialiasing);
    p2.translate(size.height()/2,size.height()/2);
    p2.rotate(-gamma);
    p2.translate(-size.height()/2,-size.height()/2);
    p2.drawPixmap(0, 0, rotatedPixmap2);
    p2.end();
    QPixmap rotatedPixmap3(":/res/Object3.png");
    QPainter p3(this);
    p3.setRenderHint(QPainter::Antialiasing);
    p3.translate(size.height()/2,size.height()/2);
    p3.rotate(gamma/3);
    p3.translate(-size.height()/2,-size.height()/2);
    p3.drawPixmap(0, 0, rotatedPixmap3);
    p3.end();
    QPainter p(this);
    QRect mrect = QRect(0,height()-20,width(),20);
    p.drawText(mrect, Message);
    p.end();
    e->accept();
}
