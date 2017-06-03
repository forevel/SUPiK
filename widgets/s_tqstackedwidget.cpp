/* s_tqStackedWidget - Stacked transparent widget made with help from this page:
 * http://www.qtcentre.org/threads/28514-How-can-i-achieve-an-auto-resizing-QStackedWidget
*/

#include "s_tqstackedwidget.h"

s_tqStackedWidget::s_tqStackedWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    setStyleSheet("QWidget {background-color: rgba(0,0,0,0);}");
    lyout = new QVBoxLayout(this);
    curr_index = 0;
}

int s_tqStackedWidget::count()
{
    return widgets.count();
}

void s_tqStackedWidget::addWidget(QWidget *w)
{
    widgets.append(w);
    lyout->addWidget(w);
    connect(w, SIGNAL(destroyed(QObject*)),this,SLOT(setCurrentIndexLast()));
    setCurrentIndexLast();
}

void s_tqStackedWidget::setCurrentIndex(int i)
{
    if ((i >= count()) || (i < 0))
        return;
    curr_index = i;
    showCurrentWidget();
}

void s_tqStackedWidget::DeleteWidgetFromLayout(QWidget *w)
{
    for (int j=0; j<lyout->count(); ++j)
    {
        if (lyout->itemAt(j)->widget() == w)
        {
            QLayoutItem *item = lyout->itemAt(j);
            if (item)
            {
                lyout->removeItem(item);
                if (w)
                {
                    lyout->removeWidget(w);
                    w->deleteLater();
                }
                delete item;
            }
            return;
        }
    }
}

void s_tqStackedWidget::setCurrentIndexLast()
{
    curr_index = count()-1;
    if (curr_index >= 0)
        showCurrentWidget();
}

void s_tqStackedWidget::showCurrentWidget()
{
    if ((widgets.count() > 0) && (curr_index < count()))
    {
        foreach (QWidget *widget, widgets)
            widget->hide();
        widgets.at(curr_index)->show();
        updateGeometry();
    }
}

QWidget *s_tqStackedWidget::currentWidget()
{
    if ((curr_index < count()) && (curr_index >= 0))
        return widgets.at(curr_index);
    else
        return NULL;
}

QSize s_tqStackedWidget::sizeHint()
{
    if (auto_resize && count() > 0)
        return currentWidget()->minimumSize();
    else
        return QWidget::sizeHint();
}

void s_tqStackedWidget::setCurrentWidget(QWidget *w)
{
    for (int i=0; i<count(); ++i)
    {
        if (widgets.at(i) == w)
        {
            setCurrentIndex(i);
            return;
        }
    }
}

void s_tqStackedWidget::removeWidget(QWidget *w)
{
    for (int i=0; i<count(); ++i)
    {
        if (widgets.at(i) == w)
        {
            DeleteWidgetFromLayout(w);
            widgets.removeAt(i);
            if (curr_index == i)
                setCurrentIndexLast();
            return;
        }
    }
}

void s_tqStackedWidget::clear()
{
    while (widgets.size())
    {
        QWidget *w = widgets.at(0);
        removeWidget(w);
    }
    curr_index = 0;
}

