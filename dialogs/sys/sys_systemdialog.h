#ifndef SYS_SYSTEMDIALOG_H
#define SYS_SYSTEMDIALOG_H

#include <QPoint>
#include <QDialog>
#include <QPaintEvent>
#include <QModelIndex>

#define SYSSDBG     DBGMSG(PublicClass::ER_SYS, __LINE__)
#define SYSSWARN    WARNMSG(PublicClass::ER_SYS,__LINE__)
#define SYSSINFO(a) INFOMSG(PublicClass::ER_SYS,__LINE__,a)

class sys_systemdialog : public QDialog
{
    Q_OBJECT
public:
    explicit sys_systemdialog(QWidget *parent = 0);

signals:
    void closeslvdlg();

public slots:

private:
    QHash <QString, void (sys_systemdialog::*)()> pf;

    void SetupUI();
    void SetSysTree();
    void SetMainTree();
    QString getMainIndex(int column);
    void AddToTree (QString);
    void ChangeAdditionalFields (QString id);

    // функции пунктов меню системы
    void MainMenuEditor();
    void SystemMenuEditor();
    void SystemDirEditor();
    void TablesEditor();
    void DirEditor();
    void PersEditor();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void SetSlave(QModelIndex idx);
    void EditTable();
    void NewTable();
    void DeleteTable();
    void RemoveWidget();
    void TablesEditorContextMenu(QPoint);
};

#endif // SYS_SYSTEMDIALOG_H
