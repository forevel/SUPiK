#ifndef S_TQGROUPBOX_H
#define S_TQGROUPBOX_H

#include <QGroupBox>
#include <QVariant>

class s_tqGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit s_tqGroupBox(QString Color="", QWidget *parent = 0);
/*    QVariant getAData();
    void setAData(QVariant dat); */

signals:

public slots:

private:
//    QVariant adata;
};

#endif // S_TQGROUPBOX_H
