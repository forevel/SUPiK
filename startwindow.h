#ifndef STARTWINDOW_H
#define STARTWINDOW_H

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
    explicit StartWindow(QWidget *parent = 0);
    ~StartWindow();
    supik supik_main_window;

signals:

public slots:

private slots:
    void OkPBClicked();
    void PasswdLEReturnPressed();
    void UNameLEReturnPressed();
    void SystemPBClicked();

private:
    QSplashScreen *StartWindowSplashScreen;
    QGridLayout *StartWindowLayout;
    QHBoxLayout *StartWindowHBoxLayout;
//    QWidget *CentralWidget;
    QLabel *UNameL;
    QLabel *PasswdL;
    QLineEdit *UNameLE;
    QLineEdit *PasswdLE;
    QPushButton *OkPB;
    QCheckBox *SaveCB;
    QLabel *SaveL;
    QPushButton *SystemPB;
    QStatusBar *statusBar;
//    QIcon StartWindowIcon, SystemPBIcon;
    sys_settingsdialog qssd;

    void showEvent(QShowEvent *event);
    void SetupUI();
    void LoadLanguage();
    void Startup();
    void OpenAndCheckDB(QSqlDatabase db);
    void DBCheck();
    void ReminderCheck();
    void CatalogueCheck();
};

#endif // STARTWINDOW_H
