#include "whplacestreeitem.h"

WhPlacesTreeItem::WhPlacesTreeItem(WhPlacesTreeItem *parent)
{
    parentItem = parent;
}

WhPlacesTreeItem::~WhPlacesTreeItem()
{
    qDeleteAll(childItems);
}

QString WhPlacesTreeItem::data(int column) const
{
    if (column < itemData.size())
        return itemData.at(column);
    else
        return QString();
}

bool WhPlacesTreeItem::setData(int column, const QString &value)
{
    if (column < 0)
        return false;
    if (column < itemData.size())
        itemData.replace(column, value);
    else
        itemData.append(value);
    return true;
}

WhPlacesTreeItem *WhPlacesTreeItem::child(int row)
{
    if ((row < childItems.size()) && (row != -1))
        return childItems.at(row);
    else
        return 0;
}

int WhPlacesTreeItem::childCount() const
{
    return childItems.count();
}

int WhPlacesTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<WhPlacesTreeItem*>(this));
    return 0;
}

int WhPlacesTreeItem::columnCount() const
{
    return itemData.count();
}

WhPlacesTreeItem *WhPlacesTreeItem::insertChild(int position, int columns)
{
    QStringList data;
    if (position < 0)
        return 0;
    for (int i = 0; i < columns; i++)
        data << "";
    WhPlacesTreeItem *item = new WhPlacesTreeItem(this);
    if (position < childItems.size())
        childItems.insert(position, item);
    else
        childItems.append(item);
    return item;
}

WhPlacesTreeItem *WhPlacesTreeItem::parent()
{
    return parentItem;
}

bool WhPlacesTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || ((position + count) > childItems.size()))
        return false;
    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);
    return true;
}

bool WhPlacesTreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || ((position + columns) > itemData.size()))
        return false;
    for (int column = 0; column < columns; column++)
        itemData.removeAt(position);
    foreach (WhPlacesTreeItem *child, childItems)
        child->removeColumns(position, columns);
    return true;
}
