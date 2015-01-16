#ifndef S_COLORTABBAR_H
#define S_COLORTABBAR_H

#include <QTabBar>
#include <QMap>
#include <QStyleOptionTabV3>
#include <QStylePainter>
#include <QEvent>
#include <QColor>
#include <QPaintEvent>
#include <QFontMetrics>

#include "../inc/publicclass.h"

#define SYS 1

class S_ColorTabBar : public QTabBar
{
    Q_OBJECT
public:
    S_ColorTabBar(QWidget *parent = 0);

signals:

public slots:

protected:
    bool event (QEvent *);
    void paintEvent(QPaintEvent *);
    QSize tabSizeHint(int index) const;
    QSize sizeHint() const;

private:
    mutable int cachedHeight;

private slots:

};

#endif // S_COLORTABBAR_H
