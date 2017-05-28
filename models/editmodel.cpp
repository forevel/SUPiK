#include "editmodel.h"
#include <QFile>
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"


// ######################################## Переопределение методов QAbStractTableModel ####################################

EditModel::EditModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    Colors[0] = Qt::black;
    Colors[1] = Qt::red;
    Colors[2] = Qt::blue;
    Colors[3] = Qt::darkRed;
    Colors[4] = Qt::darkGreen;
    Colors[5] = Qt::gray;
    QFont fontB = QFont("MS Sans Serif", -1, QFont::Bold);
    QFont fontN = QFont("MS Sans Serif", -1, QFont::Normal);
    Fonts[0] = Fonts[3] = Fonts[5] = fontN;
    Fonts[1] = Fonts[2] = Fonts[4] = fontB;
    Icons[0] = QIcon(":/res/hr.png");
    Icons[1] = QIcon(":/res/ok.png");
    Icons[2] = QIcon(":/res/cross.png");
    Hdr.clear();
    MainData.clear();
}

EditModel::~EditModel()
{
}

QVariant EditModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole || role == Qt::EditRole) && (section < Hdr.size()))
        return Hdr.at(section);
    return QVariant();
}

QVariant EditModel::data(const QModelIndex &index, int role) const
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
            else if (role == LinksRole)
                return MainData.at(index.row())->LinksData(index.column());
            else if (role == HeaderTextRole)
                return MainData.at(index.row())->HeaderData(index.column());
            else if (role == CellInfoRole)
                return MainData.at(index.row())->CellInfoData();
            else if (role == TableNumberRole)
                return MainData.at(index.row())->TableNumberData(index.column());
        }
    }
    return QVariant();
}

bool EditModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
        else if (role == LinksRole)
        {
            MainData.at(index.row())->SetLinksData(index.column(), value.toString()); // пишем значение вспомогательного поля
            return true;
        }
        else if (role == HeaderTextRole)
        {
            MainData.at(index.row())->SetHeaderData(index.column(), value.toString()); // пишем значение вспомогательного поля
            return true;
        }
        else if (role == CellInfoRole)
        {
            MainData.at(index.row())->SetCellInfoData(value.toString());
            return true;
        }
        else if (role == TableNumberRole)
        {
            MainData.at(index.row())->SetTableNumberData(index.column(), value.toString());
            return true;
        }
        else if (role == Qt::DecorationRole)
        {
            if (value.toString() == "1")
                MainData.at(index.row())->SetIcon(index.column(), QIcon(":/res/ok.png"));
            else
                MainData.at(index.row())->SetIcon(index.column(), QIcon(":/res/cross.png"));
            MainData.at(index.row())->SetData(index.column(), value.toString());
            return true;
        }
    }
    return false;
}

Qt::ItemFlags EditModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QModelIndex EditModel::index(int row, int Column, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return createIndex(row, Column);
}

bool EditModel::insertColumns(int position, int Columns, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertColumns(QModelIndex(), position, position+Columns-1);
    for (int i = 0; i < Columns; i++)
        Hdr.insert(position, "");
    endInsertColumns();
    return true;
}

bool EditModel::removeColumns(int position, int Columns, const QModelIndex &index)
{
    beginRemoveColumns(index, position, position + Columns - 1);
    if ((position+Columns) > Hdr.size())
        return false;
    for (int i = 0; i < Columns; i++)
        Hdr.removeAt(position);
    endRemoveColumns();
    return true;
}

bool EditModel::insertRows(int position, int rows, const QModelIndex &index)
{
    int i, j;
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);
    for (i = 0; i < rows; i++)
    {
        EditModelItem *Item = new EditModelItem;
        for (j = 0; j < Hdr.size(); j++)
        {
            Item->SetData(j, "");
            Item->SetLinksData(j, "");
        }
        MainData.insert(position, Item);
    }
    endInsertRows();
    return true;
}

bool EditModel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(index, position, position + rows - 1);
    if ((position+rows) > MainData.size())
        return false;
    for (int i = 0; i < rows; i++)
    {
        EditModelItem *Item = MainData.at(position);
        MainData.removeAt(position);
        delete Item;
    }
    endRemoveRows();
    return true;
}

int EditModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Hdr.size();
}

int EditModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return MainData.size();
}

// ###################################### Свои методы ############################################

void EditModel::AddColumn(const QString Hdrtext)
{
    int lastEntry = Hdr.size();
    insertColumns(lastEntry, 1, QModelIndex());
    Hdr.replace(lastEntry, Hdrtext);
}

QString EditModel::Value(int row, int column)
{
    QString tmps;
    tmps = data(index(row, column, QModelIndex()), Qt::DisplayRole).toString();
    QString tablenum = data(index(row,column,QModelIndex()),TableNumberRole).toString();
    if (tablenum != "") // если значение относится к полю типа DLINK, то добавляем в начало номер таблицы и спецсимвол
    {
        tmps.insert(0,tablenum);
        tmps.insert(0,'_');
    }
    QString Links = data(index(row,column,QModelIndex()),LinksRole).toString();
    PublicClass::FieldFormat ff;
    pc.getFFfromLinks(Links, ff);
    if (ff.ftype == FW_CRYPT)
    {
        tmps.insert(0, ".");
        tmps.insert(0, ff.link.last());
    }
    return tmps;
}

// процедура заполнения модели значениями headers и по table:tablefields из таблицы tble в sup.tablefields по одному id
// предназначена для диалога редактирования справочников (twocoldialog)

int EditModel::Setup(QString Table, QString Id)
{
    ClearModel();
    if (Table.isEmpty())
    {
        WARNMSG("");
        return RESULTBAD;
    }
    QStringList fl = QStringList() << "table" << "tablefields" << "header" << "links";
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByFields("sup", "tablefields", fl, QStringList("tablename"), QStringList(Table), "fieldsorder", true);
    if ((sqlc.result) || (vl.isEmpty()))
    {
        WARNMSG("");
        return RESULTBAD;
    }
    int i;
    int idpos = -1;
    bool IsAliasExist = false;
    QStringList TableHeadersSl;
    QStringList HeadersSl;
    QStringList TableLinksSl;
    QStringList tmpsl = vl.at(0).at(0).split(".");
    QString PlainDb, PlainTable;
    if (tmpsl.size()>1)
    {
        PlainDb = tmpsl.at(0);
        PlainTable = tmpsl.at(1);
    }
    else
    {
        WARNMSG("");
        return RESULTBAD;
    }
    // ищем ключевые поля - <tble>, alias и id<tble>
    for (i = 0; i < vl.size(); i++)
    {
        QStringList sl = vl.at(i);
        if (sl.size()<4)
            continue;
        HeadersSl.append(sl.at(2));
        if (sl.at(2) == "ИД")
            idpos = i;
        if (sl.at(1) == PlainTable)
            IsAliasExist = true;
        TableHeadersSl.append(sl.at(1));
        TableLinksSl.append(sl.at(3));
    }
    if (idpos == -1)
    {
        INFOMSG("Не найдено ключевое поле в таблице "+Table);
        return RESULTBAD;
    }
    if (!IsAliasExist) // в таблице нет поля с именем таблицы - плохо, должно быть
    {
        INFOMSG("Не найдено поле " + PlainTable + " в таблице "+Table);
        return RESULTBAD;
    }
    // устанавливаем столбцы модели
    AddColumn("Имя поля");
    AddColumn("Значение поля");

    // заполняем модель
    QStringList ValuesSl = sqlc.GetValuesFromTableByID(PlainDb, PlainTable, TableHeadersSl, Id);
    if (sqlc.result)
    {
        WARNMSG("");
        return RESULTBAD;
    }
    for (i = 0; i < HeadersSl.size(); i++)
    {
        if ((HeadersSl.at(i) == "Дата") || (HeadersSl.at(i) == "ИДПольз"))
            continue;
        QStringList ValuesToAdd;
        ValuesToAdd.append(HeadersSl.at(i));
        if ((i < ValuesSl.size() && (i < TableLinksSl.size())))
        {
            QString tmps;
            tfl.idtov(TableLinksSl.at(i), ValuesSl.at(i), tmps);
            ValuesToAdd.append(tmps);
            ValuesToAdd.append(TableLinksSl.at(i));
        }
        AddRow(ValuesToAdd);
    }
    return RESULTOK;
}

// процедура заполнения модели значениями полей из таблицы db:tble по одному id
// предназначена для диалога редактирования справочников (twocoldialog)

int EditModel::SetupRaw(QString Db, QString Tble, QString Id)
{
    ClearModel();
    if (Db.isEmpty() || Tble.isEmpty())
    {
        WARNMSG("");
        return RESULTBAD;
    }
    int i;
    // устанавливаем столбцы модели
    AddColumn("Имя поля");
    AddColumn("Значение поля");

    QStringList TableHeadersSl = sqlc.GetColumnsFromTable(Db, Tble);
    // заполняем модель
    QStringList ValuesSl = sqlc.GetValuesFromTableByID(Db, Tble, TableHeadersSl, Id);
    if (sqlc.result)
    {
        WARNMSG("");
        return RESULTBAD;
    }
    for (i = 0; i < TableHeadersSl.size(); ++i)
    {
        QStringList ValuesToAdd;
        ValuesToAdd.append(TableHeadersSl.at(i));
        if (i < ValuesSl.size())
            ValuesToAdd.append(ValuesSl.at(i));
        ValuesToAdd.append("1.8"); // links
        AddRow(ValuesToAdd);
    }
    return RESULTOK;
}

// sl[0] = field, sl[1] = value, sl[2] = links

void EditModel::AddRow(QStringList &sl)
{
    int LastIndex = rowCount();
    insertRows(LastIndex, 1);
    setData(index(LastIndex, FIELDCOLUMN, QModelIndex()), sl.at(0), Qt::EditRole);
    setData(index(LastIndex, VALUECOLUMN, QModelIndex()), sl.at(1), Qt::EditRole);
    setData(index(LastIndex, VALUECOLUMN, QModelIndex()), sl.at(2), EditModel::LinksRole);
}

QStringList EditModel::Values()
{
    QStringList sl;
    for (int i=0; i<rowCount(); i++)
        sl.append(Value(i, VALUECOLUMN)); // данные хранятся в 1-м столбце
    return sl;
}

QStringList EditModel::Links()
{
    QStringList sl;
    for (int i=0; i<rowCount(); i++)
        sl.append(data(index(i,VALUECOLUMN,QModelIndex()),LinksRole).toString()); // данные хранятся в 1-м столбце
    return sl;
}

QStringList EditModel::Headers()
{
    QStringList sl;
    for (int i=0; i<rowCount(); i++)
        sl.append(data(index(i, FIELDCOLUMN, QModelIndex()), Qt::DisplayRole).toString());
    return sl;
}

void EditModel::ClearModel()
{
    beginResetModel();
    while (rowCount() > 0)
        removeRows(0, 1);
    Hdr.clear();
    MainData.clear();
    endResetModel();
}

// ######################### EDITMODELITEM ##############################

EditModelItem::EditModelItem(EditModelItem *parent)
{
    Q_UNUSED(parent);
}

QString EditModelItem::Data(int Column) const
{
    if (Column < ItemData.size())
        return ItemData.at(Column);
    else
        return QString();
}

QString EditModelItem::LinksData(int Column) const
{
    if (Column < LData.size())
        return LData.at(Column);
    else
        return QString();
}

QString EditModelItem::HeaderData(int Column) const
{
    if (Column < HData.size())
        return HData.at(Column);
    else
        return QString();
}

void EditModelItem::SetData(int Column, const QString &Data)
{
    if (Column < ItemData.size())
        ItemData.replace(Column, Data);
    else
        ItemData.append(Data);
}

void EditModelItem::SetLinksData(int Column, const QString &Data)
{
    if (Column < LData.size())
        LData.replace(Column, Data);
    else
        LData.append(Data);
}

void EditModelItem::SetHeaderData(int Column, const QString &Data)
{
    if (Column < HData.size())
        HData.replace(Column, Data);
    else
        HData.append(Data);
}

// проверка элементов в определённых позициях на "пустость"

bool EditModelItem::IsEmpty(QList<int> ColumnsToCheck)
{
    for (int i = 0; i < ColumnsToCheck.size(); i++)
        if ((ItemData.at(ColumnsToCheck.at(i))) != "")
            return false;
    return true;
}

void EditModelItem::SetColor(int Column, QColor Color)
{
    if (Column < ItemColor.size())
        ItemColor.replace(Column, Color);
    else
        ItemColor.append(Color);
}

void EditModelItem::SetFont(int Column, QFont Font)
{
    if (Column < ItemFont.size())
        ItemFont.replace(Column, Font);
    else
        ItemFont.append(Font);
}

void EditModelItem::SetIcon(int Column, QIcon Icon)
{
    if (Column < ItemIcon.size())
        ItemIcon.replace(Column, Icon);
    else
        ItemIcon.append(Icon);
}

void EditModelItem::SetTableNumberData(int Column, QString Str)
{
    if (Column < TableNumbers.size())
        TableNumbers.replace(Column, Str);
    else
        TableNumbers.append(Str);
}

void EditModelItem::SetCellInfoData(QString Str)
{
    CellInfo = Str;
}

QColor EditModelItem::Color(int Column)
{
    if (Column < ItemColor.size())
        return ItemColor.at(Column);
    else
        return QColor();
}

QFont EditModelItem::Font(int Column)
{
    if (Column < ItemFont.size())
        return ItemFont.at(Column);
    else
        return QFont();
}

QIcon EditModelItem::Icon(int Column)
{
    if (Column < ItemIcon.size())
        return ItemIcon.at(Column);
    else
        return QIcon();
}

QString EditModelItem::CellInfoData()
{
    return CellInfo;
}

QString EditModelItem::TableNumberData(int Column)
{
    if (Column < TableNumbers.size())
        return TableNumbers.at(Column);
    else
        return QString();
}
