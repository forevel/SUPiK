#include <QSplashScreen>
#include <QSettings>
#include <QInputDialog>
#include <QGridLayout>
#include <QDir>
#include <QThread>
#include <QCoreApplication>

#include "startwindow.h"
#include "gen/s_sql.h"
#include "gen/s_tablefields.h"
#include "widgets/s_tqcheckbox.h"
#include "dialogs/gen/messagebox.h"
#include "gen/client.h"
#include "gen/log.h"
#include "widgets/wd_func.h"
#include "widgets/s_tqradiobutton.h"

Log *SupLog;

StartWindow::StartWindow(QWidget *parent) : QMainWindow(parent)
{
    Cli = new Client;
    SupLog = new Log;
    SupLog->Init("sup.log");
    SupLog->info("=== Log started ===");
    QPixmap StartWindowSplashPixmap(":/res/2.x.png");
    QSplashScreen *StartWindowSplashScreen = new QSplashScreen(StartWindowSplashPixmap);
    StartWindowSplashScreen->show();

    StartWindowSplashScreen->showMessage("Загрузка языков...", Qt::AlignRight, Qt::white);
    LoadLanguage();

    StartWindowSplashScreen->showMessage("Настройка пользовательского окружения...", Qt::AlignRight, Qt::white);
    SetupUI();

    StartWindowSplashScreen->showMessage("Загрузка переменных окружения...", Qt::AlignRight, Qt::white);
    Startup();

    StartWindowSplashScreen->showMessage("Создание необходимых каталогов...", Qt::AlignRight, Qt::white);
    CreateDirs();

    StartWindowSplashScreen->showMessage("Запуск логов...", Qt::AlignRight, Qt::white);
    StartLogs();

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
    setMinimumSize(QSize(213, 152));
    setMaximumSize(QSize(213, 152));
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
    s_tqPushButton *pb = new s_tqPushButton("Сменить пароль");
    connect(pb,SIGNAL(clicked()),this,SLOT(ChangePassword()));
    StartWindowLayout->addWidget(pb, 4, 0, 1, 3);

    StartWindowLayout->setColumnStretch(0, 0);
    StartWindowLayout->setColumnStretch(2, 0);

    pb = new s_tqPushButton("Вход с помощью кода активации");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(ActivatedEnter()));
    StartWindowLayout->addWidget(pb, 5, 0, 1, 3);

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

int StartWindow::ClientConnect(int Mode)
{
    pc.InitiatePublicClass();
    int res = Cli->Connect(pc.SupikServer, pc.SupikPort, Mode);
    if ((res != Client::CLIER_NOERROR) && (res != Client::CLIER_TIMEOUT))
    {
        MessageBox2::error(this, "Ошибка!", "Ошибка подключения к серверу");
        return RESULTBAD;
    }
    return RESULTOK;
}

void StartWindow::ActivatedEnter()
{
    bool ok = false;
    QString Code = QInputDialog::getText(this,"Ввод кода активации","Введите код:", QLineEdit::Password, "", &ok);
    if (!ok)
        return;
    Cli->Disconnect();
    if (ClientConnect(Client::CLIMODE_TEST) != RESULTOK)
        return;
    pc.AutonomousMode = false;
    QString newpsw;
    if (EnterNewPassword(newpsw) != RESULTOK)
        MessageBox2::error(this,"Ошибка!","Ошибка при смене пароля");
    else
        Activate(Code, newpsw);
}

void StartWindow::ChangePassword()
{
    WDFunc::LEData(this, "UNameLE", pc.PersLogin);
    WDFunc::LEData(this, "PasswdLE", pc.PersPsw);
    if (ClientConnect(Client::CLIMODE_WORK) != RESULTOK)
        return;
    pc.AutonomousMode = false;
    QStringList cmpfl = QStringList() << "login";
    QStringList cmpvl = QStringList() << pc.PersLogin;
    QString idPers = sqlc.GetValueFromTableByFields("sup","personel","idpersonel",cmpfl,cmpvl);
    if (sqlc.result)
        MessageBox2::error(this,"Ошибка!","Не найден пользователь с таким логином");
    else
    {
        QString newpsw, code;
        if (EnterNewPassword(newpsw) != RESULTOK)
            MessageBox2::error(this,"Ошибка!","Ошибка при вводе пароля");
        else
            WDFunc::SetLEData(this, "PasswdLE", newpsw);
        qsrand(QDateTime::currentDateTime().time().second());
        code.fill((qrand()%26)+0x40); // rand(A-Z)
        QStringList fl = QStringList() << "ИД" << "Строка активации";
        QStringList vl = QStringList() << idPers << code;
        tfl.Update("Персонал_полн", fl, vl);
        if (tfl.result != TFRESULT_NOERROR)
        {
            MessageBox2::error(this,"Ошибка!","Ошибка при смене пароля");
            return;
        }
        Activate(code, newpsw);
    }
}

void StartWindow::Activate(const QString &code, const QString &newpsw)
{
    QStringList sl;
    sl << code << newpsw;
    if (!Cli->isConnected())
        return;
    Cli->SendCmd(M_ACTIVATE, sl);
    while (Cli->Busy)
    {
        QThread::msleep(10);
        qApp->processEvents(QEventLoop::AllEvents);
    }
    if (Cli->DetectedError != Client::CLIER_NOERROR)
        MessageBox2::error(this,"Ошибка!","Ошибка при активации, обратитесь к администратору СУПиК");
    else
    {
        WDFunc::SetLEData(this, "PasswdLE", newpsw);
        WDFunc::SetLEData(this, "UNameLE", Cli->ResultStr);
        MessageBox2::information(this,"Успешно!","Активация произведена успешно! Можно заходить под своим именем");
    }
}

int StartWindow::EnterNewPassword(QString &newpsw)
{
    bool ok = false;
    newpsw = QInputDialog::getText(this,"Сменить пароль","Новый пароль:", QLineEdit::Password, "", &ok);
    if (!ok)
        return RESULTBAD;
    QString new2psw = QInputDialog::getText(this,"Сменить пароль","Повторите ввод:", QLineEdit::Password, "", &ok);
    if (!ok)
        return RESULTBAD;
    if (newpsw != new2psw)
    {
        MessageBox2::error(this,"Ошибка!","Введённые строки не совпадают");
        return RESULTBAD;
    }
    return RESULTOK;
}

void StartWindow::OkPBClicked()
{
    QString tmpString;

    WDFunc::LEData(this, "UNameLE", pc.PersLogin);
    WDFunc::LEData(this, "PasswdLE", pc.PersPsw);
    QPixmap StartWindowSplashPixmap(":/res/2.x.png");
    QSplashScreen *StartWindowSplashScreen = new QSplashScreen(StartWindowSplashPixmap);
    StartWindowSplashScreen->show();

    StartWindowSplashScreen->showMessage("Подключение к серверу СУПиК...", Qt::AlignRight, Qt::white);
    Cli->Disconnect();
    pc.AutonomousMode = true;
    int res = Cli->Connect(pc.SupikServer, pc.SupikPort, Client::CLIMODE_WORK);
    switch (res)
    {
    case Client::CLIER_LOGIN:
        MessageBox2::error(this, "Ошибка!", "Не найден такой пользователь");
        return;
        break;
    case Client::CLIER_PSW:
        MessageBox2::error(this, "Ошибка!", "Пароль неверен");
        return;
        break;
    case Client::CLIER_GROUP:
        MessageBox2::error(this, "Ошибка!", "Не найдена группа доступа");
        return;
        break;
    case Client::CLIER_NOERROR:
    {
        bool chbdata;
        WDFunc::ChBData(this, "SaveCB", chbdata);
        if (chbdata)
        {
            pc.SetRegValue("login/login", pc.PersLogin);
            pc.SetRegValue("login/psw", pc.PersPsw);
            pc.SetRegValue("login/ischecked", "true");
        }
        else
        {
            pc.SetRegValue("login/login", "");
            pc.SetRegValue("login/psw", "");
            pc.SetRegValue("login/ischecked", "false");
        }
        pc.AutonomousMode = false;
        INFOMSG("Выполнено подключение к серверу");
        SupLog->info("Server found!");
        break;
    }
    default:
        pc.AutonomousMode = true;
        ERMSG("Сервер СУПиК недоступен");
        break;
    }
    this->hide();

    if (pc.AutonomousMode)
    {
        SupLog->warning("Server not found, entering autonomous mode...");
        StartWindowSplashScreen->showMessage("Сервер недоступен. Попытка подключения к локальной БД...", Qt::AlignRight, Qt::white);
        if (!pc.OpenAndCheckDBs())
        {
            MessageBox2::error(this,"error","Нет соединения с БД, ошибка № " + QString::number(pc.DbNotOpened, 16) + "\n" + pc.LastError);
            SupLog->error("Local database not found, exiting...");
            return;
        }
        tmpString = sqlc.GetValueFromTableByField("sup", "personel", "psw", "login", pc.PersLogin);
        if (tmpString != pc.PersPsw)
        {
            MessageBox2::error(this, "Ошибка!", "Нет такого пользователя или пароль неверен!\n"+sqlc.LastError);
            return;
        }
        QStringList sl, vl;
        sl << "idpersonel" << "personel";
        vl = sqlc.GetValuesFromTableByField("sup", "personel", sl, "login", pc.PersLogin);
        if (sqlc.result)
        {
            MessageBox2::error(this, "Ошибка!", sqlc.LastError);
            SupLog->error(sqlc.LastError);
            return;
        }
        if (vl.size() > 1)
        {
            pc.idPers=vl.at(0).toInt();
            pc.Pers=vl.at(1);
        }
        else
        {
            MessageBox2::error(this, "Ошибка!", "Пользователь не найден!");
            SupLog->error("User not found");
            return;
        }
        sl = QStringList() << "group";
        vl = sqlc.GetValuesFromTableByField("sup", "perspsw", sl, "idpersonel", vl.at(0));
        if ((sqlc.result) || (vl.isEmpty()))
        {
            MessageBox2::error(this, "Ошибка!", sqlc.LastError);
            SupLog->error(sqlc.LastError);
            return;
        }
        pc.idGroup=vl.at(0).toInt(0);
        // считывание прав доступа к СУПиКу
        tmpString = sqlc.GetValueFromTableByID("sup", "groups", "access", QString::number(pc.idGroup));
        if (!tmpString.isEmpty())
            pc.access = tmpString.toLong(0, 16); // права доступа - в hex формате
        else // не нашли запись
        {
            MessageBox2::error(this, "Ошибка!", "Не найдена группа доступа, обратитесь к администратору!");
            SupLog->error("User group not found");
            return;
        }
    }
    else
    {
        QStringList sl, vl;
        sl << "idpersonel" << "personel";
        vl = sqlc.GetValuesFromTableByField("sup", "personel", sl, "login", pc.PersLogin);
        if ((sqlc.result) || (vl.size() < 2))
        {
            MessageBox2::error(this, "Ошибка!", "Пользователь не найден!");
            SupLog->error("User not found");
            return;
        }
        pc.idPers=vl.at(0).toInt();
        pc.Pers=vl.at(1);
    }


        // далее надо открыть только те БД, права на которые есть у товарища
/*            OpenAndCheckDB(pc.alt);
        OpenAndCheckDB(pc.con);
        OpenAndCheckDB(pc.dev);
        OpenAndCheckDB(pc.sch);
        OpenAndCheckDB(pc.sol); */

    StartWindowSplashScreen->finish(this);

    supik *supik_main_window = new supik;
    connect(supik_main_window,SIGNAL(stopall()),Cli,SLOT(StopThreads()));
    supik_main_window->setVisible(true);
    supik_main_window->setEnabled(true);
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
    WDFunc::SetLEData(this, "UNameLE", pc.RegValue("login/login",""));
    WDFunc::SetLEData(this, "PasswdLE", pc.RegValue("login/psw",""));
    WDFunc::SetChBData(this, "SaveCB", (pc.RegValue("login/ischecked",false) == "true") ? true : false);
    QString tmpString = pc.RegValue("settings/lang","");
    if (tmpString.isEmpty())
        OpenSettingsDialog();
    pc.InitiatePublicClass();
    WDFunc::LEData(this, "UNameLE", pc.PersLogin);
    WDFunc::LEData(this, "PasswdLE", pc.PersPsw);
}

void StartWindow::OpenSettingsDialog()
{
    sys_settingsdialog *qssd = new sys_settingsdialog;
    qssd->SetupUI();
    qssd->exec();
}

void StartWindow::CreateDirs()
{
    QStringList sl = QStringList() << "/log" << "/pers/photo" << "/tb/prot" << "/doc/dsheet" << "/certs";
    while (!sl.isEmpty())
    {
        QString Path = pc.HomeDir + sl.takeFirst();
        // http://stackoverflow.com/questions/2241808/checking-if-a-folder-exists-and-creating-folders-in-qt-c
        QDir dir(Path);
        if (!dir.exists())
            dir.mkpath(".");
    }
}

void StartWindow::StartLogs()
{
    Cli->StartLog();
    pc.StartLog();
}
