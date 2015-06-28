#ifndef SYS_SYSTEMDIALOG_H
#define SYS_SYSTEMDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QModelIndex>

class sys_systemdialog : public QDialog
{
    Q_OBJECT
public:
    explicit sys_systemdialog(QWidget *parent = 0);

signals:
    void error(int);

public slots:

private:
    QStringList AdditionalFields;
    QString tble, als;
    bool FirstShow, SlaveTVIsTree;

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
    void ShowTableContentDialog ();
    void SetSlaveTV(QModelIndex index);
    void SetSlaveTV();
    void AddChildToTree ();
    void AddRootToTree ();
    void DeleteChildFromTree ();
    void ChangeAdditionalFields ();
    void ChangeAdditionalFields (QModelIndex idx);
    void ChangeChildName ();
};

#endif // SYS_SYSTEMDIALOG_H
