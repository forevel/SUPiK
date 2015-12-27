#include <QtWidgets>
#include <QTimer>
#include <QPropertyAnimation>
#include "supik.h"
#include "dialogs/dev/dev_docdialog.h"
#include "dialogs/dev/dev_devdialog.h"
#include "dialogs/sys/sys_settingsdialog.h"
#include "dialogs/sys/sys_systemdialog.h"
#include "dialogs/dir/dir_maindialog.h"
#include "dialogs/sys/sys_probsdialog.h"
#include "dialogs/cmp/cmp_compdialog.h"
#include "dialogs/wh/wh_dialog.h"
#include "dialogs/wh/wh_editor.h"
#include "dialogs/sys/sys_backuprestoredirdialog.h"
#include "dialogs/sys/sys_importclass.h"
#include "widgets/s_tqlabel.h"
#include "widgets/s_colortabwidget.h"
#include "widgets/s_tqtableview.h"
#include "widgets/errorprotocolwidget.h"
#include "gen/publiclang.h"
#include "gen/s_sql.h"

supik::supik()
{
    ERTimer = new QTimer;
    ERTimer->setInterval(5000);
    connect(ERTimer,SIGNAL(timeout()),this,SLOT(HideErrorProtocol()));
    ERTimerIsOn = false;
    SetSupikWindow();
    SetSupikStatusBar();
    pc.supikprocs << "ExitSupik" << "SysStructEdit" << "SettingsEdit" << "Components" << "Directories" << "BackupDir" << "RestoreDir" << "ProbCheck";
    pc.supikprocs << "WhIncome" << "WhOutgoing" << "WhSearch" << "DevDoc" << "DevDev" << "Quarantine" << "" << "SysImportClass";
    pf["ExitSupik"] = &supik::ExitSupik;
    pf["SysStructEdit"] = &supik::SysStructEdit;
    pf["SettingsEdit"] = &supik::SettingsEdit;
    pf["Components"] = &supik::Components;
    pf["Directories"] = &supik::Directories;
    pf["BackupDir"] = &supik::BackupDir;
    pf["RestoreDir"] = &supik::RestoreDir;
    pf["ProbCheck"] = &supik::ProbCheck;
    pf["WhIncome"] = &supik::WhIncome;
    pf["WhOutgoing"] = &supik::WhOutgoing;
    pf["WhSearch"] = &supik::WhSearch;
    pf["WhEditor"] = &supik::WhEditor;
    pf["Quarantine"] = &supik::Quarantine;
    pf["SysDirectories"] = &supik::SysDirectories;
    pf["DevDoc"] = &supik::DevDoc;
    pf["DevDev"] = &supik::DevDev;
    pf["Dummy"]=&supik::Dummy;
    pf["SysImportClass"] = &supik::SysImportClass;
}

void supik::showEvent(QShowEvent *event)
{
    QTimer *timer1s = new QTimer;
    timer1s->setInterval(1000);
    connect (timer1s, SIGNAL(timeout()), this, SLOT(periodic1s()));
    QTimer *timer1m = new QTimer;
//    timer1m->setInterval(pc.timerperiod*60000);
    timer1m->setInterval(pc.timerperiod*3000);
    connect (timer1m, SIGNAL(timeout()), this, SLOT(periodicxm()));
    timer1s->start();
//    timer1m->start();
    event->accept();
}

void supik::SetSupikWindow()
{
    QTimer *MouseTimer = new QTimer;
    connect(MouseTimer,SIGNAL(timeout()),this,SLOT(MouseMove()));
    MouseTimer->start(50);
    QTimer *ErrorProtocolUpdateTimer = new QTimer;
    ErrorProtocolUpdateTimer->setInterval(1000);
    connect(ErrorProtocolUpdateTimer,SIGNAL(timeout()),this,SLOT(UpdateErrorProtocol()));
    ErrorProtocolUpdateTimer->start();
    QString tmps = "Супик "+QString(PROGVER);
    setWindowTitle(tmps);
    resize (984, 688);
    QIcon SupikIcon;
    SupikIcon.addFile(QString::fromUtf8(":/res/supik.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(SupikIcon);
    setStyleSheet("background-color: rgb(204, 204, 204);");
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *upperLayout = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/kniga_logo.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(Directories()));
    pb->setToolTip("Редактор справочников");
    upperLayout->addWidget(pb);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/microshema.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(Components()));
    pb->setToolTip("Редактор компонентов");
    upperLayout->addWidget(pb);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/whPB.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(WhEditor()));
    pb->setToolTip("Редактор складов");
    upperLayout->addWidget(pb);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Выход из программы");
    upperLayout->addWidget(pb);

    upperLayout->addStretch(99);
    s_tqLabel *datetime = new s_tqLabel;
    datetime->setObjectName("datetime");
    upperLayout->addWidget(datetime, 0);
    mainLayout->addLayout(upperLayout, 0);
    S_ColorTabWidget *MainTW = new S_ColorTabWidget;
    MainTW->setObjectName("MainTW");
    mainLayout->addWidget(MainTW, 100);

    QWidget *wdgt = new QWidget;
    wdgt->setLayout(mainLayout);
    setCentralWidget(wdgt);
    SetSupikMenuBar();

    ErrorProtocolWidget *ErrorWidget = new ErrorProtocolWidget(this);
    ErrorWidget->setObjectName("errorwidget");
    QString ErrWss = "QWidget {background-color: "+QString(ERPROTCLR)+";}";
    ErrorWidget->setStyleSheet(ErrWss);
    ErrorWidget->setAutoFillBackground(true);
    ErrorWidget->setMinimumHeight(150);
    ErrorWidget->hide();
    ERGeometry = ErrorWidget->geometry();
    ERHide = true;
}

void supik::SetSupikMenuBar()
{
    QMenu *tmpMenu;
    QAction* tmpAction;
    QString tmpString;
    int tmpInt;
    tmpMenu = new QMenu;
    QMenuBar *SupikMenuBar = new QMenuBar;
    SupikMenuBar->setObjectName("MenuBar");
    QSqlQuery get_mainmenu (pc.sup);
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    tmpString="SELECT `idmainmenu`,`alias`,`access`,`tooltip`,`method` FROM `mainmenu` WHERE " \
                      "`idalias`=0 AND `deleted`=0 ORDER BY `order` ASC;";
    get_mainmenu.exec(tmpString);
    while (get_mainmenu.next())
    {
        if (get_mainmenu.value(2).toString().toLong(0, 16) & pc.access)
        {
            tmpInt = get_mainmenu.value(0).toInt(0);
            tmpMenu = AddChildToMenu (tmpInt);
            tmpString = get_mainmenu.value(1).toString();
            if (tmpMenu == NULL) // нет потомков
            {
                tmpAction = new QAction(this);
                tmpAction->setText(get_mainmenu.value(1).toString());
                connect (tmpAction, SIGNAL(triggered()), this, SLOT(ExecuteSub()));
                tmpString = sqlc.GetValueFromTableByField(sqlc.GetDB("sup"),"mainmenumethods","mainmenumethods","idmainmenumethods",get_mainmenu.value(4).toString());
                if (!sqlc.result)
                    tmpAction->setData(tmpString);
                tmpAction->setStatusTip(get_mainmenu.value(3).toString());
                if (tmpAction->text() == "Внимание!")
                {
                    tmpAction->setObjectName("warning");
/*                    if (pc.probsdetected)
                        tmpAction->setVisible(true);
                    else
                        tmpAction->setVisible(false); */
                }
                SupikMenuBar->addAction(tmpAction);
            }

            else
            {
                tmpMenu->setTitle(get_mainmenu.value(1).toString());
                tmpMenu->setStyleSheet("background: " + QString (SUPIKMENU_ITEM) + \
                                       "; QMenu::item::selected {background: " + QString(SUPIKMENU_ITEM_BG_SELECTED) + \
                                       "; color: " + QString(SUPIKMENU_ITEM_COLOR_SELECTED) + ";}");
                SupikMenuBar->addMenu (tmpMenu);
            }
        }
    }
    SupikMenuBar->setStyleSheet("QMenuBar::item {background: " + QString (SUPIKMENUBAR_BG) + \
                                ";} QMenuBar::item::selected {background: " + QString (SUPIKMENUBAR_ITEM_SELECTED) + ";}");

    setMenuBar(SupikMenuBar);
    QApplication::restoreOverrideCursor();}

QMenu *supik::AddChildToMenu(int id)
{
    QMenu *tmpMenu = new QMenu;
    QMenu *tmptmpMenu = new QMenu;
    QAction *action;
    bool hasChildren = false; // если нет потомков
    QString tmpString;
    QSqlQuery get_child_mainmenu (pc.sup);

    get_child_mainmenu.exec("SELECT `idmainmenu`,`alias`,`access`,`tooltip`,`method` FROM `mainmenu` WHERE "
                            "`idalias`=" + QString::number(id, 10) + " AND `deleted`=0 ORDER BY `idmainmenu` ASC;");

    while (get_child_mainmenu.next())
    {
        hasChildren = true;
        tmpMenu->setStyleSheet("background: " + QString (SUPIKMENU_ITEM) + \
                               "; QMenu::item::selected {background: " + QString(SUPIKMENU_ITEM_BG_SELECTED) + \
                               "; color: " + QString(SUPIKMENU_ITEM_COLOR_SELECTED) + ";}");
        if (get_child_mainmenu.value(2).toString().toLongLong(0, 16) & pc.access)
        {
            tmptmpMenu = AddChildToMenu (get_child_mainmenu.value(0).toInt(0));
            if (tmptmpMenu != NULL)
            {
                tmptmpMenu->setTitle(get_child_mainmenu.value(1).toString());
                tmpMenu->addMenu(tmptmpMenu);
            }
            else
            {
                action = new QAction (this);
                action->setText(get_child_mainmenu.value(1).toString());
                tmpString = get_child_mainmenu.value(3).toString();
                if (tmpString != "")
                    action->setStatusTip(tmpString);
                tmpString = sqlc.GetValueFromTableByField(sqlc.GetDB("sup"),"mainmenumethods","mainmenumethods","idmainmenumethods",get_child_mainmenu.value(4).toString());
                if (!sqlc.result)
                {
                    if (tmpString != "")
                    {
                        connect (action, SIGNAL(triggered()), this, SLOT(ExecuteSub()));
                        action->setData(tmpString);
                    }
                }
//                tmpString = get_child_mainmenu.value(4).toString();
                tmpMenu->addAction(action);
            }
        }
    }
    if (hasChildren) return tmpMenu;
    else return NULL;
}

void supik::ExecuteSub()
{
    QAction *action = dynamic_cast<QAction *>(sender());
    QString tmpString = action->data().toString();

    if (tmpString == "")
        return;
    if (pf.keys().contains(tmpString))
        (this->*pf[tmpString])();
}

int supik::CheckForWidget(int tmpi)
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return -1;
    for (int i = 0; i < MainTW->tabBar()->count(); i++)
        if (MainTW->tabBar()->tabData(i).toInt() == tmpi)
            return i;
    return -1;
}

void supik::SetSupikStatusBar()
{
    QStatusBar *SupikStatusBar = new QStatusBar;
    SupikStatusBar->setObjectName("ssb");
    SupikStatusBar->setStyleSheet(" border: 1px black;"
                                  " background-color: rgb(234, 234, 214);"
                                  " font: bold");
    SupikStatusBar->showMessage("Готов");
    setStatusBar(SupikStatusBar);
}

// ##################################### ДЕЙСТВИЯ ###################################

// Выход из СУПиКа

void supik::ExitSupik()
{
    this->close();
}

// Редактор настроек СУПиКа

void supik::SettingsEdit()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(pc.TW_SET);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    sys_settingsdialog *qssda = new sys_settingsdialog;
    qssda->SetupUI();
    qssda->setAttribute(Qt::WA_DeleteOnClose);

    int ids = MainTW->addTab(qssda, "Система::Редактор настроек");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_SET));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

// Редактор системы

void supik::SysStructEdit()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(pc.TW_SYSST);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    sys_systemdialog *qsyda = new sys_systemdialog;
    int ids = MainTW->addTab(qsyda, "Система::Редактор параметров");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_SYSST));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

// Импорт классификатора

void supik::SysImportClass()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(pc.TW_SYSIC);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    sys_ImportClass *dlg = new sys_ImportClass;
    int ids = MainTW->addTab(dlg, "Система::Импорт классификатора ЕСКД");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_SYSIC));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

// Редактор компонентов

void supik::Components()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    if (!(pc.access & (ACC_SYS_WR | ACC_ALT_WR)))
    {
        ERMSG(PublicClass::ER_SUPIK,__LINE__,"Недостаточно прав для продолжения!");
        return;
    }
    int idx = CheckForWidget(pc.TW_COMP);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    cmp_compdialog *qccda = new cmp_compdialog(CMP_ALTIUM);

    int ids = MainTW->addTab(qccda, "Компоненты::Altium");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_COMP));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::Directories()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(pc.TW_DIR);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    dir_maindialog *dird = new dir_maindialog("Справочники");

    int ids = MainTW->addTab(dird, "Справочники::Справочники");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_DIR));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::SysDirectories()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(pc.TW_SYSDIR);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    dir_maindialog *dird = new dir_maindialog("Справочники системные");

    int ids = MainTW->addTab(dird, "Справочники::системные");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_SYSDIR));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::ProbCheck()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(pc.TW_PROB);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }
    if (pc.allprobs.size() == 0)
        pc.fillallprob();
    pc.NewNotifyHasArrived = false; // чтобы перестала мигать надпись "Внимание"
    sys_probsdialog *probDialog = new sys_probsdialog;
    int ids = MainTW->addTab(probDialog, "Сообщения: "+QString::number(pc.allprobs.size()));
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_PROB));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    connect (this, SIGNAL(newnotify()), probDialog, SLOT(updatemainTV()));
    connect (probDialog, SIGNAL(editdirneeded()), this, SLOT(executeDirDialog()));
    connect (probDialog, SIGNAL(updateprobsnumber()), this, SLOT(updateprobsnumberintabtext()));
    MainTW->repaint();
}

void supik::WhIncome()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    if (!(pc.access & (ACC_SYS_WR | ACC_WH_WR)))
    {
        ERMSG(PublicClass::ER_SUPIK,__LINE__,"Недостаточно прав для продолжения!");
        return;
    }

    wh_dialog *whd = new wh_dialog (true, ""); // isIncoming = true
    int ids = MainTW->addTab(whd, "Склады::Приём на склад");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_WH));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();

/*    s_ncdialog *whd = new s_ncdialog;
    whd->setupUI("whincome", ":/res/WhWallpaper.jpg", DT_GENERAL);
    connect(whd,SIGNAL(error(int,int)),this,SLOT(ShowErMsg(int,int)));
    int ids = MainTW->addTab(whd, "Приём на склад");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_WH));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint(); */
}

void supik::WhOutgoing()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
/*    if (!(pc.access & (SYS_FULL | WH_FULL)))
    {
        ERMSG(PublicClass::ER_SUPIK,__LINE__,"Недостаточно прав для продолжения!");
        return;
    }

    whd = new wh_dialog (false, ""); // isIncoming = false

    int ids = MainTW->addTab(whd, "Выдача со склада");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_WH));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint(); */
}

void supik::WhSearch()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
}

void supik::WhEditor()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    if (!(pc.access & (ACC_SYS_WR | ACC_WH_WR)))
    {
        ERMSG(PublicClass::ER_SUPIK,__LINE__,"Недостаточно прав для продолжения!");
        return;
    }

    Wh_Editor *whd = new Wh_Editor;

    int ids = MainTW->addTab(whd, "Склады::Редактор складов");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_WH));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::DevDoc() // редактор документов на изделия
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    if (!(pc.access & (ACC_ALT_WR | ACC_SYS_WR | ACC_WH_WR)))
    {
        ERMSG(PublicClass::ER_SUPIK,__LINE__,"Недостаточно прав для продолжения!");
        return;
    }

    dev_docdialog *ddd = new dev_docdialog;

    int ids = MainTW->addTab(ddd, "Изделия::Документация");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_DEV));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::DevDev() // редактор изделий (классификатор)
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    if (!(pc.access & (ACC_SYS_WR | ACC_DOC_WR)))
    {
        ERMSG(PublicClass::ER_SUPIK,__LINE__,"Недостаточно прав для продолжения!");
        return;
    }

    dev_devdialog *ddd = new dev_devdialog;

    int ids = MainTW->addTab(ddd, "Изделия::Классификатор");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_DEV));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::Dummy()
{
    // пустышка
}

void supik::BackupDir()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(pc.TW_SYSBU);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    sys_backuprestoredirdialog *brd = new sys_backuprestoredirdialog (false); // isIncoming = false

    int ids = MainTW->addTab(brd, "Экспорт в файл");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_SYSBU));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::RestoreDir()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(pc.TW_SYSRS);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    sys_backuprestoredirdialog *brd = new sys_backuprestoredirdialog (true);

    int ids = MainTW->addTab(brd, "Импорт из файла");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_SYSRS));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::Quarantine()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(pc.TW_QUAR);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    sys_backuprestoredirdialog *brd = new sys_backuprestoredirdialog (true);

    int ids = MainTW->addTab(brd, "Импорт из файла");
    MainTW->tabBar()->setTabData(ids, QVariant(pc.TW_QUAR));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::executeDirDialog()
{
    Directories();
}

void supik::periodic1s()
{
    pc.DateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    s_tqLabel *le = this->findChild<s_tqLabel *>("datetime");
    le->setText(pc.DateTime);
/*    QAction *ta = this->findChild<QAction *>("warning");
    if (pc.NewNotifyHasArrived)
    {
        int idx = CheckForWidget(pc.TW_PROB);
        if (idx == -1)
        {
            if (ta->isVisible())
                ta->setVisible(false);
            else
                ta->setVisible(true);
        }
        else
            pc.NewNotifyHasArrived = false;
    }
    else if (pc.allprobs.size())
        ta->setVisible(true);
    else
        ta->setVisible(false); */
}

void supik::periodicxm()
{
    if (!pc.allprobs.size())
    {
        pc.NewNotifyHasArrived = false;
        QAction *ta = this->findChild<QAction *>("warning");
        ta->setVisible(false);
    }
    pc.minutetest();
    if ((pc.NewNotifyHasArrived) && !pc.UpdateInProgress) // если пришли новые сообщения и не обновляется список сообщений в настоящий момент
    {
        pc.fillallprob();
        emit newnotify();
        updateprobsnumberintabtext();
        if ((pc.notify & PR_Q) && !pc.Acknowledged)
        {
//            INFOMSG(PublicClass::ER_SUPIK,__LINE__,"Поступили новые элементы в карантин");
            pc.Acknowledged = true;
        }
    }
}

void supik::updateprobsnumberintabtext()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(pc.TW_PROB);
    if (idx != -1)
        MainTW->tabBar()->setTabText(idx, "Сообщения: "+QString::number(pc.allprobs.size()));
}

void supik::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);
    if (!ERHide)
    {
        ErrorProtocolWidget *erw = this->findChild<ErrorProtocolWidget *>("errorwidget");
        if (erw == 0)
            return;
        erw->setGeometry(QRect(0, height()-erw->height(), width(), erw->height()));
    }
}

void supik::MouseMove()
{
    QPoint curPos = mapFromGlobal(QCursor::pos());
    if ((abs(curPos.y() - height()) < 10) && (curPos.x() > 0) && (curPos.x() < width()))
    {
        if (ERHide)
            ShowOrHideSlideER();
    }
    else if ((abs(curPos.y() - height()) > 120) && (curPos.x() > 0) && (curPos.x() < width()))
    {
        if ((!ERHide) && (!ERTimerIsOn))
            ShowOrHideSlideER();
    }
}

void supik::ShowOrHideSlideER()
{
    ErrorProtocolWidget *w = this->findChild<ErrorProtocolWidget *>("errorwidget");
    if (w == 0)
        return;
    if (w->isHidden())
        w->show();
    if (ERHide)
        w->setGeometry(ERGeometry);
    QPropertyAnimation *ani = new QPropertyAnimation(w, "geometry");
    ani->setDuration(500);
    QRect startRect(0, height(), width(), 0);
    QRect endRect(0, height()-w->height(), width(), w->height());
    if (ERHide)
    {
        ani->setStartValue(startRect);
        ani->setEndValue(endRect);
    }
    else
    {
        ani->setStartValue(endRect);
        ani->setEndValue(startRect);
    }
    ani->start();
    ERHide = !ERHide;
}

void supik::UpdateErrorProtocol()
{
    ErrorProtocolWidget *ErWidget = this->findChild<ErrorProtocolWidget *>("errorwidget");
    if (ErWidget == 0)
    {
        DBGMSG(PublicClass::ER_SUPIK,__LINE__);
        return;
    }
    if (pc.ermsgpool.isEmpty())
        return;
    if (!ERTimerIsOn)
    {
        ERTimerIsOn = true;
        ERHide = true;
        ShowOrHideSlideER();
    }
    while (!pc.ermsgpool.isEmpty())
    {
        ErWidget->AddRowToProt(pc.ermsgpool.first());
        pc.ermsgpool.removeFirst();
    }
    ERTimer->start();
}

void supik::HideErrorProtocol()
{
    ERTimer->stop();
    ERTimerIsOn = false;
    ShowOrHideSlideER();
}
