#include "../inc/s_aitem.h"

s_aitem::s_aitem(const QList<QVariant> &data, QColor color, QFont font, s_aitem *parent)
{
    parentItem = parent;
    itemData = data;
    itemColor = color;
    itemFont = font;
}

s_aitem::s_aitem(const QStringList &list, QColor color, QFont font, s_aitem *parent)
{
    QList<QVariant> data;
    int i;

    for (i = 0; i < list.size(); i++)
        data << list.at(i);

    parentItem = parent;
    itemData = data;
    itemColor = color;
    itemFont = font;
}

s_aitem::~s_aitem()
{
    qDeleteAll(childItems);
}

s_aitem *s_aitem::child(int row)
{
    return childItems.value(row);
}

int s_aitem::childCount() const
{
    return childItems.count();
}

int s_aitem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<s_aitem*>(this));
    return 0;
}

int s_aitem::columnCount() const
{
    return itemData.count();
}

QVariant s_aitem::data(int column) const
{
    return itemData.value(column);
}

bool s_aitem::insertChildren(int position, int count, int columns, QColor color, QFont font)
{
    QList<QVariant> data;
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        for (int i = 0; i < columns; i++)
            data << "";
//        QVector<QVariant> data(columns);
        s_aitem *item = new s_aitem(data, color, font, this);
        childItems.insert(position, item);
    }
    return true;
}

bool s_aitem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (s_aitem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}

s_aitem *s_aitem::parent()
{
    return parentItem;
}

bool s_aitem::removeChildren(int position, int count)
{
    if (position < 0 || ((position + count) > childItems.size()))
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool s_aitem::removeColumns(int position, int columns)
{
    if (position < 0 || ((position + columns) > itemData.size()))
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.removeAt(position);

    foreach (s_aitem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

bool s_aitem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

int s_aitem::icon()
{
    return item0icon;
}

void s_aitem::setIcon(int iconindex)
{
    item0icon = iconindex;
}

QColor s_aitem::color()
{
    return itemColor;
}

QFont s_aitem::font()
{
    return itemFont;
}
