#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

#include "supik.h"
#include "dialogs/sys/sys_settingsdialog.h"
#include "gen/publicclass.h"
#include "gen/publiclang.h"
#include "widgets/waitwidget.h"

class StartWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit StartWindow(QWidget *parent = 0);
    ~StartWindow();

signals:

public slots:

private slots:
    void OkPBClicked();
    void PasswdLEReturnPressed();
    void UNameLEReturnPressed();
    void OpenSettingsDialog();
    void ChangePassword();
    void ActivatedEnter();
    void StartWaitWidget();
    void StopWaitWidget();

private:
    sys_settingsdialog qssd;
    WaitWidget *ww;

    void showEvent(QShowEvent *event);
    void SetupUI();
    void LoadLanguage();
    void Startup();
    void CreateDirs();
    void StartLogs();
    int ClientConnect(int Mode);
    int EnterNewPassword(QString &newpsw);
    void Activate(const QString &code, const QString &newpsw);
};

#endif // STARTWINDOW_H
