#ifndef S_TQRADIOBUTTON_H
#define S_TQRADIOBUTTON_H

#include <QRadioButton>

class s_tqRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit s_tqRadioButton(const QString &text, QWidget *parent = 0);

signals:

public slots:
};

#endif // S_TQRADIOBUTTON_H
