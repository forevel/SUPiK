#ifndef DIR_MAINDIALOG_H
#define DIR_MAINDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QPoint>

#include "../gen/publicclass.h"
#include "../models/s_ntmodel.h"
#include "../models/s_ncmodel.h"

class dir_maindialog : public QDialog
{
    Q_OBJECT
public:
    explicit dir_maindialog(QWidget *parent = 0);

signals:
    void error(int);

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
    bool MainTVIsTree, SlaveTVIsTree, IsQuarantine;
    bool firstShow;
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

protected:
    void showEvent(QShowEvent *e);
    void paintEvent(QPaintEvent *e);
};

#endif // DIR_MAINDIALOG_H
