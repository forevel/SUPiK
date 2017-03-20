#ifndef S_TQLABEL_H
#define S_TQLABEL_H

#include <QLabel>
#include <QVariant>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

class s_tqLabel : public QLabel
{
    Q_OBJECT
public:
    explicit s_tqLabel(QWidget *parent = 0);
    s_tqLabel(QString txt, QWidget *parent = 0);

    void SetColor(QColor color);

signals:
    void clicked();

public slots:

private:

private slots:

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *e);
};

#endif // S_TQLABEL_H
