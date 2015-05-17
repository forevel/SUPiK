#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QFrame>
#include <QCloseEvent>
#include <QShowEvent>
#include <QPainter>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>

#include "sys_settingsdialog.h"

sys_settingsdialog::sys_settingsdialog(QWidget *parent) :
    QDialog(parent)
{
}

void sys_settingsdialog::showEvent(QShowEvent *event)
{
    event->accept();
}

void sys_settingsdialog::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void sys_settingsdialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/SysWallpaper.png"));

    event->accept();
    //    QDialog::paintEvent(event);
}

void sys_settingsdialog::SetupUI ()
{
    setWindowTitle("Супик :: Настройки");
    setMinimumSize(QSize(384, 194));
    setMaximumSize(QSize(16661, 11616));
    QIcon qssdIcon;
    qssdIcon.addFile(QString::fromUtf8(":/res/supik.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(qssdIcon);
    s_tqLabel *SettingsL = new s_tqLabel("Настройки");
    QFont font;
    font.setPointSize(15);
    SettingsL->setFont(font);
    s_tqLabel *PathToLibsL = new s_tqLabel("Путь к каталогам библиотек Altium:");
    s_tqLineEdit *PathToLibsLE = new s_tqLineEdit(pc.LandP->value("settings/pathtolibs","////FSERVER//PCAD//Altium//Libs//").toString());
    PathToLibsLE->setObjectName("PathToLibs");
    s_tqLabel *PathToSupikL = new s_tqLabel("Путь к рабочему каталогу СУПиКа");
    s_tqLineEdit *PathToSupikLE = new s_tqLineEdit(pc.LandP->value("settings/pathtosup","////NS//SUPiK").toString());
    PathToSupikLE->setObjectName("PathToSupik");
    s_tqPushButton *isOKPB = new s_tqPushButton (QColor(0,135,0), "Ага");
    s_tqPushButton *CancelPB = new s_tqPushButton (QColor(185,0,0), "Неа");
    CancelPB->setMaximumSize(75, 23);
    s_tqPushButton *ChooseDirPB = new s_tqPushButton("...");
    s_tqPushButton *ChooseSDirPB = new s_tqPushButton("...");
    s_tqLabel *LangL = new s_tqLabel("Язык системы:");
    s_tqComboBox *LangCB = new s_tqComboBox;
    connect(LangCB,SIGNAL(currentIndexChanged(QString)),this,SLOT(LangChoosed(QString)));
    QIcon RusIcon;
    QIcon EngIcon;
    RusIcon.addFile(":/res/LangRU.png", QSize(), QIcon::Normal, QIcon::Off);
    LangCB->addItem(RusIcon, "RU");
    EngIcon.addFile(":/res/langGB.png", QSize(), QIcon::Normal, QIcon::Off);
    LangCB->addItem(EngIcon, "EN");
    LangCB->setCurrentText(pc.LandP->value("settings/lang","EN").toString());
    s_tqLabel *timerperiodL = new s_tqLabel("Период обновления информации о новых компонентах (мин)");
    s_tqSpinBox *timerperiodSB = new s_tqSpinBox;
    timerperiodSB->setMinimum(0.1);
    timerperiodSB->setMaximum(9999.0);
    timerperiodSB->setDecimals(1);
    timerperiodSB->setSingleStep(0.1);
    connect(timerperiodSB,SIGNAL(valueChanged(double)),this,SLOT(TimerPeriodChoosed(double)));
    timerperiodSB->setValue(pc.LandP->value("settings/timerperiod","1").toDouble());
    s_tqLineEdit *SQLPathLE = new s_tqLineEdit(pc.LandP->value("settings/SQLPath","localhost").toString());
    SQLPathLE->setObjectName("SQLPath");
    s_tqLabel *SQLPathL = new s_tqLabel("Адрес SQL-сервера:");

    QVBoxLayout *MainLayout = new QVBoxLayout;
    QFrame *MainFrame = new QFrame;
    MainFrame->setFrameShape(QFrame::NoFrame);
    MainFrame->setMinimumHeight(80);
    QGridLayout *GridLayout = new QGridLayout;
    GridLayout->addWidget(PathToLibsL, 0, 0);
    GridLayout->addWidget(PathToLibsLE, 0, 1);
    GridLayout->addWidget(ChooseDirPB, 0, 2);
    GridLayout->addWidget(PathToSupikL, 1, 0);
    GridLayout->addWidget(PathToSupikLE, 1, 1);
    GridLayout->addWidget(ChooseSDirPB, 1, 2);
    GridLayout->addWidget(SQLPathL, 2, 0);
    GridLayout->addWidget(SQLPathLE, 2, 1,1,2);
    GridLayout->addWidget(LangL, 3, 0);
    GridLayout->addWidget(LangCB, 3, 1,1,2);
    GridLayout->addWidget(timerperiodL, 4, 0,1,2);
    GridLayout->addWidget(timerperiodSB, 4, 2);
    GridLayout->setColumnStretch(0, 0);
    GridLayout->setColumnStretch(1, 100);
    GridLayout->setColumnStretch(2, 0);
    MainFrame->setAttribute(Qt::WA_TranslucentBackground);
    MainFrame->setLayout(GridLayout);

    QFrame *PBFrame = new QFrame;
    PBFrame->setFrameShape(QFrame::NoFrame);
    PBFrame->setAttribute(Qt::WA_TranslucentBackground);
    QHBoxLayout *PBLayout = new QHBoxLayout;
    PBLayout->addWidget(isOKPB);
    PBLayout->addWidget(CancelPB);
    PBFrame->setLayout(PBLayout);

    MainLayout->addWidget(SettingsL);
    MainLayout->setAlignment(SettingsL, Qt::AlignRight);
    MainLayout->addWidget(MainFrame);
    MainLayout->addWidget(PBFrame);
    MainLayout->addStretch(50);
    setLayout(MainLayout);

    connect (isOKPB, SIGNAL(clicked()), this, SLOT(OKPBClicked()));
    connect (CancelPB, SIGNAL(clicked()), this, SLOT(CancelPBClicked()));\
    connect (ChooseDirPB, SIGNAL(clicked()), this, SLOT(ChooseDirPBClicked()));
    connect (ChooseSDirPB, SIGNAL(clicked()), this, SLOT(ChooseSDirPBClicked()));
}

void sys_settingsdialog::LangChoosed(QString lang)
{
    Lang = lang;
}

void sys_settingsdialog::TimerPeriodChoosed(double dbl)
{
    TimerPeriod = dbl;
}

void sys_settingsdialog::CancelPBClicked()
{
    this->close();
}

void sys_settingsdialog::OKPBClicked()
{
    QLineEdit *PathToLibsLE = this->findChild<QLineEdit *>("PathToLibs");
    if (PathToLibsLE == 0)
        return;
    QLineEdit *PathToSupikLE = this->findChild<QLineEdit *>("PathToSupik");
    if (PathToSupikLE == 0)
        return;
    QLineEdit *SQLPathLE = this->findChild<QLineEdit *>("SQLPath");
    if (SQLPathLE == 0)
        return;
    QString tmpString = PathToLibsLE->text() + "//Symbols//Capasitors.SchLib";
    QFile file;

    file.setFileName(tmpString);
    if (!file.exists())
    {
        QMessageBox::warning(this,"Внимание!",\
                              "Неправильный путь к файлам библиотеки",\
                              QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    else
    {
        pc.LandP->setValue("settings/pathtolibs",PathToLibsLE->text());
        pc.PathToLibs = PathToLibsLE->text();
        pc.LandP->setValue("settings/pathtosup",PathToSupikLE->text());
        pc.PathToSup = PathToSupikLE->text();
        pc.LandP->setValue("settings/lang",Lang);
        pc.SQLPath = SQLPathLE->text();
        pc.LandP->setValue("settings/SQLPath",SQLPathLE->text());
        pc.LandP->setValue("settings/timerperiod",TimerPeriod);
        pc.InitiatePublicClass();

        if (!pl.InitLang())
            pl.SetDefaultLang();

        this->close();
    }
}

void sys_settingsdialog::ChooseDirPBClicked()
{
    QLineEdit *PathToLibsLE = this->findChild<QLineEdit *>("PathToLibs");
    if (PathToLibsLE == 0)
        return;
    QString DirName = QFileDialog::getExistingDirectory(this, \
                                                        "Выберите каталог с файлами библиотек (содержит каталоги Symbols и Footprints)"                                                        , \
                                                        pc.PathToLibs,
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (DirName != "")
        PathToLibsLE->setText(DirName);
}

void sys_settingsdialog::ChooseSDirPBClicked()
{
    QLineEdit *PathToSupikLE = this->findChild<QLineEdit *>("PathToSupik");
    if (PathToSupikLE == 0)
        return;
    QString DirName = QFileDialog::getExistingDirectory(this, \
                                                        "Выберите каталог файлами СУПиК", \
                                                        pc.PathToSup,
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (DirName != "")
        PathToSupikLE->setText(DirName);
}
