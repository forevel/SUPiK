#ifndef S_TQPROGRESSBAR_H
#define S_TQPROGRESSBAR_H

#include <QProgressBar>
#include <QVariant>
#include <QMouseEvent>

class s_tqProgressBar : public QWidget
{
    Q_OBJECT
public:
    s_tqProgressBar(const QString &txt="", QWidget *parent = 0);
    void setText(const QString &txt);

signals:

public slots:
    void setValue(quint64 value);
    void setRange(quint64 min, quint64 max);

private:
    QProgressBar *PRB;

private slots:

protected:
};

#endif // S_TQPROGRESSBAR_H
