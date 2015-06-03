#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

#include "supik.h"
#include "dialogs/sys_settingsdialog.h"
#include "gen/publicclass.h"
#include "gen/publiclang.h"

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

private:
    sys_settingsdialog qssd;

    void showEvent(QShowEvent *event);
    void SetupUI();
    void LoadLanguage();
    void Startup();
    void OpenAndCheckDB(QSqlDatabase db, int signid);
    void DBCheck();
    void ReminderCheck();
    void CatalogueCheck();
};

#endif // STARTWINDOW_H