#include "../inc/s_tqcalendarwidget.h"

s_tqCalendarWidget::s_tqCalendarWidget(QWidget *parent) :
    QCalendarWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
//    connect(this, SIGNAL(activated(QDate)), this, SLOT(dateChoosed(QDate)));
}

/*void s_tqCalendarWidget::dateChoosed(QDate dte)
{
    this->close();
}
*/
