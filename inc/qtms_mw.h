#ifndef QTMS_MW_H
#define QTMS_MW_H

#include <QMainWindow>

#include "supik.h"
#include "sys_settingsdialog.h"
#include "../inc/publicclass.h"
#include "../inc/publiclang.h"

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

class QTMS_MW : public QMainWindow
{
    Q_OBJECT
    
public:
    supik supik_main_window;
    QTMS_MW ();

private slots:
    void OkPBClicked();

    void PasswdLEReturnPressed();

    void UNameLEReturnPressed();

    void SystemPBClicked();

private:
    QSplashScreen *QTMS_MWSplashScreen;
    QGridLayout *QTMS_MWLayout;
    QHBoxLayout *QTMS_MWHBoxLayout;
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
    QIcon QTMS_MWIcon, SystemPBIcon;

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

#endif // QTMS_MW_H
