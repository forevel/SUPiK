#ifndef S_TQSTACKEDWIDGET_H
#define S_TQSTACKEDWIDGET_H

#include <QStackedWidget>

class s_tqStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    s_tqStackedWidget(QWidget *parent=0);
};

#endif // S_TQSTACKEDWIDGET_H
