#include "s_whitem.h"

s_whitem::s_whitem(QStringList data, s_whitem *parent)
{
    Q_UNUSED(parent);
    itemData = data;
}

s_whitem::~s_whitem()
{
}

QString s_whitem::data(int column) const
{
    return itemData.at(column);
}

bool s_whitem::setData(int column, const QString &data)
{
    if (column < itemData.size())
        itemData.replace(column, data);
    else
        itemData.append(data);
    return true;
}

bool s_whitem::isEmpty(QList<int> columnsToCheck)
{
    for (int i = 0; i < columnsToCheck.size(); i++)
        if ((itemData.at(columnsToCheck.at(i))) != "")
            return false;
    return true;
}
