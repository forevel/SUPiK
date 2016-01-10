#include "treeitem.h"

#include <QVariant>

TreeItem::TreeItem(TreeItem *parent)
{
    ParentItem = parent;
}

TreeItem::~TreeItem()
{
    qDeleteAll(ChildItems);
}

QString TreeItem::Data(int Column) const
{
    if (Column < ItemData.size())
        return ItemData.at(Column);
    else
        return QString();
}

bool TreeItem::SetData(int Column, const QString &Value)
{
    if (Column < 0)
        return false;
    if (Column < ItemData.size())
        ItemData.replace(Column, Value);
    else
        ItemData.append(Value);
    return true;
}

QString TreeItem::AData(int Column) const
{
    if (Column < ItemAData.size())
        return ItemAData.at(Column);
    else
        return QString();
}

bool TreeItem::SetAData(int Column, const QString &Value)
{
    if (Column < 0)
        return false;
    if (Column < ItemAData.size())
        ItemAData.replace(Column, Value);
    else
        ItemAData.append(Value);
    return true;
}

/*bool TreeItem::setLinksData(int column, const QString &data)
{
    if (column < 0)
        return false;
    if (column < linksData.size())
        linksData.replace(column, data);
    else
        linksData.append(data);
    return true;
}*/

int TreeItem::Row() const
{
    if (ParentItem)
        return ParentItem->ChildItems.indexOf(const_cast<TreeItem*>(this));
    return 0;
}

int TreeItem::ColumnCount() const
{
    return ItemData.count();
}

bool TreeItem::RemoveColumns(int Position, int Columns)
{
    if (Position < 0 || ((Position + Columns) > ItemData.size()))
        return false;
    for (int column = 0; column < Columns; column++)
        ItemData.removeAt(Position);
    foreach (TreeItem *Child, ChildItems)
        Child->RemoveColumns(Position, Columns);
    return true;
}

void TreeItem::SetColor(int Column, QColor Color)
{
    if (Column < ItemColor.size())
        ItemColor.replace(Column, Color);
    else
        ItemColor.append(Color);
}

void TreeItem::SetFont(int Column, QFont Font)
{
    if (Column < ItemFont.size())
        ItemFont.replace(Column, Font);
    else
        ItemFont.append(Font);
}

void TreeItem::SetIcon(int Column, QIcon Icon)
{
    if (Column < ItemIcon.size())
        ItemIcon.replace(Column, Icon);
    else
        ItemIcon.append(Icon);
}

QColor TreeItem::Color(int Column)
{
    if (Column < ItemColor.size())
        return ItemColor.at(Column);
    else
        return QColor();
}

QFont TreeItem::Font(int Column)
{
    if (Column < ItemFont.size())
        return ItemFont.at(Column);
    else
        return QFont();
}

QIcon TreeItem::Icon(int Column)
{
    if (Column < ItemIcon.size())
        return ItemIcon.at(Column);
    else
        return QIcon();
}
