#include <QHeaderView>
#include <QVBoxLayout>
#include "goodbadwidget.h"

GoodBadWidget::GoodBadWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    tm = new GoodBadTableModel;
    tv = new TreeView;
    tv->horizontalHeader()->setVisible(false);
    tv->verticalHeader()->setVisible(false);
    tv->setModel(tm);
//    connect(tm,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),tv, SLOT(resizeColumnsToContents()));
//    tv->resizeColumnsToContents();
//    tv->resizeRowsToContents();
    QVBoxLayout *lyout = new QVBoxLayout;
    lyout->addWidget(tv);
    setLayout(lyout);
}

GoodBadWidget::~GoodBadWidget()
{
    delete tm;
}

bool GoodBadWidget::SetItemsCount(int NumItems)
{
    return tm->Setup(NumItems);
}

bool GoodBadWidget::SetItem(int ItemNum, int ItemType)
{
    return tm->SetItem(ItemNum, ItemType);
}

void GoodBadWidget::Resize()
{
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();
}

GoodBadTableModel::GoodBadTableModel(QObject *parent) : QAbstractTableModel(parent)
{
//    ClearModel();
    Items.clear();
    Icons[GBIT_GOOD] = QIcon(":/res/ans_good.gif");
    Icons[GBIT_BAD] = QIcon(":/res/ans_bad.gif");
    Icons[GBIT_NEUTRAL] = QIcon(":/res/ans_neutral.gif");
}

QVariant GoodBadTableModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if ((index.column() < Items.size()) && (role == Qt::DecorationRole))
        {
            int tmpi = Items.at(index.column());
            if (tmpi < 3)
                return Icons[Items.at(index.column())];
        }
    }
    return QVariant();
}

bool GoodBadTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ((index.isValid()) && (role == Qt::EditRole) && (index.column() < Items.size()))
    {
        Items.replace(index.column(), value.toInt()); // пишем само значение
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

Qt::ItemFlags GoodBadTableModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled;
}

bool GoodBadTableModel::insertColumns(int position, int columns, const QModelIndex &index)
{
    Q_UNUSED(index);
    if (position > Items.size())
        return false;
    beginInsertColumns(QModelIndex(), position, position+columns-1);
    for (int i = 0; i < columns; i++)
        Items.insert(position, GBIT_NEUTRAL);
    endInsertColumns();
    return true;
}

bool GoodBadTableModel::removeColumns(int position, int columns, const QModelIndex &index)
{
    Q_UNUSED(index);
    if ((position+columns) > Items.size())
        return false;
    for (int i = 0; i < columns; i++)
        Items.removeAt(position);
    return true;
}
/*
bool GoodBadTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row+count-1);
    if ((row+count) < 2)
        ++Rows;
    endInsertRows();
    return true;
}

bool GoodBadTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row+count-1);
    if ((row+count) <= Rows)
        --Rows;
    endRemoveRows();
    return true;
}
*/
int GoodBadTableModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return 1; // строк всегда адын штук
}

int GoodBadTableModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Items.size();
}

bool GoodBadTableModel::Setup(int NumItems)
{
//    ClearModel();
//    insertRows(0, 1, QModelIndex());
    return insertColumns(0, NumItems, QModelIndex());
}

bool GoodBadTableModel::SetItem(int ItemNum, int ItemType)
{
    if (ItemNum > Items.size())
        return false;
    return setData(index(0, ItemNum), QVariant(ItemType), Qt::EditRole);
}
/*
void GoodBadTableModel::ClearModel()
{
    Rows = 0;
    beginResetModel();
    while (rowCount() > 0)
        removeRows(0, 1, QModelIndex());
    Items.clear();
    endResetModel();
}
*/
