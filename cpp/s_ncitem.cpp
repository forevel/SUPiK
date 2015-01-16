#include "../inc/s_ncitem.h"

s_ncitem::s_ncitem(s_ncitem *parent)
{
    Q_UNUSED(parent);
}

QString s_ncitem::data(int column) const
{
    return itemData.at(column);
}

QString s_ncitem::linksdata(int column) const
{
    if (column < linksData.size())
        return linksData.at(column);
    else
        return QString();
}

void s_ncitem::setData(int column, const QString &data)
{
    if (column < itemData.size())
        itemData.replace(column, data);
    else
        itemData.append(data);
}

void s_ncitem::setLinksData(int column, const QString &data)
{
    if (column < linksData.size())
        linksData.replace(column, data);
    else
        linksData.append(data);
}

// проверка элементов в определённых позициях на "пустость"

bool s_ncitem::isEmpty(QList<int> columnsToCheck)
{
    for (int i = 0; i < columnsToCheck.size(); i++)
        if ((itemData.at(columnsToCheck.at(i))) != "")
            return false;
    return true;
}

void s_ncitem::setColor(int column, QColor color)
{
    if (column < itemColor.size())
        itemColor.replace(column, color);
    else
        itemColor.append(color);
}

void s_ncitem::setFont(int column, QFont font)
{
    if (column < itemFont.size())
        itemFont.replace(column, font);
    else
        itemFont.append(font);
}

void s_ncitem::setIcon(int column, QIcon icon)
{
    if (column < itemIcon.size())
        itemIcon.replace(column, icon);
    else
        itemIcon.append(icon);
}

void s_ncitem::setaData(QString str)
{
    AData = str;
}

QColor s_ncitem::color(int column)
{
    if (column < itemColor.size())
        return itemColor.at(column);
    else
        return QColor();
}

QFont s_ncitem::font(int column)
{
    if (column < itemFont.size())
        return itemFont.at(column);
    else
        return QFont();
}

QIcon s_ncitem::icon(int column)
{
    if (column < itemIcon.size())
        return itemIcon.at(column);
    else
        return QIcon();
}

QString s_ncitem::aData()
{
    return AData;
}
