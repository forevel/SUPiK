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

sys_settingsdialog::sys_settingsdialog()
{
    SetupUI();
    FirstShow = true;
}

void sys_settingsdialog::showEvent(QShowEvent *event)
{
    if (FirstShow)
    {
        SettingsL->setText("Настройки");
        PathToLibsL->setText("Путь к каталогам библиотек Altium:");
        LangL->setText("Язык системы:");
        SQLPathL->setText("Адрес SQL-сервера:");
        isOKPB->setText("Ага");
        CancelPB->setText("Неа");
        PathToSupikL->setText("Путь к рабочему каталогу СУПиКа");
        timerperiodL->setText("Период обновления информации о новых компонентах (мин)");
        setWindowTitle(pl.WindowTitlesMessages[1]);
        FirstShow = false;
    }
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
//    resize(384, 374);
    setMinimumSize(QSize(384, 194));
    setMaximumSize(QSize(16661, 11616));
    qssdIcon.addFile(QString::fromUtf8(":/res/supik.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(qssdIcon);
    SettingsL = new s_tqLabel;
    font.setPointSize(15);
    SettingsL->setFont(font);
    PathToLibsL = new s_tqLabel;
    PathToLibsLE = new s_tqLineEdit;
    PathToSupikL = new s_tqLabel;
    PathToSupikLE = new s_tqLineEdit;
//    QColor color(0, 135, 0);
    isOKPB = new s_tqPushButton (QColor(0,135,0));
//    isOKPB->setStyleSheet("color: rgb(0, 135, 0);");
//    isOKPB->setMaximumSize(75, 23);
//    color.setRgb(185, 0, 0);
    CancelPB = new s_tqPushButton (QColor(185,0,0));
//    CancelPB->setStyleSheet("color: rgb(185, 0, 0);");
    CancelPB->setMaximumSize(75, 23);
    ChooseDirPB = new s_tqPushButton("...");
    ChooseSDirPB = new s_tqPushButton("...");
    LangL = new s_tqLabel;
    LangCB = new s_tqComboBox;
    RusIcon.addFile(":/res/LangRU.png", QSize(), QIcon::Normal, QIcon::Off);
    LangCB->addItem(RusIcon, "RU");
    EngIcon.addFile(":/res/langGB.png", QSize(), QIcon::Normal, QIcon::Off);
    LangCB->addItem(EngIcon, "EN");
    LangCB->setCurrentIndex(0);
    timerperiodL = new s_tqLabel;
    timerperiodSB = new s_tqspinbox;
    SQLPathLE = new s_tqLineEdit;
    SQLPathL = new s_tqLabel;

    MainLayout = new QVBoxLayout;
    MainFrame = new QFrame;
    MainFrame->setFrameShape(QFrame::NoFrame);
    MainFrame->setMinimumHeight(80);
    GridLayout = new QGridLayout;
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

    PBFrame = new QFrame;
    PBFrame->setFrameShape(QFrame::NoFrame);
    PBFrame->setAttribute(Qt::WA_TranslucentBackground);
    PBLayout = new QHBoxLayout;
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

void sys_settingsdialog::CancelPBClicked()
{
    this->close();
}

void sys_settingsdialog::OKPBClicked()
{
    QString tmpString = PathToLibsLE->text() + "//Symbols//Capasitors.SchLib";
    QFile file;

    file.setFileName(tmpString);
    if (!file.exists())
    {
        QMessageBox::warning(this,pl.DialogMessages[201],\
                              pl.DialogMessages[206],\
                              QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    else
    {
        pc.LandP->setValue("settings/pathtolibs",PathToLibsLE->text());
        pc.PathToLibs = PathToLibsLE->text();
        pc.LandP->setValue("settings/pathtosup",PathToSupikLE->text());
        pc.PathToSup = PathToSupikLE->text();
        pc.LandP->setValue("settings/lang",LangCB->currentText());
        pc.SQLPath = SQLPathLE->text();
        pc.LandP->setValue("settings/SQLPath",SQLPathLE->text());
        pc.timerperiod = timerperiodSB->value();
        pc.LandP->setValue("settings/timerperiod",timerperiodSB->text());
        pc.InitiatePublicClass(); // 0.4-a

        if (!pl.InitLang())
            pl.SetDefaultLang();

        this->close();
    }
}

void sys_settingsdialog::ChooseDirPBClicked()
{
    QString DirName = QFileDialog::getExistingDirectory(this, \
                                                        pl.DialogMessages[9], \
                                                        pc.PathToLibs,
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (DirName != "")
        PathToLibsLE->setText(DirName);
}

void sys_settingsdialog::ChooseSDirPBClicked()
{
    QString DirName = QFileDialog::getExistingDirectory(this, \
                                                        pl.DialogMessages[9], \
                                                        pc.PathToSup,
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (DirName != "")
        PathToSupikLE->setText(DirName);
}
