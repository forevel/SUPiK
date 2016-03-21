#include "s_tqpushbutton.h"

s_tqPushButton::s_tqPushButton(QWidget *parent) :
    QPushButton(parent)
{
    setStyleSheet("QPushButton {background-color: rgba(0,0,0,0); border: 1px solid gray; border-radius: 5px; border-style: outset; padding: 2px 5px;}"
                  "QPushButton:pressed {border-style: inset;}"
                  "QPushButton:disabled {border: none;}");
    connect (this, SIGNAL(clicked()), this, SLOT(PBclicked()));
}

s_tqPushButton::s_tqPushButton(QString str, int r, int g, int b, int a, QWidget *parent) : QPushButton(parent)
{
    setText(str);
    setStyleSheet("QPushButton {background-color: rgba("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+","+QString::number(a)+"); "
                  "border: 2px solid gray; border-radius: 3px; border-style: outset; padding: 2px 5px;}"
                  "QPushButton:pressed {border-style: inset;}"
                  "QPushButton:disabled {border: none;}");
    connect (this, SIGNAL(clicked()), this, SLOT(PBclicked()));
}

s_tqPushButton::s_tqPushButton(QColor &color, QString str, QWidget *parent) : QPushButton(parent)
{
    setStyleSheet("QPushButton {color: rgb(" + QString::number(color.red(),10) + "," + QString::number(color.green(),10) + "," + \
                  QString::number(color.blue(),10) + "); background-color: rgba(0,0,0,0); border: 2px solid gray; border-radius: 3px; border-style: outset;}"
                  "QPushButton:pressed {border-style: inset;}"
                  "QPushButton:disabled {border: none;}");
    setText(str);
    connect (this, SIGNAL(clicked()), this, SLOT(PBclicked()));
}

void s_tqPushButton::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqPushButton::getAData()
{
    return this->adata;
}

void s_tqPushButton::PBclicked()
{
    emit clicked(this);
}
