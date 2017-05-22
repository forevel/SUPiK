#include "treemodel.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QMap>
#include <QSortFilterProxyModel>

TreeModel::TreeModel(QObject *parent) : QAbstractTableModel(parent)
{
    // color and font sets:
    // 0 = обычный текст (чёрный, нежирный)
    // 1 = тревога (красный, жирный)
    // 2 = средний уровень (синий, жирный)
    // 3 = обычный текст с выделением (тёмно-красный, нежирный)
    // 4 = слабый уровень (зелёный, жирный)
    // 5 = неактивный (серый, нежирный)
    // 6 = совсем слабый уровень (зелёный, нежирный)

    Colors[0] = Qt::black;
    Colors[1] = Qt::red;
    Colors[2] = Qt::blue;
    Colors[3] = Qt::darkRed;
    Colors[4] = Qt::darkGreen;
    Colors[5] = Qt::gray;
    Colors[6] = Qt::darkGreen;
    QFont FontB = QFont("MS Sans Serif", -1, QFont::Bold);
    QFont FontN = QFont("MS Sans Serif", -1, QFont::Normal);
    Fonts[0] = Fonts[3] = Fonts[5] = Fonts[6] = FontN;
    Fonts[1] = Fonts[2] = Fonts[4] = FontB;
    Icons[0] = QIcon(":/res/hr.png");
    Icons[1] = QIcon(":/res/ok.png");
    Icons[2] = QIcon(":/res/cross.png");
    Icons[3] = QIcon(":/res/book-angle.png");
    Icons[4] = QIcon(":/res/book-open.png");
//    RootIDs.push("0");
    TreeType = TT_SIMPLE;
}

TreeModel::~TreeModel()
{
    ClearModel();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return hdr.size();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if ((index.row() < maindata.size()) && (index.column() < hdr.size()))
        {
            if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
                return maindata.at(index.row())->Data(index.column());
            else if (role == Qt::FontRole)
                return QVariant::fromValue(QFont(maindata.at(index.row())->Font(index.column())));
            else if (role == Qt::ForegroundRole)
                return QVariant::fromValue(QColor(maindata.at(index.row())->Color(index.column())));
            else if (role == Qt::DecorationRole)
                return QVariant::fromValue(QIcon(maindata.at(index.row())->Icon(index.column())));
            else if (role == CellInfoRole)
                return maindata.at(index.row())->Info(index.column());
            else if (role == LinksRole)
                return maindata.at(index.row())->Links(index.column());
            else if (role == HeaderTextRole)
                return maindata.at(index.row())->Header(index.column());
            else if (role == TableNumberRole)
                return maindata.at(index.row())->TableNumber(index.column());
        }
    }
    return QVariant();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        if (role == Qt::EditRole)
        {
            maindata.at(index.row())->SetData(index.column(), value.toString()); // пишем само значение
            emit dataChanged(index,index);
            return true;
        }
        else if (role == Qt::ForegroundRole)
        {
            maindata.at(index.row())->SetColor(index.column(), value.value<QColor>());
            return true;
        }
        else if (role == Qt::FontRole)
        {
            maindata.at(index.row())->SetFont(index.column(), value.value<QFont>());
            return true;
        }
        else if (role == Qt::DecorationRole)
        {
            maindata.at(index.row())->SetIcon(index.column(), value.value<QIcon>());
            return true;
        }
        else if (role == CellInfoRole)
        {
            maindata.at(index.row())->SetInfo(index.column(), value.toString());
            return true;
        }
        else if (role == HeaderTextRole)
        {
            maindata.at(index.row())->SetHeader(index.column(), value.toString());
            return true;
        }
        else if (role == LinksRole)
        {
            maindata.at(index.row())->SetLinks(index.column(), value.toString());
            return true;
        }
        else if (role == TableNumberRole)
        {
            maindata.at(index.row())->SetTableNumber(index.column(), value.toString());
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole || role == Qt::EditRole) && (section < hdr.size()))
        return hdr.at(section);
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return createIndex(row, column);
}

bool TreeModel::insertColumns(int column, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertColumns(QModelIndex(), column, column+count-1);
    for (int i = 0; i < count; i++)
        hdr.insert(column, "");
    endInsertColumns();
    return true;
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    int i, j;
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row+count-1);
    for (i = 0; i < count; i++)
    {
        TreeItem *Item = new TreeItem;
        for (j = 0; j < hdr.size(); j++)
        {
            Item->SetData(j, "");
        }
        maindata.insert(row, Item);
    }
    endInsertRows();
    return true;
}

bool TreeModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    if (!maindata.isEmpty())
        return false;
    if ((column+count) > hdr.size())
        return false;
    for (int i = 0; i < count; i++)
        hdr.removeAt(column);
    endRemoveColumns();
    return true;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    if ((row+count) > maindata.size())
        return false;
    for (int i = 0; i < count; i++)
    {
        TreeItem *Item = maindata.at(row);
        maindata.removeAt(row);
        delete Item;
    }
    endRemoveRows();
    return true;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return maindata.size();
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if ((orientation == Qt::Horizontal) && (section < hdr.size()) && (role == Qt::EditRole))
    {
        hdr.replace(section, value.toString());
        return true;
    }
    return false;
}

// свои методы

// слот, вызываемый по раскрытию дерева

void TreeModel::GoIntoIndex(QModelIndex idx)
{
    // 1. вытащить ИД кликнутого элемента
    // 2. добавить в стек ИД элемента
    // 3. очистить модель
    // 4. построить новую модель от этого элемента

    // 0
    int ClickedRow = idx.row()+1;
    if ((ClickedRow < RootIDs.size()) && (RootIDs.size() != 1)) // кликнули по первой строке, значит, надо свернуть на один уровень
    {
        // 2
        while (ClickedRow < RootIDs.size())
            RootIDs.pop();
        // 3
        ClearOnlyData();
        // 4
        if (BuildTree())
            WARNMSG("");
        return;
    }
    else
    {
        if (data(index(idx.row(),0,QModelIndex()),CellInfoRole) == TM_SIMPLE_ELEMENT) // если простой элемент
            return;
        // 1
        QString IndexID = data(index(idx.row(),0,QModelIndex()),Qt::DisplayRole).toString(); // ИД должен быть в нулевом столбце
        // 2
        IndexID.remove(QChar(0xFFFF));
        RootIDs.push(IndexID);
        // 3
        ClearOnlyData();
        // 4
        if (BuildTree())
            WARNMSG("");
    }
}

// проверка наличия потомков у элемента

bool TreeModel::HaveChildren(int row)
{
    QString AData = data(index(row, 0, QModelIndex()), CellInfoRole).toString();
    return (AData == TM_ELEMENT_WITH_CHILDREN);
}

// установка цвета, шрифта и иконки у последнедобавленного элемента

void TreeModel::SetLastItem(QColor FColor, QColor BColor, QFont Font, QIcon Icon, QString AData)
{
    if (rowCount() == 0)
        return;
    int LastIndex = rowCount()-1;
    for (int i=0; i<columnCount(); i++)
    {
        setData(index(LastIndex, i, QModelIndex()), FColor, Qt::ForegroundRole);
        setData(index(LastIndex, i, QModelIndex()), BColor, Qt::BackgroundRole);
        setData(index(LastIndex, i, QModelIndex()), Font, Qt::FontRole);
    }
    setData(index(LastIndex, 0, QModelIndex()), Icon, Qt::DecorationRole);
    setData(index(LastIndex, 0, QModelIndex()), AData, CellInfoRole);
}

int TreeModel::SetupFile(QString Filename, QString StringToFind)
{
    ClearModel();
    insertColumns(0,1,QModelIndex());
    TreeType = TT_SIMPLE;
    QString tmpString;
    char *tmpChar;
    int filepos = 0;
    QFile file;
    file.setFileName(Filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        WARNMSG("Невозможно открыть файл "+Filename);
        return TM_BADSOURCE;
    }
    QByteArray ba = file.readAll();
    while ((filepos = ba.indexOf(StringToFind, filepos)) != -1)
    {
        tmpChar = ba.data();
        tmpChar += filepos;
        while (*tmpChar)
        {
            if (tmpChar[0] == '=')
            {
                tmpChar++;
                filepos++;
                tmpString = "";
                while ((tmpChar[0]) && (tmpChar[0] != '|'))
                {
                    tmpString += QString::fromLocal8Bit(tmpChar,1);
                    tmpChar++;
                    filepos++;
                }
                break;
            }
            tmpChar++;
            filepos++;
        }
        QList<PublicClass::ValueStruct> vl;
        PublicClass::ValueStruct tmpvl;
        tmpvl.Type = VS_STRING;
        tmpvl.Value = tmpString;
        vl.append(tmpvl);
        AddItemToTree(vl);
        SetLastItem(Colors[0], Qt::transparent, Fonts[0], Icons[0], TM_SIMPLE_ELEMENT);
    }
    file.close();
    return TM_OK;
}

// процедура инициализации модели данными из таблицы table и построение дерева по полям <tble> и idalias
// cond = true => проверять права доступа при выдаче из БД

int TreeModel::Setup(QString Table, bool Cond)
{
    QStringList sl = QStringList() << Table;
    return Setup(sl, TT_SIMPLE, Cond);
}

int TreeModel::Setup(QStringList Tables, int Type, bool Cond)
{
    IsConditional = Cond;
    TreeType = Type;
    ClearModel();

    // 1. взять столбцы tablefields из tablefields, где tablename=table
    // 2. найти среди них столбцы <db>.<tble>.<tble> и <db>.<tble>.idalias. Если нет - это не дерево, выход
    // 3. взять значения столбцов <tble> и idalias из таблицы <db>.<tble>
    // 4. построить по ним дерево

    // 1
    if (Tables.isEmpty())
    {
        WARNMSG("Передан пустой список таблиц");
        return TM_BADRESULT;
    }
    int i;
    for (i=0; i<Tables.size(); i++)
    {
        if (PrepareTable(Tables.at(i)))
            return TM_BADRESULT;
    }
    if (BuildTree()) // элементы записываются в виде: <номер_таблицы>.<ИД>
    {
        WARNMSG("");
        return TM_BADRESULT;
    }
    return TM_OK;
}

//

/*int TreeModel::SetupRawComp(QString db, QString tble)
{
    return SetupRaw(db, tble, "id", "PartNumber");
}*/

// инициализация таблицы по реальным именам в БД

int TreeModel::SetupRaw(QString db, QString tble, QStringList fields)
{
    RightsFieldNum = -1;
    IsConditional = false;
    if (fields.isEmpty())
        fields = sqlc.GetColumnsFromTable(db, tble);
    int i;
    int ididx = fields.indexOf("id"+tble);
    if (ididx == -1)
        ididx = fields.indexOf("id");
    if (fields.isEmpty())
        return TM_BADSOURCE;
    QStringList TableHeadersSl = QStringList() << fields;
    QStringList TableLinksSl;
    for (i=0; i<TableHeadersSl.size(); ++i)
    {
        if (i == ididx)
            TableLinksSl.append("4.19..7");
        else
            TableLinksSl.append("1.8");
    }
    ClearModel();
    IsRaw = true;
    TableHeaders.append(TableHeadersSl);
    TableLinks.append(TableLinksSl);
    TableIsTree.append(false);
    DBs.append(db);
    Tables.append(tble);
    TablesNum++; // количество таблиц увеличиваем
    TreeType = TT_SIMPLE;
    for (i = columnCount(); i < TableHeadersSl.size(); i++) // добавим нехватающее количество столбцов в общую модель
        insertColumns(i,1,QModelIndex());
    if (BuildTree()) // элементы записываются в виде: <номер_таблицы>.<ИД>
    {
        WARNMSG("");
        return TM_BADRESULT;
    }
    return TM_OK;
}

int TreeModel::PrepareTable(QString Table)
{
    RightsFieldNum = -1;
    if (Table.isEmpty())
    {
        DBGMSG;
        return TM_BADRESULT;
    }
    QStringList fl = QStringList() << "table" << "tablefields" << "header" << "links";
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByFields("sup", "tablefields", fl, QStringList("tablename"), QStringList(Table), "fieldsorder", true);
    if ((sqlc.result) || (vl.size() == 0))
    {
        WARNMSG(sqlc.LastError);
        return TM_BADRESULT;
    }
    int i;
    int idpos = -1;
    bool IsIdAliasExist = false, IsAliasExist = false;
    QStringList TableHeadersSl;
    QStringList TableLinksSl;

    QStringList tmpsl = vl.at(0).at(0).split(".");
    QString PlainTable;
    if (tmpsl.size()>1)
        PlainTable = tmpsl.at(1);
    // ищем ключевые поля - tble, <tble> и id<tble>, а также "Права доступа"
    for (i = 0; i < vl.size(); i++)
    {
        QStringList sl = vl.at(i);
        if (sl.size()<4)
            continue;
        if (sl.at(1) == "idalias")
        {
            IsIdAliasExist = true;
            continue;
        }
        if (sl.at(2) == "ИД")
        {
            idpos = i;
            continue;
        }
        if (sl.at(2) == "Права доступа")
            RightsFieldNum = i;
        if (sl.at(1) == PlainTable)
            IsAliasExist = true;
        TableHeadersSl.append(sl.at(1));
        TableLinksSl.append(sl.at(3));
    }
    if (idpos == -1)
    {
        INFOMSG("Не найдено поле ИД в таблице "+Table);
        return TM_BADRESULT;
    }
    if (!IsIdAliasExist)
    {
        if (TreeType == TT_SIMPLE)
            TreeType = TT_TABLE;
        TableIsTree.append(false);
    }
    else if (!IsAliasExist) // если есть idalias, но нет <tble> - это не дело
    {
        INFOMSG("Не найдено поле " + PlainTable + " в таблице "+Table);
        return TM_BADRESULT;
    }
    else
        TableIsTree.append(true);

    TableHeaders.append(TableHeadersSl); // список полей, которые необходимо отображать в дереве
    TableLinks.append(TableLinksSl);
    int ColumnNeeded = TableHeadersSl.size()+1; // +1 на ИД
    if (columnCount()<ColumnNeeded) // если число столбцов в дереве меньше числа полей в очередной таблице
    {
        for (i = columnCount(); i < ColumnNeeded; i++) // добавим нехватающее количество столбцов в общую модель
            insertColumns(i,1,QModelIndex());
    }

    // обновляем списки заголовков, БД и таблиц
    tmpsl = vl.at(idpos).at(0).split("."); // tmpsl - таблица в виде db.tble
    if (tmpsl.size()<2)
    {
        DBGMSG;
        return TM_BADRESULT;
    }
    DBs.append(tmpsl.at(0));
    Tables.append(tmpsl.at(1));
    TablesNum++; // количество таблиц увеличиваем
    return TM_OK;
}

// процедура построения дерева по данному ИД в данной таблице. TableDotId в виде <Table>.<ID>

int TreeModel::BuildTree()
{
    QStringList TableId = RootIDs.top().split(".");
    if (TableId.size()<2)
    {
        DBGMSG;
        return TM_BADSOURCE;
    }
    int Table = TableId.at(0).toInt();
    QString Id = QString::number(TableId.at(1).toInt()); // убираем лишние нули
    if (SetFirstTreeElements()) // нарисовать первый элемент его самого с открытой книгой
    {
        WARNMSG("");
        return TM_BADRESULT;
    }
    if (TableIsTree.at(Table))
    {
        if (SetTree(Table, Id))
        {
            WARNMSG("");
            return TM_BADRESULT;
        }
    }
    else
    {
        if (SetTable(Table, Id))
        {
            WARNMSG("");
            return TM_BADRESULT;
        }
    }
    // построили по текущему уровню и текущей таблице, надо проверить следующую по списку таблицу на наличие элементов, связанных с текущим Id
    if (RootIDs.top() != "0.0")
        Table++; // переходим на новый уровень дерева
    else
        return TM_OK;
    if (Table >= TablesNum) // больше нет таблиц
        return TM_OK; // выход
    if (TableIsTree.at(Table))
    {
        if (SetNextTree(Table, Id))
        {
            WARNMSG("");
            return TM_BADRESULT;
        }
    }
    else
    {
        if (SetNextTable(Table, Id))
        {
            WARNMSG("");
            return TM_BADRESULT;
        }
    }
    return TM_OK;
}

// установка первого элемента в виде раскрытой книги

int TreeModel::SetFirstTreeElements()
{
    Indentation = 0;
    for (int i = 1; i < RootIDs.size(); i++) // 0-й элемент всегда "0.0"
    {
        QString IndentSpaces;
        IndentSpaces.fill(0xFFFF, Indentation);
        QStringList RIDsl = RootIDs.at(i).split(".");
        if (RIDsl.size()<2)
        {
            DBGMSG;
            return TM_BADRESULT;
        }
        bool ok;
        int Table = RIDsl.at(0).toInt(&ok);
        if (!ok)
        {
            DBGMSG;
            return TM_BADRESULT;
        }
        QString Id = RIDsl.at(1);
        QStringList tmpsl = sqlc.GetValuesFromTableByID(DBs.at(Table), Tables.at(Table), TableHeaders.at(Table), Id);
        if (sqlc.result)
        {
            WARNMSG(sqlc.LastError);
            return TM_BADRESULT;
        }
        Id = QString("%1").arg(Id.toInt(0), 7, 10, QChar('0'));
        Id.insert(0, IndentSpaces);
        QList<PublicClass::ValueStruct> vl;
        PublicClass::ValueStruct tmpvl;
        tmpvl.Type = VS_STRING;
        tmpvl.Value = Id;
        vl.append(tmpvl);
        for (int j=0; j<tmpsl.size(); j++)
        {
            PublicClass::ValueStruct tmpvl;
            tfl.idtov(TableLinks.at(Table).at(j), tmpsl.at(j), tmpvl);
            tmpvl.Value.insert(0, IndentSpaces);
            vl.append(tmpvl);
        }
        AddItemToTree(vl);
        SetLastItem(Colors[4], Qt::transparent, Fonts[4], Icons[4], TM_ELEMENT_WITH_CHILDREN); // раскрытая книга
        Indentation++;
    }
    return TM_OK;
}

int TreeModel::SetTree(int Table, QString Id)
{
    QString IndentSpaces;
    IndentSpaces.fill(0xFFFF, Indentation);
    QStringList tmpsl = TableHeaders.at(Table);
    if (!IsRaw)
        tmpsl.insert(0, "id"+Tables.at(Table));
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByFields(DBs.at(Table), Tables.at(Table), tmpsl, QStringList("idalias"), QStringList(Id), Tables.at(Table));
    if (sqlc.result == 2) // пустой ответ вполне имеет право быть, т.к. есть ещё проверка на подчинённые таблицы с данным ИД
    {
        WARNMSG(sqlc.LastError);
        return TM_BADRESULT;
    }
    for (int i=0; i<vl.size(); i++)
    {
        if ((IsConditional) && (RightsFieldNum >= 0))
        {
            QString Rights = vl.at(i).at(RightsFieldNum);
            if (!(Rights.toUInt(0,16) & pc.access)) // пропускаем, если права не соответствуют
                continue;
        }
        tmpsl = vl.at(i);
        QString RootId = tmpsl.at(0);
        QString tmps = QString::number(Table)+"."+QString("%1").arg(RootId.toInt(0), 7, 10, QChar('0')); // добавка нулей
        tmps.insert(0, IndentSpaces);
        QList<PublicClass::ValueStruct> vsl;
        PublicClass::ValueStruct tmpvl;
        tmpvl.Type = VS_STRING;
        tmpvl.Value = tmps;
        vsl.append(tmpvl);
        for (int j=1; j<tmpsl.size(); j++)
        {
            PublicClass::ValueStruct tmpvl;
            tfl.idtov(TableLinks.at(Table).at(j-1), tmpsl.at(j), tmpvl);
            tmpvl.Value.insert(0, IndentSpaces);
            vsl.append(tmpvl);
        }
        AddItemToTree(vsl);
        // проверка наличия потомков у элемента
        tmps = sqlc.GetValueFromTableByField(DBs.at(Table), Tables.at(Table), Tables.at(Table), "idalias", RootId);
        if (sqlc.result == SQLC_FAILED)
        {
            WARNMSG(sqlc.LastError);
            return TM_BADRESULT;
        }
        // если есть хотя бы один потомок, надо ставить "книжку"
        if (!tmps.isEmpty()) // есть потомки
            SetLastItem(Colors[4],Qt::transparent,Fonts[4],Icons[3],TM_ELEMENT_WITH_CHILDREN); // закрытая книга
        else // нет потомков в своей таблице - надо покопаться в подчинённой
        {
            int NextTable = Table+1;
            if (NextTable < TablesNum) // есть ещё таблицы
            {
                if (IsRaw)
                    tmps = sqlc.GetValueFromTableByField(DBs.at(NextTable), Tables.at(NextTable),"id","id"+Tables.at(Table), RootId);
                else
                    tmps = sqlc.GetValueFromTableByField(DBs.at(NextTable), Tables.at(NextTable),"id"+Tables.at(NextTable),"id"+Tables.at(Table), RootId);
                if (!tmps.isEmpty())
                    SetLastItem(Colors[4],Qt::transparent,Fonts[4],Icons[3],TM_ELEMENT_WITH_CHILDREN); // закрытая книга
                else
                    SetLastItem(Colors[0],Qt::transparent,Fonts[4],Icons[0],TM_SIMPLE_ELEMENT);
            }
            else
                SetLastItem(Colors[0],Qt::transparent,Fonts[4],Icons[0],TM_SIMPLE_ELEMENT);
        }
    }
    return TM_OK;
}

int TreeModel::SetTable(int Table, QString Id)
{
    if (Id == "0") // процедура установки таблицы имеет смысл только для корневого элемента
    {
        QString IndentSpaces;
        IndentSpaces.fill(0xFFFF, Indentation);
        QString MainTable = Tables.at(Table);
        QStringList tmpsl = TableHeaders.at(Table);
        if (!IsRaw)
            tmpsl.insert(0, "id"+MainTable);
        QList<QStringList> vl = sqlc.GetMoreValuesFromTableByFields(DBs.at(Table), MainTable, tmpsl, QStringList("deleted"), QStringList("0"), tmpsl.at(0));
        if (sqlc.result)
        {
            WARNMSG(sqlc.LastError);
            return TM_BADRESULT;
        }
        int NewTable = Table+1;
        bool NewTableExist = (NewTable < TablesNum);
        for (int i=0; i<vl.size(); i++)
        {
            if ((IsConditional) && (RightsFieldNum >= 0))
            {
                QString Rights = vl.at(i).at(RightsFieldNum);
                if (!(Rights.toUInt(0,16) & pc.access)) // пропускаем, если права не соответствуют
                    continue;
            }
            tmpsl = vl.at(i);
            QString RootId = tmpsl.at(0);
            QString tmps = QString::number(Table)+"."+QString("%1").arg(RootId.toInt(),7, 10, QChar('0')); // добавка нулей
            tmps.insert(0, IndentSpaces);
            QList<PublicClass::ValueStruct> vlsl;
            PublicClass::ValueStruct tmpvl;
            tmpvl.Type = VS_STRING;
            tmpvl.Value = tmps;
            vlsl.append(tmpvl);
            for (int j=1; j<tmpsl.size(); j++)
            {
                tfl.idtov(TableLinks.at(Table).at(j-1), tmpsl.at(j), tmpvl);
                tmpvl.Value.insert(0, IndentSpaces);
                vlsl.append(tmpvl);
            }
            AddItemToTree(vlsl);
            // проверка наличия потомков у элемента
            if (NewTableExist) // если дальше ещё есть таблицы
            {
                QString NewTableId = (IsRaw) ? "id" : "id"+Tables.at(NewTable+1);
                if ((TreeType == TT_TYPE2) && (Tables.size() >= (NewTable+1)))
                    tmps = sqlc.GetValueFromTableByField(DBs.at(NewTable+1), Tables.at(NewTable+1), NewTableId, "id"+MainTable, RootId);
                else
                    tmps = sqlc.GetValueFromTableByField(DBs.at(NewTable), Tables.at(NewTable), NewTableId, "id"+MainTable, RootId);
                if (sqlc.result == SQLC_FAILED)
                {
                    WARNMSG(sqlc.LastError);
                    return TM_BADRESULT;
                }
                // если есть хотя бы один потомок, надо ставить "книжку"
                if (tmps.isEmpty()) // нет потомков
                    SetLastItem(Colors[0],Qt::transparent,Fonts[4],Icons[0],TM_SIMPLE_ELEMENT);
                else
                    SetLastItem(Colors[4],Qt::transparent,Fonts[4],Icons[3],TM_ELEMENT_WITH_CHILDREN); // закрытая книга
            }
            else
                SetLastItem(Colors[0],Qt::transparent,Fonts[0],Icons[0],TM_SIMPLE_ELEMENT);
        }
    }
    return TM_OK;
}

int TreeModel::SetNextTree(int Table, QString Id)
{
    QString IndentSpaces;
    IndentSpaces.fill(0xFFFF, Indentation);
    QStringList tmpsl = TableHeaders.at(Table);
    if (!IsRaw)
        tmpsl.insert(0, "id"+Tables.at(Table));
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByFields(DBs.at(Table), Tables.at(Table), tmpsl, QStringList("id"+Tables.at(Table-1)), QStringList(Id), tmpsl.at(0));
    if (sqlc.result)
    {
        WARNMSG(sqlc.LastError);
        return TM_BADRESULT;
    }
    for (int i=0; i<vl.size(); i++)
    {
        if ((IsConditional) && (RightsFieldNum >= 0))
        {
            QString Rights = vl.at(i).at(RightsFieldNum);
            if (!(Rights.toUInt(0,16) & pc.access)) // пропускаем, если права не соответствуют
                continue;
        }
        tmpsl = vl.at(i);
        QString RootId = tmpsl.at(0);
        QString tmps = QString::number(Table)+"."+QString("%1").arg(RootId.toInt(),7, 10, QChar('0')); // добавка нулей
        tmps.insert(0, IndentSpaces);
        QList<PublicClass::ValueStruct> vl;
        PublicClass::ValueStruct tmpvl;
        tmpvl.Type = VS_STRING;
        tmpvl.Value = tmps;
        vl.append(tmpvl);
        for (int j=1; j<tmpsl.size(); j++)
        {
            PublicClass::ValueStruct tmpvl;
            tfl.idtov(TableLinks.at(Table).at(j-1), tmpsl.at(j), tmpvl);
            tmpvl.Value.insert(0, IndentSpaces);
            vl.append(tmpvl);
        }
        AddItemToTree(vl);
        // проверка наличия потомков у элемента
        tmps = sqlc.GetValueFromTableByField(DBs.at(Table), Tables.at(Table), Tables.at(Table), "idalias", RootId);
        if (sqlc.result == SQLC_FAILED)
        {
            WARNMSG(sqlc.LastError);
            return TM_BADRESULT;
        }
        // если есть хотя бы один потомок, надо ставить "книжку"
        if (tmps.isEmpty()) // нет потомков
            SetLastItem(Colors[0],Qt::transparent,Fonts[4],Icons[0],TM_SIMPLE_ELEMENT);
        else
            SetLastItem(Colors[4],Qt::transparent,Fonts[4],Icons[3],TM_ELEMENT_WITH_CHILDREN); // закрытая книга
    }
    return TM_OK;
}

int TreeModel::SetNextTable(int Table, QString Id)
{
    QString IndentSpaces;
    IndentSpaces.fill(0xFFFF, Indentation);
    QList<QStringList> vl;
    QString MainTable; // Table = "docclasses", Table-1 = "devices", Table+1 = "documents"
    QStringList tmpsl;
    if ((TreeType == TT_TYPE2) && (Table == 1)) // второй уровень для типа 2 - особенный
    {
        if (Tables.size() < 3)
        {
            DBGMSG;
            return TM_BADRESULT;
        }
        tmpsl = TableHeaders.at(Table+1);
        if (!IsRaw)
            tmpsl.insert(0, "id"+Tables.at(Table+1));
        if (tmpsl.indexOf("id"+Tables.at(Table)) == -1)
            tmpsl.append("id"+Tables.at(Table));
        vl = sqlc.GetMoreValuesFromTableByFields(DBs.at(Table+1), Tables.at(Table+1), tmpsl, QStringList("id"+Tables.at(Table-1)), QStringList(Id), tmpsl.at(0));
        if (sqlc.result)
        {
            WARNMSG(sqlc.LastError);
            return TM_BADRESULT;
        }
        int tmpidx = tmpsl.indexOf("id"+Tables.at(Table));
        if (tmpidx == -1)
        {
            DBGMSG;
            return TM_BADRESULT;
        }
        // надо подобрать уникальные iddocclasses
        QStringList idsl;
        for (int i=0; i<vl.size(); i++)
        {
            if ((IsConditional) && (RightsFieldNum >= 0))
            {
                QString Rights = vl.at(i).at(RightsFieldNum);
                if (!(Rights.toUInt(0,16) & pc.access)) // пропускаем, если права не соответствуют
                    continue;
            }
            if (idsl.indexOf(vl.at(i).at(tmpidx)) == -1)
                idsl << vl.at(i).at(tmpidx);
        }
        tmpsl = TableHeaders.at(Table);
        if (!IsRaw)
            tmpsl.insert(0, "id"+Tables.at(Table));
        for (int i=0; i<idsl.size(); i++)
        {
            QStringList sl = sqlc.GetValuesFromTableByField(DBs.at(Table), Tables.at(Table), tmpsl, "id"+Tables.at(Table), idsl.at(i));
            if (sqlc.result)
                WARNMSG(sqlc.LastError);
            else
            {
                QString tmps = QString::number(Table)+"."+QString("%1").arg(sl.at(0).toInt(),7, 10, QChar('0')); // добавка нулей
                tmps.insert(0, IndentSpaces);
                QList<PublicClass::ValueStruct> vl;
                PublicClass::ValueStruct tmpvl;
                tmpvl.Type = VS_STRING;
                tmpvl.Value = tmps;
                vl.append(tmpvl);
                for (int j=1; j<tmpsl.size(); j++)
                {
                    PublicClass::ValueStruct tmpvl;
                    tfl.idtov(TableLinks.at(Table).at(j-1), tmpsl.at(j), tmpvl);
                    tmpvl.Value.insert(0, IndentSpaces);
                    vl.append(tmpvl);
                }
                AddItemToTree(vl);
                SetLastItem(Colors[4],Qt::transparent,Fonts[4],Icons[3], TM_ELEMENT_WITH_CHILDREN); // закрытая книга, т.к. если есть запись в таблице 2, значит, есть соответствующая запись в таблице 3
            }
        }
    }
    else
    {
        MainTable = Tables.at(Table);
        tmpsl = TableHeaders.at(Table);
        if (!IsRaw)
            tmpsl.insert(0, "id"+Tables.at(Table));
        vl = sqlc.GetMoreValuesFromTableByFields(DBs.at(Table), Tables.at(Table), tmpsl, QStringList("id"+Tables.at(Table-1)), QStringList(Id), tmpsl.at(0));
        if (sqlc.result == 2)
        {
            WARNMSG(sqlc.LastError);
            return TM_BADRESULT;
        }
        int NewTable = Table+1;
        bool NewTableExist = (NewTable < TablesNum);
        for (int i=0; i<vl.size(); i++)
        {
            tmpsl = vl.at(i);
            QString RootId = tmpsl.at(0);
            QString tmps = QString::number(Table)+"."+QString("%1").arg(tmpsl.at(0).toInt(),7, 10, QChar('0')); // добавка нулей
            tmps.insert(0, IndentSpaces);
            QList<PublicClass::ValueStruct> vl;
            PublicClass::ValueStruct tmpvl;
            tmpvl.Type = VS_STRING;
            tmpvl.Value = tmps;
            vl.append(tmpvl);
            for (int j=1; j<tmpsl.size(); j++)
            {
                PublicClass::ValueStruct tmpvl;
                tfl.idtov(TableLinks.at(Table).at(j-1), tmpsl.at(j), tmpvl);
                tmpvl.Value.insert(0, IndentSpaces);
                vl.append(tmpvl);
            }
            AddItemToTree(vl);
            if (NewTableExist) // если дальше ещё есть таблицы
            {
                tmps = sqlc.GetValueFromTableByField(DBs.at(NewTable), Tables.at(NewTable), "id"+Tables.at(NewTable), "id"+MainTable, RootId);
                if (sqlc.result == SQLC_FAILED)
                {
                    WARNMSG(sqlc.LastError);
                    return TM_BADRESULT;
                }
                // если есть хотя бы один потомок, надо ставить "книжку"
                if (tmps.isEmpty()) // нет потомков
                    SetLastItem(Colors[0],Qt::transparent,Fonts[4],Icons[0],TM_SIMPLE_ELEMENT);
                else
                    SetLastItem(Colors[4],Qt::transparent,Fonts[4],Icons[3], TM_ELEMENT_WITH_CHILDREN); // закрытая книга
            }
            else
                SetLastItem(Colors[0],Qt::transparent,Fonts[0],Icons[0],TM_SIMPLE_ELEMENT);
        }
    }
    return TM_OK;
}

void TreeModel::AddItemToTree(QList<PublicClass::ValueStruct> &vl)
{
    int LastIndex = rowCount();
    insertRows(LastIndex, 1);
    for (int column = 0; column < vl.size(); ++column)
    {
        if (vl.at(column).Type == VS_ICON)
        {
            setData(index(LastIndex, column, QModelIndex()), QIcon(vl.at(column).Value), Qt::DecorationRole);
            setData(index(LastIndex, column, QModelIndex()), QVariant("i."+vl.at(column).Value), CellInfoRole);
        }
        else
            setData(index(LastIndex, column, QModelIndex()), vl.at(column).Value, Qt::EditRole);
    }
}

void TreeModel::ClearModel()
{
    beginResetModel();
    while (rowCount() > 0)
        removeRows(0, 1);
    hdr.clear();
    maindata.clear();
    RootIDs.clear();
    RootIDs.push("0.0");
    DBs.clear();
    Tables.clear();
    TableHeaders.clear();
    TableLinks.clear();
    TableIsTree.clear();
    IsRaw = false;
    TablesNum = 0;
    endResetModel();
}

void TreeModel::ClearOnlyData()
{
    beginResetModel();
    while (rowCount() > 0)
        removeRows(0, 1);
    maindata.clear();
    endResetModel();
}

void TreeModel::Refresh()
{
    ClearOnlyData();
    if (BuildTree())
        WARNMSG("");
}
