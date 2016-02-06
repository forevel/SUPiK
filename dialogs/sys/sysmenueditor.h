#ifndef SYSMENUEDITOR_H
#define SYSMENUEDITOR_H

#include <QDialog>
#include <QModelIndex>
#include <QPoint>

#define SYSMDBG     DBGMSG(PublicClass::ER_SYSMENU, __LINE__)
#define SYSMWARN    WARNMSG(PublicClass::ER_SYSMENU,__LINE__)
#define SYSMINFO(a) INFOMSG(PublicClass::ER_SYSMENU,__LINE__,a)

class SysmenuEditor : public QDialog
{
    Q_OBJECT
public:
    SysmenuEditor(QWidget *parent = 0);

    void SetupUI(QString tble);

signals:

private slots:
    void ChangeFieldsSlot(QModelIndex);
    void ChangeFields();
    void ChangeFields(QString);
    void Context(QPoint);
    void AddChild();
    void AddRoot();
//    void ChangeName();
    void Delete();
    void AddToTree(QString);

private:
    QStringList AdditionalFields;
    QString tble, als;

    QString GetMainIndex(int column);
    void UpdateTree();
};

#endif // SYSMENUEDITOR_H
