#include "sys_systemdialog.h"
#include "s_2cdialog.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"
#include "../gen/s_tablefields.h"
#include "../models/s_ntmodel.h"
#include "../models/s_duniversal.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqframe.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqsplitter.h"
#include "sysdlg/sysmenueditor.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QFont>
#include <QHeaderView>
#include <QModelIndex>
#include <QStackedWidget>

sys_systemdialog::sys_systemdialog(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
    SetSysTree();
    pf["mainmenueditor"] = &sys_systemdialog::MainMenuEditor;
    pf["sysmenueditor"] = &sys_systemdialog::SystemMenuEditor;

}

void sys_systemdialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/Sys2Wallpaper.png"));
    event->accept();
}

void sys_systemdialog::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Структура системы");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    lyout->addWidget(lbl, 0);
    lyout->setAlignment(lbl, Qt::AlignRight);
    s_tqTreeView *MainTV = new s_tqTreeView;
    QStackedWidget *wdgt = new QStackedWidget;
    MainTV->setObjectName("MainTV");
    wdgt->setObjectName("sw");
    s_tqSplitter *spl = new s_tqSplitter;
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
    lyout->addWidget(spl, 90);
    setLayout(lyout);
}

// отображение основного системного дерева в левой части окна

void sys_systemdialog::SetSysTree()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    s_tqTreeView *MainTV = this->findChild<s_tqTreeView *>("MainTV");
    if (MainTV == 0)
    {
        emit error(ER_SYS+0x01);
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
    s_tqTreeView *MainTV = this->findChild<s_tqTreeView *>("MainTV");
    if (MainTV == 0)
    {
        emit error(ER_SYS+0x21);
        return;
    }
    if (MainTV->model()->rowCount(MainTV->currentIndex()) != 0); // ветви, имеющие потомков, не имеют своего дочернего дерева
    else
    {
        QString tmpString = getMainIndex(1);
        QStringList tmpsl = tfl.htovlc("Системное меню_полн","Подчинённая таблица","Наименование",tmpString); // получить наименование таблицы, по которой строить подчинённое дерево/таблицу
        tmpString = tmpsl.at(0);
        if (tfl.result)
        {
            emit error(ER_SYS+tfl.result);
            return;
        }
        if (tmpString == "")
            return;

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
        emit error(ER_DIRMAIN+0x81);
        return QString();
    }
    QString tmpString = MainTV->model()->index(MainTV->currentIndex().row(), column, MainTV->model()->parent(MainTV->currentIndex())).data(Qt::DisplayRole).toString();
    if (!column) // в нулевом столбце всегда ИД элемента с нулями в начале, надо незначащие нули убрать
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}

void sys_systemdialog::MainMenuEditor()
{
    QStackedWidget *wdgt = this->findChild<QStackedWidget *>("sw");
    if (wdgt == 0)
    {
        emit error(ER_SYS+0xA1);
        return;
    }
    SysmenuEditor *dlg = new SysmenuEditor;
    connect(dlg,SIGNAL(error(int)),this,SIGNAL(error(int)));
    dlg->SetupUI("Главное меню");
    wdgt->addWidget(dlg);
    wdgt->repaint();
}

void sys_systemdialog::SystemMenuEditor()
{
    QStackedWidget *wdgt = this->findChild<QStackedWidget *>("sw");
    if (wdgt == 0)
    {
        emit error(ER_SYS+0xA1);
        return;
    }
    SysmenuEditor *dlg = new SysmenuEditor;
    connect(dlg,SIGNAL(error(int)),this,SIGNAL(error(int)));
    dlg->SetupUI("Системное меню");
    wdgt->addWidget(dlg);
    wdgt->repaint();
}
