#ifndef DIR_MAINDIALOG_H
#define DIR_MAINDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QPoint>
#include <QSqlDatabase>

//#include "../../models/s_ntmodel.h"
#include "../../models/treemodel.h"
#include "../../models/s_ncmodel.h"

#define DIRMDBG         DBGMSG(PublicClass::ER_DIRMAIN,__LINE__)
#define DIRMWARN        WARNMSG(PublicClass::ER_DIRMAIN,__LINE__)
#define DIRMINFO(a)     INFOMSG(PublicClass::ER_DIRMAIN,__LINE__,a)
#define DIRMER(a)       ERMSG(PublicClass::ER_DIRMAIN,__LINE__,a)

class dir_maindialog : public QDialog
{
    Q_OBJECT
public:
    explicit dir_maindialog(QString tble, QWidget *parent = 0);

signals:

public slots:

private slots:
    void mainContextMenu(QPoint);
    void SystemSlaveContextMenu (QPoint);
    void showDirDialog(QModelIndex idx = QModelIndex());
    void AddDirDialog();
    void EditDirDialog();
    void EditItem ();
    void DeleteData();
    void DeleteDir();
    void DeleteDataUnconditional(QString id);
    void AddNew();

private:
    bool IsQuarantine;
    bool isNewID;
    QString MainTable;
    QStringList Tables;
    int SlaveTVAccess;
    TreeModel *SlaveTreeModel;
    s_ncmodel *MainTableModel;
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
    void RefreshSlaveTV();

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // DIR_MAINDIALOG_H
