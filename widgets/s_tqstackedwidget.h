#ifndef S_TQSTACKEDWIDGET_H
#define S_TQSTACKEDWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class s_tqStackedWidget : public QWidget
{
    Q_OBJECT
public:
    s_tqStackedWidget(QWidget *parent=0, Qt::WindowFlags f = 0);
    void addWidget(QWidget *);
    int count();
    QWidget *currentWidget();
    void setAutoResize(bool yes=true);
    QSize sizeHint();
    void setCurrentWidget(QWidget *w);
    void removeWidget(QWidget *w);
    void clear();

public slots:
    void setCurrentIndex(int);
    void setCurrentIndexLast();

private:
    bool auto_resize;
    int curr_index, last_index;
    QList<QWidget *> widgets;
    QVBoxLayout *lyout;

    void DeleteWidgetFromLayout(QWidget *w);

protected:
    void showCurrentWidget();

};

#endif // S_TQSTACKEDWIDGET_H
