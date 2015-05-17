#ifndef S_TQSPINBOX_H
#define S_TQSPINBOX_H

#include <QDoubleSpinBox>

class s_tqSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit s_tqSpinBox(QWidget *parent = 0);
    QVariant getAData();
    void setAData(QVariant dat);

signals:

public slots:

private:
    QVariant adata;
};

#endif // S_TQSPINBOX_H
