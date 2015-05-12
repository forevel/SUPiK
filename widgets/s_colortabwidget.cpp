#include "s_colortabwidget.h"

S_ColorTabWidget::S_ColorTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    setStyleSheet("QTabWidget::pane {border-top: 2px solid #00FFFF;}");
    S_ColorTabBar *colortabbar = new S_ColorTabBar(this);
    colortabbar->setDrawBase(false);
    setTabBar(colortabbar);
}

int S_ColorTabWidget::addTab(QWidget *widget, const QString &str)
{
    int idx = QTabWidget::addTab(widget, str);
    connect (this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    return idx;
}

void S_ColorTabWidget::closeTab(int idx)
{
    this->widget(idx)->close();
}

void S_ColorTabWidget::resizeEvent(QResizeEvent *event)
{
    pc.S_TabWidgetWidth = this->width();
    QTabWidget::resizeEvent(event);
}

void S_ColorTabWidget::paintEvent(QPaintEvent *event)
{
    event->accept();
}
