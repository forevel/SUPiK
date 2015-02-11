#ifndef DIR_MAINDIALOG_H
#define DIR_MAINDIALOG_H

#include <QDialog>

#include "publicclass.h"
#include "s_tqlabel.h"
#include "s_tqtreeview.h"
#include "s_tqframe.h"
#include "s_tqtableview.h"
#include "sys_acceptexist.h"
<<<<<<< .merge_file_a04812
#include "../inc/s_ntmodel.h"
#include "../inc/s_ncmodel.h"
=======
#include "s_aitemmodel.h"
>>>>>>> .merge_file_a05028
#include "s_sqlfieldsdialog.h"
#include "s_duniversal.h"

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
    void setMainTVExpanded (QModelIndex index);
    void setSlaveTVExpanded(QModelIndex index);
    void showDirDialog(QModelIndex);
    void showDirDialog();
    void AddDirDialog();
    void EditDirDialog();
    void ChangeAdditionalFields ();
    void ChangeAdditionalFields (QModelIndex index);
    void AddDataChild();
    void AddSubDataChild();
    void DeleteData();

private:
    s_tqtreeview *MainTV, *SlaveTV;
    s_duniversal *gridItemDelegate;
<<<<<<< .merge_file_a04812
    s_ntmodel *MainTreeModel, *SlaveTreeModel;
    s_ncmodel *MainTableModel, *SlaveTableModel;
    bool MainTVIsTree, SlaveTVIsTree;
=======
    s_aitemmodel *MainTreeModel, *SlaveTreeModel;
>>>>>>> .merge_file_a05028
    s_tqLabel *MainL;
    QVBoxLayout *MainLayout;
    QHBoxLayout *MainFrameLayout;
    bool firstShow;
    s_tqFrame *MainFrame;
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
    void AddChild(QString, QString);

protected:
    void showEvent(QShowEvent *e);
    void paintEvent(QPaintEvent *e);
};

#endif // DIR_MAINDIALOG_H
