#ifndef SYS_PROBSDIALOG_H
#define SYS_PROBSDIALOG_H

#include <QDialog>
#include <QPoint>
#include <QStringListModel>

#include "../../models/s_ncmodel.h"

class sys_probsdialog : public QDialog
{
    Q_OBJECT
public:
    explicit sys_probsdialog(QWidget *parent = 0);

signals:
    void editdirneeded();
    void updateprobsnumber();

public slots:
    void updatemainTV();

private:
    void SetupUI();
    void diredit(QString tble);

    QStringListModel *slmodel;
    s_ncmodel *mainmodel;
    QString field;
    QString tble;
//    QSqlDatabase db;
    QDialog *dlg;

private slots:
    void mainTVcontextmenu(QPoint);
    void removeProb(QModelIndex);
    void removeProb();
    void refresh();
    void addcol();
};

#endif // SYS_PROBSDIALOG_H
