#ifndef S_ACCEPTDIALOG_H
#define S_ACCEPTDIALOG_H

#include <QDialog>

class s_acceptdialog: public QDialog
{
    Q_OBJECT
public:
    explicit s_acceptdialog(QWidget *parent=0);
    void setupUI(QString title, QString str);

signals:

private slots:
    void OkPressed();
};

#endif // S_ACCEPTDIALOG_H
