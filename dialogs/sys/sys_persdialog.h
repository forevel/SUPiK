#ifndef SYS_PERSDIALOG_H
#define SYS_PERSDIALOG_H

#include <QDialog>

#define SYSPDBG     DBGMSG(PublicClass::ER_SYSPERS, __LINE__)
#define SYSPWARN    WARNMSG(PublicClass::ER_SYSPERS,__LINE__)
#define SYSPINFO(a) INFOMSG(PublicClass::ER_SYSPERS,__LINE__,a)

class sys_persdialog : public QDialog
{
    Q_OBJECT
public:
    sys_persdialog(QWidget *parent = 0);

private:
    void SetupUI();
};

#endif // SYS_PERSDIALOG_H
