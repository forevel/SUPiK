#include "../inc/s_whitemmodel.h"
#include "../inc/s_sql.h"
#include <QVariant>

s_whitemmodel::s_whitemmodel(QObject *parent) :
    QAbstractTableModel(parent)
{
    hdr.clear();
    md.clear();
    columnsToCheck.clear();
}

s_whitemmodel::~s_whitemmodel()
{
}

int s_whitemmodel::columnCount(const QModelIndex &/*parent*/) const
{
    return hdr.size();
}

QVariant s_whitemmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole || role == Qt::EditRole) &&\
            (section < hdr.size()))
        return hdr.at(section);
    return QVariant();
}

int s_whitemmodel::getHeaderPosition(QVariant hdrtext, Qt::Orientation orientation, int role)
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole || role == Qt::EditRole))
        return hdr.indexOf(hdrtext.toString());
    return -1;
}

QVariant s_whitemmodel::data(const QModelIndex &index, int role) const
{
    QString tmpString;
    QStringList tmpStringList;
    if (index.isValid() && ((role == Qt::DisplayRole) || (role == Qt::EditRole)))
    {
        if ((index.row() < md.size()) && (index.column() < hdr.size()))
        {
            tmpStringList = links.at(index.column()).split(".");
            if (tmpStringList.at(0).toInt() == FW_EQUAT) // вычисляемое выражение
            {
                int tmpInt = OPER_MAP[tmpStringList.at(1)];
                tmpString = getEq(tmpStringList.at(2), tmpStringList.at(3), tmpInt, index);
                return tmpString;
            }
            else
                return md.at(index.row())->data(index.column());
        }
    }
    else if (role == Qt::UserRole)
    {
        if (index.column() < hdr.size())
            return links.at(index.column());
    }
    return QVariant();
}

bool s_whitemmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        if (index.column() < hdr.size())
        {
            bool result = md.at(index.row())->setData(index.column(), value.toString());
            if (result)
                emit dataChanged(index, index);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags s_whitemmodel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QModelIndex s_whitemmodel::index(int row, int column, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return createIndex(row, column);
}

bool s_whitemmodel::insertColumns(int position, int columns, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertColumns(QModelIndex(), position, position+columns-1);
    if (!md.isEmpty()) // если в модели есть какие-то данные, то уже нельзя менять размерность таблицы по столбцам
        return false;
    for (int i = 0; i < columns; i++)
        hdr.insert(position, "");
    endInsertColumns();
    return true;
}

bool s_whitemmodel::insertRows(int position, int rows, const QModelIndex &index)
{
    int i;
    Q_UNUSED(index);
    QStringList sl;
    beginInsertRows(QModelIndex(), position, position+rows-1);
    sl.clear();
    for (i = 0; i < hdr.size(); i++)
        sl << "";
    for (i = 0; i < rows; i++)
    {
        s_whitem *item = new s_whitem(sl);
        md.insert(position, item);
    }
    endInsertRows();
    return true;
}

bool s_whitemmodel::removeColumns(int position, int columns, const QModelIndex &index)
{
    beginRemoveColumns(index, position, position + columns - 1);
    if (!md.isEmpty())
        return false;
    if ((position+columns) > hdr.size())
        return false;
    for (int i = 0; i < columns; i++)
        hdr.removeAt(position);
    endRemoveColumns();
    return true;
}

bool s_whitemmodel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(index, position, position + rows - 1);
    if ((position+rows) > md.size())
        return false;
    for (int i = 0; i < rows; i++)
    {
        s_whitem *item = md.at(position);
        md.removeAt(position);
        delete item;
    }
    endRemoveRows();
    return true;
}

int s_whitemmodel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return md.size();
}

bool s_whitemmodel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}

bool s_whitemmodel::addColumn(const QString hdrtext, QString sqllink)
{
    hdr << hdrtext;
    links << sqllink;
    int tmpInt = sqllink.left(1).toInt();
    if ((tmpInt != FW_AUTONUM) && (tmpInt != FW_NUMBER) && (tmpInt != FW_EQUAT) && (tmpInt != FW_MAXLINK))
        columnsToCheck << links.size()-1; // если колонка не с автозаполнением, учитывать её "пустость" в соответствующей проверке
    return true;
}

bool s_whitemmodel::addRow(QStringList sl)
{

    QString tmpString;
    int lastEntry = md.size();
    insertRows(lastEntry, 1, QModelIndex());
    for (int i = 0; i < hdr.size(); i++)
    {
        if (i < sl.size())
        {
            QStringList tmpsl = links.at(i).split(".");
            int tmpInt = tmpsl.at(0).toInt();
            switch (tmpInt)
            {
            case FW_MAXLINK:
            {
                bool ok;
                int tmpInt2 = tmpsl.at(4).toInt(&ok);
                if (ok) // в поз. 4 находится число => ссылка на ячейку таблицы ордера
                {
                    tmpString = sqlc.getlastvaluefromtablebyfield(sqlc.getdb(tmpsl.at(1)), tmpsl.at(2), tmpsl.at(5), tmpsl.at(3),\
                                                              data(index(lastEntry, tmpInt2, QModelIndex()), Qt::DisplayRole).toString());
                    setData(index(lastEntry, i, QModelIndex()), tmpString, Qt::EditRole);
                }
                else
                {
                    tmpString = sqlc.getlastvaluefromtablebyfield(sqlc.getdb(tmpsl.at(1)), tmpsl.at(2), tmpsl.at(5), tmpsl.at(3), tmpsl.at(4));
                    setData(index(lastEntry, i, QModelIndex()), tmpString, Qt::EditRole);
                }
                break;
            }
            case FW_NUMBER:
            {
                setData(index(lastEntry, i, QModelIndex()), tmpsl.at(1), Qt::EditRole);
                break;
            }
            case FW_ALLINK:
            case FW_DLINK:
            case FW_LINK:
            case FW_MASKED:
            case FW_EQUAT:
            {
                setData(index(lastEntry, i, QModelIndex()), sl.at(i), Qt::EditRole);
                break;
            }
            case FW_AUTONUM:
            {
                setData(index(lastEntry, i, QModelIndex()), QString::number(lastEntry+1), Qt::EditRole);
                break;
            }
            default:
                break;
            }
        }
        else
            setData(index(lastEntry, i, QModelIndex()), "", Qt::EditRole);
    }
    return true;
}

QString s_whitemmodel::getEq(QString arg1, QString arg2, int oper, const QModelIndex index) const
{
    float operand1 = getOperand(arg1, index);
    float operand2 = getOperand(arg2, index);
    switch (oper)
    {
    case OP_PLUS:
        return QString::number(static_cast<float>(operand1+operand2), 'f', 2);
    case OP_MINUS:
        return QString::number(static_cast<float>(operand1-operand2), 'f', 2);
    case OP_MULT:
        return QString::number(static_cast<float>(operand1*operand2), 'f', 2);
    case OP_DIV:
        return QString::number(static_cast<float>(operand1/operand2), 'f', 2);
    default:
        return QString();
    }
}

float s_whitemmodel::getOperand(QString str, const QModelIndex index) const
{
    int i = 0;
    float res, result;
    int bStep = 0;
    bool isCell = false;
    QChar tmpChar, prevChar, oper;
    QString tmpString;
    int tmpInt;

    prevChar = 0;
    oper = 0;

    while (i < str.size())
    {
        tmpChar = str.at(i);
        switch (bStep)
        {
        case 0:
        {
            i++;
            if (tmpChar == 'c')
            {
                tmpString = "";
                isCell = true;
            }
            else if (tmpChar.isDigit())
            {
                isCell = false;
                tmpString = tmpChar;
            }
            bStep++;
            break;
        }
        case 1:
        {
            i++;
            if (tmpChar.isDigit())
                tmpString += tmpChar;
            else if ((tmpChar == "+") || (tmpChar == '*') || (tmpChar == '/') || (tmpChar == '-'))
            {
                tmpInt = index.row();
                res = (isCell)?(index.sibling(index.row(), tmpString.toInt(0, 10)).data(Qt::DisplayRole).toFloat()):(tmpString.toFloat());
                if (oper == 0)
                {
                    oper = tmpChar;
                    result = res;
                    bStep = 0;
                    break;
                }
                else
                {
                    if (oper == '+') result += res;
                    if (oper == '*') result *= res;
                    if (oper == '/') result /= res;
                    if (oper == '-') result -= res;
                    oper = tmpChar;
                    bStep = 0;
                    break;
                }
            }
            break;
        }
        default:
            break;
        }
    }
    res = (isCell)?(index.sibling(index.row(), tmpString.toInt(0, 10)).data(Qt::DisplayRole).toFloat()):(tmpString.toFloat());
    if (oper == '+') result += res;
    if (oper == '*') result *= res;
    if (oper == '/') result /= res;
    if (oper == '-') result -= res;

    return result;
}

bool s_whitemmodel::checkforEmptyRows()
{
    for (int i = 0; i < md.size(); i++)
        if (md.at(i)->isEmpty(columnsToCheck))
            return true;
    return false;
}
