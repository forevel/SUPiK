#ifndef S_TQWIDGET_H
#define S_TQWIDGET_H

#include <QWidget>
#include <QVariant>

class s_tqwidget : public QWidget
{
    Q_OBJECT
public:
    explicit s_tqwidget(QWidget *parent = 0);
    QVariant getAData();
    void setAData(QVariant dat);
signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);

private:
    QVariant adata;
};

#endif // S_TQWIDGET_H
