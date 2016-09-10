#include "waitwidget.h"
#include "s_tqlabel.h"
#include <QPixmap>
#include <QBitmap>
#include <QImage>
#include <QPainter>
#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QDateTime>
#include <QTimer>
#include <math.h>

WaitWidget::WaitWidget(QWidget *parent) : QWidget(parent)
{
    Finished = false;
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    gamma = 0.0;
    Message = "";
    resize(800, 220);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    qsrand(QDateTime::currentDateTime().toTime_t());
    vel1 = 0.5;
    vel2 = -1;
    vel3 = 1;
    vel4 = static_cast<float>(-0.2);
    vel5 = 3.5;
}

WaitWidget::~WaitWidget()
{

}

void WaitWidget::Start()
{
    show();
    QTimer *tmr = new QTimer;
    tmr->setInterval(10);
    connect(tmr,SIGNAL(timeout()),this,SLOT(Rotate()));
    tmr->start();
/*    QTime tme;
    while (!Finished)
    {
        tme.start();
        while (tme.elapsed() < 100)
            qApp->processEvents();
    }
    emit finished(); */
}

void WaitWidget::Stop()
{
    hide();
    Finished = true;
}

void WaitWidget::SetMessage(QString msg)
{
    Message = msg;
}

void WaitWidget::Rotate()
{
    gamma += 0.5;
/*    vel1 += (static_cast<float>(qrand()%100)-50)/5000; // числа от -0.01 до 0.01
    vel2 += (static_cast<float>(qrand()%100)-50)/5000; // числа от -0.01 до 0.01
    vel3 += (static_cast<float>(qrand()%100)-50)/5000; // числа от -0.01 до 0.01
    vel4 += (static_cast<float>(qrand()%100)-50)/5000; // числа от -0.01 до 0.01
    vel5 += (static_cast<float>(qrand()%100)-50)/5000; // числа от -0.01 до 0.01
    Message = QString::number(vel1); */
    update();
}

void WaitWidget::paintEvent(QPaintEvent *e)
{
    QPixmap rotatedPixmap(":/res/Object1-0.png");
    QSize size = rotatedPixmap.size();
    QSize wsize = QSize(800,220);
    int left = wsize.width()/2-size.width()/2;
    QPainter p1(this);
    p1.setRenderHint(QPainter::Antialiasing);
    p1.translate(wsize.width()/2,size.height()/2);
    p1.rotate(gamma*vel1);
    p1.translate(-wsize.width()/2,-size.height()/2);
    p1.drawPixmap(left, 0, rotatedPixmap);
    p1.end();
    QPixmap rotatedPixmap2(":/res/Object2.png");
    QPainter p2(this);
    p2.setRenderHint(QPainter::Antialiasing);
    p2.translate(wsize.width()/2,size.height()/2);
    p2.rotate(gamma*vel2);
    p2.translate(-wsize.width()/2,-size.height()/2);
    p2.drawPixmap(left, 0, rotatedPixmap2);
    p2.end();
    QPixmap rotatedPixmap3(":/res/Object3.png");
    QPainter p3(this);
    p3.setRenderHint(QPainter::Antialiasing);
    p3.translate(wsize.width()/2,size.height()/2);
    p3.rotate(gamma*vel3);
    p3.translate(-wsize.width()/2,-size.height()/2);
    p3.drawPixmap(left, 0, rotatedPixmap3);
    p3.end();
    QPixmap rotatedPixmap4(":/res/Object4.png");
    QPainter p4(this);
    p4.setRenderHint(QPainter::Antialiasing);
    p4.translate(wsize.width()/2,size.height()/2);
    p4.rotate(gamma*vel4);
    p4.translate(-wsize.width()/2,-size.height()/2);
    p4.drawPixmap(left, 0, rotatedPixmap4);
    p4.end();
    QPixmap rotatedPixmap5(":/res/Object5.png");
    QPainter p5(this);
    p5.setRenderHint(QPainter::Antialiasing);
    p5.translate(wsize.width()/2,size.height()/2);
    p5.rotate(gamma*vel5);
    p5.translate(-wsize.width()/2,-size.height()/2);
    p5.drawPixmap(left, 0, rotatedPixmap5);
    p5.end();
    QPainter p(this);
    QRect mrect = QRect(0,height()-20,width(),20);
    p.drawText(mrect, Qt::AlignCenter, Message);
    p.end();
    e->accept();
}
