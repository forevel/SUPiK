#ifndef s_MaskedLineEdit_H
#define s_MaskedLineEdit_H

#define MONEY   1
#define INTEG2  2
#define UNKNOWN 255

#include <QLineEdit>
#include <QKeyEvent>

class s_MaskedLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit s_MaskedLineEdit(QString regexp, QWidget *parent = 0);

signals:

public slots:

private slots:

protected:
//    bool event(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
};

#endif // s_MaskedLineEdit_H
