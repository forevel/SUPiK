#include "proxymodel.h"
#include "s_ntmodel.h"

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
