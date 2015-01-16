#include "../inc/s_ntitem.h"

#include <QVariant>

s_ntitem::s_ntitem(s_ntitem *parent)
{
    parentItem = parent;
}

s_ntitem::~s_ntitem()
{
    qDeleteAll(childItems);
}

QString s_ntitem::data(int column) const
{
    if (column < itemData.size())
        return itemData.at(column);
    else
        return QString();
}

QString s_ntitem::linksdata(int column) const
{
    if (column < linksData.size())
        return linksData.at(column);
    else
        return QString();
}

bool s_ntitem::setData(int column, const QString &value)
{
    if (column < 0)
        return false;
    if (column < itemData.size())
        itemData.replace(column, value);
    else
        itemData.append(value);
    return true;
}

bool s_ntitem::setLinksData(int column, const QString &data)
{
    if (column < 0)
        return false;
    if (column < linksData.size())
        linksData.replace(column, data);
    else
        linksData.append(data);
    return true;
}

s_ntitem *s_ntitem::child(int row)
{
    if (row < childItems.size())
        return childItems.at(row);
    else
        return 0;
}

int s_ntitem::childCount() const
{
    return childItems.count();
}

int s_ntitem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<s_ntitem*>(this));
    return 0;
}

int s_ntitem::columnCount() const
{
    return itemData.count();
}

s_ntitem *s_ntitem::insertChild(int position, int columns)
{
    QStringList data;
    if (position < 0)
        return 0;
    for (int i = 0; i < columns; i++)
        data << "";
    s_ntitem *item = new s_ntitem(this);
    if (position < childItems.size())
        childItems.insert(position, item);
    else
        childItems.append(item);
    return item;
}

bool s_ntitem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;
    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QString());
    foreach (s_ntitem *child, childItems)
        child->insertColumns(position, columns);
    return true;
}

s_ntitem *s_ntitem::parent()
{
    return parentItem;
}

bool s_ntitem::removeChildren(int position, int count)
{
    if (position < 0 || ((position + count) > childItems.size()))
        return false;
    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);
    return true;
}

bool s_ntitem::removeColumns(int position, int columns)
{
    if (position < 0 || ((position + columns) > itemData.size()))
        return false;
    for (int column = 0; column < columns; ++column)
        itemData.removeAt(position);
    foreach (s_ntitem *child, childItems)
        child->removeColumns(position, columns);
    return true;
}

void s_ntitem::setColor(int column, QColor color)
{
    if (column < itemColor.size())
        itemColor.replace(column, color);
    else
        itemColor.append(color);
}

void s_ntitem::setFont(int column, QFont font)
{
    if (column < itemFont.size())
        itemFont.replace(column, font);
    else
        itemFont.append(font);
}

void s_ntitem::setIcon(int column, QIcon icon)
{
    if (column < itemIcon.size())
        itemIcon.replace(column, icon);
    else
        itemIcon.append(icon);
}

QColor s_ntitem::color(int column)
{
    if (column < itemColor.size())
        return itemColor.at(column);
    else
        return QColor();
}

QFont s_ntitem::font(int column)
{
    if (column < itemFont.size())
        return itemFont.at(column);
    else
        return QFont();
}

QIcon s_ntitem::icon(int column)
{
    if (column < itemIcon.size())
        return itemIcon.at(column);
    else
        return QIcon();
}
