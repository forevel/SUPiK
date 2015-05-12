#ifndef SYS_SETTINGSDIALOG_H
#define SYS_SETTINGSDIALOG_H

#include <QWidget>
#include <QIcon>
#include "../gen/publicclass.h"
#include "../gen/publiclang.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqcombobox.h"
#include "../widgets/s_tqspinbox.h"

QT_BEGIN_NAMESPACE
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QFont;
class QFrame;
QT_END_NAMESPACE

#define SYS_BGCOLOR "#FF6666"

class sys_settingsdialog : public QWidget
{
    Q_OBJECT
    
public:
    s_tqLineEdit *PathToLibsLE, *SQLPathLE, *PathToSupikLE;
    s_tqComboBox *LangCB;
    s_tqspinbox *timerperiodSB;
    bool Completed;
    sys_settingsdialog();

private slots:

    void CancelPBClicked();
    void OKPBClicked();
    void ChooseDirPBClicked();
    void ChooseSDirPBClicked();

private:
    QGridLayout *GridLayout;
    QVBoxLayout *MainLayout;
    QFrame *MainFrame, *PBFrame;
    QHBoxLayout *PBLayout;
    QFont font;
    s_tqLabel *SettingsL, *PathToLibsL, *LangL, *SQLPathL, *PathToSupikL, *timerperiodL;
    s_tqPushButton *ChooseDirPB, *isOKPB, *CancelPB, *ChooseSDirPB;
    QIcon qssdIcon, RusIcon, EngIcon;
    bool FirstShow;

    void SetupUI ();
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // SYS_SETTINGSDIALOG_H
