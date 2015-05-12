#ifndef DIR_MAINDIALOG_H
#define DIR_MAINDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>

#include "../gen/publicclass.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqframe.h"
#include "../widgets/s_tqtableview.h"
#include "sys_acceptexist.h"
#include "../models/s_ntmodel.h"
#include "../models/s_ncmodel.h"
#include "s_sqlfieldsdialog.h"
#include "../models/s_duniversal.h"

QT_BEGIN_NAMESPACE
class QHBoxLayout;
class QVBoxLayout;
QT_END_NAMESPACE

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
//    void setMainTVExpanded (QModelIndex index);
//    void setSlaveTVExpanded(QModelIndex index);
    void showDirDialog(QModelIndex idx = QModelIndex());
    void AddDirDialog();
    void EditDirDialog();
    void ChangeAdditionalFields (QModelIndex index = QModelIndex());
//    void AddDataChild();
//    void AddSubDataChild();
    void DeleteData();
    void AddNew();

private:
    s_tqtreeview *MainTV, *SlaveTV;
    s_duniversal *gridItemDelegate;
    s_ntmodel *MainTreeModel, *SlaveTreeModel;
    s_ncmodel *MainTableModel, *SlaveTableModel;
    QSortFilterProxyModel *SlaveProxyModel;
    bool MainTVIsTree, SlaveTVIsTree, IsQuarantine;
    s_tqLabel *MainL;
    QVBoxLayout *MainLayout;
    QHBoxLayout *MainFrameLayout;
    bool firstShow, SlaveTVIsFilling;
    s_sqlfieldsdialog *sqldialog;
    QSqlDatabase db;
    QString tble;
    int SlaveTVAccess;

    void SetupUI();
    void setDirTree();
    void fillSlaveWindow();
    void setFieldsAliases();
    void fillDirTableStructure();
    void clearSlaveFrame();
    void ShowSlaveTree(QString);
    void ChangeAdditionalFields(QString str);
    QString getMainIndex(int column);
    QString getSlaveIndex(int column);
//    void AddChild(QString, QString);

protected:
    void showEvent(QShowEvent *e);
    void paintEvent(QPaintEvent *e);
};

#endif // DIR_MAINDIALOG_H
