#ifndef SUPIK_H
#define SUPIK_H

#include <QMainWindow>
#include <QPaintEvent>

#include "../inc/sys_settingsdialog.h"
#include "../inc/sys_systemdialog.h"
#include "../inc/dir_maindialog.h"
#include "../inc/sys_probsdialog.h"
#include "../inc/cmp_compdialog.h"
#include "../inc/wh_dialog.h"
#include "../inc/sys_backuprestoredirdialog.h"
#include "../inc/s_ncdialog.h"

#include "../inc/publicclass.h"
#include "../inc/publiclang.h"
#include "../inc/s_colortabwidget.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMenuBar;
class QStatusBar;
class QIcon;
class QTableView;
class QHBoxLayout;
class QVBoxLayout;
class QTimer;
QT_END_NAMESPACE

#define SUPIKMENUBAR_BG "transparent"
#define SUPIKMENUBAR_ITEM_SELECTED "#EEEEEE"
#define SUPIKMENU_ITEM "#CCCCCC"
#define SUPIKMENU_ITEM_BG_SELECTED "#FFFFFF"
#define SUPIKMENU_ITEM_COLOR_SELECTED "#000000"
#define SYS_TAB_BGCOLOR "#FF373A"

#define SYSSTYLESHEET "background-image: url(:/Pic/pic/SysWallpaper.png);"

class supik : public QMainWindow
{
    Q_OBJECT
public:
    supik ();

signals:
    void newnotify();

public slots:

private:
    QStatusBar *SupikStatusBar;
    QMenuBar *SupikMenuBar;
    QIcon SupikIcon;
    S_ColorTabWidget *MainTW;
    QTimer *timer1s, *timer1m;

    int WarningActionIndex;
    QHash <QString, void (supik::*)()> pf;
    void ClearSupikMenuBar();
    void SetSupikMenuBar();
    QMenu *AddChildToMenu(int);
    void AddChildToAction (QAction *, int);
    void SetSupikStatusBar();
    void SetSupikWindow();
    int CheckForWidget (QWidget *, QString);

    void ExitSupik ();
    void Components();
    void Directories();
    void BackupDir();
    void RestoreDir();
    void SysStructEdit ();
    void SettingsEdit ();
    void ProbCheck();
    void WhIncome();
    void WhOutgoing();
    void WhSearch();
    void DialogEdit();
    void Quarantine();

    void showEvent(QShowEvent *event);

    cmp_compdialog *qccda;
    sys_probsdialog *probDialog;
    sys_settingsdialog *qssda;
    sys_systemdialog *qsyda;
    dir_maindialog *dird;
//    wh_dialog *whd;
    s_ncdialog *whd;
    sys_backuprestoredirdialog *brd;

private slots:
    void ExecuteSub();
    void executeDirDialog();
    void updateprobsnumberintabtext();
    void periodic1s();
    void periodicxm();
};

#endif // SUPIK_H
