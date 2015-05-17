#ifndef S_SQLTREEDIALOG_H
#define S_SQLTREEDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QCloseEvent>
#include <QShowEvent>
#include <QSortFilterProxyModel>
#include "../models/s_aitemmodel.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqpushbutton.h"

QT_BEGIN_NAMESPACE
class QVBoxLayout;
QT_END_NAMESPACE

class s_sqltreedialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_sqltreedialog(QWidget *parent = 0);
    void SetupUI(QSqlDatabase db, QString tble, QString index);
    void SetupUI(QSqlDatabase db1, QString tble1, QSqlDatabase db2, QString tble2, QString index);
    void SetupUI(QString index);

signals:
    void changeshasbeenMade (QString);

public slots:

private:
    QVBoxLayout *MainLayout;
    s_aitemmodel *treeModel;
    s_tqTreeView *mainTV;
    QSortFilterProxyModel *prModel, *filterModel;
    QSqlDatabase db1, db2;
    QString tble1, tble2;


private slots:
    void choosed(QModelIndex index);
    void setMainTVExpanded(QModelIndex index);
    void commitChanges();
    void commitChanges(QModelIndex &index);
    void filter(QString str);
    void search();
    void execSqlFieldsDialog();
    void SqlFieldsDialogFinished(int);

};

#endif // S_SQLTREEDIALOG_H
