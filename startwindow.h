#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

#include "supik.h"
#include "dialogs/sys/sys_settingsdialog.h"
#include "gen/publicclass.h"
#include "gen/publiclang.h"

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

private:
    sys_settingsdialog qssd;

    void showEvent(QShowEvent *event);
    void SetupUI();
    void LoadLanguage();
    void Startup();
    int ClientConnect(int Mode);
    int EnterAndChangePassword(const QString &idPers, QString &newpsw);
};

#endif // STARTWINDOW_H
