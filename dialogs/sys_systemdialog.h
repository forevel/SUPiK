#ifndef SYS_SYSTEMDIALOG_H
#define SYS_SYSTEMDIALOG_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

class sys_systemdialog : public QWidget
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
    void SetSlaveTV(QModelIndex);
    void ShowTableContentDialog ();
    void SetSlaveTV();
    void AddChildToTree ();
    void AddRootToTree ();
    void DeleteChildFromTree ();
    void ChangeAdditionalFields ();
    void ChangeAdditionalFields (QModelIndex index);
    void ChangeChildName ();
};

#endif // SYS_SYSTEMDIALOG_H
