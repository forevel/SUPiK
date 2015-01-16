#ifndef S_TQSPINBOX_H
#define S_TQSPINBOX_H

#include <QSpinBox>

class s_tqspinbox : public QSpinBox
{
    Q_OBJECT
public:
    explicit s_tqspinbox(QWidget *parent = 0);
    QVariant getAData();
    void setAData(QVariant dat);

signals:

public slots:

private:
    QVariant adata;
};

#endif // S_TQSPINBOX_H
