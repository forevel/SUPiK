#include "baseitem.h"

#include <QVariant>

BaseItem::BaseItem(int columns, BaseItem *parent)
{
    Q_UNUSED(parent);
    int i;
    for (i=0; i<columns; ++i)
    {
        SetData(i, "");
        SetToolTip(i, "");
        SetHeader(i, "");
        SetColor(i, Qt::black);
        SetFont(i, QFont());
        SetIcon(i, QIcon());
    }
}

BaseItem::~BaseItem()
{
}

QString BaseItem::Data(int column) const
{
    if (column < ItemData.size())
        return ItemData.at(column);
    else
        return QString();
}

bool BaseItem::SetData(int column, const QString &Value)
{
    if (column < 0)
        return false;
    if (column < ItemData.size())
        ItemData.replace(column, Value);
    else
        ItemData.append(Value);
    return true;
}

QString BaseItem::Header(int column) const
{
    if (column < Headers.size())
        return Headers.at(column);
    else
        return QString();
}

bool BaseItem::SetHeader(int column, const QString &Value)
{
    if (column < 0)
        return false;
    if (column < Headers.size())
        Headers.replace(column, Value);
    else
        Headers.append(Value);
    return true;
}

void BaseItem::SetColor(int column, QColor Color)
{
    if (column < ItemColor.size())
        ItemColor.replace(column, Color);
    else
        ItemColor.append(Color);
}

void BaseItem::SetFont(int column, QFont Font)
{
    if (column < ItemFont.size())
        ItemFont.replace(column, Font);
    else
        ItemFont.append(Font);
}

void BaseItem::SetIcon(int column, QIcon Icon)
{
    if (column < ItemIcon.size())
        ItemIcon.replace(column, Icon);
    else
        ItemIcon.append(Icon);
}

bool BaseItem::SetToolTip(int column, const QString &Value)
{
    if ((column < 0) || (column > ToolTipData.size()))
        return false;
    if (column < ToolTipData.size())
        ToolTipData.replace(column, Value);
    else
        ToolTipData.append(Value);
    return true;
}

QString BaseItem::ToolTip(int column) const
{
    if (column < ToolTipData.size())
        return ToolTipData.at(column);
    else
        return QString();
}
QColor BaseItem::Color(int column)
{
    if (column < ItemColor.size())
        return ItemColor.at(column);
    else
        return QColor();
}

QFont BaseItem::Font(int column)
{
    if (column < ItemFont.size())
        return ItemFont.at(column);
    else
        return QFont();
}

QIcon BaseItem::Icon(int column)
{
    if (column < ItemIcon.size())
        return ItemIcon.at(column);
    else
        return QIcon();
}
