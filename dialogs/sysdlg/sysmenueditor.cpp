#include <QApplication>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QMessageBox>
#include "sysmenueditor.h"
#include "../../widgets/s_tqtreeview.h"
#include "../../models/s_ncmodel.h"
#include "../../models/s_ntmodel.h"
#include "../../models/s_duniversal.h"
#include "../../gen/s_tablefields.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"
#include "../../dialogs/s_2cdialog.h"

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
    s_tqTreeView *tv = new s_tqTreeView;
    tv->setObjectName("tv");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    s_ntmodel *treemodel = new s_ntmodel;
    int res = treemodel->Setup(tble + "_сокращ");
    if (res)
    {
        QApplication::restoreOverrideCursor();
        emit error(res+ER_SYSMENU,0x01);
        return;
    }
    tv->setModel(treemodel);
    connect(tv, SIGNAL(expanded(QModelIndex)), treemodel, SLOT(addExpandedIndex(QModelIndex)));
    connect(tv, SIGNAL(collapsed(QModelIndex)), treemodel, SLOT(removeExpandedIndex(QModelIndex)));
    tv->header()->setDefaultAlignment(Qt::AlignCenter);
    tv->header()->setVisible(true);
    tv->setIndentation(2);
    tv->setAnimated(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    connect(gridItemDelegate,SIGNAL(error(int,int)),this,SIGNAL(error(int,int)));
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
    Q_UNUSED(pt);
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
        emit error(ER_SYSMENU,0x11);
        return;
    }
    if (tv->model()->rowCount(tv->currentIndex()) != 0); // для родителей запрещено иметь дополнительные поля
    else
    {
        tv->setCurrentIndex(idx);
        ChangeFields(GetIndex(0));
    }
}

void SysmenuEditor::ChangeFields()
{
    ChangeFields(GetIndex(0)); // взять ИД элемента
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
        emit error(ER_SYSMENU,0x21);
        return;
    }
}

void SysmenuEditor::Delete()
{
    QString tmpString = GetIndex(0);
    QStringList sl = tfl.tablefields(tble, "ИД"); // возьмём реальное имя таблицы из tablefields. sl(0) - <table>, sl(1) - <tablefields>, sl(2) - <links>
    if (tfl.result)
    {
        emit error(ER_SYSMENU+tfl.result,0x31);
        return;
    }
    QString tmpdb = sl.at(0).split(".").at(0);
    QString tmptble = sl.at(0).split(".").at(1);
    sl = QStringList() << "id"+tmptble;
    sl = sqlc.getvaluesfromtablebyfield(sqlc.getdb(tmpdb), tmptble, sl, "idalias", tmpString);
    if (!sqlc.result) // есть записи с данным idalias
    {
        if (QMessageBox::question(this, "Вы уверены?", \
                                  "Категория содержит подкатегории.\nВы уверены, что хотите удалить её?", \
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            tfl.remove(tble, tmpString);
            if (tfl.result)
            {
                emit error(ER_SYSMENU,0x32);
                return;
            }
            sqlc.deletefromdb(sqlc.getdb(tmpdb), tmptble, "idalias", tmpString);
            if (sqlc.result)
            {
                emit error(ER_SYSMENU,0x33);
                return;
            }
            QMessageBox::information(this, "Успешно!", "Записано успешно!");
        }
    }
    else
    {
        if (QMessageBox::question(this, "Вы уверены?", \
                                  "Вы уверены?", \
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            tfl.remove(tble, tmpString);
            if (tfl.result)
                emit error(ER_SYSMENU,0x34);
        }
        QMessageBox::information(this, "Успешно!", "Записано успешно!");
    }
    UpdateTree();
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
    fields << "Наименование" << "ИД_а" << "Права доступа";
    values << NewClass << str << "0007";
    QString newID = tfl.insert(tble+"_полн");
    if (tfl.result)
    {
        emit error(ER_SYSMENU+tfl.result, 0x41);
        return;
    }
    fields.insert(0,"ИД");
    values.insert(0,newID);
    tfl.idtois(tble+"_полн",fields,values);
    if (!tfl.result)
        ChangeFields(newID);
    else
        emit error(ER_SYSMENU+tfl.result,0x42);
}

void SysmenuEditor::ChangeName()
{
    QString tmpString, tmpValue;
    bool ok;

    tmpString = GetIndex(1);

    tmpValue = QInputDialog::getText(this, "Изменение наименования",
                                      "Введите новое наименование",
                                      QLineEdit::Normal,
                                      tmpString, &ok);
    if (ok && !tmpValue.isEmpty())
    {
        tmpString = tfl.toid(tble, "Наименование", tmpString);
        if (tfl.result)
        {
            emit error(ER_SYSMENU+tfl.result,0x51);
            return;
        }
        QStringList fl = QStringList() << "Наименование" << "ИД";
        QStringList vl = QStringList() << tmpValue << tmpString;
        tfl.idtois(tble,fl,vl);
        if (tfl.result)
        {
            emit error(ER_SYSMENU+tfl.result,0x52);
            return;
        }
        QMessageBox::warning (this, "Успешно", "Изменение проведено!", \
                              QMessageBox::Ok, QMessageBox::NoButton);
    }
    UpdateTree();
}

QString SysmenuEditor::GetIndex(int column)
{
    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("tv");
    if (tv == 0)
    {
        emit error(ER_SYSMENU,0x51);
        return QString();
    }
    QModelIndex index = tv->model()->index(tv->currentIndex().row(), column, tv->model()->parent(tv->currentIndex()));

    QString tmpString = index.data(Qt::DisplayRole).toString();
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}

void SysmenuEditor::UpdateTree()
{
    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("tv");
    if (tv == 0)
    {
        emit error(ER_SYSMENU,0x61);
        return;
    }
    s_ntmodel *mdl = dynamic_cast<s_ntmodel *>(tv->model());
    int res = mdl->Setup(tble + "_сокращ");
    if (res)
    {
        emit error(res+ER_SYSMENU,0x62);
        return;
    }
}
