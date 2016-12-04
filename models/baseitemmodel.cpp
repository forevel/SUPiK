#include "baseitemmodel.h"

BaseItemModel::BaseItemModel()
{
    ClearModel();
    IconList.append(ICON_NO);
    IconList.append(ICON_YES);
}

BaseItemModel::~BaseItemModel()
{
    ClearModel();
}

QVariant BaseItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole || role == Qt::EditRole) && (section < Hdr.size()))
        return Hdr.at(section);
    return QVariant();
}

QVariant BaseItemModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if ((index.row() < MainData.size()) && (index.column() < Hdr.size()))
        {
            if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
                return MainData.at(index.row())->Data(index.column());
            else if (role == Qt::FontRole)
                return QVariant::fromValue(QFont(MainData.at(index.row())->Font(index.column())));
            else if (role == Qt::ForegroundRole)
                return QVariant::fromValue(QColor(MainData.at(index.row())->Color(index.column())));
            else if (role == Qt::DecorationRole)
                return QVariant::fromValue(QIcon(MainData.at(index.row())->Icon(index.column())));
            else if (role == Qt::ToolTipRole)
                return MainData.at(index.row())->ToolTip(index.column());
        }
    }
    return QVariant();
}

bool BaseItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        if (role == Qt::EditRole)
        {
            if (index.column() < Hdr.size())
            {
                MainData.at(index.row())->SetData(index.column(), value.toString()); // пишем само значение
                emit dataChanged(index,index);
                return true;
            }
        }
        else if (role == Qt::ForegroundRole)
        {
            MainData.at(index.row())->SetColor(index.column(), value.value<QColor>());
            return true;
        }
        else if (role == Qt::FontRole)
        {
            MainData.at(index.row())->SetFont(index.column(), value.value<QFont>());
            return true;
        }
        else if (role == Qt::DecorationRole)
        {
            MainData.at(index.row())->SetIcon(index.column(), QIcon(value.toString()));
            return true;
        }
        else if (role == Qt::ToolTipRole)
        {
            MainData.at(index.row())->SetToolTip(index.column(), value.toString());
            return true;
        }
    }
    return false;
}

Qt::ItemFlags BaseItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QModelIndex BaseItemModel::index(int row, int Column, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return createIndex(row, Column);
}

bool BaseItemModel::insertColumns(int position, int columns, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertColumns(QModelIndex(), position, position+columns-1);
    for (int i = 0; i < columns; i++)
        Hdr.insert(position, "");
    endInsertColumns();
    return true;
}

bool BaseItemModel::removeColumns(int position, int columns, const QModelIndex &index)
{
    beginRemoveColumns(index, position, position + columns - 1);
    if ((position+columns) > Hdr.size())
        return false;
    for (int i = 0; i < columns; i++)
        Hdr.removeAt(position);
    endRemoveColumns();
    return true;
}

bool BaseItemModel::insertRows(int position, int rows, const QModelIndex &index)
{
    int i, j;
    Q_UNUSED(index);
    if ((position > MainData.size()) || (position < 0))
        return false;
    beginInsertRows(QModelIndex(), position, position+rows-1);
    for (i = 0; i < rows; i++)
    {
        BaseItem *Item = new BaseItem(Hdr.size());
        MainData.insert(position, Item);
    }
    endInsertRows();
    return true;
}

bool BaseItemModel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(index, position, position + rows - 1);
    if ((position+rows) > MainData.size())
        return false;
    for (int i = 0; i < rows; i++)
    {
        BaseItem *Item = MainData.at(position);
        MainData.removeAt(position);
        delete Item;
    }
    endRemoveRows();
    return true;
}

int BaseItemModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Hdr.size();
}

int BaseItemModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return MainData.size();
}

void BaseItemModel::ClearModel()
{
    beginResetModel();
    while (rowCount() > 0)
        removeRows(0, 1);
    Hdr.clear();
    MainData.clear();
    endResetModel();
}

void BaseItemModel::AddColumn(const QString Hdrtext)
{
    int lastEntry = Hdr.size();
    insertColumns(lastEntry, 1, QModelIndex());
    Hdr.replace(lastEntry, Hdrtext);
}

int BaseItemModel::AddRow()
{
    int rcount = rowCount();
    insertRows(rcount, 1, QModelIndex());
    return rcount;
}

bool BaseItemModel::SetModelData(int row, int column, const QVariant &value, int role)
{
    if ((row < 0) || (row >= rowCount()) || (column < 0) || (column >= columnCount()))
        return false;
    return setData(index(row, column, QModelIndex()), value, role);
}
