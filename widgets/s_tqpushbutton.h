#ifndef S_TQPUSHBUTTON_H
#define S_TQPUSHBUTTON_H

#include <QPushButton>
#include <QVariant>

class s_tqPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit s_tqPushButton(QWidget *parent = 0);
    explicit s_tqPushButton(QString str, int r=0, int g=0, int b=0, int a=0, QWidget *parent = 0);
    explicit s_tqPushButton(QColor &color, QString str="", QWidget *parent = 0);
/*    QVariant getAData();
    void setAData(QVariant dat); */

signals:
//    void clicked (s_tqPushButton *ptr);

public slots:

private:
//    QVariant adata;

private slots:
//    void PBclicked();

};

#endif // S_TQPUSHBUTTON_H
