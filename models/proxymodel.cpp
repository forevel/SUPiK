#include "proxymodel.h"
#include "s_ntmodel.h"
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

// добавление в список открытых корней дерева по событию expanded от view

void ProxyModel::addExpandedIndex(const QModelIndex &index)
{
    if (index.isValid())
    {
        s_ntmodel *mdl = static_cast<s_ntmodel *>(sourceModel());
        mdl->addExpandedIndex(mapToSource(index));
//        TreeModel *mdl = static_cast<TreeModel *>(sourceModel());
//        mdl->GoIntoIndex(mapToSource(index));
    }
}

// удаление из списка открытых корней дерева по событию collapsed от view

void ProxyModel::removeExpandedIndex(const QModelIndex &index)
{
    if (index.isValid())
    {
        s_ntmodel *mdl = static_cast<s_ntmodel *>(sourceModel());
        mdl->removeExpandedIndex(mapToSource(index));
    }
}

QModelIndexList ProxyModel::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const
{
    Q_UNUSED(flags);
    QModelIndexList mil;
    int StartRow = start.row();
    int StartColumn = start.column();
    for (int i=StartRow; i<rowCount(); i++)
    {
        for (int j=StartColumn; j<columnCount(); j++)
        {
            QModelIndex idx = index(i, j, QModelIndex());
            if (!idx.isValid())
                continue;
            QVariant vl = data(idx, role);
            if (vl == value)
                mil.append(idx);
            if (hits >= mil.size())
                return mil;
        }
    }
    return mil;
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
