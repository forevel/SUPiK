#include <QtWidgets>
#include <QTimer>
#include <QThread>
#include <QTextEdit>
#include <QPropertyAnimation>
#include "supik.h"
#include "dialogs/cmp/cmp_compdialog.h"
#include "dialogs/dev/dev_devdialog.h"
#include "dialogs/dev/dev_docdialog.h"
#include "dialogs/dir/dir_maindialog.h"
#include "dialogs/sys/serverdialog.h"
#include "dialogs/sys/sys_backuprestoredirdialog.h"
#include "dialogs/sys/sys_erdialog.h"
#include "dialogs/sys/sys_importclass.h"
#include "dialogs/sys/sys_probsdialog.h"
#include "dialogs/sys/sys_settingsdialog.h"
#include "dialogs/sys/sys_systemdialog.h"
#include "dialogs/tb/tb_maindialog.h"
#include "dialogs/tb/tb_examdialog.h"
#include "dialogs/wh/wh_dialog.h"
#include "dialogs/wh/wh_editor.h"
#include "gen/client.h"
#include "gen/publiclang.h"
#include "gen/s_sql.h"
#include "widgets/s_colortabwidget.h"
#include "widgets/s_statusbar.h"
#include "widgets/s_tqlabel.h"
#include "widgets/waitwidget.h"
#include "widgets/wd_func.h"

supik::supik()
{
    IsProblemsDetected = false;
    PeriodicOddSecond = true;
    PingSecCounter = CurrRefreshCounter = CurrCounter = 0;
    SetSupikWindow();
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
    pf["TBExam"] = &supik::TBExam;
    pf["TBMain"] = &supik::TBMain;
    ErMsgNum = 0;
    Curr = new Currency;
    Curr->GetRates(Currency::GOOGLE);
}

void supik::showEvent(QShowEvent *event)
{
    QTimer *timer1s = new QTimer;
    timer1s->setInterval(1000);
    connect (timer1s, SIGNAL(timeout()), this, SLOT(periodic1s()));
    timer1s->start();
    event->accept();
}

void supik::closeEvent(QCloseEvent *e)
{
    emit stopall();
    e->accept();
}

void supik::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_S:
    {
        if (event->modifiers() == (Qt::AltModifier | Qt::ControlModifier)) // "Alt + S" => Status window
            ShowServerStatus();
        break;
    }
    }
    QMainWindow::keyPressEvent(event);
}

void supik::SetSupikWindow()
{
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
    pb->setIcon(QIcon(":/res/ErNo.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(ErrorProtocol()));
    pb->setToolTip("Протокол ошибок");
    pb->setObjectName("errorprotpb");
    upperLayout->addWidget(pb);
    upperLayout->addStretch(99);
    upperLayout->addWidget(WDFunc::NewLBL(this, "currencies"), 0);
    s_tqLabel *datetime = new s_tqLabel;
    datetime->setObjectName("datetime");
    upperLayout->addWidget(datetime, 0);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Выход из программы");
    upperLayout->addWidget(pb);
    mainLayout->addLayout(upperLayout, 0);
    S_ColorTabWidget *MainTW = new S_ColorTabWidget;
    MainTW->setObjectName("MainTW");
    mainLayout->addWidget(MainTW, 100);

    s_StatusBar *SB = new s_StatusBar;
    connect(Cli,SIGNAL(BytesRead(quint64)),SB,SLOT(UpdateIncomeBytes(quint64)));
    connect(Cli,SIGNAL(BytesWritten(quint64)),SB,SLOT(UpdateOutgoingBytes(quint64)));
    connect(Cli,SIGNAL(Disconnected()),SB,SLOT(SetStatusDisconnected()));
    connect(Cli,SIGNAL(Connected()),SB,SLOT(SetStatusConnected()));
    mainLayout->addWidget(SB, 0);

    QWidget *wdgt = new QWidget;
    wdgt->setLayout(mainLayout);
    setCentralWidget(wdgt);
    WaitWidget *w = new WaitWidget;
    w->Start();
    w->SetMessage("Подготовка главного меню...");
    SetSupikMenuBar();
    SetSupikStatusBar();
    w->Stop();
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
//    QSqlQuery get_mainmenu (pc.sup);
    QStringList fl = QStringList() << "idmainmenu" << "mainmenu" << "access" << "tooltip" << "method";
    QStringList cmpfl = QStringList() << "idalias";
    QStringList cmpvl = QStringList() << "0";
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByFields("sup","mainmenu",fl,cmpfl,cmpvl,"order",true);
    for (int i=0; i<vl.size(); i++)
    {
        quint32 tmpu = vl.at(i).at(2).toLong(0, 16);
        if (tmpu & pc.access)
        {
            tmpInt = vl.at(i).at(0).toInt(0);
            tmpMenu = AddChildToMenu (tmpInt);
            tmpString = vl.at(i).at(1);
            if (tmpMenu == NULL) // нет потомков
            {
                tmpAction = new QAction(this);
                tmpAction->setText(vl.at(i).at(1));
                connect (tmpAction, SIGNAL(triggered()), this, SLOT(ExecuteSub()));
                tmpString = sqlc.GetValueFromTableByField("sup","mainmenumethods","mainmenumethods","idmainmenumethods",vl.at(i).at(4));
                if (!sqlc.result)
                    tmpAction->setData(tmpString);
                tmpAction->setStatusTip(vl.at(i).at(3));
                if (tmpAction->text() == "Внимание!")
                    tmpAction->setObjectName("warning");
                SupikMenuBar->addAction(tmpAction);
            }

            else
            {
                tmpMenu->setTitle(vl.at(i).at(1));
                tmpMenu->setStyleSheet("background: " + QString (SUPIKMENU_ITEM) + \
                                       "; QMenu::item::selected {background: " + QString(SUPIKMENU_ITEM_BG_SELECTED) + \
                                       "; color: " + QString(SUPIKMENU_ITEM_COLOR_SELECTED) + ";}");
                SupikMenuBar->addMenu (tmpMenu);
            }
        }
    }
    SupikMenuBar->setStyleSheet("QMenuBar::item {background: transparent;} " \
                                "QMenuBar::item::selected {background: " + QString (SUPIKMENUBAR_ITEM_SELECTED) + ";}");

    setMenuBar(SupikMenuBar);
}

QMenu *supik::AddChildToMenu(int id)
{
    QMenu *tmpMenu = new QMenu;
    QMenu *tmptmpMenu = new QMenu;
    QAction *action;
    bool hasChildren = false; // если нет потомков
    QString tmpString;


/*    QSqlQuery get_child_mainmenu (pc.sup);

    get_child_mainmenu.exec("SELECT `idmainmenu`,`mainmenu`,`access`,`tooltip`,`method` FROM `mainmenu` WHERE "
                            "`idalias`=" + QString::number(id, 10) + " AND `deleted`=0 ORDER BY `idmainmenu` ASC;");
*/
    QStringList fl = QStringList() << "idmainmenu" << "mainmenu" << "access" << "tooltip" << "method";
    QStringList cmpfl = QStringList() << "idalias";
    QStringList cmpvl = QStringList() << QString::number(id, 10);
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByFields("sup","mainmenu",fl,cmpfl,cmpvl,"idmainmenu",true);
    for (int i=0; i<vl.size(); i++)
    {

/*    while (get_child_mainmenu.next())
{  */
        hasChildren = true;
        tmpMenu->setStyleSheet("background: " + QString (SUPIKMENU_ITEM) + \
                               "; QMenu::item::selected {background: " + QString(SUPIKMENU_ITEM_BG_SELECTED) + \
                               "; color: " + QString(SUPIKMENU_ITEM_COLOR_SELECTED) + ";}");
        if (vl.at(i).at(2).toLongLong(0, 16) & pc.access)
        {
            tmptmpMenu = AddChildToMenu (vl.at(i).at(0).toInt(0));
            if (tmptmpMenu != NULL)
            {
                tmptmpMenu->setTitle(vl.at(i).at(1));
                tmpMenu->addMenu(tmptmpMenu);
            }
            else
            {
                action = new QAction (this);
                action->setText(vl.at(i).at(1));
                tmpString = vl.at(i).at(3);
                if (tmpString != "")
                    action->setStatusTip(tmpString);
                tmpString = sqlc.GetValueFromTableByField("sup","mainmenumethods","mainmenumethods","idmainmenumethods",vl.at(i).at(4));
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
    sys_settingsdialog *qssda = new sys_settingsdialog;
    if (CreateTab(ACC_ALL, qssda, "Система::Редактор настроек", pc.TW_SET) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

// Редактор системы

void supik::SysStructEdit()
{
    sys_systemdialog *qsyda = new sys_systemdialog;
    if (CreateTab(ACC_SYS_WR, qsyda, "Система::Редактор параметров", pc.TW_SYSST) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

// Импорт классификатора

void supik::SysImportClass()
{
    sys_ImportClass *dlg = new sys_ImportClass;
    if (CreateTab(ACC_SYS_RO, dlg, "Система::Импорт из XLSX", pc.TW_SYSIC) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

// Редактор компонентов

void supik::Components()
{
    cmp_compdialog *qccda = new cmp_compdialog(CMP_ALTIUM);
    if (CreateTab(ACC_ALT_RO, qccda, "Компоненты::Altium", pc.TW_COMP) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

void supik::Directories()
{
    dir_maindialog *dird = new dir_maindialog("Справочники");
    if (CreateTab(ACC_ALL, dird, "Справочники::Справочники", pc.TW_DIR) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

void supik::SysDirectories()
{
    dir_maindialog *dird = new dir_maindialog("Справочники системные");
    if (CreateTab(ACC_SYS_WR, dird, "Справочники::системные", pc.TW_SYSDIR) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

void supik::ProbCheck()
{
    SysProblemsDialog *probDialog = new SysProblemsDialog;
    probDialog->setObjectName("ProblemsDialog");
    connect (probDialog, SIGNAL(ProblemsNumberUpdated()), this, SLOT(UpdateProblemsNumberInTab()));
    if (CreateTab(ACC_ALL, probDialog, "Сообщения: "+QString::number(pc.ProblemsList.size()), pc.TW_PROB) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

void supik::WhIncome()
{
    wh_dialog *whd = new wh_dialog (true, ""); // isIncoming = true
    if (CreateTab(ACC_WH_WR, whd, "Склады::Приём на склад", pc.TW_WH) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
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
    Wh_Editor *whd = new Wh_Editor;
    if (CreateTab(ACC_WH_WR, whd, "Склады::Редактор складов", pc.TW_WH) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

void supik::DevDoc() // редактор документов на изделия
{
    dev_docdialog *ddd = new dev_docdialog;
    if (CreateTab(ACC_ALT_WR | ACC_WH_WR, ddd, "Изделия::Документация", pc.TW_DEV) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

void supik::DevDev() // редактор изделий (классификатор)
{
    dev_devdialog *ddd = new dev_devdialog;
    if (CreateTab(ACC_DOC_WR, ddd, "Изделия::Классификатор", pc.TW_DEV) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

// [0] - access needed, [1] - QDialog *, [2] - TabName, [3] - TabType

void supik::TBExam()
{
    tb_examdialog *tbm = new tb_examdialog;
    if (CreateTab(ACC_TB_RO | ACC_TB_WR, tbm, "Экзамен ОТ и ТБ", pc.TW_TB) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

void supik::TBMain()
{
    tb_maindialog *dlg = new tb_maindialog;
    if (CreateTab(ACC_TB_WR, dlg, "Информация ОТ и ТБ", pc.TW_TB) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

void supik::Dummy()
{
    // пустышка
}

int supik::CreateTab(quint32 access, QDialog *dlg, QString tabname, int tabtype)
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return RESULTBAD;
    if (!(pc.access & (access)))
    {
        ERMSG("Недостаточно прав для продолжения!");
        return RESULTBAD;
    }

    int ids = MainTW->addTab(dlg, tabname);
    MainTW->tabBar()->setTabData(ids, QVariant(tabtype));
    MainTW->tabBar()->tabButton(ids,QTabBar::RightSide)->hide();
    MainTW->tabBar()->setCurrentIndex(ids);
    MainTW->repaint();
    return RESULTOK;
}

void supik::BackupDir()
{
    sys_backuprestoredirdialog *brd = new sys_backuprestoredirdialog (false); // isIncoming = false
    if (CreateTab(ACC_SYS_RO, brd, "Экспорт в файл", pc.TW_SYSBU) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

void supik::RestoreDir()
{
    sys_backuprestoredirdialog *brd = new sys_backuprestoredirdialog (true);
    if (CreateTab(ACC_SYS_RO, brd, "Импорт из файла", pc.TW_SYSRS) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

void supik::Quarantine()
{
/*    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
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
    MainTW->repaint(); */
}

void supik::ErrorProtocol()
{
    s_tqPushButton *pb = this->findChild<s_tqPushButton *>("errorprotpb");
    if (pb == 0)
    {
        DBGMSG;
        return;
    }
    pb->setIcon(QIcon(":/res/ErNo.png"));
    ErMsgNum = pc.ermsgpool.size();
    SysErDialog *serr = new SysErDialog;
    serr->InitiateDialog();
    if (CreateTab(ACC_ALL, serr, "Протокол ошибок", pc.TW_ERPROT) == RESULTBAD)
    {
        WARNMSG("Невозможно открыть вкладку");
        return;
    }
}

void supik::executeDirDialog()
{
    Directories();
}

void supik::periodic1s()
{
    ++PingSecCounter;
    ++CurrRefreshCounter;
#ifndef DEBUGISON
    if (PingSecCounter > SUPIK_PINGPERIOD)
    {
        PingSecCounter = 0;
        Cli->SendCmd(M_PING); // проверка связи
    }
#endif
    if (CurrRefreshCounter > SUPIK_CURRPERIOD)
    {
        CurrRefreshCounter = 0;
        ++CurrCounter;
        if (CurrCounter >= CURNUM)
            CurrCounter = 0;
        WDFunc::SetLBLText(this, "currencies", \
                           "1 " + Curr->Curs()[CurrCounter] + " = " + QString::number(Curr->Rates.at(CurrCounter),'f', 3) + " " + Curr->GetBaseCurrency());
    }
    PeriodicOddSecond = !PeriodicOddSecond;
    pc.DateTime = QDateTime::currentDateTime().toString(DATETIMEFORMAT);
    s_tqLabel *le = this->findChild<s_tqLabel *>("datetime");
    le->setText(pc.DateTime);
    if (IsProblemsDetected)
    {
        QAction *ta = this->findChild<QAction *>("warning");
        if (ta == 0)
        {
            DBGMSG;
            return;
        }
        int idx = CheckForWidget(pc.TW_PROB);
        if (idx == -1)
        {
            if (PeriodicOddSecond)
                ta->setEnabled(false);
            else
                ta->setEnabled(true);
        }
    }
    // проверка, не пришло ли новых сообщений в протокол ошибок
    if (ErMsgNum < pc.ermsgpool.size())
    {
        s_tqPushButton *pb = this->findChild<s_tqPushButton *>("errorprotpb");
        if (pb == 0)
        {
            DBGMSG;
            return;
        }
        pb->setIcon(QIcon(":/res/ErYes.png"));
    }
}

void supik::ClearProblems()
{
    IsProblemsDetected = false;
    QAction *ta = this->findChild<QAction *>("warning");
    if (ta == 0)
    {
        DBGMSG;
        return;
    }
    int idx = CheckForWidget(pc.TW_PROB);
    if (idx == -1)
        ta->setVisible(false);
}

void supik::UpdateProblemsNumberInTab()
{
    S_ColorTabWidget *MainTW = this->findChild<S_ColorTabWidget *>("MainTW");
    if (MainTW == 0)
        return;
    int idx = CheckForWidget(pc.TW_PROB);
    if (idx != -1)
    {
        SysProblemsDialog *dlg = this->findChild<SysProblemsDialog *>("ProblemsDialog");
        if (dlg != 0)
        {
            MainTW->tabBar()->setTabText(idx, "Сообщения: "+QString::number(pc.ProblemsList.size()));
            if (pc.ProblemsList.isEmpty()) // убрали все проблемы
            {
                dlg->close();
                ClearProblems();
            }
        }
    }
}

void supik::ShowServerStatus()
{
    if (pc.access & ACC_SYS_RO)
    {
        ServerDialog *dlg = new ServerDialog;
        dlg->exec();
    }
}
