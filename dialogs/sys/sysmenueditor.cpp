#include <QApplication>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QVBoxLayout>
#include "sysmenueditor.h"
#include "../../widgets/treeview.h"
#include "../../models/treemodel.h"
#include "../../models/griddelegate.h"
#include "../../gen/s_tablefields.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"
#include "../messagebox.h"
#include "../s_2cdialog.h"

SysmenuEditor::SysmenuEditor(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground);
}

void SysmenuEditor::SetupUI(QString tble) // tble - имя таблицы, из которой брать дерево для редактирования
{
    this->tble = tble;
    QVBoxLayout *lyout = new QVBoxLayout;
    TreeView *tv = new TreeView(TreeView::TV_EXPANDABLE);
    tv->setObjectName("tv");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    TreeModel *treemodel = new TreeModel;
    int res = treemodel->Setup(tble + "_сокращ");
    if (res)
    {
        QApplication::restoreOverrideCursor();
        WARNMSG("");
        return;
    }
    tv->setModel(treemodel);
    GridDelegate *gridItemDelegate = new GridDelegate;
    tv->setItemDelegate(gridItemDelegate);
    tv->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (tv, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(Context(QPoint)));
    connect (tv, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ChangeFieldsSlot(QModelIndex)));
    lyout->addWidget(tv);
    setLayout(lyout);
    tv->resizeColumnsToContents();
    QApplication::restoreOverrideCursor();
}

void SysmenuEditor::Context(QPoint pt)
{
    Q_UNUSED(pt);
    QAction *AddChild = new QAction("Добавить субэлемент", this);
    AddChild->setSeparator(0);
    connect (AddChild, SIGNAL(triggered()), this, SLOT(AddChild()));
    QAction *AddRoot = new QAction("Добавить элемент", this);
    AddRoot->setSeparator(0);
    connect (AddRoot, SIGNAL(triggered()), this, SLOT(AddRoot()));
    QAction *Separator2Child = new QAction ("", this);
    Separator2Child->setSeparator(1);
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
    ContextMenu->addAction(DeleteChild);
    ContextMenu->addAction(SeparatorChild);
    ContextMenu->addAction(ChangeDataChild);
    ContextMenu->exec(QCursor::pos());
}

void SysmenuEditor::ChangeFieldsSlot(QModelIndex idx)
{
    TreeView *tv = this->findChild<TreeView *>("tv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    TreeModel *mdl= static_cast<TreeModel *>(tv->model());
    int row = idx.row();
    if (mdl->HaveChildren(row)); // родителям запрещено иметь дополнительные поля
    else
    {
        tv->setCurrentIndex(idx);
        ChangeFields(GetMainIndex(0));
    }
}

void SysmenuEditor::ChangeFields()
{
    ChangeFields(GetMainIndex(0)); // взять ИД элемента
}

void SysmenuEditor::ChangeFields(QString str)
{
    s_2cdialog *newdialog = new s_2cdialog(tble);
    newdialog->setup(tble+"_полн", MODE_EDIT, str);
    if (!newdialog->result)
    {
        newdialog->setModal(true);
        newdialog->exec();
        UpdateTree();
    }
    else
    {
        WARNMSG("");
        return;
    }
}

void SysmenuEditor::Delete()
{
    QString tmpString = GetMainIndex(0);
    QStringList sl;
    QString table = tble+"_полн";
    QString idfield = "ИД";
    tfl.tablefields(table, idfield, sl); // возьмём реальное имя таблицы из tablefields. sl(0) - <table>, sl(1) - <tablefields>, sl(2) - <links>
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    QString tmpdb = sl.at(0).split(".").at(0);
    QString tmptble = sl.at(0).split(".").at(1);
    sl = QStringList() << "id"+tmptble;
    sl = sqlc.GetValuesFromTableByField(tmpdb, tmptble, sl, "idalias", tmpString);
    if (!sqlc.result) // есть записи с данным idalias
    {
        if (MessageBox2::question(this, "Вы уверены?", "Категория содержит подкатегории.\nВы уверены, что хотите удалить её?"))
        {
            tfl.Remove(table, tmpString);
            if (tfl.result == TFRESULT_ERROR)
            {
                WARNMSG("");
                return;
            }
            sqlc.DeleteFromDB(tmpdb, tmptble, "idalias", tmpString);
            if (sqlc.result)
            {
                WARNMSG("");
                return;
            }
            MessageBox2::information(this, "Внимание", "Записано успешно!");
        }
    }
    else
    {
        if (MessageBox2::question(this, "Вы уверены?", "Вы уверены?"))
        {
            tfl.Remove(table, tmpString);
            if (tfl.result == TFRESULT_ERROR)
            {
                WARNMSG("");
                return;
            }
        }
        MessageBox2::information(this, "Внимание", "Записано успешно!");
    }
    UpdateTree();
}

void SysmenuEditor::AddChild()
{
    AddToTree(GetMainIndex(0));
}

void SysmenuEditor::AddRoot()
{
    AddToTree("0"); // корневой элемент не имеет ссылки на родителя
}

// добавление элемента в SlaveTree. В str - ИД родительского элемента

void SysmenuEditor::AddToTree(QString str)
{
    QString table = tble + "_полн";
    QString NewClass = QInputDialog::getText(this, "СУПиК :: новый элемент", \
                                               "Введите имя нового элемента", \
                                               QLineEdit::Normal, "", 0, 0, 0);
    if (NewClass == "") return;
    str = QString::number(str.toInt(0));

    QStringList fields, values;
    fields << "Наименование" << "ИД_а" << "Права доступа";
    values << NewClass << str << "0007";
    QString newID;
    tfl.Insert(table, newID);
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    fields.insert(0,"ИД");
    values.insert(0,newID);
    tfl.Update(table,fields,values);
    if (tfl.result != TFRESULT_ERROR)
        ChangeFields(newID);
    else
        WARNMSG("");
}
/*
void SysmenuEditor::ChangeName()
{
    QString tmpString, tmpValue;
    bool ok;

    tmpString = GetMainIndex(1);

    tmpValue = QInputDialog::getText(this, "Изменение наименования",
                                      "Введите новое наименование",
                                      QLineEdit::Normal,
                                      tmpString, &ok);
    if (ok && !tmpValue.isEmpty())
    {
        tmpString = tfl.toid(tble, "Наименование", tmpString);
        if (tfl.result == TFRESULT_ERROR)
        {
            WARNMSG("");
            return;
        }
        QStringList fl = QStringList() << "Наименование" << "ИД";
        QStringList vl = QStringList() << tmpValue << tmpString;
        tfl.Update(tble,fl,vl);
        if (tfl.result == TFRESULT_ERROR)
        {
            WARNMSG("");
            return;
        }
        INFOMSG(PublicClass::ER_CMPMAIN,__LINE__,"Изменение проведено!");
    }
    UpdateTree();
} */

QString SysmenuEditor::GetMainIndex(int column)
{
    TreeView *tv = this->findChild<TreeView *>("tv");
    if (tv == 0)
    {
        DBGMSG;
        return QString();
    }
    QModelIndex index = tv->model()->index(tv->currentIndex().row(), column, tv->model()->parent(tv->currentIndex()));

    QString tmpString = index.data(Qt::DisplayRole).toString();
    pc.ConvertId(!column, tmpString);
    return tmpString;
}

void SysmenuEditor::UpdateTree()
{
    TreeView *tv = this->findChild<TreeView *>("tv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    TreeModel *mdl = dynamic_cast<TreeModel *>(tv->model());
    int res = mdl->Setup(tble + "_сокращ");
    if (res)
    {
        WARNMSG("");
        return;
    }
}
