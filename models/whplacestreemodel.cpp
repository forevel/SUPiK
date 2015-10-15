#include "whplacestreemodel.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"

#include <QSqlRecord>

WhPlacesTreeModel::WhPlacesTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{

}

WhPlacesTreeModel::~WhPlacesTreeModel()
{
    delete rootItem;
}

int WhPlacesTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

QVariant WhPlacesTreeModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    if (!index.isValid())
        return QString();
    WhPlacesTreeItem *item=getItem(index);
    return item->data(index.column());
}

bool WhPlacesTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);
    if (index.isValid())
    {
        WhPlacesTreeItem *item=getItem(index);
        bool result = item->setData(index.column(), value.toString());
        if (result)
            emit dataChanged(index, index);
        return result;
    }
    else
        return false;
}

Qt::ItemFlags WhPlacesTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant WhPlacesTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();
}

QModelIndex WhPlacesTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
    WhPlacesTreeItem *parentItem=getItem(parent);
    WhPlacesTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool WhPlacesTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    bool success;
    WhPlacesTreeItem *parentItem=getItem(parent);
    beginInsertRows(parent, position, position + rows - 1);
    for (int i = 0; i < rows; i++)
    {
        WhPlacesTreeItem *child = parentItem->insertChild(position, rootItem->columnCount());
        if (child)
            success = true;
        else
            success = false;
    }
    endInsertRows();
    return success;
}

QModelIndex WhPlacesTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();
    WhPlacesTreeItem *childItem=getItem(index);
    WhPlacesTreeItem *parentItem = childItem->parent();
    if (parentItem == rootItem)
        return QModelIndex();
    return createIndex(parentItem->row(), 0, parentItem);
}

bool WhPlacesTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;
    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();
    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());
    return success;
}

bool WhPlacesTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    WhPlacesTreeItem *parentItem=getItem(parent);
    bool success = true;
    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();
    return success;
}

int WhPlacesTreeModel::rowCount(const QModelIndex &parent) const
{
    WhPlacesTreeItem *parentItem=getItem(parent);
    return parentItem->childCount();
}

bool WhPlacesTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    Q_UNUSED(role);
    bool result = rootItem->setData(section, value.toString());
    if (result)
        emit headerDataChanged(orientation, section, section);
    return result;
}

// свои методы

QModelIndex WhPlacesTreeModel::getIndex(WhPlacesTreeItem *item) const
{
    if (item == rootItem)
        return QModelIndex();
    QModelIndex parentIdx;
    WhPlacesTreeItem *parentItem = item->parent();
    if (parentItem == rootItem)
        parentIdx = QModelIndex();
    else
        parentIdx = index(parentItem->row(), 0, QModelIndex());
    return index(item->row(), 0, parentIdx);
}

WhPlacesTreeItem *WhPlacesTreeModel::getItem(const QModelIndex &index) const
{
    WhPlacesTreeItem *item;
    if (index.isValid())
        item = static_cast<WhPlacesTreeItem*>(index.internalPointer());
    else
        item = rootItem;
    return item;
}

void WhPlacesTreeModel::addRow(int position, QModelIndex &parent)
{
    insertRows(position, 1, parent);
}

// процедура инициализации модели данными из таблицы table в tablefields и построение дерева по полям alias и idalias

int WhPlacesTreeModel::Setup(QString table, int index)
{
    ClearModel();
    // 1. взять столбцы tablefields из tablefields, где tablename=table
    // 2. найти среди них столбцы <db>.<tble>.alias и <db>.<tble>.idalias. Если нет - это не дерево, выход
    // 3. взять значения столбцов alias и idalias из таблицы <db>.<tble>
    // 4. построить по ним дерево

    // 1
    int i;
    QStringList fl = QStringList() << "table" << "tablefields" << "header";
    vl = sqlc.GetMoreValuesFromTableByField(sqlc.GetDB("sup"), "tablefields", fl, "tablename", table, "fieldsorder", true);
    if (sqlc.result)
        return 1;
    // 2
    vlidalpos=-1;
    bool IsIdAliasExist = false, IsAliasExist = false;
    for (i = 0; i < vl.size(); i++)
    {
        if (vl.at(i).at(1) == "idalias")
        {
            vlidalpos = i;
            IsIdAliasExist = true;
        }
        if (vl.at(i).at(1) == "alias")
            IsAliasExist = true;
    }
    if (!IsIdAliasExist || !IsAliasExist)
        return 1;
    // 3
    catlist = vl.at(vlidalpos).at(0).split("."); // catlist - таблица, из которой брать категории
    vlsize = vl.at(1).size();
    for (i = 0; i < vl.size(); i++)
        setHeaderData(i, Qt::Horizontal, vl.at(i).at(2), Qt::EditRole);
    // 4
    int res = BuildTree(QString::number(index));
    if (res)
        return 1;
    return 0;
}

// процедура построения дерева
// на входе catlist (ссылка на таблицу категорий с полем idalias) и slvtble (название таблицы в chooselists, из которой брать записи категорий)

int WhPlacesTreeModel::BuildTree(QString id)
{
    int res;
    QStringList tmpStringList;
    QString tmpString;
    // считываем все данные из таблицы
    QSqlQuery get_child_from_db1 (sqlc.GetDB(catlist.at(0)));
    tmpString = "SELECT ";
    for (int i=0; i<vlsize; i++)
        tmpString += "`" + vl.at(1).at(i) + "`,";
    tmpString += "` FROM `"+catlist.at(1)+"` WHERE `idalias`=\""+id+"\" AND `deleted`=0 ORDER BY `id"+catlist.at(1)+"` ASC;";
    get_child_from_db1.exec(tmpString);
    // увеличиваем уровень дерева
    position++;
    if (id == "0") position = 0; // для корневых элементов position д.б. равен нулю
// строим дерево в модели model
    while (get_child_from_db1.next())
    {
        tmpStringList.clear();
        for (int i=0; i<get_child_from_db1.record().count(); i++)
            tmpStringList << get_child_from_db1.value(i).toString();
        additemtotree(position, tmpStringList);
        if (vlidalpos < get_child_from_db1.record().count())
            res = BuildTree(get_child_from_db1.value(vlidalpos).toString()); // в качестве аргумента функции используется индекс поля idalias
        if (res)
            return 1;
    }
    position--; // после добавления всех детишек уровень понижается
    return 0;
}

void WhPlacesTreeModel::additemtotree(int position, QStringList sl)
{
    if (position > indentations.last()) {
        // The last child of the current parent is now the new parent
        // unless the current parent has no children.

        if (parents.last()->childCount() > 0) {
            parents << parents.last()->child(parents.last()->childCount()-1);
            indentations << position;
        }
    } else {
        while (position < indentations.last() && parents.count() > 0) {
            parents.pop_back();
            indentations.pop_back();
        }
    }
    WhPlacesTreeItem *parent = parents.last();
    QModelIndex parentIndex = getIndex(parent);
    addRow(parent->childCount(), parentIndex);
    for (int column = 0; column < sl.size(); ++column)
        setData(index(parent->childCount()-1, column, parentIndex), sl.at(column), Qt::EditRole);
}

void WhPlacesTreeModel::ClearModel()
{
    beginResetModel();
    while (rowCount() > 0)
        removeRows(0, 1);
    endResetModel();

}

void WhPlacesTreeModel::Load(QString table, int index)
{
    //
}
