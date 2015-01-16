#ifndef S_TRANSPARENTQLABEL_H
#define S_TRANSPARENTQLABEL_H

#include <QLabel>
#include <QPainter>
#include <QPaintEvent>

class s_transparentQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit s_transparentQLabel(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // S_TRANSPARENTQLABEL_H
