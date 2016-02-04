#include "proxymodel.h"
#include "treemodel.h"

ProxyModel::ProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

ProxyModel::~ProxyModel()
{

}

QVariant ProxyModel::data(const QModelIndex &index, int role) const
{
    QModelIndex SourceIndex = mapToSource(index);
    return sourceModel()->data(SourceIndex, role);
}

bool ProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QModelIndex SourceIndex = mapFromSource(index);
    return sourceModel()->setData(SourceIndex, value, role);
}

/*
// добавление в список открытых корней дерева по событию expanded от view

void ProxyModel::addExpandedIndex(const QModelIndex &index)
{
    if (index.isValid())
    {
        TreeModel *mdl = static_cast<TreeModel *>(sourceModel());
        mdl->addExpandedIndex(mapToSource(index));
    }
}

// удаление из списка открытых корней дерева по событию collapsed от view

void ProxyModel::removeExpandedIndex(const QModelIndex &index)
{
    if (index.isValid())
    {
        TreeModel *mdl = static_cast<TreeModel *>(sourceModel());
        mdl->removeExpandedIndex(mapToSource(index));
    }
} */

QModelIndex ProxyModel::Find(int column, const QVariant &value)
{
    for (int i=0; i<rowCount(); i++)
    {
        for (int j=column; j<columnCount(); j++)
        {
            QModelIndex idx = index(i, j, QModelIndex());
            if (!idx.isValid())
                continue;
            QVariant vl = data(idx, Qt::DisplayRole);
            if (vl == value)
                return idx;
        }
    }
   return QModelIndex();
}

/*
bool ProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QString LeftData = sourceModel()->data(left).toString();
    QString RightData = sourceModel()->data(right).toString();
    QString LeftColumn0Data = sourceModel()->data(left.sibling(left.row(), 0)).toString();
    QString RightColumn0Data = sourceModel()->data(right.sibling(right.row(), 0)).toString();
    if (LeftColumn0Data < RightColumn0Data)
        return true; // если в столбце 0 данные left меньше, то и весь left меньше, выходим
    if (LeftData < RightData)
        return true;
    else
        return false;
}
*/
