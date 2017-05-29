#include "sys_systemdialog.h"
#include "../gen/messagebox.h"
#include "../dir/dir_maindialog.h"
#include "../dir/dir_adddialog.h"
#include "../../gen/s_sql.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../../models/treemodel.h"
#include "../../models/griddelegate.h"
#include "../../widgets/treeview.h"
#include "../../widgets/s_tqframe.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqsplitter.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/s_tqwidget.h"
#include "sysmenueditor.h"
#include "sysdireditor.h"
#include "sys_persdialog.h"

#include <QPainter>
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
    pf["perseditor"] = &sys_systemdialog::PersEditor;
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
    TreeView *MainTV = new TreeView(TreeView::TV_EXPANDABLE);
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
    TreeView *MainTV = this->findChild<TreeView *>("MainTV");
    if (MainTV == 0)
    {
        DBGMSG;
        return;
    }
    MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(MainTV,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(SetSlave(QModelIndex)));

    TreeModel *mainmodel = new TreeModel;
    mainmodel->Setup("Системное меню_сокращ");
    MainTV->setModel(mainmodel);
//    connect(MainTV,SIGNAL(clicked(QModelIndex)),this,SLOT(SetSlave(QModelIndex)));
    GridDelegate *gridItemDelegate = new GridDelegate;
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->resizeColumnsToContents();
    QApplication::restoreOverrideCursor();
}

// ############################################ SLOTS ####################################################

// отображение соответствующего дочернего дерева

void sys_systemdialog::SetSlave(QModelIndex idx)
{
    emit closeslvdlg();
    TreeView *MainTV = this->findChild<TreeView *>("MainTV");
    if (MainTV == 0)
    {
        DBGMSG;
        return;
    }
    TreeModel *mdl = static_cast<TreeModel *>(MainTV->model());
    int row = idx.row();
    if (mdl->HaveChildren(row)); // ветви, имеющие потомков, не имеют своего дочернего дерева
    else
    {
        QString tmpString = getMainIndex(1);
        QStringList tmpsl;
        QString table = "Системное меню_полн";
        QString field = "Вызываемая функция";
        QString cmpfield = "Наименование";
        tfl.GetValuesByColumnAndField(table,field,cmpfield,tmpString, tmpsl); // получить имя вызываемой функции
        if ((tfl.result == TFRESULT_ERROR) || (tmpsl.isEmpty()))
        {
            WARNMSG("");
            return;
        }
        tmpString = tmpsl.at(0);
        tmpString = sqlc.GetValueFromTableByField("sup","sysmenumethods","sysmenumethods","idsysmenumethods",tmpString);
        if (sqlc.result)
        {
            WARNMSG("");
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
    TreeView *MainTV = this->findChild<TreeView *>("MainTV");
    if (MainTV == 0)
    {
        DBGMSG;
        return QString();
    }
    QString tmpString = MainTV->model()->index(MainTV->currentIndex().row(), column, QModelIndex()).data(Qt::DisplayRole).toString();
    pc.ConvertId(!column, tmpString);
    return tmpString;
}

void sys_systemdialog::MainMenuEditor()
{
    s_tqStackedWidget *wdgt = this->findChild<s_tqStackedWidget *>("sw");
    if (wdgt == 0)
    {
        DBGMSG;
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
        DBGMSG;
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
        DBGMSG;
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
        DBGMSG;
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
    TreeView *tv = new TreeView;
    TreeModel *mdl = new TreeModel;
    mdl->ClearModel();
    int i = 0;
    QSqlQuery get_tables(sqlc.GetDB("sup"));
    get_tables.exec("SELECT DISTINCT `tablename` FROM `tablefields` ORDER BY `tablename` ASC;");
    while (get_tables.next())
    {
        QStringList sl = QStringList() << QString("%1").arg(i, 5, 10, QChar('0')) << get_tables.value(0).toString();
        mdl->AddItemToTree(sl);
        mdl->SetLastItem(Qt::black,Qt::transparent,QFont("MS Sans Serif", -1, QFont::Normal),QIcon(":/res/hr.png"),TM_SIMPLE_ELEMENT);
    }
    tv->setModel(mdl);
    tv->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (tv, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(TablesEditorContextMenu(QPoint)));
    GridDelegate *GridItemDelegate = new GridDelegate;
    tv->setItemDelegate(GridItemDelegate);
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();
    tv->setObjectName("tabletv");
    connect(tv,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(EditTable()));
    QVBoxLayout *lyout = new QVBoxLayout;
    lyout->addWidget(tv);
    wdt->setLayout(lyout);
    connect(this,SIGNAL(closeslvdlg()),wdt,SLOT(close()));
    wdgt->addWidget(wdt);
    wdgt->repaint();
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
        DBGMSG;
        return;
    }
    wdgt->removeWidget(wdgt->currentWidget());
}

void sys_systemdialog::NewTable()
{
    QStackedWidget *sw = this->findChild<QStackedWidget *>("stw");
    if (sw == 0)
    {
        DBGMSG;
        return;
    }
    dir_adddialog *dlg = new dir_adddialog(false,"");
    sw->addWidget(dlg);
//    sw->setCurrentWidget(dlg);
    repaint();
}

void sys_systemdialog::EditTable()
{
    TreeView *tv = this->findChild<TreeView *>("tabletv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QString tblename = tv->model()->data(tv->model()->index(tv->currentIndex().row(),1,QModelIndex()),Qt::DisplayRole).toString();
    QStackedWidget *sw = this->findChild<QStackedWidget *>("stw");
    if (sw == 0)
    {
        DBGMSG;
        return;
    }
    dir_adddialog *dlg = new dir_adddialog(true,"",tblename);
    sw->addWidget(dlg);
//    sw->setCurrentWidget(dlg);
    repaint();
}

void sys_systemdialog::DeleteTable()
{
    TreeView *tv = this->findChild<TreeView *>("tabletv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QString tblename = tv->model()->data(tv->model()->index(tv->currentIndex().row(),1,QModelIndex()),Qt::DisplayRole).toString();
    QStringList TableHeaders;
    tfl.tableheaders(tblename, TableHeaders);
    if (tfl.result == TFRESULT_ERROR)
        return;
    if (!(MessageBox2::question(this, "Уверены?", "Вы уверены, что хотите удалить все сведения о таблице?")))
        return;
    for (int i=0; i<TableHeaders.size(); i++)
    {
        QStringList fl = QStringList() << "tablename" << "header";
        QStringList vl = QStringList() << tblename << TableHeaders.at(i);
        sqlc.RealDeleteFromDB("sup","tablefields",fl,vl);
        if (sqlc.result)
        {
            WARNMSG("");
            return;
        }
    }
    MessageBox2::information(this, "Внимание", "Удалено успешно!");
    SetSlave(QModelIndex());
}

void sys_systemdialog::DirEditor()
{
    s_tqStackedWidget *wdgt = this->findChild<s_tqStackedWidget *>("sw");
    if (wdgt == 0)
    {
        DBGMSG;
        return;
    }
    SysDirEditor *dlg = new SysDirEditor;
    connect(this,SIGNAL(closeslvdlg()),dlg,SLOT(close()));
    wdgt->addWidget(dlg);
    wdgt->repaint();
}

void sys_systemdialog::PersEditor()
{
    s_tqStackedWidget *wdgt = this->findChild<s_tqStackedWidget *>("sw");
    if (wdgt == 0)
    {
        DBGMSG;
        return;
    }
    sys_persdialog *dlg = new sys_persdialog;
    connect(this,SIGNAL(closeslvdlg()),dlg,SLOT(close()));
    wdgt->addWidget(dlg);
    wdgt->repaint();
}
