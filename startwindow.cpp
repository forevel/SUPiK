#include <QSplashScreen>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QEventLoop>
#include <QTime>
#include <QInputDialog>
#include <QGridLayout>

#include "startwindow.h"
#include "gen/s_sql.h"
#include "widgets/s_tqcheckbox.h"
#include "dialogs/messagebox.h"
#include "gen/sftp.h"

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
    s_tqPushButton *pb = new s_tqPushButton("Сменить пароль");
    connect(pb,SIGNAL(clicked()),this,SLOT(ChangePassword()));
    StartWindowLayout->addWidget(pb, 3, 0, 1, 3);
    StartWindowLayout->addWidget(OkPB, 4, 0, 1, 2);
    StartWindowLayout->addWidget(SystemPB, 4, 2);

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

void StartWindow::ChangePassword()
{
    s_tqLineEdit *UNameLE = this->findChild<s_tqLineEdit *>("UNameLE");
    s_tqLineEdit *PasswdLE = this->findChild<s_tqLineEdit *>("PasswdLE");
    if ((UNameLE == 0) || (PasswdLE == 0))
    {
        MessageBox::error(this,"Системная ошибка","Отсутствуют элементы в диалоге, строка "+QString::number(__LINE__));
        return;
    }
    QString login = UNameLE->text();
    QString psw = PasswdLE->text();
    QStringList cmpfl = QStringList() << "login" << "psw";
    QStringList cmpvl = QStringList() << login << psw;
    sqlc.GetValueFromTableByFields(sqlc.GetDB("sup"),"personel","idpersonel",cmpfl,cmpvl);
    if (sqlc.result)
        MessageBox::error(this,"Ошибка!","Не найден пользователь с таким логином/паролем");
    else
    {
        bool ok = false;
        QString newpsw = QInputDialog::getText(this,"Сменить пароль","Новый пароль:", QLineEdit::Password, "", &ok);
        if (!ok)
            return;
        QString new2psw = QInputDialog::getText(this,"Сменить пароль","Повторите ввод:", QLineEdit::Password, "", &ok);
        if (!ok)
            return;
        if (newpsw != new2psw)
        {
            MessageBox::error(this,"Ошибка!","Введённые строки не совпадают");
            return;
        }
        sqlc.UpdateValuesInTable(sqlc.GetDB("sup"),"personel",QStringList("psw"),QStringList(newpsw),"login",login);
        if (sqlc.result)
            MessageBox::error(this,"Ошибка!","Ошибка при смене пароля");
        else
            PasswdLE->setText(newpsw);
    }
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
            STARTER("Пользователь не найден!");
            return;
        }

        // считывание прав доступа к СУПиКу
        tmpString = sqlc.GetValueFromTableByID(pc.sup, "groups", "access", QString::number(pc.idGroup));
        if (!tmpString.isEmpty())
            pc.access = tmpString.toLong(0, 16); // права доступа - в hex формате
        else // не нашли запись
        {
            STARTER("Не найдена группа доступа, обратитесь к администратору!");
            return;
        }

        this->hide();

        QPixmap StartWindowSplashPixmap(":/res/1.x.png");
        QSplashScreen *StartWindowSplashScreen = new QSplashScreen(StartWindowSplashPixmap);
        StartWindowSplashScreen->show();

        StartWindowSplashScreen->showMessage("Проверка наличия подключения к каталогу СУПиК...", Qt::AlignRight, Qt::white);
        bool FtpCheck = sftp.CheckFtp();
        if (!FtpCheck)
            STARTER("Каталог СУПиК недоступен");

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
