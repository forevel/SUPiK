#ifndef S_TQLABEL_H
#define S_TQLABEL_H

#include <QLabel>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

class s_tqLabel : public QLabel
{
    Q_OBJECT
public:
    explicit s_tqLabel(QWidget *parent = 0);
    s_tqLabel(QString txt, QWidget *parent = 0);
    void setData(QVariant);
    QVariant getData();

signals:

public slots:

private:
    QVariant data;

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // S_TQLABEL_H
