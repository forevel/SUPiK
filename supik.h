#ifndef SUPIK_H
#define SUPIK_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QCloseEvent>
#include <QResizeEvent>

#define SUPIKMENUBAR_BG "transparent"
#define SUPIKMENUBAR_ITEM_SELECTED "#EEEEEE"
#define SUPIKMENU_ITEM "#CCCCCC"
#define SUPIKMENU_ITEM_BG_SELECTED "#FFFFFF"
#define SUPIKMENU_ITEM_COLOR_SELECTED "#000000"
#define SYS_TAB_BGCOLOR "#FF373A"

#define SUPIK_PINGPERIOD    10
#define SUPIK_CURRPERIOD    5 // период обновления данных о валютах

//#define SYSSTYLESHEET "background-image: url(:/res/SysWallpaper.png);"

class supik : public QMainWindow
{
    Q_OBJECT
public:
    supik ();

    void InitiateCurrency();

signals:
    void newnotify();
    void stopall();

public slots:

private:
    bool IsProblemsDetected, PeriodicOddSecond;
    int PingSecCounter, CurrRefreshCounter, CurrCounter, WarningActionIndex;
    QHash <QString, void (supik::*)()> pf;
    int ErMsgNum;

    void SetSupikMenuBar();
    QMenu *AddChildToMenu(int);
    void AddChildToAction (QAction *, int);
    void SetSupikStatusBar();

    int CheckForWidget (int);
    int CreateTab(quint32 access, QDialog *dlg, QString tabname, int tabtype);

    void GetCurrencyRates();
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
    void ClearProblems();
    void UpdateErStatus();
    void TBExam();
    void TBMain();
    void ShowServerStatus();

private slots:
    void ExecuteSub();
    void executeDirDialog();
    void UpdateProblemsNumberInTab();
    void periodic1s();
    void ErrorProtocol();

    void Components();
    void Directories();
    void WhEditor();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *event);
};

#endif // SUPIK_H
