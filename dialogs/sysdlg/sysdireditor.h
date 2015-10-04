#ifndef SYSDIREDITOR_H
#define SYSDIREDITOR_H

#include <QDialog>
#include <QPoint>

#define SYSDDBG     DBGMSG(PublicClass::ER_SYSDIR, __LINE__)
#define SYSDWARN    WARNMSG(PublicClass::ER_SYSDIR,__LINE__)
#define SYSDINFO(a) INFOMSG(PublicClass::ER_SYSDIR,__LINE__,a)

class SysDirEditor : public QDialog
{
    Q_OBJECT
public:
    SysDirEditor(QWidget *parent = 0);

private:
    void SetupUI();
    void FillTable();

private slots:
    void TvMenu(QPoint p);
    void EditDir();
    void DeleteDir();
};

#endif // SYSDIREDITOR_H
