#include "s_ncitem.h"

s_ncitem::s_ncitem(s_ncitem *parent)
{
    Q_UNUSED(parent);
}

QString s_ncitem::data(int column) const
{
    if (column < itemData.size())
        return itemData.at(column);
    else
        return QString();
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

void s_ncitem::setTData(int column, QString str)
{
    if (column < tDataList.size())
        tDataList.replace(column, str);
    else
        tDataList.append(str);
}

void s_ncitem::setAData(QString str)
{
    aData = str;
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

QString s_ncitem::AData()
{
    return aData;
}

QString s_ncitem::TData(int column)
{
    if (column < tDataList.size())
        return tDataList.at(column);
    else
        return QString();
}
