#ifndef S_COLORTABWIDGET_H
#define S_COLORTABWIDGET_H

#include <QTabWidget>
#include <QPaintEvent>

#include "../inc/s_colortabbar.h"
#include "../inc/publicclass.h"

class S_ColorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    int addTab(QWidget *widget, const QString &);
    S_ColorTabWidget (QWidget *parent = 0);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

private:

private slots:
    void closeTab (int);
};

#endif // S_COLORTABWIDGET_H
