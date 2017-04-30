#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QDialog>

class ServerDialog : public QDialog
{
    Q_OBJECT
public:
    ServerDialog(QWidget *parent = 0);

private slots:
    void ShowLogFiles();
    void ShowServerStatus();
    void ShowLog();
};

#endif // SERVERDIALOG_H
