#ifndef S_TQCALENDARWIDGET_H
#define S_TQCALENDARWIDGET_H

#include <QCalendarWidget>
#include <QDate>

class s_tqCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
public:
    explicit s_tqCalendarWidget(QWidget *parent = 0);

signals:

public slots:

private slots:
//    void dateChoosed(QDate);
};

#endif // S_TQCALENDARWIDGET_H
