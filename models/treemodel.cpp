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
        if (Build())
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
        if (Build())
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
    beginResetModel();
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
        return RESULTBAD;
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
        AddItemToTree(QStringList(tmpString));
        SetLastItem(Colors[0], Qt::transparent, Fonts[0], Icons[0], TM_SIMPLE_ELEMENT);
    }
    file.close();
    endResetModel();
    return RESULTOK;
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
    beginResetModel();
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
        return RESULTBAD;
    }
    int i;
    for (i=0; i<Tables.size(); i++)
    {
        if (PrepareTable(Tables.at(i)))
            return RESULTBAD;
    }
    if (Build()) // элементы записываются в виде: <номер_таблицы>.<ИД>
    {
        WARNMSG("");
        return RESULTBAD;
    }
    endResetModel();
    return RESULTOK;
}

//

/*int TreeModel::SetupRawComp(QString db, QString tble)
{
    return SetupRaw(db, tble, "id", "PartNumber");
}*/

// инициализация таблицы по реальным именам в БД

int TreeModel::SetupRaw(QString db, QString tble, QStringList fields)
{
    beginResetModel();
    RightsFieldNum = -1;
    IsConditional = false;
    if (fields.isEmpty())
        fields = sqlc.GetColumnsFromTable(db, tble);
    int i;
    int ididx = fields.indexOf("id"+tble);
    if (ididx == -1)
        ididx = fields.indexOf("id");
    if (fields.isEmpty())
        return RESULTBAD;
    QStringList TableLinksSl;
    for (i=0; i<fields.size(); ++i)
    {
        if (i == ididx)
            TableLinksSl.append("4.19..7");
        else
            TableLinksSl.append("1.8");
    }
    ClearModel();
    IsRaw = true;
    TableHeaders.append(fields);
    TableLinks.append(TableLinksSl);
    TableIsTree.append(false);
    DBs.append(db);
    Tables.append(tble);
    TablesNum++; // количество таблиц увеличиваем
    TreeType = TT_SIMPLE;
    for (i = columnCount(); i < fields.size(); i++) // добавим нехватающее количество столбцов в общую модель
        insertColumns(i,1,QModelIndex());
    if (Build()) // элементы записываются в виде: <номер_таблицы>.<ИД>
    {
        WARNMSG("");
        return RESULTBAD;
    }
    endResetModel();
    return RESULTOK;
}

int TreeModel::PrepareTable(QString Table)
{
    RightsFieldNum = -1;
    if (Table.isEmpty())
    {
        DBGMSG;
        return RESULTBAD;
    }
    QStringList fl = QStringList() << "table" << "tablefields" << "header" << "links";
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByFields("sup", "tablefields", fl, QStringList("tablename"), QStringList(Table), "fieldsorder", true);
    if ((sqlc.result) || (vl.size() == 0))
    {
        WARNMSG(sqlc.LastError);
        return RESULTBAD;
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
        return RESULTBAD;
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
        return RESULTBAD;
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
        return RESULTBAD;
    }
    DBs.append(tmpsl.at(0));
    Tables.append(tmpsl.at(1));
    TablesNum++; // количество таблиц увеличиваем
    return RESULTOK;
}

// процедура построения дерева по данному ИД в данной таблице. TableDotId в виде <Table>.<ID>

int TreeModel::Build()
{
    QStringList TableId = RootIDs.top().split(".");
    if (TableId.size()<2)
    {
        DBGMSG;
        return RESULTBAD;
    }
    int Table = TableId.at(0).toInt();
    QString Id = QString::number(TableId.at(1).toInt()); // убираем лишние нули
    if (SetFirstTreeElements()) // нарисовать первый элемент его самого с открытой книгой
    {
        WARNMSG("");
        return RESULTBAD;
    }
    if (TableIsTree.at(Table))
    {
        if (Set(Table, Id, TM_STR))
        {
            WARNMSG("");
            return RESULTBAD;
        }
    }
    else
    {
        if (Set(Table, Id, TM_STT))
        {
            WARNMSG("");
            return RESULTBAD;
        }
    }
    // построили по текущему уровню и текущей таблице, надо проверить следующую по списку таблицу на наличие элементов, связанных с текущим Id
    if (RootIDs.top() != "0.0")
        Table++; // переходим на новый уровень дерева
    else
        return RESULTOK;
    if (Table >= TablesNum) // больше нет таблиц
        return RESULTOK; // выход
    if (TableIsTree.at(Table))
    {
        if (Set(Table, Id, TM_SNTR))
        {
            WARNMSG("");
            return RESULTBAD;
        }
    }
    else
    {
        if (Set(Table, Id, TM_SNTT))
        {
            WARNMSG("");
            return RESULTBAD;
        }
    }
    return RESULTOK;
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
            return RESULTBAD;
        }
        bool ok;
        int Table = RIDsl.at(0).toInt(&ok);
        if (!ok)
        {
            DBGMSG;
            return RESULTBAD;
        }
        QString Id = RIDsl.at(1);
        QStringList tmpsl = sqlc.GetValuesFromTableByID(DBs.at(Table), Tables.at(Table), TableHeaders.at(Table), Id);
        if (sqlc.result)
        {
            WARNMSG(sqlc.LastError);
            return RESULTBAD;
        }
        Id = QString("%1").arg(Id.toInt(0), 7, 10, QChar('0'));
        Id.insert(0, IndentSpaces);
        QStringList vl;
        vl.append(Id);
        for (int j=0; j<tmpsl.size(); j++)
        {
            QString tmps;
            tfl.idtov(TableLinks.at(Table).at(j), tmpsl.at(j), tmps);
            tmps.insert(0, IndentSpaces);
            vl.append(tmps);
        }
        AddItemToTree(vl);
        SetLastItem(Colors[4], Qt::transparent, Fonts[4], Icons[4], TM_ELEMENT_WITH_CHILDREN); // раскрытая книга
        Indentation++;
    }
    return RESULTOK;
}

int TreeModel::Set(int table, QString &id, int type)
{
//    bool Type2 = false; // (type == TM_SNTT) && (TreeType == TT_TYPE2) && (Table == 1)
    if ((type == TM_STT) && (id != "0")) // процедура установки таблицы имеет смысл только для корневого элемента
        return RESULTOK;
    QString IndentSpaces;
    QStringList tmpsl;
//    int tmpidx;
    QList<QStringList> vl;
    IndentSpaces.fill(0xFFFF, Indentation);
/*    if ((type == TM_SNTT) && (TreeType == TT_TYPE2) && (Table == 1))
    {
        // второй уровень для типа 2 - особенный
        // он составляется по данным нескольких таблиц - devices -> docclasses -> documents
        Type2 = true;
        if (Tables.size() < 3)
        {
            DBGMSG;
            return RESULTBAD;
        }
        tmpsl = TableHeaders.at(table+1);
    }
    else */
        tmpsl = TableHeaders.at(table);
    if (!IsRaw)
        tmpsl.insert(0, "id"+Tables.at(table));
    if (type == TM_STR)
        vl = sqlc.GetMoreValuesFromTableByFields(DBs.at(table), Tables.at(table), tmpsl, QStringList("idalias"), QStringList(id), Tables.at(table));
    else if (type == TM_STT)
        vl = sqlc.GetMoreValuesFromTableByFields(DBs.at(table), Tables.at(table), tmpsl, QStringList("deleted"), QStringList("0"), tmpsl.at(0));
    else if ((type == TM_SNTR) || (type == TM_SNTT))
        vl = sqlc.GetMoreValuesFromTableByFields(DBs.at(table), Tables.at(table), tmpsl, QStringList("id"+Tables.at(table-1)), QStringList(id), tmpsl.at(0));
/*    else // Type2
    {
        if (tmpsl.indexOf("id"+Tables.at(Table)) == -1)
            tmpsl.append("id"+Tables.at(Table));
        tmpidx = tmpsl.indexOf("id"+Tables.at(Table));
        if (tmpidx == -1)
        {
            DBGMSG;
            return RESULTBAD;
        }
        vl = sqlc.GetMoreValuesFromTableByFields(DBs.at(Table+1), Tables.at(Table+1), tmpsl, QStringList("id"+Tables.at(Table-1)), QStringList(Id), tmpsl.at(0));
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
                QStringList sl;
                sl.append(tmps);
                for (int j=1; j<tmpsl.size(); j++)
                {
                    tfl.idtov(TableLinks.at(Table).at(j-1), tmpsl.at(j), tmps);
                    tmps.insert(0, IndentSpaces);
                    sl.append(tmps);
                }
                AddItemToTree(sl);
                SetLastItem(Colors[4],Qt::transparent,Fonts[4],Icons[3], TM_ELEMENT_WITH_CHILDREN); // закрытая книга, т.к. если есть запись в таблице 2, значит, есть соответствующая запись в таблице 3
            }
        }
    } */
    if (sqlc.result)
    {
        WARNMSG(sqlc.LastError);
        return RESULTBAD;
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
        QString tmps = QString::number(table)+"."+QString("%1").arg(RootId.toInt(0), 7, 10, QChar('0')); // добавка нулей
        tmps.insert(0, IndentSpaces);
        QStringList sl;
        sl.append(tmps);
        for (int j=1; j<tmpsl.size(); j++)
        {
            tfl.idtov(TableLinks.at(table).at(j-1), tmpsl.at(j), tmps);
            tmps.insert(0, IndentSpaces);
            sl.append(tmps);
        }
        AddItemToTree(sl);
        if ((type == TM_STR) || (type == TM_SNTR))
        {
            // проверка наличия потомков у элемента
            tmps = sqlc.GetValueFromTableByField(DBs.at(table), Tables.at(table), Tables.at(table), "idalias", RootId);
            if (sqlc.result == SQLC_FAILED)
            {
                WARNMSG(sqlc.LastError);
                return RESULTBAD;
            }
            // если есть хотя бы один потомок, надо ставить "книжку"
            if (!tmps.isEmpty()) // есть потомки
                SetLastItem(Colors[4],Qt::transparent,Fonts[4],Icons[3],TM_ELEMENT_WITH_CHILDREN); // закрытая книга
            else
                SetLastItem(Colors[0],Qt::transparent,Fonts[4],Icons[0],TM_SIMPLE_ELEMENT);
            continue;
        }
        if (type != TM_SNTR)
        {
            // надо покопаться в подчинённой
            int nexttable = table+1;
            if (nexttable < TablesNum) // есть ещё таблицы
            {
                if (IsRaw)
                    tmps = sqlc.GetValueFromTableByField(DBs.at(nexttable), Tables.at(nexttable),"id","id"+Tables.at(table), RootId);
                else
                    tmps = sqlc.GetValueFromTableByField(DBs.at(nexttable), Tables.at(nexttable),"id"+Tables.at(nexttable),"id"+Tables.at(table), RootId);
                if (sqlc.result == SQLC_FAILED)
                {
                    WARNMSG(sqlc.LastError);
                    return RESULTBAD;
                }
                if (!tmps.isEmpty())
                    SetLastItem(Colors[4],Qt::transparent,Fonts[4],Icons[3],TM_ELEMENT_WITH_CHILDREN); // закрытая книга
                else
                    SetLastItem(Colors[0],Qt::transparent,Fonts[4],Icons[0],TM_SIMPLE_ELEMENT);
            }
            else
                SetLastItem(Colors[0],Qt::transparent,Fonts[4],Icons[0],TM_SIMPLE_ELEMENT);
        }
    }
    return RESULTOK;
}

void TreeModel::AddItemToTree(QStringList &vl)
{
    int LastIndex = rowCount();
    insertRows(LastIndex, 1);
    for (int column = 0; column < vl.size(); ++column)
        setData(index(LastIndex, column, QModelIndex()), vl.at(column), Qt::EditRole);
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
    if (Build())
        WARNMSG("");
}
