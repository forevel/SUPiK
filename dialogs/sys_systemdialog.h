#ifndef SYS_SYSTEMDIALOG_H
#define SYS_SYSTEMDIALOG_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "../gen/publicclass.h"
#include "../widgets/s_tqtreewidget.h"
#include "../widgets/s_tqlabel.h"
#include "sys_cantdeletedialog.h"
#include "s_sqlfieldsdialog.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqtableview.h"
#include "../models/s_aitemmodel.h"
#include "../models/s_duniversal.h"

QT_BEGIN_NAMESPACE
class QAction;
class QTreeView;
class QIcon;
class QTableView;
class QHBoxLayout;
class QVboxLayout;
class QFrame;
QT_END_NAMESPACE

class sys_systemdialog : public QWidget
{
    Q_OBJECT
public:
    explicit sys_systemdialog(QWidget *parent = 0);

signals:

public slots:

private:
    s_tqTreeView *MainTV, *SlaveTV;
    s_duniversal *gridItemDelegate;
    s_aitemmodel *MainTreeModel, *SlaveTreeModel;
    s_tqLabel *MainL;
    QStringList AdditionalFields;
    QVBoxLayout *MainLayout;
    QHBoxLayout *MainFrameLayout;
    QFrame *MainFrame;
    QString tble, als;
    QSqlDatabase db;
    sys_cantdeletedialog s_cdd;
    s_sqlfieldsdialog *sqldialog;
    bool FirstShow;

    void SetupUI();
    void SetSysTree();
    void SetMainTree();
    void ShowSlaveTree(QString);
    QString getMainIndex(int column);
    QString getSlaveIndex(int column);
    void AddToTree (QString);
    void ChangeAdditionalFields (QString id);

protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void SystemContextMenu (QPoint);
    void SystemSlaveContextMenu (QPoint);
    void setMainTVExpanded (QModelIndex index);
    void setSlaveTVExpanded(QModelIndex index);
    void ShowTableContentDialog ();
    void ShowAppropriateSlaveTree ();
    void ShowAppropriateSlaveTree(QModelIndex);
    void AddChildToTree ();
    void AddRootToTree ();
    void DeleteChildFromTree ();
    void ChangeAdditionalFields ();
    void ChangeAdditionalFields (QModelIndex index);
    void ChangeChildName ();
};

#endif // SYS_SYSTEMDIALOG_H
