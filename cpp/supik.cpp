#include <QtWidgets>
#include <QTimer>

#include "../inc/supik.h"
#include "../inc/s_tqlabel.h"

supik::supik()
{
    SetSupikWindow();
    SetSupikStatusBar();
    pc.supikprocs << "ExitSupik" << "SysStructEdit" << "SettingsEdit" << "Components" << "Directories" << "BackupDir" << "RestoreDir" << "ProbCheck";
    pc.supikprocs << "WhIncome" << "WhOutgoing" << "WhSearch" << "DocView" << "DialogEdit" << "";
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
    pf["DialogEdit"] = &supik::DialogEdit;
}

void supik::showEvent(QShowEvent *event)
{
    setWindowTitle(pl.WindowTitlesMessages[2]);
    SetSupikMenuBar();
    timer1s = new QTimer;
    timer1s->setInterval(1000);
    connect (timer1s, SIGNAL(timeout()), this, SLOT(periodic1s()));
    timer1m = new QTimer;
//    timer1m->setInterval(pc.timerperiod*60000);
    timer1m->setInterval(pc.timerperiod*3000);
    connect (timer1m, SIGNAL(timeout()), this, SLOT(periodicxm()));
    timer1s->start();
    timer1m->start();
    event->accept();
}

void supik::SetSupikWindow()
{
    resize (984, 688);
    SupikIcon.addFile(QString::fromUtf8(":/pic/Pic/supik.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(SupikIcon);
    setStyleSheet("background-color: rgb(204, 204, 204);");
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *upperLayout = new QHBoxLayout;
    QToolBar *MainToolBar = new QToolBar;
    MainToolBar = addToolBar("123");
    MainToolBar->setMovable(true);
    MainToolBar->setMaximumHeight(25);
    QAction *Preferences = new QAction (QIcon(":/pic/Pic/Preferences.png"), pl.DialogMessages[4], this);
    MainToolBar->addAction(Preferences);
    upperLayout->addWidget(MainToolBar, 100);
    s_tqLabel *datetime = new s_tqLabel;
    datetime->setObjectName("datetime");
    upperLayout->addWidget(datetime, 0);
    mainLayout->addLayout(upperLayout, 0);
    MainTW = new S_ColorTabWidget;
    mainLayout->addWidget(MainTW, 100);
    QWidget *Widget234 = new QWidget;
    Widget234->setLayout(mainLayout);
    setCentralWidget(Widget234);
}

void supik::ClearSupikMenuBar()
{
    if (SupikMenuBar == (void*)0) delete SupikMenuBar;
}

void supik::SetSupikMenuBar()
{
    QMenu *tmpMenu;
    QAction* tmpAction;
    QString tmpString;
    int tmpInt;
    tmpMenu = new QMenu;
    SupikMenuBar = new QMenuBar;
    SupikMenuBar->setObjectName("MenuBar");
    QSqlQuery get_mainmenu (pc.sup);
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    tmpString="SELECT `idmainmenu`,`alias`,`access`,`tooltip`,`method` FROM `mainmenu` WHERE " \
                      "`idalias`=0 AND `idmainmenu`>2 ORDER BY `idmainmenu` ASC;";
    get_mainmenu.exec(tmpString);
    while (get_mainmenu.next())
    {
<<<<<<< .merge_file_a03852
        if (get_mainmenu.value(2).toString().toLong(0, 16) & pc.access)
=======
        if (get_mainmenu.value(2).toString().toLong(0, 16) && pc.access)
>>>>>>> .merge_file_a04232
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
                            "`idalias`=" + QString::number(id, 10) + " AND `idmainmenu`>2 ORDER BY `idmainmenu` ASC;");

    while (get_child_mainmenu.next())
    {
        hasChildren = true;
        tmpMenu->setStyleSheet("background: " + QString (SUPIKMENU_ITEM) + \
                               "; QMenu::item::selected {background: " + QString(SUPIKMENU_ITEM_BG_SELECTED) + \
                               "; color: " + QString(SUPIKMENU_ITEM_COLOR_SELECTED) + ";}");
<<<<<<< .merge_file_a03852
        if (get_child_mainmenu.value(2).toString().toLongLong(0, 16) & pc.access)
=======
        if (get_child_mainmenu.value(2).toString().toLongLong(0, 16) && pc.access)
>>>>>>> .merge_file_a04232
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

int supik::CheckForWidget(QWidget *dlg, QString str)
{
    if (dlg)
        for (int i = 0; i < MainTW->tabBar()->count(); i++)
            if (MainTW->tabBar()->tabData(i).toString() == str)
                return i;

    return -1;
}

void supik::SetSupikStatusBar()
{
    SupikStatusBar = new QStatusBar;
    SupikStatusBar->setStyleSheet(" border: 1px black;"
                                  " background-color: rgb(234, 234, 214);"
                                  " font: bold");
    setStatusBar(SupikStatusBar);
    SupikStatusBar->showMessage(pl.DialogMessages[SUPIK_D]);
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
    int idx = CheckForWidget(reinterpret_cast<QWidget *>(qssda), "set");
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    qssda = new sys_settingsdialog;
    qssda->PathToLibsLE->setText(pc.LandP->value("settings/pathtolibs","X://Altium//Libs//").toString());
    qssda->PathToSupikLE->setText(pc.LandP->value("settings/pathtosup","////NS//SUPiK//").toString());
    qssda->LangCB->setCurrentText(pc.LandP->value("settings/lang","EN").toString());
    qssda->SQLPathLE->setText(pc.LandP->value("settings/SQLPath","localhost").toString());
    qssda->setAttribute(Qt::WA_DeleteOnClose);

    int ids = MainTW->addTab(qssda, "Редактор настроек");
    MainTW->tabBar()->setTabData(ids, QVariant("set"));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

// Редактор системы

void supik::SysStructEdit()
{
    int idx = CheckForWidget(reinterpret_cast<QWidget *>(qsyda), "sys");
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    qsyda = new sys_systemdialog;
    qsyda->setAttribute(Qt::WA_DeleteOnClose);

    int ids = MainTW->addTab(qsyda, "Редактор системных параметров");
    MainTW->tabBar()->setTabData(ids, QVariant("sys"));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

// Редактор диалоговых окон

void supik::DialogEdit()
{
    int idx = CheckForWidget(reinterpret_cast<QWidget *>(qsyda), "sys");
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    qsyda = new sys_systemdialog;
    qsyda->setAttribute(Qt::WA_DeleteOnClose);

    int ids = MainTW->addTab(qsyda, "Редактор диалоговых окон");
    MainTW->tabBar()->setTabData(ids, QVariant("sys"));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

// Редактор компонентов

void supik::Components()
{
    if (!(pc.access & (SYS_FULL | ALT_FULL)))
    {
        QMessageBox::warning(this, "warning!", "Недостаточно прав для продолжения!");
        return;
    }
    int idx = CheckForWidget(reinterpret_cast<QWidget *>(qccda), "cmp");
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    qccda = new cmp_compdialog;
    qccda->setAttribute(Qt::WA_DeleteOnClose);

    int ids = MainTW->addTab(qccda, "Компоненты");
    MainTW->tabBar()->setTabData(ids, QVariant("cmp"));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::Directories()
{
    int idx = CheckForWidget(reinterpret_cast<QWidget *>(dird), "dir");
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    dird = new dir_maindialog;
    dird->setAttribute(Qt::WA_DeleteOnClose);

    int ids = MainTW->addTab(dird, "Справочники");
    MainTW->tabBar()->setTabData(ids, QVariant("dir"));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::ProbCheck()
{
    int idx = CheckForWidget(reinterpret_cast<QWidget *>(probDialog), "gen");
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }
    if (pc.allprobs.size() == 0)
        pc.fillallprob();
    pc.NewNotifyHasArrived = false; // чтобы перестала мигать надпись "Внимание"
    probDialog = new sys_probsdialog;
    int ids = MainTW->addTab(probDialog, "Сообщения: "+QString::number(pc.allprobs.size()));
    MainTW->tabBar()->setTabData(ids, QVariant("gen"));
    MainTW->tabBar()->setCurrentIndex(ids);
    connect (this, SIGNAL(newnotify()), probDialog, SLOT(updatemainTV()));
    connect (probDialog, SIGNAL(editdirneeded()), this, SLOT(executeDirDialog()));
    connect (probDialog, SIGNAL(updateprobsnumber()), this, SLOT(updateprobsnumberintabtext()));
    MainTW->repaint();
}

void supik::WhIncome()
{
    if (!(pc.access & (SYS_FULL | WH_FULL)))
    {
        QMessageBox::warning(this, "warning!", "Недостаточно прав для продолжения!");
        return;
    }
/*    int idx = CheckForWidget(reinterpret_cast<QWidget *>(whd), "whs");
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    whd = new wh_dialog (true, ""); // isIncoming = true
    whd->setAttribute(Qt::WA_DeleteOnClose);

    int ids = MainTW->addTab(whd, "Приём на склад");
    MainTW->tabBar()->setTabData(ids, QVariant("whs"));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint(); */
    int idx = CheckForWidget(reinterpret_cast<QWidget *>(whd), "whs");
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    whd = new s_ncdialog();
    whd->setupUI("whincome", ":/pic/Pic/WhWallpaper.jpg", DT_GENERAL);
    int ids = MainTW->addTab(whd, "Приём на склад");
    MainTW->tabBar()->setTabData(ids, QVariant("whs"));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::WhOutgoing()
{
/*    if (!(pc.access & (SYS_FULL | WH_FULL)))
    {
        QMessageBox::warning(this, "warning!", "Недостаточно прав для продолжения!");
        return;
    }
    int idx = CheckForWidget(reinterpret_cast<QWidget *>(whd), "whs");
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    whd = new wh_dialog (false, ""); // isIncoming = false
    whd->setAttribute(Qt::WA_DeleteOnClose);

    int ids = MainTW->addTab(whd, "Выдача со склада");
    MainTW->tabBar()->setTabData(ids, QVariant("whs"));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint(); */
}

void supik::WhSearch()
{

}

void supik::BackupDir()
{
    int idx = CheckForWidget(reinterpret_cast<QWidget *>(brd), "sys");
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    brd = new sys_backuprestoredirdialog (false); // isIncoming = false

    int ids = MainTW->addTab(brd, "Экспорт в файл");
    MainTW->tabBar()->setTabData(ids, QVariant("sys"));
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
}

void supik::RestoreDir()
{
    int idx = CheckForWidget(reinterpret_cast<QWidget *>(brd), "sys");
    if (idx != -1)
    {
        MainTW->setCurrentIndex(idx);
        return;
    }

    brd = new sys_backuprestoredirdialog (true);

    int ids = MainTW->addTab(brd, "Импорт из файла");
    MainTW->tabBar()->setTabData(ids, QVariant("sys"));
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
        int idx = CheckForWidget(reinterpret_cast<QWidget *>(probDialog), "gen");
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
        int idx = CheckForWidget(reinterpret_cast<QWidget *>(probDialog), "gen");
        if (idx != -1)
            MainTW->tabBar()->setTabText(idx, "Сообщения: "+QString::number(pc.allprobs.size()));
        if ((pc.notify & PR_Q) && !pc.Acknowledged)
        {
//            QMessageBox::warning(this, "warning!", "Поступили новые элементы в карантин");
            pc.Acknowledged = true;
        }
    }
}

void supik::updateprobsnumberintabtext()
{
    int idx = CheckForWidget(reinterpret_cast<QWidget *>(probDialog), "gen");
    if (idx != -1)
        MainTW->tabBar()->setTabText(idx, "Сообщения: "+QString::number(pc.allprobs.size()));
}
