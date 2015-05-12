#ifndef S_TQFRAME_H
#define S_TQFRAME_H

#include <QFrame>
#include <QVariant>

class s_tqFrame : public QFrame
{
    Q_OBJECT
public:
    explicit s_tqFrame(QWidget *parent = 0);
    QVariant getAData();
    void setAData(QVariant dat);

signals:

public slots:

private:
    QVariant adata;
};

#endif // S_TQFRAME_H
