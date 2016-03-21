#ifndef SYS_SETTINGSDIALOG_H
#define SYS_SETTINGSDIALOG_H

#include <QDialog>
#include <QIcon>
#include "../../gen/publicclass.h"
#include "../../gen/publiclang.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqspinbox.h"

#define SYS_BGCOLOR "#FF6666"

#define SYSSETWARN    WARNMSG(PublicClass::ER_SYSSET, __LINE__)
#define SYSSETDBG     DBGMSG(PublicClass::ER_SYSSET, __LINE__)
#define SYSSETINFO(a) INFOMSG(PublicClass::ER_SYSSET, __LINE__,a)

class sys_settingsdialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit sys_settingsdialog(QWidget *parent=0);
    void SetupUI();

private slots:
    void LangChoosed(QString);
    void TimerPeriodChoosed(double);
    void ErWidgetPeriodChoosed(double);
    void ErWidgetEnabled(bool enabled);
    void CancelPBClicked();
    void OKPBClicked();
    void PathToLibsChoosed(QVariant);
    void SupikDirChoosed(QVariant);
    void FtpNameChoosed(QString);
    void SqlPathChoosed(QString);

private:
    void paintEvent(QPaintEvent *event);
};

#endif // SYS_SETTINGSDIALOG_H
