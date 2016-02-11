#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

#include "supik.h"
#include "dialogs/sys/sys_settingsdialog.h"
#include "gen/publicclass.h"
#include "gen/publiclang.h"

#define STARTER(a)  ERMSG(PublicClass::ER_START,__LINE__,a)
#define STARTWARN   WARNMSG(PublicClass::ER_START,__LINE__)

class StartWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit StartWindow(QWidget *parent = 0);
    ~StartWindow();

signals:
    void DBOpened(int);

public slots:

private slots:
    void OkPBClicked();
    void PasswdLEReturnPressed();
    void UNameLEReturnPressed();
    void OpenSettingsDialog();
    void ChangePassword();

private:
    sys_settingsdialog qssd;

    void showEvent(QShowEvent *event);
    void SetupUI();
    void LoadLanguage();
    void Startup();
    void OpenAndCheckDB(QSqlDatabase db, int signid);
};

#endif // STARTWINDOW_H
