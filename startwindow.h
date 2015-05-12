#ifndef StartWindow_H
#define StartWindow_H

#include <QMainWindow>

#include "supik.h"
#include "dialogs/sys_settingsdialog.h"
#include "gen/publicclass.h"
#include "gen/publiclang.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
class QLabel;
class QCheckBox;
class QIcon;
class QGridLayout;
class QHBoxLayout;
class QPixmap;
class QSplashScreen;
QT_END_NAMESPACE

class StartWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    supik supik_main_window;
    StartWindow ();

private slots:
    void OkPBClicked();

    void PasswdLEReturnPressed();

    void UNameLEReturnPressed();

    void SystemPBClicked();

private:
    QSplashScreen *StartWindowSplashScreen;
    QGridLayout *StartWindowLayout;
    QHBoxLayout *StartWindowHBoxLayout;
    QWidget *CentralWidget;
    QLabel *UNameL;
    QLabel *PasswdL;
    QLineEdit *UNameLE;
    QLineEdit *PasswdLE;
    QPushButton *OkPB;
    QCheckBox *SaveCB;
    QLabel *SaveL;
    QPushButton *SystemPB;
    QStatusBar *statusBar;
    QIcon StartWindowIcon, SystemPBIcon;

    void showEvent(QShowEvent *event);

    void SetupUI();
    void LoadLanguage();
    void Startup();
    void OpenAndCheckDB(QSqlDatabase db);
    void DBCheck();
    void ReminderCheck();
    void CatalogueCheck();
    sys_settingsdialog qssd;
};

#endif // StartWindow_H
