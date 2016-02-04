#ifndef SUPIK_H
#define SUPIK_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QCloseEvent>
#include <QResizeEvent>
#include "gen/publicclass.h"
#include "widgets/waitwidget.h"

#define SUPIKMENUBAR_BG "transparent"
#define SUPIKMENUBAR_ITEM_SELECTED "#EEEEEE"
#define SUPIKMENU_ITEM "#CCCCCC"
#define SUPIKMENU_ITEM_BG_SELECTED "#FFFFFF"
#define SUPIKMENU_ITEM_COLOR_SELECTED "#000000"
#define SYS_TAB_BGCOLOR "#FF373A"

// Макросы для выдачи сообщений
#define SUPIKER(a)    ERMSG(PublicClass::ER_SUPIK, __LINE__, a)
#define SUPIKDBG      DBGMSG(PublicClass::ER_SUPIK,__LINE__)
#define SUPIKINFO(a)  INFOMSG(PublicClass::ER_SUPIK, __LINE__, a)
#define SUPIKWARN     WARNMSG(PublicClass::ER_SUPIK,__LINE__)


#define SYSSTYLESHEET "background-image: url(:/res/SysWallpaper.png);"

class supik : public QMainWindow
{
    Q_OBJECT
public:
    supik ();

signals:
    void newnotify();

public slots:

private:
    bool ERHide, ERTimerIsOn, ProblemsDetected;
    QTimer *ERTimer;
    QRect ERGeometry;
    int WarningActionIndex;
    QHash <QString, void (supik::*)()> pf;
    void SetSupikMenuBar();
    QMenu *AddChildToMenu(int);
    void AddChildToAction (QAction *, int);
    void SetSupikStatusBar();

    int CheckForWidget (int);

    void SetSupikWindow();
    void ExitSupik ();
    void BackupDir();
    void RestoreDir();
    void SysStructEdit();
    void SettingsEdit();
    void SysImportClass();
    void ProbCheck();
    void WhIncome();
    void WhOutgoing();
    void WhSearch();
    void DevDoc();
    void DevDev();
    void Quarantine();
    void Dummy();
    void SysDirectories();
    void ShowOrHideSlideER();
    void ClearProblems();

private slots:
    void ExecuteSub();
    void executeDirDialog();
    void UpdateProblemsNumberInTab();
    void periodic1s();
    void NewProblemsDetected(PublicClass::ProblemStruct &prob);
    void MouseMove();
    void UpdateErrorProtocol();
    void HideErrorProtocol();

    void Components();
    void Directories();
    void WhEditor();

protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *e);
};

#endif // SUPIK_H
