#ifndef DIR_MAINDIALOG_H
#define DIR_MAINDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QPoint>

#include "../gen/publicclass.h"

class dir_maindialog : public QDialog
{
    Q_OBJECT
public:
    explicit dir_maindialog(QWidget *parent = 0);

signals:

public slots:

private slots:
    void mainContextMenu(QPoint);
    void SystemSlaveContextMenu (QPoint);
    void showDirDialog(QModelIndex idx = QModelIndex());
    void AddDirDialog();
    void EditDirDialog();
    void EditItem (QModelIndex index = QModelIndex());
    void DeleteData();
    void AddNew();

private:
//    s_tqTreeView *MainTV, *SlaveTV;
//    s_ntmodel *MainTreeModel, *SlaveTreeModel;
//    s_ncmodel *MainTableModel, *SlaveTableModel;
//    QSortFilterProxyModel *SlaveProxyModel;
    bool MainTVIsTree, SlaveTVIsTree, IsQuarantine;
//    s_tqLabel *MainL;
//    QVBoxLayout *MainLayout;
//    QHBoxLayout *MainFrameLayout;
    bool firstShow;
//    s_sqlfieldsdialog *sqldialog;
    QSqlDatabase db;
    QString tble;
    int SlaveTVAccess;
    quint32 result;

    void SetupUI();
    void setDirTree();
    void fillSlaveWindow();
    void setFieldsAliases();
    void fillDirTableStructure();
    void clearSlaveFrame();
    void ShowSlaveTree(QString);
    void EditItem(QString str);
    QString getMainIndex(int column);
    QString getSlaveIndex(int column);
    void ShowErMsg(int ermsg);

protected:
    void showEvent(QShowEvent *e);
    void paintEvent(QPaintEvent *e);
};

#endif // DIR_MAINDIALOG_H
