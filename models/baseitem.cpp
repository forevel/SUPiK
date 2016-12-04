#include "treeitem.h"

#include <QVariant>

TreeItem::TreeItem(TreeItem *parent)
{
    Q_UNUSED(parent);
}

TreeItem::~TreeItem()
{
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

QString TreeItem::Links(int Column) const
{
    if (Column < LinksData.size())
        return LinksData.at(Column);
    else
        return QString();
}

bool TreeItem::SetLinks(int Column, const QString &Value)
{
    if (Column < 0)
        return false;
    if (Column < LinksData.size())
        LinksData.replace(Column, Value);
    else
        LinksData.append(Value);
    return true;
}

/*int TreeItem::Row() const
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
} */

QString TreeItem::TableNumber(int Column) const
{
    if (Column < TableNumbers.size())
        return TableNumbers.at(Column);
    else
        return QString();
}

bool TreeItem::SetTableNumber(int Column, const QString &Value)
{
    if (Column < 0)
        return false;
    if (Column < TableNumbers.size())
        TableNumbers.replace(Column, Value);
    else
        TableNumbers.append(Value);
    return true;
}

QString TreeItem::Header(int Column) const
{
    if (Column < Headers.size())
        return Headers.at(Column);
    else
        return QString();
}

bool TreeItem::SetHeader(int Column, const QString &Value)
{
    if (Column < 0)
        return false;
    if (Column < Headers.size())
        Headers.replace(Column, Value);
    else
        Headers.append(Value);
    return true;
}

QString TreeItem::Info(int Column) const
{
    if (Column < Infos.size())
        return Infos.at(Column);
    else
        return QString();
}

bool TreeItem::SetInfo(int Column, const QString &Value)
{
    if (Column < 0)
        return false;
    if (Column < Infos.size())
        Infos.replace(Column, Value);
    else
        Infos.append(Value);
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
