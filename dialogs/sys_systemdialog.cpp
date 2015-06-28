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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QFont>
#include <QHeaderView>
#include <QCursor>
#include <QMessageBox>
#include <QInputDialog>
#include <QModelIndex>

sys_systemdialog::sys_systemdialog(QWidget *parent) :
    QDialog(parent)
{
    SetupUI();
    SetSysTree();
    FirstShow = true;
    SlaveTVIsTree = false;
}

void sys_systemdialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/Sys2Wallpaper.png"));

    event->accept();
}

void sys_systemdialog::showEvent(QShowEvent *event)
{
/*    if (FirstShow)
    {
        SetSysTree();
        FirstShow = false;
    } */
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
    s_tqTreeView *SlaveTV = new s_tqTreeView;
    s_tqTreeView *MainTV = new s_tqTreeView;
    MainTV->setObjectName("MainTV");
    SlaveTV->setObjectName("SlaveTV");
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
    rlyout->addWidget(SlaveTV);
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
    connect (MainTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SystemContextMenu(QPoint)));
    connect(MainTV,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(SetSlaveTV(QModelIndex)));

    s_ntmodel *mainmodel = new s_ntmodel;
    mainmodel->Setup("Системное меню_сокращ");
    MainTV->setModel(mainmodel);
    connect(MainTV, SIGNAL(expanded(QModelIndex)), mainmodel, SLOT(addExpandedIndex(QModelIndex)));
    connect(MainTV, SIGNAL(collapsed(QModelIndex)), mainmodel, SLOT(removeExpandedIndex(QModelIndex)));
    connect(MainTV,SIGNAL(clicked(QModelIndex)),this,SLOT(SetSlaveTV(QModelIndex)));
    MainTV->header()->setVisible(false);
    MainTV->setIndentation(2);
    MainTV->setAnimated(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->ResizeColumnsToContents();
    QApplication::restoreOverrideCursor();
}

// ############################################ SLOTS ####################################################

// ########################################## CONTEXTS ###################################################

// контекстное меню основного дерева

void sys_systemdialog::SystemContextMenu(QPoint)
{
    QAction *OpenAction = new QAction("Открыть", this);
    connect (OpenAction, SIGNAL(triggered()), this, SLOT(SetSlaveTV()));

    QMenu *SystemContextMenu = new QMenu;
    SystemContextMenu->addAction(OpenAction);
    SystemContextMenu->exec(QCursor::pos());
}

// контекстное меню дочернего дерева

void sys_systemdialog::SystemSlaveContextMenu(QPoint)
{
    QAction *ChangeDataChild;
    QAction *AddChild = new QAction("Добавить субэлемент", this);
    AddChild->setSeparator(0);
    if (SlaveTVIsTree) // если не дерево, нет и субэлементов
        AddChild->setEnabled(true);
    else
        AddChild->setEnabled(false);
    connect (AddChild, SIGNAL(triggered()), this, SLOT(AddChildToTree()));
    QAction *AddRoot = new QAction("Добавить элемент", this);
    AddRoot->setSeparator(0);
    connect (AddRoot, SIGNAL(triggered()), this, SLOT(AddRootToTree()));
    QAction *Separator2Child = new QAction ("", this);
    Separator2Child->setSeparator(1);
    QAction *ChangeChild = new QAction("Изменить имя элемента", this);
    ChangeChild->setSeparator(0);
    connect (ChangeChild, SIGNAL(triggered()), this, SLOT(ChangeChildName()));
    QAction *DeleteChild = new QAction("Удалить элемент", this);
    DeleteChild->setSeparator(0);
    connect(DeleteChild, SIGNAL(triggered()), this, SLOT(DeleteChildFromTree()));
    QAction *SeparatorChild = new QAction ("", this);
    SeparatorChild->setSeparator(1);
    ChangeDataChild = new QAction ("Изменить сопутствующие данные", this);
    ChangeDataChild->setSeparator(false);
    connect(ChangeDataChild, SIGNAL(triggered()), this, SLOT(ChangeAdditionalFields()));

    QMenu *ContextMenu = new QMenu;
    ContextMenu->setTitle("Context menu");
    ContextMenu->addAction(AddRoot);
    ContextMenu->addAction(AddChild);
    ContextMenu->addAction(Separator2Child);
    ContextMenu->addAction(ChangeChild);
    ContextMenu->addAction(DeleteChild);
    ContextMenu->addAction(SeparatorChild);
    ContextMenu->addAction(ChangeDataChild);
    ContextMenu->exec(QCursor::pos());
}

//

void sys_systemdialog::ShowTableContentDialog()
{

}

void sys_systemdialog::SetSlaveTV(QModelIndex index)
{
    Q_UNUSED(index);
    SetSlaveTV();
}

// отображение соответствующего дочернего дерева

void sys_systemdialog::SetSlaveTV()
{
    QString tmpString, PathString, NewLibName;
    QStringList tmpList;
    QFile tmpFile;

    s_tqTreeView *MainTV = this->findChild<s_tqTreeView *>("MainTV");
    if (MainTV == 0)
    {
        emit error(ER_SYS+0x21);
        return;
    }
    if (MainTV->model()->rowCount(MainTV->currentIndex()) != 0); // ветви, имеющие потомков, не имеют своего дочернего дерева
    else
    {
        ShowSlaveTree(getMainIndex(1));
    }
/*    if (ui->qmmw_Tree->currentIndex().parent().data(Qt::DisplayRole).toString() == "Компоненты") // 0.34-ag
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

// отображение подчинённого дерева в правой части

void sys_systemdialog::ShowSlaveTree(QString str)
{
    QSqlQuery get_additional_fields (pc.sup);
    QString tmpString;
    QStringList tmpStringlist;
    int i;

    s_tqTreeView *SlaveTV = this->findChild<s_tqTreeView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        emit error(ER_SYS+0x31);
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QStringList tmpsl = tfl.htovlc("Системное меню_полн","Права доступа","Наименование",str); // получить права на таблицу, по которой строить подчинённое дерево/таблицу
    if (tfl.result)
    {
        emit error(ER_SYS+tfl.result);
        return;
    }
    if (tmpsl.at(0).toUInt(0,16) & pc.access)
    {
        tmpsl = tfl.htovlc("Системное меню_полн","Подчинённая таблица","Наименование",str); // получить наименование таблицы, по которой строить подчинённое дерево/таблицу
        if (tfl.result)
        {
            emit error(ER_SYS+tfl.result);
            return;
        }
        s_ntmodel *SlaveTreeModel = new s_ntmodel;
        int res = SlaveTreeModel->Setup(tmpsl.at(0) + "_сокращ");
        if (res == ER_NTMODEL) // это не дерево
        {
            s_ncmodel *SlaveTableModel = new s_ncmodel;
            int res = SlaveTableModel->setup(tmpsl.at(0) + "_сокращ");
            if (res)
            {
                emit error(res+ER_SYS);
                QApplication::restoreOverrideCursor();
                return;
            }
            SlaveTV->setModel(SlaveTableModel);
            SlaveTVIsTree = false;
        }
        else
        {
            SlaveTV->setModel(SlaveTreeModel);
            SlaveTVIsTree = true;
            connect(SlaveTV, SIGNAL(expanded(QModelIndex)), SlaveTreeModel, SLOT(addExpandedIndex(QModelIndex)));
            connect(SlaveTV, SIGNAL(collapsed(QModelIndex)), SlaveTreeModel, SLOT(removeExpandedIndex(QModelIndex)));
        }
        SlaveTV->header()->setDefaultAlignment(Qt::AlignCenter);
        SlaveTV->header()->setVisible(true);
        SlaveTV->setIndentation(2);
        SlaveTV->setAnimated(false);
        s_duniversal *gridItemDelegate = new s_duniversal;
        SlaveTV->setItemDelegate(gridItemDelegate);
        for (i = 0; i < SlaveTV->header()->count(); i++)
            SlaveTV->resizeColumnToContents(i);
        SlaveTV->setContextMenuPolicy(Qt::CustomContextMenu);
        connect (SlaveTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SystemSlaveContextMenu(QPoint)));
        connect (SlaveTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ChangeAdditionalFields(QModelIndex)));
    }
    else
        emit error(ER_SYS+0x34);
    QApplication::restoreOverrideCursor();
}

void sys_systemdialog::AddChildToTree()
{
    AddToTree(getSlaveIndex(0));
}

void sys_systemdialog::AddRootToTree()
{
    AddToTree("0"); // корневой элемент не имеет ссылки на родителя
}

// добавление элемента в SlaveTree. В str - ИД родительского элемента

void sys_systemdialog::AddToTree(QString str)
{
    QString NewClass = QInputDialog::getText(this, "СУПиК :: новый элемент", \
                                               "Введите имя нового элемента", \
                                               QLineEdit::Normal, "", 0, 0, 0);
    if (NewClass == "") return;
    str = QString::number(str.toInt(0));

    QStringList fields, values;
    if (SlaveTVIsTree)
    {
        fields << "alias" << "idalias" << "access";
        values << NewClass << str << "0007";
    }
    else
    {
        fields << tble << "access";
        values << NewClass << "0007";
    }
/*    QString newID = sqlc.insertvaluestotable(db, tble, fields, values);

    if (!sqlc.result) ChangeAdditionalFields(newID);
    else
        QMessageBox::warning (this, "Ошибка при записи!", QString::number(sqlc.result), \
                              QMessageBox::Ok, QMessageBox::NoButton);
*/
}

void sys_systemdialog::DeleteChildFromTree()
{
    QString tmpString;
    QStringList tmpList;

    tmpString = getSlaveIndex(0);

/*    QSqlQuery delete_classes (db);
    QSqlQuery get_children (db);
    get_children.exec("SELECT `id" + tble + "` FROM `" + tble + "` WHERE `idalias`=" + tmpString + ";");

    if (get_children.size() > 0)
    {
        if (QMessageBox::question(this, "Вы уверены?", \
                                  "Категория содержит подкатегории.\nВы уверены, что хотите удалить её?", \
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            delete_classes.exec("DELETE FROM `" + tble + "` WHERE `idalias`=" + tmpString + ";");
            if (delete_classes.isActive())
            {
                QMessageBox::information(this,"Warning!",\
                                     "Удалено успешно",\
                                     QMessageBox::Ok, QMessageBox::NoButton);
            }
            else
            {
                QMessageBox::information(this,"Ошибка при удалении!",\
                                     delete_classes.lastError().text(),\
                                     QMessageBox::Ok, QMessageBox::NoButton);
            }
        }
    }
    delete_classes.exec("DELETE FROM `" + tble + "` WHERE `id" + tble + "`=" + tmpString + ";");
    if (delete_classes.isActive())
    {
        QMessageBox::information(this,"Warning!",\
                             "Удалено успешно",\
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
    else
    {
        QMessageBox::information(this,"Ошибка при удалении!",\
                             delete_classes.lastError().text(),\
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
    SetSlaveTV(); */
}

void sys_systemdialog::ChangeAdditionalFields(QModelIndex idx)
{
    s_tqTreeView *SlaveTV = this->findChild<s_tqTreeView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        emit error(ER_SYS+0x51);
        return;
    }
    if (SlaveTV->model()->rowCount(SlaveTV->currentIndex()) != 0); // для родителей запрещено иметь дополнительные поля
    else
    {
        SlaveTV->setCurrentIndex(idx);
        ChangeAdditionalFields();
    }
}

void sys_systemdialog::ChangeAdditionalFields()
{
    ChangeAdditionalFields(getSlaveIndex(0));
}

void sys_systemdialog::ChangeAdditionalFields(QString id)
{
    s_2cdialog *newdialog = new s_2cdialog;
    newdialog->setup("Структура_системы", MODE_CHOOSE, id, "Структура системы");
    if (!newdialog->result)
    {
        newdialog->setModal(true);
        newdialog->exec();
        SetSlaveTV();
    }
    else
    {
        QMessageBox::information(this,"Ошибка при создании диалога!",\
                             QString::number(newdialog->result),\
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
/*    sqldialog = new s_sqlfieldsdialog;
    if (!(res = sqldialog->SetupUI(db, tble, id, "Структура системы"))) // берём ИД текущего элемента
    {
        sqldialog->exec();
        SetSlaveTV();
    }
    else
    {
        delete sqldialog;
        QMessageBox::information(this,"Ошибка при создании диалога!",\
                             QString::number(res),\
                             QMessageBox::Ok, QMessageBox::NoButton);
    } */
}

void sys_systemdialog::ChangeChildName()
{
    QString tmpString, tmpValue;
    bool ok;

    tmpString = getSlaveIndex(0);

    tmpValue = QInputDialog::getText(this, "Изменение наименования",
                                      "Введите новое наименование",
                                      QLineEdit::Normal,
                                      tmpString, &ok);
/*    if (ok && !tmpValue.isEmpty())
    {
        QSqlQuery update_class (db);
        tmpString = "UPDATE `" + tble + "` SET `alias` = \"" + tmpString + "\" WHERE `alias` = \"" + tmpValue + "\";";
        update_class.exec(tmpString);

        if (update_class.isActive())
        {
            QMessageBox::warning (this, "Успешно", "Изменение проведено!", \
                                  QMessageBox::Ok, QMessageBox::NoButton);
        }
        else
            QMessageBox::warning (this, "Ошибка при записи!", update_class.lastError().text(), \
                                  QMessageBox::Ok, QMessageBox::NoButton);
    }
    SetSlaveTV(); */
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

QString sys_systemdialog::getSlaveIndex(int column)
{
    s_tqTreeView *SlaveTV = this->findChild<s_tqTreeView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        emit error(ER_SYS+0x91);
        return QString();
    }
    QModelIndex  index;
    if (SlaveTVIsTree)
        index = SlaveTV->model()->index(SlaveTV->currentIndex().row(), column, SlaveTV->model()->parent(SlaveTV->currentIndex()));
    else
        index = SlaveTV->model()->index(SlaveTV->currentIndex().row(), column, QModelIndex());

    QString tmpString = index.data(Qt::DisplayRole).toString();
    if (!column)
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}
