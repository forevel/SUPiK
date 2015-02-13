#include "../inc/s_ncmodel.h"
#include "../inc/s_sql.h"

static const QMap<QString, int> OPER_MAP = s_ncmodel::opers();


// ######################################## Переопределение методов QAbstractTableModel ####################################

s_ncmodel::s_ncmodel(QObject *parent) :
    QAbstractTableModel(parent)
{
    colors[0] = Qt::black;
    colors[1] = Qt::red;
    colors[2] = Qt::blue;
    colors[3] = Qt::darkRed;
    colors[4] = Qt::darkGreen;
    colors[5] = Qt::gray;
    QFont fontB = QFont("MS Sans Serif", -1, QFont::Bold);
    QFont fontN = QFont("MS Sans Serif", -1, QFont::Normal);
    fonts[0] = fonts[3] = fonts[5] = fontN;
    fonts[1] = fonts[2] = fonts[4] = fontB;
    icons[0] = QIcon(":/pic/Pic/hr.png");
    icons[1] = QIcon(":/pic/Pic/ok.png");
    icons[2] = QIcon(":/pic/Pic/cross.png");
    hdr.clear();
    maindata.clear();
    fieldstoCheck.clear();
    maxcolswidth.clear();
}

s_ncmodel::~s_ncmodel()
{
}

QVariant s_ncmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole || role == Qt::EditRole) && (section < hdr.size()))
        return hdr.at(section);
    return QVariant();
}

bool s_ncmodel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (section < maxcolswidth.size())
    {
        if (maxcolswidth.at(section) < value.toString().size())
            maxcolswidth.replace(section, value.toString().size());
    }
    return QAbstractTableModel::setHeaderData(section, orientation, value, role);
}

QVariant s_ncmodel::data(const QModelIndex &index, int role) const
{
    QString tmpString;
    s_ncmodel::fieldformat ff;
    if (index.isValid())
    {
        if ((index.row() < maindata.size()) && (index.column() < hdr.size()))
        {
            if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
            {
                tmpString = maindata.at(index.row())->linksdata(index.column());
                ff = getFFfromLinks(tmpString);
                switch (ff.ftype)
                {
                case FW_AUTONUM:
                {
                    return QVariant(index.row()+1);
                    break;
                }
                case FW_NUMBER:
                {
                    return QVariant(ff.link.at(0));
                    break;
                }
                case FW_ID:
                {
                    int num = ff.link.at(0).toInt();
                    return QVariant(QString("%1").arg(maindata.at(index.row())->data(index.column()).toInt(), num, 10, QChar('0')));
                    break;
                }
                }

                if (ff.ftype == FW_EQUAT) // вычисляемое выражение
                {
                    int tmpInt = OPER_MAP[ff.link.at(0)];
                    bool byRow = (ff.link.at(1) == "r");
                    tmpString = getEq(ff.link.at(1), ff.link.at(2), tmpInt, index, byRow);
                    return tmpString;
                }
                else
                    return maindata.at(index.row())->data(index.column());
            }
            else if (role == Qt::FontRole)
                return QVariant::fromValue(QFont(maindata.at(index.row())->font(index.column())));
            else if (role == Qt::ForegroundRole)
                return QVariant::fromValue(QColor(maindata.at(index.row())->color(index.column())));
            else if (role == Qt::DecorationRole)
                return QVariant::fromValue(QIcon(maindata.at(index.row())->icon(index.column())));
            else if (role == Qt::UserRole)
                return maindata.at(index.row())->linksdata(index.column());
            else if (role == Qt::UserRole+1)
                return maindata.at(index.row())->aData();
        }
    }
    return QVariant();
}

// value должен представлять из себя запись вида: <value>.<links>, где links - вспомогательное поле, определяющее
// порядок работы с полем - подставляемый делегат, ссылку на списки и формат отображения

bool s_ncmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        if (role == Qt::EditRole)
        {
            if (index.column() < hdr.size())
            {
                maindata.at(index.row())->setData(index.column(), value.toString()); // пишем само значение
                if (index.column() < maxcolswidth.size())
                {
                    if (maxcolswidth.at(index.column()) < value.toString().size())
                        maxcolswidth.replace(index.column(), value.toString().size());
                }
                emit dataChanged(index, index);
                return true;
            }
        }
        if (role == Qt::UserRole)
        {
            if (index.column() < hdr.size())
            {
                maindata.at(index.row())->setLinksData(index.column(), value.toString()); // пишем значение вспомогательного поля
                return true;
            }
        }
        if (role == Qt::UserRole+1)
        {
            maindata.at(index.row())->setaData(value.toString());
            return true;
        }
    }
    return false;
}

Qt::ItemFlags s_ncmodel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QModelIndex s_ncmodel::index(int row, int column, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return createIndex(row, column);
}

bool s_ncmodel::insertColumns(int position, int columns, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertColumns(QModelIndex(), position, position+columns-1);
    if (!maindata.isEmpty()) // если в модели есть какие-то данные, то уже нельзя менять размерность таблицы по столбцам
        return false;
    for (int i = 0; i < columns; i++)
    {
        maxcolswidth.insert(position, 0);
        hdr.insert(position, "");
    }
    endInsertColumns();
    return true;
}

bool s_ncmodel::removeColumns(int position, int columns, const QModelIndex &index)
{
    beginRemoveColumns(index, position, position + columns - 1);
    if (!maindata.isEmpty())
        return false;
    if ((position+columns) > hdr.size())
        return false;
    for (int i = 0; i < columns; i++)
        hdr.removeAt(position);
    endRemoveColumns();
    return true;
}

bool s_ncmodel::insertRows(int position, int rows, const QModelIndex &index)
{
    int i, j;
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);
    for (i = 0; i < rows; i++)
    {
        s_ncitem *item = new s_ncitem();
        for (j = 0; j < hdr.size(); j++)
        {
            item->setData(j, "");
            item->setLinksData(j, "");
        }
        maindata.insert(position, item);
    }
    endInsertRows();
    return true;
}

bool s_ncmodel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(index, position, position + rows - 1);
    if ((position+rows) > maindata.size())
        return false;
    for (int i = 0; i < rows; i++)
    {
        s_ncitem *item = maindata.at(position);
        maindata.removeAt(position);
        delete item;
    }
    endRemoveRows();
    return true;
}

int s_ncmodel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return hdr.size();
}

int s_ncmodel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return maindata.size();
}

// ###################################### Свои методы ############################################

int s_ncmodel::getHeaderPosition(QVariant hdrtext, Qt::Orientation orientation, int role)
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole || role == Qt::EditRole))
        return hdr.indexOf(hdrtext.toString());
    return -1;
}

void s_ncmodel::addColumn(const QString hdrtext)
{
    int lastEntry = hdr.size();
    insertColumns(lastEntry, 1, QModelIndex());
    hdr.replace(lastEntry, hdrtext);
}

void s_ncmodel::addRow()
{
    int lastEntry = maindata.size();
    insertRows(lastEntry, 1, QModelIndex());
    for (int i = 0; i < hdr.size(); i++)
        setData(index(lastEntry, i, QModelIndex()), "", Qt::EditRole);
}

void s_ncmodel::setcolumnlinks(int column, QString links)
{
    for (int i = 0; i < rowCount(); i++)
        setData(index(i, column, QModelIndex()), links, Qt::UserRole);
}

void s_ncmodel::setcolumnlinks(int column, QStringList links)
{
    for (int i = 0; i < rowCount(); i++)
    {
        if (i < links.size())
            setData(index(i, column, QModelIndex()), links.at(i), Qt::UserRole);
        else
            setData(index(i, column, QModelIndex()), links.last(), Qt::UserRole);
    }
}

void s_ncmodel::setrowlinks(int row, QStringList links)
{
    for (int i = 0; i < columnCount(QModelIndex()); i++)
    {
        if (i < links.size())
            setData(index(row, i, QModelIndex()), links.at(i), Qt::UserRole);
        else
            setData(index(row, i, QModelIndex()), links.last(), Qt::UserRole);
    }
}

// подготовка модели для того, чтобы можно было писать в links до записи, собственно, данных

void s_ncmodel::prepareModel(QList<int> sl)
{
    int i;
    if (sl.size()>hdr.size()) // в переданном списке больше колонок, чем в модели
    {
        for (i = hdr.size(); i < sl.size(); i++)
            addColumn("");
    }
    int max = sl.at(0);
    for (i = 0; i < sl.size(); i++)
        if (sl.at(i) > max)
            max = sl.at(i);
    for (i = 0; i < max; i++)
    {
        if (i >= rowCount())
            addRow();
    }
}

void s_ncmodel::fillModel(QList<QStringList> sl)
{
    int i;
    int j;
    fieldformat ff;
    QString vl;
    if (sl.size()>hdr.size()) // в переданном списке больше колонок, чем в модели
    {
        for (i = hdr.size(); i < sl.size(); i++)
            addColumn("");
    }
    for (i = 0; i < sl.at(0).size(); i++)
    {
        if (i >= rowCount())
            addRow();
        for (j = 0; j < sl.size(); j++)
        {
            if (i > sl.at(j).size())
                setData(index(i, j, QModelIndex()), "", Qt::EditRole);
            else
            {
                ff = getFFfromLinks(data(index(i,j,QModelIndex()),Qt::UserRole).toString());
                switch (ff.ftype)
                {
                case FW_ALLINK:
                case FW_LINK:
                {
                    vl = tfl.GetOneValueByOneRowAndId(ff.link.at(0), ff.link.at(1), sl.at(j).at(i)).at(0);
                    break;
                }
                case FW_ID:
                {
                    int num = ff.link.at(0).toInt();
                    vl = QString("%1").arg(sl.at(j).at(i).toInt(), num, 10, QChar('0'));
                    break;
                }
                default:
                {
                    vl = sl.at(j).at(i);
                    break;
                }
                }
                setData(index(i, j, QModelIndex()), vl, Qt::EditRole);
            }
        }
    }
}

s_ncmodel::fieldformat s_ncmodel::getFFfromLinks(QString links) const
{
    fieldformat ff;
    ff.ftype = 8;
    ff.delegate = FD_SIMGRID;
    ff.dependson = -1;
    ff.link.clear();
    if (links.isEmpty())
        return ff;
    QStringList tmpsl = links.split(".");
    if (!tmpsl.size())
        return ff;
    ff.delegate = tmpsl.at(0).toInt();
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    ff.ftype = tmpsl.at(0).toInt();
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    bool ok;
    ff.dependson = tmpsl.at(0).toInt(&ok, 10);
    if (!ok)
        ff.dependson = -1;
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    for (int i = 0; i < tmpsl.size(); i++)
        ff.link << tmpsl.at(i);
    return ff;
}

QString s_ncmodel::getEq(QString arg1, QString arg2, int oper, const QModelIndex index, bool byRow) const
{
    float operand1 = getOperand(arg1, index, byRow);
    float operand2 = getOperand(arg2, index, byRow);
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

float s_ncmodel::getOperand(QString str, const QModelIndex index, bool byRow) const
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
                if (byRow)
                    res = (isCell)?(index.sibling(index.row(), tmpString.toInt(0, 10)).data(Qt::DisplayRole).toFloat()):(tmpString.toFloat());
                else
                    res = (isCell)?(index.sibling(tmpString.toInt(0, 10), index.column()).data(Qt::DisplayRole).toFloat()):(tmpString.toFloat());
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
    if (byRow)
        res = (isCell)?(index.sibling(index.row(), tmpString.toInt(0, 10)).data(Qt::DisplayRole).toFloat()):(tmpString.toFloat());
    else
        res = (isCell)?(index.sibling(tmpString.toInt(0, 10), index.column()).data(Qt::DisplayRole).toFloat()):(tmpString.toFloat());
    if (oper == '+') result += res;
    if (oper == '*') result *= res;
    if (oper == '/') result /= res;
    if (oper == '-') result -= res;

    return result;
}

bool s_ncmodel::checkforEmptyRows()
{
    for (int i = 0; i < maindata.size(); i++)
        if (maindata.at(i)->isEmpty(fieldstoCheck))
            return true;
    return false;
}

void s_ncmodel::setCellAttr(QModelIndex index, int fcset, int icon)
{
    if (icon != -1)
        maindata.at(index.row())->setIcon(index.column(), icons[icon]);
    maindata.at(index.row())->setColor(index.column(), colors[fcset]);
    maindata.at(index.row())->setFont(index.column(), fonts[fcset]);
}

// процедура выдаёт длину строки максимальную, среди строк столбца №column

int s_ncmodel::maxcolwidth(int column)
{
    if (column < maxcolswidth.size())
        return maxcolswidth.at(column);
    else
        return -1;
}

// процедура выдаёт размер массива длин строк

int s_ncmodel::maxcolwidthsize()
{
    return maxcolswidth.size();
}

// процедура выдаёт значение атрибута links у ячейки

QString s_ncmodel::getCellLinks(QModelIndex index)
{
    return data(index, Qt::UserRole).toString();
}

// процедура заполнения модели из таблицы tble в sup.tablefields

int s_ncmodel::setup(QString tble)
{
    int i;
    ClearModel();
    QList<QStringList> lsl = tfl.GetAllValues(tble);
    if (tfl.result)
        return (CM_ERROR+tfl.result);
    // в lsl.at(1) содержатся links, в lsl.at(0) - заголовки
    QStringList headers = lsl.at(0);
    QStringList links = lsl.at(1);
    lsl.removeAt(0);
    lsl.removeAt(0);
    for (i = 0; i < headers.size(); i++)
        addColumn(headers.at(i));
    QList<int> il;
    for (i = 0; i < lsl.size(); i++)
        il << lsl.at(i).size();
    prepareModel(il);
    for (i = 0; i < links.size(); i++)
        setcolumnlinks(i, links.at(i));
    fillModel(lsl);
    return 0;
}

void s_ncmodel::ClearModel()
{
    hdr.clear();
    maindata.clear();
    fieldstoCheck.clear();
    maxcolswidth.clear();
}
