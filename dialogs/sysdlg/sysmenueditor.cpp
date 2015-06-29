#include <QApplication>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QVBoxLayout>
#include "sysmenueditor.h"
#include "../../widgets/s_tqtreeview.h"
#include "../../models/s_ncmodel.h"
#include "../../models/s_duniversal.h"
#include "../../gen/s_tablefields.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"
#include "../../dialogs/s_2cdialog.h"

SysmenuEditor::SysmenuEditor(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

void SysmenuEditor::SetupUI(QString tble) // tble - имя таблицы, из которой брать дерево для редактирования
{
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqTreeView *tv = new s_tqTreeView;
    tv->setObjectName("tv");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    s_ntmodel *treemodel = new s_ntmodel;
    int res = treemodel->Setup(tble + "_сокращ");
    tv->setModel(treemodel);
    connect(tv, SIGNAL(expanded(QModelIndex)), treemodel, SLOT(addExpandedIndex(QModelIndex)));
    connect(tv, SIGNAL(collapsed(QModelIndex)), treemodel, SLOT(removeExpandedIndex(QModelIndex)));
    tv->header()->setDefaultAlignment(Qt::AlignCenter);
    tv->header()->setVisible(true);
    tv->setIndentation(2);
    tv->setAnimated(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    tv->setItemDelegate(gridItemDelegate);
    tv->ResizeColumnsToContents();
    tv->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (tv, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(Context(QPoint)));
    connect (tv, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ChangeFields(QModelIndex)));
    lyout->addWidget(tv);
    setLayout(lyout);
    QApplication::restoreOverrideCursor();
}

void SysmenuEditor::Context(QPoint pt)
{
    QAction *AddChild = new QAction("Добавить субэлемент", this);
    AddChild->setSeparator(0);
    connect (AddChild, SIGNAL(triggered()), this, SLOT(AddChild()));
    QAction *AddRoot = new QAction("Добавить элемент", this);
    AddRoot->setSeparator(0);
    connect (AddRoot, SIGNAL(triggered()), this, SLOT(AddRoot()));
    QAction *Separator2Child = new QAction ("", this);
    Separator2Child->setSeparator(1);
    QAction *ChangeChild = new QAction("Изменить имя элемента", this);
    ChangeChild->setSeparator(0);
    connect (ChangeChild, SIGNAL(triggered()), this, SLOT(ChangeName()));
    QAction *DeleteChild = new QAction("Удалить элемент", this);
    DeleteChild->setSeparator(0);
    connect(DeleteChild, SIGNAL(triggered()), this, SLOT(Delete()));
    QAction *SeparatorChild = new QAction ("", this);
    SeparatorChild->setSeparator(1);
    QAction *ChangeDataChild = new QAction ("Дополнительные данные", this);
    ChangeDataChild->setSeparator(false);
    connect(ChangeDataChild, SIGNAL(triggered()), this, SLOT(ChangeFields()));

    QMenu *ContextMenu = new QMenu;
    ContextMenu->setTitle("Меню");
    ContextMenu->addAction(AddRoot);
    ContextMenu->addAction(AddChild);
    ContextMenu->addAction(Separator2Child);
    ContextMenu->addAction(ChangeChild);
    ContextMenu->addAction(DeleteChild);
    ContextMenu->addAction(SeparatorChild);
    ContextMenu->addAction(ChangeDataChild);
    ContextMenu->exec(QCursor::pos());
}

void SysmenuEditor::ChangeFields(QModelIndex idx)
{
    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("tv");
    if (tv == 0)
    {
        emit error(ER_SYSMENU+0x51);
        return;
    }
    if (tv->model()->rowCount(tv->currentIndex()) != 0); // для родителей запрещено иметь дополнительные поля
    else
    {
        tv->setCurrentIndex(idx);
        ChangeFields();
    }
}

void SysmenuEditor::ChangeFields()
{
    QString tmps = GetIndex(0); // взять ИД элемента
    s_2cdialog *newdialog = new s_2cdialog;
    newdialog->setup("Структура_системы", MODE_CHOOSE, tmps, "Структура системы");
    if (!newdialog->result)
    {
        newdialog->setModal(true);
        newdialog->exec();
//        SetSlaveTV();
    }
    else
    {
        emit error(ER_SYSMENU+0x61);
        return;
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

void SysmenuEditor::Delete()
{
    QString tmpString;
    QStringList tmpList;

    tmpString = GetIndex(0);

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

void SysmenuEditor::AddChild()
{
    AddToTree(GetIndex(0));
}

void SysmenuEditor::AddRoot()
{
    AddToTree("0"); // корневой элемент не имеет ссылки на родителя
}

// добавление элемента в SlaveTree. В str - ИД родительского элемента

void SysmenuEditor::AddToTree(QString str)
{
    QString NewClass = QInputDialog::getText(this, "СУПиК :: новый элемент", \
                                               "Введите имя нового элемента", \
                                               QLineEdit::Normal, "", 0, 0, 0);
    if (NewClass == "") return;
    str = QString::number(str.toInt(0));

    QStringList fields, values;
    fields << "alias" << "idalias" << "access";
    values << NewClass << str << "0007";
/*    QString newID = sqlc.insertvaluestotable(db, tble, fields, values);

    if (!sqlc.result) ChangeAdditionalFields(newID);
    else
        QMessageBox::warning (this, "Ошибка при записи!", QString::number(sqlc.result), \
                              QMessageBox::Ok, QMessageBox::NoButton);
*/
}

void SysmenuEditor::ChangeName()
{
    QString tmpString, tmpValue;
    bool ok;

    tmpString = GetIndex(0);

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

QString SysmenuEditor::GetIndex(int column)
{
    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("tv");
    if (tv == 0)
    {
        emit error(ER_SYSMENU+0xA1);
        return QString();
    }
    QModelIndex index = tv->model()->index(tv->currentIndex().row(), column, tv->model()->parent(tv->currentIndex()));

    QString tmpString = index.data(Qt::DisplayRole).toString();
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}
