#include "sys_systemdialog.h"
#include "../models/s_aitemmodel.h"
#include "../gen/s_sql.h"
#include "s_2cdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QCursor>
#include <QMessageBox>
#include <QInputDialog>

sys_systemdialog::sys_systemdialog(QWidget *parent) :
    QWidget(parent)
{
    SetupUI();
    FirstShow = true;
}

void sys_systemdialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/Sys2Wallpaper.png"));

    event->accept();
}

void sys_systemdialog::showEvent(QShowEvent *event)
{
    if (FirstShow)
    {
        SetSysTree();
        FirstShow = false;
    }
    event->accept();
}

void sys_systemdialog::SetupUI()
{
    MainLayout = new QVBoxLayout;
    MainFrameLayout = new QHBoxLayout;
    SlaveTV = new s_tqTreeView;
    MainTV = new s_tqTreeView;
    gridItemDelegate = new s_duniversal;
    MainFrame = new QFrame;
    MainFrame->setAttribute(Qt::WA_TranslucentBackground);
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setMaximumWidth(2);
    MainFrameLayout->addWidget(MainTV, 10);
    MainFrameLayout->addWidget(line, 1);
    MainFrameLayout->addWidget(MainFrame, 25);
    MainL = new s_tqLabel;
    MainL->setText("Структура системы");
    QFont font;
    font.setPointSize(15);
    MainL->setFont(font);
    MainLayout->addWidget(MainL, 0);
    MainLayout->setAlignment(MainL, Qt::AlignRight);
    MainLayout->addLayout (MainFrameLayout);
    setLayout(MainLayout);
}

// отображение основного системного дерева в левой части окна

void sys_systemdialog::SetSysTree()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
//    if ((pc.ent_access.sys & 0x01) || (pc.ent_access.alt & 0x01))
//    {
        MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
        disconnect(MainTV, SIGNAL(customContextMenuRequested(QPoint)), 0, 0);
        connect (MainTV, SIGNAL(customContextMenuRequested(QPoint)), \
                 this, SLOT(SystemContextMenu(QPoint)));
        disconnect(MainTV, SIGNAL(doubleClicked(QModelIndex)), 0, 0);
        connect (MainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ShowAppropriateSlaveTree(QModelIndex)));

        if (MainTreeModel == (void*)0) delete MainTreeModel;
        MainTreeModel = new s_aitemmodel(pc.sup, "syslist", QSqlDatabase(), "", false); // если второй таблицы нет, в tble2 должна содержаться пустая строка (требование к таблице dirlist)
        QItemSelectionModel *m = MainTV->selectionModel();
        MainTreeModel->isEditable = false;
        MainTV->setModel(MainTreeModel);
        delete m;
        connect(MainTV, SIGNAL(expanded(QModelIndex)), MainTreeModel, SLOT(addExpandedIndex(QModelIndex)));
        connect(MainTV, SIGNAL(collapsed(QModelIndex)), MainTreeModel, SLOT(removeExpandedIndex(QModelIndex)));
        connect(MainTV, SIGNAL(clicked(QModelIndex)), this, SLOT(setMainTVExpanded(QModelIndex)));
        MainTV->header()->setVisible(false);
        MainTV->setIndentation(2);
        MainTV->setAnimated(false);
        MainTV->setItemDelegate(gridItemDelegate);
        for (int i = 0; i < MainTV->header()->count(); i++)
            MainTV->resizeColumnToContents(i);
//    }
    QApplication::restoreOverrideCursor();
}

// ############################################ SLOTS ####################################################

// ########################################## CONTEXTS ###################################################

// контекстное меню основного дерева

void sys_systemdialog::SystemContextMenu(QPoint)
{
    QAction *OpenAction = new QAction("Открыть", this);
    connect (OpenAction, SIGNAL(triggered()), this, SLOT(ShowAppropriateSlaveTree()));

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
    if (SlaveTreeModel->isTree) // если не дерево, нет и субэлементов
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

// обработка раскрывания корней дерева

void sys_systemdialog::setMainTVExpanded(QModelIndex index)
{
    if (!index.column())
    {
        if (MainTV->isExpanded(index))
            MainTV->setExpanded(index, false);
        else
            MainTV->setExpanded(index, true);
    }
    for (int i = MainTV->header()->count(); i >= 0; --i)
        MainTV->resizeColumnToContents(i);
}

// обработка раскрывания корней дочернего дерева

void sys_systemdialog::setSlaveTVExpanded(QModelIndex index)
{
    if (!index.column())
    {
        if (SlaveTV->isExpanded(index))
            SlaveTV->setExpanded(index, false);
        else
            SlaveTV->setExpanded(index, true);
    }
    for (int i = SlaveTV->header()->count(); i >= 0; --i)
        SlaveTV->resizeColumnToContents(i);
}

//

void sys_systemdialog::ShowTableContentDialog()
{

}

// отображение соответствующего дочернего дерева

void sys_systemdialog::ShowAppropriateSlaveTree()
{
    QString tmpString, PathString, NewLibName;
    QStringList tmpList;
    QFile tmpFile;

    if (MainTV->model()->rowCount(MainTV->currentIndex()) != 0)
        setMainTVExpanded(MainTV->currentIndex());
    else
    {
        MainFrameLayout->replaceWidget(MainFrame, SlaveTV, Qt::FindDirectChildrenOnly);
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

void sys_systemdialog::ShowAppropriateSlaveTree(QModelIndex index)
{
    MainTV->setCurrentIndex(index);
    ShowAppropriateSlaveTree();
}

// отображение подчинённого дерева в правой части

void sys_systemdialog::ShowSlaveTree(QString str)
{
    QSqlQuery get_additional_fields (pc.sup);
    QString tmpString;
    QStringList tmpStringlist;
    int i;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    get_additional_fields.exec("SELECT `pc` FROM `syslist` WHERE `alias`=\"" + str + "\";");
    if (get_additional_fields.next())
    {
        tble = get_additional_fields.value(0).toString();
        db = sqlc.getdb(tble.left(3));
        if (!db.isValid())
        {
            QMessageBox::warning(this, "warning!", "Нет такой базы данных: "+tble.left(3));
            QApplication::restoreOverrideCursor();
            return;
        }
        tble = tble.right(tble.size()-4);
        if (SlaveTreeModel == (void*)0) delete SlaveTreeModel;
        SlaveTreeModel = new s_aitemmodel(db, tble, QSqlDatabase(), "", false); // если второй таблицы нет, в tble2 должна содержаться пустая строка (требование к таблице dirlist)
        QItemSelectionModel *m = SlaveTV->selectionModel();
        SlaveTreeModel->isEditable = false;
        SlaveTV->setModel(SlaveTreeModel);
        delete m;
        SlaveTV->header()->setDefaultAlignment(Qt::AlignCenter);
        SlaveTV->header()->setVisible(true);
        SlaveTV->setIndentation(2);
        SlaveTV->setAnimated(false);
        SlaveTV->setItemDelegate(gridItemDelegate);
        for (i = 0; i < SlaveTV->header()->count(); i++)
            SlaveTV->resizeColumnToContents(i);
        SlaveTV->setContextMenuPolicy(Qt::CustomContextMenu);
        disconnect(SlaveTV, SIGNAL(customContextMenuRequested(QPoint)), 0, 0);
        connect (SlaveTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SystemSlaveContextMenu(QPoint)));
        disconnect(SlaveTV, SIGNAL(doubleClicked(QModelIndex)), 0, 0);
        connect (SlaveTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ChangeAdditionalFields(QModelIndex)));
        disconnect(SlaveTV, SIGNAL(expanded(QModelIndex)), 0, 0);
        connect(SlaveTV, SIGNAL(expanded(QModelIndex)), SlaveTreeModel, SLOT(addExpandedIndex(QModelIndex)));
        disconnect(SlaveTV, SIGNAL(collapsed(QModelIndex)), 0, 0);
        connect(SlaveTV, SIGNAL(collapsed(QModelIndex)), SlaveTreeModel, SLOT(removeExpandedIndex(QModelIndex)));
        disconnect(SlaveTV, SIGNAL(clicked(QModelIndex)), 0, 0);
        connect(SlaveTV, SIGNAL(clicked(QModelIndex)), this, SLOT(setSlaveTVExpanded(QModelIndex)));
    }
    else
        QMessageBox::warning(this, "warning", "Не найдена ссылка на таблицу справочника!", QMessageBox::Ok, QMessageBox::NoButton);
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
    if (SlaveTreeModel->isTree)
    {
        fields << "alias" << "idalias" << "access";
        values << NewClass << str << "0007";
    }
    else
    {
        fields << tble << "access";
        values << NewClass << "0007";
    }
    QString newID = sqlc.insertvaluestotable(db, tble, fields, values);

    if (!sqlc.result) ChangeAdditionalFields(newID);
    else
        QMessageBox::warning (this, "Ошибка при записи!", QString::number(sqlc.result), \
                              QMessageBox::Ok, QMessageBox::NoButton);

}

void sys_systemdialog::DeleteChildFromTree()
{
    QString tmpString;
    QStringList tmpList;

    tmpString = getSlaveIndex(0);

/*    if (tble == "classes")
    {

        // здесь следует проверка на то, есть ли номенклатурные элементы, которые
        // имеют категорию ту же, что и удаляемая. При желании здесь можно вставить
        // проверку на наличие оных, имеющих одну из вложенных в удаляемую категорий

        QSqlQuery get_nk (db);
// 0.4b        get_nk.exec("SELECT `nk` FROM `nk` WHERE `nkclass`=" + tmpString + ";");
        get_nk.exec("SELECT `nk` FROM `nk` WHERE `nkclass`=" + tmpString + " AND `deleted`=0;");
        tmpList.clear();
        while (get_nk.next())
        {
            tmpList << get_nk.value(0).toString();
        }
        if (tmpList.isEmpty());
        else
        {
            tmpModel = new QStringListModel(tmpList);
//            tmpModel.setStringList(tmpList);
            s_cdd.DTV->setModel(tmpModel);
            s_cdd.DTV->setEditTriggers(QAbstractItemView::NoEditTriggers);
            s_cdd.DTV->resizeColumnsToContents();
            s_cdd.DTV->resizeRowsToContents();
            s_cdd.exec();
            return;
        }
    }
*/
    QSqlQuery delete_classes (db);
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
    ShowAppropriateSlaveTree();
}

void sys_systemdialog::ChangeAdditionalFields(QModelIndex index)
{
    if (SlaveTV->model()->rowCount(SlaveTV->currentIndex()) != 0) // для родителей запрещено иметь дополнительные поля
        setSlaveTVExpanded(SlaveTV->currentIndex());
    else
    {
        SlaveTV->setCurrentIndex(index);
        ChangeAdditionalFields();
    }
}

void sys_systemdialog::ChangeAdditionalFields()
{
    ChangeAdditionalFields(getSlaveIndex(0));
}

void sys_systemdialog::ChangeAdditionalFields(QString id)
{
    int res;
    s_2cdialog *newdialog = new s_2cdialog("Структура системы");
    if (!(res = newdialog->setup("Структура_системы", id)))
    {
        newdialog->setModal(true);
        newdialog->exec();
        ShowAppropriateSlaveTree();
    }
    else
    {
        delete newdialog;
        QMessageBox::information(this,"Ошибка при создании диалога!",\
                             QString::number(res),\
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
/*    sqldialog = new s_sqlfieldsdialog;
    if (!(res = sqldialog->SetupUI(db, tble, id, "Структура системы"))) // берём ИД текущего элемента
    {
        sqldialog->exec();
        ShowAppropriateSlaveTree();
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
    if (ok && !tmpValue.isEmpty())
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
    ShowAppropriateSlaveTree();
}

QString sys_systemdialog::getMainIndex(int column)
{
    QModelIndex index = MainTV->model()->index(MainTV->currentIndex().row(), column, MainTV->model()->parent(MainTV->currentIndex()));
    QString tmpString = index.data(Qt::DisplayRole).toString();
    if (!column)
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}

QString sys_systemdialog::getSlaveIndex(int column)
{
    QModelIndex index = SlaveTV->model()->index(SlaveTV->currentIndex().row(), column, SlaveTV->model()->parent(SlaveTV->currentIndex()));
    QString tmpString = index.data(Qt::DisplayRole).toString();
    if (!column)
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}
