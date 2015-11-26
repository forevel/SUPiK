#include "sys_systemdialog.h"
#include "../s_2cdialog.h"
#include "../messagebox.h"
#include "../dir/dir_maindialog.h"
#include "../dir/dir_adddialog.h"
#include "../../gen/s_sql.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../../models/s_ntmodel.h"
#include "../../models/s_ncmodel.h"
#include "../../models/s_duniversal.h"
#include "../../widgets/s_tqtreeview.h"
#include "../../widgets/s_tqtableview.h"
#include "../../widgets/s_tqframe.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqsplitter.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/s_tqwidget.h"
#include "sysmenueditor.h"
#include "sysdireditor.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QFont>
#include <QHeaderView>
#include <QModelIndex>
#include <QStackedWidget>
#include <QAction>
#include <QMenu>
#include <QMessageBox>

sys_systemdialog::sys_systemdialog(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
    SetSysTree();
    pf["mainmenueditor"] = &sys_systemdialog::MainMenuEditor;
    pf["sysmenueditor"] = &sys_systemdialog::SystemMenuEditor;
    pf["sysdireditor"] = &sys_systemdialog::SystemDirEditor;
    pf["tableseditor"] = &sys_systemdialog::TablesEditor;
    pf["direditor"] = &sys_systemdialog::DirEditor;
}

void sys_systemdialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/Sys2Wallpaper.png"));
    event->accept();
}

void sys_systemdialog::SetupUI()
{
    QVBoxLayout *mlyout = new QVBoxLayout;
    QStackedWidget *sw = new QStackedWidget;
    QWidget *mainwidget = new QWidget;
    sw->setObjectName("stw");
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);
    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel("Структура системы");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);
    s_tqTreeView *MainTV = new s_tqTreeView;
    s_tqStackedWidget *wdgt = new s_tqStackedWidget;
    connect(this,SIGNAL(closeslvdlg()),this,SLOT(RemoveWidget()));
    MainTV->setObjectName("MainTV");
    wdgt->setObjectName("sw");
    s_tqSplitter *spl = new s_tqSplitter;
    spl->setObjectName("spl");
    QList<int> il;
    int tmpi = this->geometry().width() / 2 - 150;
    il << tmpi << tmpi;
    s_tqFrame *left = new s_tqFrame;
    QVBoxLayout *leftlyout = new QVBoxLayout;
    leftlyout->addWidget(MainTV);
    left->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    left->setLineWidth(1);
    left->setLayout(leftlyout);
    spl->addWidget(left);
    s_tqFrame *right = new s_tqFrame;
    QVBoxLayout *rlyout = new QVBoxLayout;
    rlyout->addWidget(wdgt);
    right->setLayout(rlyout);
    right->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    right->setLineWidth(1);
    spl->addWidget(right);
    spl->setOrientation(Qt::Horizontal);
    spl->setSizes(il);
    lyout->addWidget(spl, 90);
    mainwidget->setLayout(lyout);
    sw->addWidget(mainwidget);
    mlyout->addWidget(sw);
    setLayout(mlyout);
}

// отображение основного системного дерева в левой части окна

void sys_systemdialog::SetSysTree()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    s_tqTreeView *MainTV = this->findChild<s_tqTreeView *>("MainTV");
    if (MainTV == 0)
    {
        SYSSDBG;
        return;
    }
    MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(MainTV,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(SetSlave(QModelIndex)));

    s_ntmodel *mainmodel = new s_ntmodel;
    mainmodel->Setup("Системное меню_сокращ");
    MainTV->setModel(mainmodel);
    connect(MainTV, SIGNAL(expanded(QModelIndex)), mainmodel, SLOT(addExpandedIndex(QModelIndex)));
    connect(MainTV, SIGNAL(collapsed(QModelIndex)), mainmodel, SLOT(removeExpandedIndex(QModelIndex)));
    connect(MainTV,SIGNAL(clicked(QModelIndex)),this,SLOT(SetSlave(QModelIndex)));
    MainTV->header()->setVisible(false);
    MainTV->setIndentation(2);
    MainTV->setAnimated(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->ResizeColumnsToContents();
    QApplication::restoreOverrideCursor();
}

// ############################################ SLOTS ####################################################

void sys_systemdialog::SetSlave(QModelIndex index)
{
    Q_UNUSED(index);
    SetSlave();
}

// отображение соответствующего дочернего дерева

void sys_systemdialog::SetSlave()
{
    emit closeslvdlg();
    s_tqTreeView *MainTV = this->findChild<s_tqTreeView *>("MainTV");
    if (MainTV == 0)
    {
        SYSSDBG;
        return;
    }
    if (MainTV->model()->rowCount(MainTV->currentIndex()) != 0); // ветви, имеющие потомков, не имеют своего дочернего дерева
    else
    {
        QString tmpString = getMainIndex(1);
        QStringList tmpsl = tfl.htovlc("Системное меню_полн","Вызываемая функция","Наименование",tmpString); // получить имя вызываемой функции
        if (tfl.result)
        {
            SYSSWARN;
            return;
        }
        tmpString = tmpsl.at(0);
        tmpString = sqlc.GetValueFromTableByField(sqlc.GetDB("sup"),"sysmenumethods","sysmenumethods","idsysmenumethods",tmpString);
        if (sqlc.result)
        {
            SYSSWARN;
            return;
        }
        if (tmpString == "")
            return;

        if (pf.keys().contains(tmpString))
            (this->*pf[tmpString])();
    }



    /*    QString tmpString, PathString, NewLibName;
    QStringList tmpList;
    QFile tmpFile;

      if (ui->qmmw_Tree->currentIndex().parent().data(Qt::DisplayRole).toString() == "Компоненты") // 0.34-ag
    {
        if (ui->qmmw_Tree->currentIndex().data(Qt::DisplayRole).toString() == "Структура меню")
        {
            qs_ClassesEditorDialog.dbase = pc.ent;
            qs_ClassesEditorDialog.als = "alias";
            qs_ClassesEditorDialog.tble = "altium";
            qs_ClassesEditorDialog.FillDialog();
            qs_ClassesEditorDialog.exec();
            pc.InitiatePublicClass();
        }
        if (ui->qmmw_Tree->currentIndex().data(Qt::DisplayRole).toString() == "Добавить библиотеки в БД")
        {
            NewLibName = QInputDialog::getText(this, "Добавление библиотек",
                                              "Введите название библиотеки",
                                              QLineEdit::Normal,
                                              "", &ok);
            if (ok && !NewLibName.isEmpty())
            {
                i = 0;
                tmpList = NewLibName.split("_", QString::KeepEmptyParts);

                PathString = "";

                tmpString = tmpList.last();
                while ((tmpList.value(i, "") != "") && (tmpList.value(i, "") != tmpString))
                {
                    PathString += "/";
                    PathString += tmpList.value(i++);
                }
                tmpString = pc.PathToLibs + "/Symbols" + PathString + "/" + NewLibName + ".SchLib";
                // проверим наличие такого файла

                tmpFile.setFileName(tmpString);
                if (!tmpFile.open(QIODevice::ReadOnly))
                {
                    QMessageBox::warning(this,"Warning!",\
                                          "Файл библиотеки с таким именем не найден!",\
                                          QMessageBox::Ok, QMessageBox::NoButton);
                    return;
                }
                else
                {
                    QSqlQuery insert_table_into_altium_db (pc.alt);
                    insert_table_into_altium_db.exec("CREATE  TABLE IF NOT EXISTS `altium`.`" + NewLibName + "` ( " \
                                                     "`id` INT(11) NOT NULL AUTO_INCREMENT ,"
                                                     "`Library Ref` VARCHAR(45) NOT NULL ,"
                                                     "`Library Path` VARCHAR(256) NOT NULL ,"
                                                     "`Footprint Ref` VARCHAR(45) NOT NULL ,"
                                                     "`Footprint Path` VARCHAR(256) NOT NULL ,"
                                                     "`Sim Description` VARCHAR(45) NULL DEFAULT NULL ,"
                                                     "`Sim File` VARCHAR(256) NULL DEFAULT NULL ,"
                                                     "`Sim Model Name` VARCHAR(45) NULL DEFAULT NULL ,"
                                                     "`Sim Parameters` VARCHAR(45) NULL DEFAULT NULL ,"
                                                     "`Manufacturer` VARCHAR(45) NOT NULL ,"
                                                     "`PartNumber` VARCHAR(64) NOT NULL ,"
                                                     "`Package` VARCHAR(15) NULL DEFAULT NULL ,"
                                                     "`Marking` VARCHAR(10) NULL DEFAULT NULL ,"
                                                     "`NominalValue` VARCHAR(10) NULL DEFAULT NULL ,"
                                                     "`NominalVoltage` VARCHAR(10) NULL DEFAULT NULL ,"
                                                     "`Tolerance` VARCHAR(5) NULL DEFAULT NULL ,"
                                                     "`OpTemperaturen` VARCHAR(5) NULL DEFAULT NULL ,"
                                                     "`OpTemperaturem` VARCHAR(5) NULL DEFAULT NULL ,"
                                                     "`Pmax` VARCHAR(15) NULL DEFAULT NULL ,"
                                                     "`TC` VARCHAR(10) NULL DEFAULT NULL ,"
                                                     "`Creator` INT(11) NOT NULL ,"
                                                     "`Modify Date` DATETIME NULL DEFAULT NULL ,"
                                                     "`Comment` VARCHAR(10) NULL DEFAULT NULL ,"
                                                     "`HelpURL` VARCHAR(512) NULL DEFAULT NULL ,"
                                                     "`RevNotes` INT(11) NULL DEFAULT NULL ,"
                                                     "`Discontinued` TINYINT(1) NOT NULL ,"
                                                     "`Description` VARCHAR(64) NOT NULL ,"
                                                     "`Notes` VARCHAR(45) NULL DEFAULT NULL ,"
                                                     "`deleted` TINYINT(1) NOT NULL ,"
                                                     "`prefix` VARCHAR(20) NULL DEFAULT NULL ,"
                                                     "`isSMD` TINYINT(1) NULL DEFAULT NULL ," // 0.34-aj
                                                     "`Nominal` VARCHAR(8) NULL DEFAULT NULL ," // 0.34-aj
                                                     "`Unit` INT(11) NULL DEFAULT NULL ," // 0.34-aj
                                                     "PRIMARY KEY (`id`) ) "
                                                     "ENGINE = MyISAM "
                                                     "AUTO_INCREMENT = 5 "
                                                     "DEFAULT CHARACTER SET = utf8 "
                                                     "COLLATE = utf8_general_ci;");
                    if (insert_table_into_altium_db.isActive())
                    {
                        QMessageBox::warning (this, "Успешно", "Библиотека добавлена!", \
                                              QMessageBox::Ok, QMessageBox::NoButton);
                    }
                    else
                    {
                        QMessageBox::warning (this, "Ошибка при записи!", insert_table_into_altium_db.lastError().text(), \
                                              QMessageBox::Ok, QMessageBox::NoButton);
                        return;
                    }
                }
            }

            pc.InitiatePublicClass();
        }
    } */
}


QString sys_systemdialog::getMainIndex(int column)
{
    s_tqTreeView *MainTV = this->findChild<s_tqTreeView *>("MainTV");
    if (MainTV == 0)
    {
        SYSSDBG;
        return QString();
    }
    QString tmpString = MainTV->model()->index(MainTV->currentIndex().row(), column, MainTV->model()->parent(MainTV->currentIndex())).data(Qt::DisplayRole).toString();
    if (!column) // в нулевом столбце всегда ИД элемента с нулями в начале, надо незначащие нули убрать
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}

void sys_systemdialog::MainMenuEditor()
{
    s_tqStackedWidget *wdgt = this->findChild<s_tqStackedWidget *>("sw");
    if (wdgt == 0)
    {
        SYSSDBG;
        return;
    }
    SysmenuEditor *dlg = new SysmenuEditor;
    connect(this,SIGNAL(closeslvdlg()),dlg,SLOT(close()));
    dlg->SetupUI("Главное меню");
    wdgt->addWidget(dlg);
    wdgt->repaint();
}

void sys_systemdialog::SystemMenuEditor()
{
    s_tqStackedWidget *wdgt = this->findChild<s_tqStackedWidget *>("sw");
    if (wdgt == 0)
    {
        SYSSDBG;
        return;
    }
    SysmenuEditor *dlg = new SysmenuEditor;
    connect(this,SIGNAL(closeslvdlg()),dlg,SLOT(close()));
    dlg->SetupUI("Системное меню");
    wdgt->addWidget(dlg);
    wdgt->repaint();
}

void sys_systemdialog::SystemDirEditor()
{
    QStackedWidget *sw = this->findChild<QStackedWidget *>("stw");
    if (sw == 0)
    {
        SYSSDBG;
        return;
    }
    dir_maindialog *dird = new dir_maindialog("Справочники системные");
    connect(this,SIGNAL(closeslvdlg()),dird,SLOT(close()));
    sw->addWidget(dird);
    sw->setCurrentWidget(dird);
    repaint();
}

void sys_systemdialog::TablesEditor()
{
    s_tqStackedWidget *wdgt = this->findChild<s_tqStackedWidget *>("sw");
    if (wdgt == 0)
    {
        SYSSDBG;
        return;
    }
    s_tqWidget *wdt = this->findChild<s_tqWidget *>("tableseditorwidget");
    if (wdt != 0) // если виджет с таким названием уже есть, значит, уже TablesEditor вызывали
    {
        wdgt->setCurrentWidget(wdt);
        return;
    }
    wdt = new s_tqWidget;
    wdt->setObjectName("tableseditorwidget");
    wdt->setAttribute(Qt::WA_DeleteOnClose);
    s_tqTableView *tv = new s_tqTableView;
    s_ncmodel *mdl = new s_ncmodel;
    QList<QStringList> lsl;
    QStringList ids, vls;
    int i = 0;
    QSqlQuery get_tables(sqlc.GetDB("sup"));
    get_tables.exec("SELECT DISTINCT `tablename` FROM `tablefields` ORDER BY `tablename` ASC;");
    while (get_tables.next())
    {
        i++;
        ids << QString("%1").arg(i, 5, 10, QChar('0'));
        vls << get_tables.value(0).toString();
    }
    if (!vls.isEmpty())
    {
        lsl.append(ids);
        lsl.append(vls);
        mdl->setDataToWrite(lsl);
        mdl->setcolumnlinks(0,"7.8");
        mdl->setcolumnlinks(1,"7.8");
        mdl->fillModel();
        tv->setModel(mdl);
        tv->setContextMenuPolicy(Qt::CustomContextMenu);
        connect (tv, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(TablesEditorContextMenu(QPoint)));
        s_duniversal *GridItemDelegate = new s_duniversal;
        tv->setItemDelegate(GridItemDelegate);
        tv->horizontalHeader()->setVisible(false);
        tv->verticalHeader()->setVisible(false);
        tv->resizeColumnsToContents();
        tv->resizeRowsToContents();
        tv->setObjectName("tabletv");
        connect(tv,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(EditTable(QModelIndex)));
        QVBoxLayout *lyout = new QVBoxLayout;
        lyout->addWidget(tv);
        wdt->setLayout(lyout);
        connect(this,SIGNAL(closeslvdlg()),wdt,SLOT(close()));
        wdgt->addWidget(wdt);
        wdgt->repaint();
    }
}

void sys_systemdialog::TablesEditorContextMenu(QPoint pt)
{
    Q_UNUSED(pt);
    QAction *NewTableEditor = new QAction ("Новая таблица", this);
    NewTableEditor->setSeparator(false);
    connect(NewTableEditor, SIGNAL(triggered()), this, SLOT(NewTable()));
    QAction *EditTable = new QAction ("Редактировать таблицу", this);
    EditTable->setSeparator(false);
    connect(EditTable, SIGNAL(triggered()), this, SLOT(EditTable()));
    QAction *DeleteTable = new QAction ("Удалить таблицу", this);
    DeleteTable->setSeparator(false);
    connect(DeleteTable, SIGNAL(triggered()), this, SLOT(DeleteTable()));
    QMenu *ContextMenu = new QMenu;
    ContextMenu->setTitle("Context menu");
    ContextMenu->addAction(NewTableEditor);
    ContextMenu->addAction(EditTable);
    ContextMenu->addAction(DeleteTable);
    ContextMenu->exec(QCursor::pos());
}

void sys_systemdialog::RemoveWidget()
{
    s_tqStackedWidget *wdgt = this->findChild<s_tqStackedWidget *>("sw");
    if (wdgt == 0)
    {
        SYSSDBG;
        return;
    }
    wdgt->removeWidget(wdgt->currentWidget());
}

void sys_systemdialog::NewTable()
{
    QStackedWidget *sw = this->findChild<QStackedWidget *>("stw");
    if (sw == 0)
    {
        SYSSDBG;
        return;
    }
    dir_adddialog *dlg = new dir_adddialog(false,"");
    sw->addWidget(dlg);
    sw->setCurrentWidget(dlg);
    repaint();
}

void sys_systemdialog::EditTable()
{
    this->EditTable(QModelIndex());
}

void sys_systemdialog::EditTable(QModelIndex idx)
{
    Q_UNUSED(idx);
    s_tqTableView *tv = this->findChild<s_tqTableView *>("tabletv");
    if (tv == 0)
    {
        SYSSDBG;
        return;
    }
    QString tblename = tv->model()->data(tv->model()->index(tv->currentIndex().row(),1,QModelIndex()),Qt::DisplayRole).toString();
    QStackedWidget *sw = this->findChild<QStackedWidget *>("stw");
    if (sw == 0)
    {
        SYSSDBG;
        return;
    }
    dir_adddialog *dlg = new dir_adddialog(true,"",tblename);
    sw->addWidget(dlg);
    sw->setCurrentWidget(dlg);
    repaint();
}

void sys_systemdialog::DeleteTable()
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("tabletv");
    if (tv == 0)
    {
        SYSSDBG;
        return;
    }
    QString tblename = tv->model()->data(tv->model()->index(tv->currentIndex().row(),1,QModelIndex()),Qt::DisplayRole).toString();
    QStringList TableHeaders = tfl.tableheaders(tblename);
    if (tfl.result)
        return;
    if (!(MessageBox::question(this, "Уверены?", "Вы уверены, что хотите удалить все сведения о таблице?")))
        return;
    for (int i=0; i<TableHeaders.size(); i++)
    {
        QStringList fl = QStringList() << "tablename" << "header";
        QStringList vl = QStringList() << tblename << TableHeaders.at(i);
        sqlc.RealDeleteFromDB(sqlc.GetDB("sup"),"tablefields",fl,vl);
        if (sqlc.result)
        {
            SYSSWARN;
            return;
        }
    }
    SYSSINFO("Удалено успешно!");
    SetSlave();
}

void sys_systemdialog::DirEditor()
{
    s_tqStackedWidget *wdgt = this->findChild<s_tqStackedWidget *>("sw");
    if (wdgt == 0)
    {
        SYSSDBG;
        return;
    }
    SysDirEditor *dlg = new SysDirEditor;
    connect(this,SIGNAL(closeslvdlg()),dlg,SLOT(close()));
    wdgt->addWidget(dlg);
    wdgt->repaint();
}
