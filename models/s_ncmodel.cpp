#include "s_ncmodel.h"
#include <QFile>
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"

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
    icons[0] = QIcon(":/res/hr.png");
    icons[1] = QIcon(":/res/ok.png");
    icons[2] = QIcon(":/res/cross.png");
    hdr.clear();
    maindata.clear();
    fieldstoCheck.clear();
    maxcolswidth.clear();
    isEditable = false;
    rcount = 0;
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
    if (index.isValid())
    {
        if ((index.row() < maindata.size()) && (index.column() < hdr.size()))
        {
            if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
                return maindata.at(index.row())->data(index.column());
            else if (role == Qt::FontRole)
                return QVariant::fromValue(QFont(maindata.at(index.row())->font(index.column())));
            else if (role == Qt::ForegroundRole)
                return QVariant::fromValue(QColor(maindata.at(index.row())->color(index.column())));
            else if (role == Qt::DecorationRole)
                return QVariant::fromValue(QIcon(maindata.at(index.row())->icon(index.column())));
            else if (role == Qt::UserRole)
                return maindata.at(index.row())->linksdata(index.column());
            else if (role == Qt::UserRole+1)
                return maindata.at(index.row())->AData();
            else if (role == Qt::UserRole+2)
                return maindata.at(index.row())->TData(index.column());
        }
    }
    return QVariant();
}

// value должен представлять из себя запись вида: <value>.<links>, где links - вспомогательное поле, определяющее
// порядок работы с полем - подставляемый делегат, ссылку на списки и формат отображения

bool s_ncmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QString tmpString;
    PublicClass::fieldformat ff;
    QVariant vl;
    if (index.isValid())
    {
        if (role == Qt::EditRole)
        {
            if (index.column() < hdr.size())
            {
                tmpString = maindata.at(index.row())->linksdata(index.column());
                vl = value;
                ff = pc.getFFfromLinks(tmpString);
                switch (ff.ftype)
                {
                case FW_AUTONUM:
                {
                    vl = QVariant(index.row()+1);
                    break;
                }
                case FW_NUMBER:
                {
                    vl = QVariant(ff.link.at(0));
                    break;
                }
                case FW_ID:
                {
                    int num = ff.link.at(0).toInt();
                    vl = QVariant(QString("%1").arg(vl.toInt(), num, 10, QChar('0')));
                    break;
                }
                case FW_EQUAT: // вычисляемое выражение
                {
                    int tmpInt = OPER_MAP[ff.link.at(0)];
                    bool byRow = (ff.link.at(1) == "r");
                    tmpString = getEq(ff.link.at(1), ff.link.at(2), tmpInt, index, byRow);
                    vl = QVariant(tmpString);
                    break;
                }
                default:
                    break;
                }
                maindata.at(index.row())->setData(index.column(), vl.toString()); // пишем само значение
                if (index.column() < maxcolswidth.size())
                {
                    if (maxcolswidth.at(index.column()) < vl.toString().size())
                        maxcolswidth.replace(index.column(), vl.toString().size());
                }
                emit dataChanged(index,index);
                return true;
            }
        }
        else if (role == Qt::ForegroundRole)
        {
            maindata.at(index.row())->setColor(index.column(), value.value<QColor>());
            return true;
        }
        else if (role == Qt::FontRole)
        {
            maindata.at(index.row())->setFont(index.column(), value.value<QFont>());
            return true;
        }
        else if (role == Qt::UserRole)
        {
            if (index.column() < hdr.size())
            {
                maindata.at(index.row())->setLinksData(index.column(), value.toString()); // пишем значение вспомогательного поля
                return true;
            }
        }
        else if (role == Qt::UserRole+1)
        {
            maindata.at(index.row())->setAData(value.toString());
            return true;
        }
        else if (role == Qt::UserRole+2)
        {
            maindata.at(index.row())->setTData(index.column(), value.toString());
            return true;
        }
    }
    return false;
}

Qt::ItemFlags s_ncmodel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;
    if (isEditable)
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable; // | Qt::ItemIsEditable;
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
    if (rcount)
        rcount--;
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

void s_ncmodel::AddColumns(QStringList hdrs)
{
    while (!hdrs.isEmpty())
    {
        addColumn(hdrs.at(0));
        hdrs.removeFirst();
    }
}

void s_ncmodel::addRow()
{
    int lastEntry = maindata.size();
    insertRows(lastEntry, 1, QModelIndex());
}

void s_ncmodel::setcolumnlinks(int column, QString links)
{
    for (int i = 0; i < rowCount(); i++)
        setData(index(i, column, QModelIndex()), QVariant(links), Qt::UserRole);
}

void s_ncmodel::setcolumnlinks(int column, QStringList links)
{
    for (int i = 0; i < rowCount(); i++)
    {
        if (i < links.size())
            setData(index(i, column, QModelIndex()), QVariant(links.at(i)), Qt::UserRole);
        else
            setData(index(i, column, QModelIndex()), QVariant(links.last()), Qt::UserRole);
    }
}

void s_ncmodel::setrowlinks(int row, QStringList links)
{
    for (int i = 0; i < columnCount(QModelIndex()); i++)
    {
        if (i < links.size())
            setData(index(row, i, QModelIndex()), QVariant(links.at(i)), Qt::UserRole);
        else
            setData(index(row, i, QModelIndex()), QVariant(links.last()), Qt::UserRole);
    }
}

// подготовка модели для того, чтобы можно было писать в links до записи, собственно, данных
// в sl передаётся список размеров столбцов

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

void s_ncmodel::setDataToWrite(QList<QStringList> sl)
{
    DataToWrite = sl;
}

void s_ncmodel::fillModel()
{
    int i;
    int j;
    QString vl;
    result = 0;
    if (DataToWrite.size()>hdr.size()) // в переданном списке больше колонок, чем в модели
    {
        for (i = hdr.size(); i < DataToWrite.size(); i++)
            addColumn("");
    }
    for (i = 0; i < DataToWrite.at(0).size(); i++)  // цикл по строкам
    {
        if (rcount >= rowCount())
            addRow();
        for (j = 0; j < DataToWrite.size(); j++) // цикл по столбцам
        {
            if (i > DataToWrite.at(j).size()) // если строк в текущем столбце меньше, чем текущий номер строки, пишем пустое значение
                setData(index(rcount, j, QModelIndex()), QVariant(""), Qt::EditRole);
            else
            {
                // значения в DataToWrite уже подготовлены в процедурах setup
                vl = DataToWrite.at(j).at(i);
                if (vl.isEmpty())
                    continue;
                if (vl.at(0) == '_') // идентификатор составного значения - номер таблицы и само значение
                {
                    setData(index(rcount,j,QModelIndex()),QVariant(vl.at(1)),Qt::UserRole+2); // пишем доп. информацию о номере таблицы для ячейки
                    vl = vl.right(vl.size()-2); // убираем номер таблицы и идентификатор
                }
                setData(index(rcount, j, QModelIndex()), QVariant(vl), Qt::EditRole);
            }
        }
        rcount++;
    }
}

// выдать значения по столбцу column в выходной QStringList

QStringList s_ncmodel::cvalues(int column)
{
    if (column > columnCount())
        return QStringList();
    result = 0;
    QStringList tmpsl;
    for (int row=0; row < rowCount(); row++)
        tmpsl.append(value(row,column));
    return tmpsl;
}

// выдать значения по строке row в выходной QStringList

QStringList s_ncmodel::rvalues(int row)
{
    if (row > rowCount())
        return QStringList();
    result = 0;
    QStringList tmpsl;
    for (int column = 0; column<columnCount(); column++)
        tmpsl.append(value(row,column));
    return tmpsl;
}

QString s_ncmodel::value(int row, int column)
{
    QString vl = data(index(row, column, QModelIndex()), Qt::DisplayRole).toString();
    QString links = data(index(row,column,QModelIndex()),Qt::UserRole).toString();
    QString tablenum = data(index(row,column,QModelIndex()),Qt::UserRole+2).toString();
    if (tablenum != "") // если значение относится к полю типа DLINK, то добавляем в начало номер таблицы и спецсимвол
    {
        vl.insert(0,tablenum);
        vl.insert(0,'_');
    }
    vl = tfl.vtoid(links, vl);
    if (tfl.result)
    {
        result=1;
        WARNMSG(PublicClass::ER_NCMODEL,__LINE__);
        return QString(); // если произошла ошибка при получении ИД по значению, добавляем пустую строку
    }
    return vl;
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

QString s_ncmodel::getCellType(int row, int column)
{
    return data(index(row,column,QModelIndex()), Qt::UserRole).toString().split(".").at(0);
}

// процедура заполнения модели из таблицы tble в sup.tablefields

void s_ncmodel::setup(QString tble)
{
    int i;
    result = 0;
    DataToWrite.clear();
    ClearModel();
    QStringList headers, links;
    DataToWrite = tfl.tbvll(tble);
    if (tfl.result)
    {
        result=1;
        WARNMSG(PublicClass::ER_NCMODEL,__LINE__);
        return;
    }
    // в DataToWrite.at(1) содержатся links, в DataToWrite.at(0) - заголовки
    headers = DataToWrite.at(0);
    links = DataToWrite.at(1);
    DataToWrite.removeAt(0);
    DataToWrite.removeAt(0);
    for (i = 0; i < headers.size(); i++)
        addColumn(headers.at(i));
    QList<int> il;
    for (i = 0; i < DataToWrite.size(); i++)
        il << DataToWrite.at(i).size();
    prepareModel(il);
    for (i = 0; i < links.size(); i++)
        setcolumnlinks(i, links.at(i));
    fillModel();
    return;
}

// процедура заполнения модели значениями headers и по table:tablefields из таблицы tble в sup.tablefields по одному id
// предназначена для диалога редактирования справочников (s_2cdialog)

void s_ncmodel::setup(QString tble, QString id)
{
    int i;
    ClearModel();
    DataToWrite.clear();
    QStringList headers = tfl.tableheaders(tble);
    if (tfl.result)
    {
        result=1;
        WARNMSG(PublicClass::ER_NCMODEL,__LINE__);
        return;
    }
    QStringList links = tfl.tablelinks(tble);
    if (tfl.result)
    {
        result=1;
        WARNMSG(PublicClass::ER_NCMODEL,__LINE__);
        return;
    }
    QString tmpString;
    DataToWrite.append(headers);
    QStringList tmpsl;
    for (i = 0; i < headers.size(); i++)
    {
        tmpString = tfl.tov(tble,headers.at(i),id);
        tmpString = tfl.idtov(links.at(i), tmpString);
        if (!tfl.result)
            tmpsl << tmpString;
        else
            tmpsl << "";
    }
    DataToWrite.append(tmpsl);
    addColumn("");
    addColumn("");
    QList<int> il;
    il << headers.size() << headers.size();
    prepareModel(il);
    setcolumnlinks(0, "7.8");
    setcolumnlinks(1, links);
    fillModel();
}

void s_ncmodel::setupcolumn(QString tble, QString header)
{
    ClearModel();
    DataToWrite.clear();
    QStringList tmpsl = tfl.htovl(tble, header);
    if (tfl.result)
    {
        WARNMSG(PublicClass::ER_NCMODEL,__LINE__);
        return;
    }
    DataToWrite.append(tmpsl);
    addColumn("");
    QList<int> il;
    il << tmpsl.size();
    prepareModel(il);
    setcolumnlinks(0, "7.8");
    fillModel();
}

int s_ncmodel::setupraw(QString db, QString tble, QStringList fl, QString orderfield)
{
    result = 0;
    ClearModel();
    DataToWrite.clear();
    QList<int> il;
    if (fl.isEmpty())
    {
        fl = sqlc.GetColumnsFromTable(sqlc.GetDB(db), tble);
        if (sqlc.result)
        {
            WARNMSG(PublicClass::ER_NCMODEL,__LINE__);
            return 1;
        }
    }
    for (int i = 0; i < fl.size(); i++)
    {
        QStringList tmpsl = sqlc.GetValuesFromTableByColumn(sqlc.GetDB(db), tble, fl.at(i),orderfield,true);
        if (sqlc.result)
        {
            WARNMSG(PublicClass::ER_NCMODEL,__LINE__);
            return 1;
        }
        DataToWrite.append(tmpsl);
        addColumn(fl.at(i));
        il << tmpsl.size();
    }
    prepareModel(il);
    for (int i=0; i<fl.size(); i++)
    {
        if (hdr.at(i) == "id")
            setcolumnlinks(i, "4.19..7");
        else
            setcolumnlinks(i, "7.8");
    }
    fillModel();
    return 0;
}

void s_ncmodel::ClearModel()
{
    beginResetModel();
    while (rowCount() > 0)
        removeRows(0, 1);
    hdr.clear();
    maindata.clear();
    fieldstoCheck.clear();
    maxcolswidth.clear();
    isEditable = false;
    rcount = 0;
    endResetModel();
}

QModelIndexList s_ncmodel::match(QString text)
{
    QModelIndexList mdlidxl;
    for (int i=0; i<maindata.size(); i++)
    {
        if (maindata.at(i)->data(0).contains(text))
            mdlidxl.append(index(i,0,QModelIndex()));
    }
    return mdlidxl;
}
