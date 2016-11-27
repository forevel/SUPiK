#ifndef SYS_PERSDIALOG_H
#define SYS_PERSDIALOG_H

#include <QDialog>
#include <QModelIndex>

class sys_persdialog : public QDialog
{
    Q_OBJECT
public:
    sys_persdialog(QWidget *parent = 0);

private:
    void SetupUI();

private slots:
    void ChangePers(QModelIndex);
};

#endif // SYS_PERSDIALOG_H
