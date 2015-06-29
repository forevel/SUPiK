#include <QtWidgets>
#include <QTimer>

#include "supik.h"
#include "widgets/s_tqlabel.h"
#include "dialogs/sys_settingsdialog.h"
#include "dialogs/sys_systemdialog.h"
#include "dialogs/dir_maindialog.h"
#include "dialogs/sys_probsdialog.h"
#include "dialogs/cmp_compdialog.h"
#include "dialogs/wh_dialog.h"
#include "dialogs/sys_backuprestoredirdialog.h"
#include "dialogs/s_ncdialog.h"
#include "widgets/s_colortabwidget.h"
#include "gen/publicclass.h"
#include "gen/publiclang.h"

supik::supik()
{
    SetSupikWindow();
    SetSupikStatusBar();
    pc.supikprocs << "ExitSupik" << "SysStructEdit" << "SettingsEdit" << "Components" << "Directories" << "BackupDir" << "RestoreDir" << "ProbCheck";
    pc.supikprocs << "WhIncome" << "WhOutgoing" << "WhSearch" << "DocView" << "Quarantine" << "";
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
    pf["Quarantine"] = &supik::Quarantine;
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
//    timer1s->start();
//    timer1m->start();
    event->accept();
}

void supik::SetSupikWindow()
{
    setWindowTitle("Супик :: главное окно");
    resize (984, 688);
    QIcon SupikIcon;
    SupikIcon.addFile(QString::fromUtf8(":/res/supik.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(SupikIcon);
    setStyleSheet("background-color: rgb(204, 204, 204);");
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *upperLayout = new QHBoxLayout;
    s_tqLabel *datetime = new s_tqLabel;
    datetime->setObjectName("datetime");
    upperLayout->addStretch(99);
    upperLayout->addWidget(datetime, 0);
    mainLayout->addLayout(upperLayout, 0);
    S_ColorTabWidget *MainTW = new S_ColorTabWidget;
    MainTW->setObjectName("MainTW");
    mainLayout->addWidget(MainTW, 100);
    QWidget *wdgt = new QWidget;
    wdgt->setLayout(mainLayout);
    setCentralWidget(wdgt);
    SetSupikMenuBar();
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
                      "`idalias`=0 AND `deleted`=0 ORDER BY `idmainmenu` ASC;";
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
                tmpAction->setData(get_mainmenu.value(4).toString());
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
                tmpString = get_child_mainmenu.value(4).toString();
                if (tmpString != "")
                {
                    connect (action, SIGNAL(triggered()), this, SLOT(ExecuteSub()));
                    action->setData(tmpString);
                }
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
    int idx = CheckForWidget(TW_SET);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    sys_settingsdialog *qssda = new sys_settingsdialog;
    qssda->SetupUI();
    qssda->setAttribute(Qt::WA_DeleteOnClose);

    int ids = MainTW->addTab(qssda, "Редактор настроек");
    MainTW->tabBar()->setTabData(ids, QVariant(TW_SET));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

// Редактор системы

void supik::SysStructEdit()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(TW_SYSST);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    sys_systemdialog *qsyda = new sys_systemdialog;
    connect(qsyda,SIGNAL(error(int)),this,SLOT(ShowErMsg(int)));
    int ids = MainTW->addTab(qsyda, "Редактор системных параметров");
    MainTW->tabBar()->setTabData(ids, QVariant(TW_SYSST));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

// Редактор компонентов

void supik::Components()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    if (!(pc.access & (SYS_FULL | ALT_FULL)))
    {
        QMessageBox::warning(this, "warning!", "Недостаточно прав для продолжения!");
        return;
    }
    int idx = CheckForWidget(TW_COMP);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    cmp_compdialog *qccda = new cmp_compdialog;
    qccda->setAttribute(Qt::WA_DeleteOnClose);

    int ids = MainTW->addTab(qccda, "Компоненты");
    MainTW->tabBar()->setTabData(ids, QVariant(TW_COMP));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::Directories()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(TW_DIR);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    dir_maindialog *dird = new dir_maindialog;
    connect(dird,SIGNAL(error(int)),this,SLOT(ShowErMsg(int)));

    int ids = MainTW->addTab(dird, "Справочники");
    MainTW->tabBar()->setTabData(ids, QVariant(TW_DIR));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::ProbCheck()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(TW_PROB);
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
    MainTW->tabBar()->setTabData(ids, QVariant(TW_PROB));
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
    if (!(pc.access & (SYS_FULL | WH_FULL)))
    {
        QMessageBox::warning(this, "warning!", "Недостаточно прав для продолжения!");
        return;
    }

    wh_dialog *whd = new wh_dialog (true, ""); // isIncoming = true
    connect(whd,SIGNAL(error(int)),this,SLOT(ShowErMsg(int)));
    int ids = MainTW->addTab(whd, "Приём на склад");
    MainTW->tabBar()->setTabData(ids, QVariant(TW_WH));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();

/*    s_ncdialog *whd = new s_ncdialog;
    whd->setupUI("whincome", ":/res/WhWallpaper.jpg", DT_GENERAL);
    int ids = MainTW->addTab(whd, "Приём на склад");
    MainTW->tabBar()->setTabData(ids, QVariant(TW_WH));
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
        QMessageBox::warning(this, "warning!", "Недостаточно прав для продолжения!");
        return;
    }

    whd = new wh_dialog (false, ""); // isIncoming = false
    connect(whd,SIGNAL(error(int)),this,SLOT(ShowErMsg(int)));

    int ids = MainTW->addTab(whd, "Выдача со склада");
    MainTW->tabBar()->setTabData(ids, QVariant(TW_WH));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint(); */
}

void supik::WhSearch()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
}

void supik::BackupDir()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(TW_SYSBU);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    sys_backuprestoredirdialog *brd = new sys_backuprestoredirdialog (false); // isIncoming = false

    int ids = MainTW->addTab(brd, "Экспорт в файл");
    MainTW->tabBar()->setTabData(ids, QVariant(TW_SYSBU));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::RestoreDir()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(TW_SYSRS);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    sys_backuprestoredirdialog *brd = new sys_backuprestoredirdialog (true);

    int ids = MainTW->addTab(brd, "Импорт из файла");
    MainTW->tabBar()->setTabData(ids, QVariant(TW_SYSRS));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::Quarantine()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(TW_QUAR);
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    sys_backuprestoredirdialog *brd = new sys_backuprestoredirdialog (true);

    int ids = MainTW->addTab(brd, "Импорт из файла");
    MainTW->tabBar()->setTabData(ids, QVariant(TW_QUAR));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::executeDirDialog()
{
    Directories();
}

void supik::periodic1s()
{
    pc.DateTime = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    s_tqLabel *le = this->findChild<s_tqLabel *>("datetime");
    le->setText(pc.DateTime);
    QAction *ta = this->findChild<QAction *>("warning");
    if (pc.NewNotifyHasArrived)
    {
        int idx = CheckForWidget(TW_PROB);
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
        ta->setVisible(false);
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
//            QMessageBox::warning(this, "warning!", "Поступили новые элементы в карантин");
            pc.Acknowledged = true;
        }
    }
}

void supik::updateprobsnumberintabtext()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(TW_PROB);
    if (idx != -1)
        MainTW->tabBar()->setTabText(idx, "Сообщения: "+QString::number(pc.allprobs.size()));
}

void supik::ShowErMsg(int ernum)
{
    QMessageBox::warning(this, "warning!", "Ошибка 0x" + QString::number(ernum,16), QMessageBox::Ok, QMessageBox::NoButton);
}
