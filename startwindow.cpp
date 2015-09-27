#include <QSplashScreen>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QEventLoop>
#include <QTime>
#include <QGridLayout>

#include "startwindow.h"
#include "gen/s_sql.h"
#include "widgets/s_tqcheckbox.h"

StartWindow::StartWindow(QWidget *parent) : QMainWindow(parent)
{
    QPixmap StartWindowSplashPixmap(":/res/1.x.png");
    QSplashScreen *StartWindowSplashScreen = new QSplashScreen(StartWindowSplashPixmap);
    StartWindowSplashScreen->show();

    StartWindowSplashScreen->showMessage("Загрузка языков...", Qt::AlignRight, Qt::white);
/*    QTime tmr;
    tmr.start();
    while (tmr.elapsed() < 1000)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100); */
    LoadLanguage();

    StartWindowSplashScreen->showMessage("Настройка пользовательского окружения...", Qt::AlignRight, Qt::white);
    SetupUI();

    StartWindowSplashScreen->showMessage("Подключение к базе данных...", Qt::AlignRight, Qt::white);
    Startup();

    StartWindowSplashScreen->finish(this);
}

StartWindow::~StartWindow()
{

}

void StartWindow::showEvent(QShowEvent *event)
{
    s_tqLineEdit *UNameLE = this->findChild<s_tqLineEdit *>("UNameLE");
    if (UNameLE != 0)
        UNameLE->setFocus();
    event->accept();
}

void StartWindow::SetupUI()
{
    setWindowTitle("Супик :: вход");
    resize(213, 136);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setSizePolicy(sizePolicy);
    setMinimumSize(QSize(213, 136));
    setMaximumSize(QSize(213, 136));
    QIcon StartWindowIcon;
    StartWindowIcon.addFile(QString::fromUtf8(":/res/supik.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(StartWindowIcon);
    setStyleSheet("background-color: rgb(204, 204, 204);");
    QWidget *CentralWidget = new QWidget(this);
    s_tqLabel *UNameL = new s_tqLabel("Имя польз.:");
    s_tqLabel *PasswdL = new s_tqLabel("Пароль:");
    s_tqLineEdit *UNameLE = new s_tqLineEdit;
    UNameLE->setObjectName("UNameLE");
    s_tqLineEdit *PasswdLE = new s_tqLineEdit;
    PasswdLE->setObjectName("PasswdLE");
    PasswdLE->setEchoMode(QLineEdit::Password);
    s_tqPushButton *OkPB = new s_tqPushButton("Вход");
    s_tqCheckBox *SaveCB = new s_tqCheckBox;
    SaveCB->setObjectName("SaveCB");
    s_tqLabel *SaveL = new s_tqLabel("Запомнить");
    s_tqPushButton *SystemPB = new s_tqPushButton;
    SystemPB->setMaximumSize(25, 25);
    QIcon SystemPBIcon;
    SystemPBIcon.addFile(":/res/Preferences.png", QSize(), QIcon::Normal, QIcon::Off);
    SystemPB->setIcon(SystemPBIcon);

    QGridLayout *StartWindowLayout = new QGridLayout;
    StartWindowLayout->addWidget(UNameL, 0, 0);
    StartWindowLayout->addWidget(UNameLE, 0, 1, 1, 2);
    StartWindowLayout->addWidget(PasswdL, 1, 0);
    StartWindowLayout->addWidget(PasswdLE, 1, 1, 1, 2);
    StartWindowLayout->addWidget(SaveL, 2, 0);
    StartWindowLayout->addWidget(SaveCB, 2, 1, 1, 2);
    StartWindowLayout->addWidget(OkPB, 3, 0, 1, 2);
    StartWindowLayout->addWidget(SystemPB, 3, 2);

    StartWindowLayout->setColumnStretch(0, 0);
    StartWindowLayout->setColumnStretch(2, 0);

    CentralWidget->setLayout(StartWindowLayout);
    setCentralWidget(CentralWidget);
    QWidget::setTabOrder(UNameLE, PasswdLE);
    QWidget::setTabOrder(PasswdLE, SaveCB);
    QWidget::setTabOrder(SaveCB, OkPB);
    QWidget::setTabOrder(OkPB, SystemPB);

    connect (OkPB, SIGNAL(clicked()), this, SLOT(OkPBClicked()));
    connect (SystemPB, SIGNAL(clicked()), this, SLOT(OpenSettingsDialog()));
    connect (UNameLE, SIGNAL(returnPressed()), this, SLOT(UNameLEReturnPressed()));
    connect (PasswdLE, SIGNAL(returnPressed()), this, SLOT(PasswdLEReturnPressed()));
}

void StartWindow::OkPBClicked()
{
    QString tmpString;

    s_tqLineEdit *UNameLE = this->findChild<s_tqLineEdit *>("UNameLE");
    if (UNameLE == 0)
        return;
    s_tqLineEdit *PasswdLE = this->findChild<s_tqLineEdit *>("PasswdLE");
    if (PasswdLE == 0)
        return;
    s_tqCheckBox *SaveCB = this->findChild<s_tqCheckBox *>("SaveCB");
    if (SaveCB == 0)
        return;
    tmpString = sqlc.GetValueFromTableByField(pc.sup, "personel", "psw", "login", UNameLE->text());
    if (tmpString == PasswdLE->text())
    {
        if (SaveCB->isChecked())
        {
            pc.LandP->setValue("login/login", UNameLE->text());
            pc.LandP->setValue("login/psw", PasswdLE->text());
            pc.LandP->setValue("login/ischecked", SaveCB->isChecked());
        }
        else
        {
            pc.LandP->setValue("login/login", "");
            pc.LandP->setValue("login/psw", "");
            pc.LandP->setValue("login/ischecked", false);
        }

        QStringList sl, vl;
        sl << "idpersonel" << "personel" << "group";
        vl = sqlc.GetValuesFromTableByField(pc.sup, "personel", sl, "login", UNameLE->text());
        if (!vl.isEmpty())
        {
            pc.idPers=vl.at(0).toInt();
            pc.Pers=vl.at(1);
            pc.idGroup=vl.at(2).toInt(0);
        }
        else
        {
            ERMSG(PublicClass::ER_START,__LINE__,"Пользователь не найден!");
            return;
        }

        // считывание прав доступа к СУПиКу
        tmpString = sqlc.GetValueFromTableByID(pc.sup, "groups", "access", QString::number(pc.idGroup));
        if (!tmpString.isEmpty())
            pc.access = tmpString.toLong(0, 16); // права доступа - в hex формате
        else // не нашли запись
        {
            ERMSG(PublicClass::ER_START,__LINE__,"Не найдена группа доступа, обратитесь к администратору!");
            return;
        }

        this->hide();

        QPixmap StartWindowSplashPixmap(":/res/1.x.png");
        QSplashScreen *StartWindowSplashScreen = new QSplashScreen(StartWindowSplashPixmap);
        StartWindowSplashScreen->show();

        StartWindowSplashScreen->showMessage("Проверка целостности данных...", Qt::AlignRight, Qt::white);
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        //        DBCheck();

        StartWindowSplashScreen->showMessage("Проверка наличия напоминаний...", Qt::AlignRight, Qt::white);
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        //        ReminderCheck();

        StartWindowSplashScreen->showMessage("Проверка структуры каталогов...", Qt::AlignRight, Qt::white);
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        //        CatalogueCheck();

        StartWindowSplashScreen->finish(this);

        supik *supik_main_window = new supik;
        supik_main_window->setVisible(true);
        supik_main_window->setEnabled(true);
    }
    else
    {
        ERMSG(PublicClass::ER_START,__LINE__,"Нет такого пользователя или пароль неверен!");
        UNameLE->setFocus();
    }
}

void StartWindow::PasswdLEReturnPressed()
{
    OkPBClicked();
}

void StartWindow::UNameLEReturnPressed()
{
    OkPBClicked();
}

void StartWindow::LoadLanguage()
{
    if (!pl.InitLang())
        pl.SetDefaultLang();
}

void StartWindow::Startup()
{
    s_tqLineEdit *UNameLE = this->findChild<s_tqLineEdit *>("UNameLE");
    if (UNameLE == 0)
        return;
    s_tqLineEdit *PasswdLE = this->findChild<s_tqLineEdit *>("PasswdLE");
    if (PasswdLE == 0)
        return;
    s_tqCheckBox *SaveCB = this->findChild<s_tqCheckBox *>("SaveCB");
    if (SaveCB == 0)
        return;
    UNameLE->setText(pc.LandP->value("login/login","").toString());
    PasswdLE->setText(pc.LandP->value("login/psw","").toString());
    SaveCB->setChecked(pc.LandP->value("login/ischecked",false).toBool());
    QString tmpString = pc.LandP->value("settings/lang","").toString();
    if (tmpString.isEmpty())
        OpenSettingsDialog();
    pc.InitiatePublicClass();
    OpenAndCheckDB(pc.alt, DB_ALT);
    OpenAndCheckDB(pc.con, DB_CON);
    OpenAndCheckDB(pc.dev, DB_DEV);
    OpenAndCheckDB(pc.ent, DB_ENT);
    OpenAndCheckDB(pc.sch, DB_SCH);
    OpenAndCheckDB(pc.sol, DB_SOL);
    OpenAndCheckDB(pc.sup, DB_SUP);
}

void StartWindow::OpenSettingsDialog()
{
    sys_settingsdialog *qssd = new sys_settingsdialog;
    qssd->SetupUI();
    qssd->exec();
}

void StartWindow::OpenAndCheckDB(QSqlDatabase db, int signid)
{
    if (!db.open())
        ERMSG(PublicClass::ER_START,__LINE__,db.lastError().text());
    else
        emit DBOpened(signid);
}

void StartWindow::DBCheck()
{
    pc.DBCheck();
}

void StartWindow::ReminderCheck()
{
    // здесь следует проверка на просроченность экзаменов по ТБ для sys|gi
    // далее следует проверка на приближение сроков окончания регистрации доменов, антивирусных лицензий для sadm
    // далее следует проверка на наличие изделий, находящихся в ремонте (БД repair) для alt
    // далее следует проверка на наличие записей в базах altium, solidworks, schemagee, devices и constructives, которые не имеют записи в номенклатуре
    // далее следует проверка на наличие записей с одинаковым <tble>, но с разным id<tble> в одной таблице
}

void StartWindow::CatalogueCheck()
{

}
