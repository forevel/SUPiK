#ifndef S_ACCEPTDIALOG_H
#define S_ACCEPTDIALOG_H

#include <QDialog>

class s_acceptdialog: public QDialog
{
    Q_OBJECT
public:
    explicit s_acceptdialog(QString title, QString str, QWidget *parent=0);

signals:
    void caccepted();

private slots:
    void OkPressed();
};

#endif // S_ACCEPTDIALOG_H
