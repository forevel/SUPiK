#ifndef S_maskedle_H
#define S_maskedle_H

#define MONEY   1
#define INTEG2  2
#define UNKNOWN 255

#include <QLineEdit>
#include <QKeyEvent>

class s_maskedle : public QLineEdit
{
    Q_OBJECT
public:
    explicit s_maskedle(QString regexp, QWidget *parent = 0);

signals:

public slots:

private slots:

protected:
//    bool event(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
};

#endif // S_maskedle_H
