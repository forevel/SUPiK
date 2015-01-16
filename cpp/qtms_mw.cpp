#include <QtWidgets>

#include "../inc/s_sql.h"
#include "../inc/qtms_mw.h"

QTMS_MW::QTMS_MW()
{
    QPixmap QTMS_MWSplashPixmap(":/pic/Pic/1.x.png");
    QTMS_MWSplashScreen = new QSplashScreen(QTMS_MWSplashPixmap);
    QTMS_MWSplashScreen->show();

    QTMS_MWSplashScreen->showMessage("Language check...", Qt::AlignRight, Qt::white);
    qApp->processEvents();
//    Sleep(1000);
    LoadLanguage();

    QTMS_MWSplashScreen->showMessage("Настройка пользовательского окружения...", Qt::AlignRight, Qt::white);
    qApp->processEvents();
//    Sleep(1000);
    SetupUI();

    QTMS_MWSplashScreen->showMessage("Подключение к базе данных...", Qt::AlignRight, Qt::white);
    qApp->processEvents();
//    Sleep(1000);
    Startup();

    QTMS_MWSplashScreen->finish(this);
}

void QTMS_MW::showEvent(QShowEvent *event)
{
    setWindowTitle("Супик :: вход");
    UNameL->setText("Имя польз.:");
    PasswdL->setText("Пароль:");
    SaveL->setText("Запомнить");
    OkPB->setText("Вход");
    UNameLE->setFocus();
    event->accept();
}

void QTMS_MW::SetupUI()
{
    resize(213, 136);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setSizePolicy(sizePolicy);
    setMinimumSize(QSize(206, 130));
//    setMaximumSize(QSize(16661, 11616));
    QTMS_MWIcon.addFile(QString::fromUtf8(":/pic/Pic/supik.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(QTMS_MWIcon);
    setStyleSheet("background-color: rgb(204, 204, 204);");
    CentralWidget = new QWidget(this);
    UNameL = new QLabel;
    PasswdL = new QLabel;
    UNameLE = new QLineEdit;
    PasswdLE = new QLineEdit;
    PasswdLE->setEchoMode(QLineEdit::Password);
    OkPB = new QPushButton;
    SaveCB = new QCheckBox;
    SaveL = new QLabel;
    SystemPB = new QPushButton;
    SystemPB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    SystemPB->setMinimumSize(10, 10);
    SystemPB->setMaximumSize(23, 23);
    SystemPB->setGeometry(QRect(187, 95, 10, 10));
    SystemPBIcon.addFile(":/pic/Pic/Preferences.png", QSize(), QIcon::Normal, QIcon::Off);
    SystemPB->setIcon(SystemPBIcon);
    OkPB = new QPushButton;

    QTMS_MWLayout = new QGridLayout(CentralWidget);
    QTMS_MWLayout->addWidget(UNameL, 0, 0);
    QTMS_MWLayout->addWidget(UNameLE, 0, 1, 1, 2);
    QTMS_MWLayout->addWidget(PasswdL, 1, 0);
    QTMS_MWLayout->addWidget(PasswdLE, 1, 1, 1, 2);
    QTMS_MWLayout->addWidget(SaveL, 2, 0);
    QTMS_MWLayout->addWidget(SaveCB, 2, 1, 1, 2);
    QTMS_MWLayout->addWidget(OkPB, 3, 0, 1, 2);
    QTMS_MWLayout->addWidget(SystemPB, 3, 2);

    QTMS_MWLayout->setColumnStretch(0, 0);
    QTMS_MWLayout->setColumnStretch(2, 0);

    setCentralWidget(CentralWidget);
    QWidget::setTabOrder(UNameLE, PasswdLE);
    QWidget::setTabOrder(PasswdLE, SaveCB);
    QWidget::setTabOrder(SaveCB, OkPB);
    QWidget::setTabOrder(OkPB, SystemPB);

    connect (OkPB, SIGNAL(clicked()), this, SLOT(OkPBClicked()));
    connect (SystemPB, SIGNAL(clicked()), this, SLOT(SystemPBClicked()));
    connect (UNameLE, SIGNAL(returnPressed()), this, SLOT(UNameLEReturnPressed()));
    connect (PasswdLE, SIGNAL(returnPressed()), this, SLOT(PasswdLEReturnPressed()));
}

void QTMS_MW::OkPBClicked()
{
    QString tmpString;

    tmpString = sqlc.getvaluefromtablebyfield(pc.sup, "personel", "psw", "login", UNameLE->text());
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
        vl = sqlc.getvaluesfromtablebyfield(pc.sup, "personel", sl, "login", UNameLE->text());
        if (!vl.isEmpty())
        {
            pc.idPers=vl.at(0).toInt();
            pc.Pers=vl.at(1);
            pc.idGroup=vl.at(2).toInt(0);
        }
        else
        {
            QMessageBox::warning(this,"Ошибка!", "Пользователь не найден!",\
                             QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }

        // считывание прав доступа к СУПиКу
        tmpString = sqlc.getvaluefromtablebyid(pc.sup, "groups", "access", QString::number(pc.idGroup));
        if (!tmpString.isEmpty())
            pc.access = tmpString.toLong(0, 16); // права доступа - в hex формате
        else // не нашли запись
        {
            QMessageBox::warning(this,"Ошибка!",\
                             "Не найдена группа доступа, обратитесь к администратору!",\
                             QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }

        this->hide();

        QPixmap QTMS_MWSplashPixmap(":/pic/Pic/1.x.png");
        QTMS_MWSplashScreen = new QSplashScreen(QTMS_MWSplashPixmap);
        QTMS_MWSplashScreen->show();

        QTMS_MWSplashScreen->showMessage("Проверка целостности данных...", Qt::AlignRight, Qt::white);
        qApp->processEvents();
    //    Sleep(1000);
//        DBCheck();

        QTMS_MWSplashScreen->showMessage("Проверка наличия напоминаний...", Qt::AlignRight, Qt::white);
        qApp->processEvents();
    //    Sleep(1000);
//        ReminderCheck();

        QTMS_MWSplashScreen->showMessage("Проверка структуры каталогов...", Qt::AlignRight, Qt::white);
        qApp->processEvents();
    //    Sleep(1000);
//        CatalogueCheck();

        QTMS_MWSplashScreen->finish(this);

        supik_main_window.setVisible(true);
        supik_main_window.setEnabled(true);
    }
    else
    {
        QMessageBox::warning(this,"Ошибка!", "Нет такого пользователя или пароль неверен!",\
                         QMessageBox::Ok, QMessageBox::NoButton);
        UNameLE->setFocus();
    }
}

void QTMS_MW::PasswdLEReturnPressed()
{
    OkPBClicked();
}

void QTMS_MW::UNameLEReturnPressed()
{
    OkPBClicked();
}

void QTMS_MW::SystemPBClicked()
{
    qssd.PathToLibsLE->setText(pc.LandP->value("settings/pathtolibs","////FSERVER//PCAD//Altium//Libs//").toString());
    qssd.PathToSupikLE->setText(pc.LandP->value("settings/pathtosup","////NS//SUPiK").toString());
    qssd.LangCB->setCurrentText(pc.LandP->value("settings/lang","RU").toString());
    qssd.SQLPathLE->setText(pc.LandP->value("settings/SQLPath","localhost").toString());
    qssd.timerperiodSB->setValue(pc.LandP->value("settings/timerperiod","1").toInt());
    qssd.show();
    qssd.setFocus();
}

void QTMS_MW::LoadLanguage()
{
    if (!pl.InitLang())
        pl.SetDefaultLang();
}

void QTMS_MW::Startup()
{
    UNameLE->setText(pc.LandP->value("login/login","").toString());
    PasswdLE->setText(pc.LandP->value("login/psw","").toString());
    SaveCB->setChecked(pc.LandP->value("login/ischecked",false).toBool());
    QString tmpString = pc.LandP->value("settings/lang","").toString();
    if (tmpString.isEmpty())
    {
        qssd.PathToLibsLE->setText(pc.LandP->value("settings/pathtolibs","////FSERVER//PCAD//Altium//Libs//").toString());
        qssd.PathToSupikLE->setText(pc.LandP->value("settings/pathtosup","////NS//SUPiK").toString());
        qssd.LangCB->setCurrentText(pc.LandP->value("settings/lang","EN").toString());
        qssd.show();
    }
    pc.InitiatePublicClass();
    OpenAndCheckDB(pc.alt);
    OpenAndCheckDB(pc.con);
    OpenAndCheckDB(pc.dev);
    OpenAndCheckDB(pc.ent);
    OpenAndCheckDB(pc.sch);
    OpenAndCheckDB(pc.sol);
    OpenAndCheckDB(pc.sup);
}

void QTMS_MW::OpenAndCheckDB(QSqlDatabase db)
{
    if (!db.open())
        QMessageBox::critical(this, "Ошибка!", db.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
}

void QTMS_MW::DBCheck()
{
    pc.DBCheck();
}

void QTMS_MW::ReminderCheck()
{
    // здесь следует проверка на просроченность экзаменов по ТБ для sys|gi
    // далее следует проверка на приближение сроков окончания регистрации доменов, антивирусных лицензий для sadm
    // далее следует проверка на наличие изделий, находящихся в ремонте (БД repair) для alt
    // далее следует проверка на наличие записей в базах altium, solidworks, schemagee, devices и constructives, которые не имеют записи в номенклатуре
    // далее следует проверка на наличие записей с одинаковым <tble>, но с разным id<tble> в одной таблице
}

void QTMS_MW::CatalogueCheck()
{

}
