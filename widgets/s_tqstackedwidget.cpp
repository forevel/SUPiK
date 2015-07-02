#include "s_tqstackedwidget.h"

s_tqStackedWidget::s_tqStackedWidget(QWidget *parent) : QStackedWidget(parent)
{
    setStyleSheet("QStackedWidget {background-color: rgba(0,0,0,0);}");
}
