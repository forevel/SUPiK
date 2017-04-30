#include "filemodel.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QMap>
#include <QSortFilterProxyModel>

FileModel::FileModel(QObject *parent) : QAbstractTableModel(parent)
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
    Icons[0] = QIcon(":/res/file.png");
    Icons[1] = QIcon(":/res/folder.png");
    Icons[2] = QIcon(":/res/folderopened.png");
//    RootIDs.push("0");
    hdr.clear();
    hdr << "#" << "Name" << "Size" << "Attr" << "Datetime";
}

FileModel::~FileModel()
{
    ClearModel();
}

int FileModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return FM_HDR_COUNT;
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if ((index.row() < maindata.size()) && (index.column() < hdr.size()))
        {
            switch(index.column())
            {
            case 0: // icon
            {
                if (role == Qt::DecorationRole)
                {
                    if (maindata.at(index.row())->Attr().at(0) == 'd') // it's folder
                        return QVariant::fromValue(Icons[1]);
                    else
                        return QVariant::fromValue(Icons[0]);
                }
                break;
            }
            case 1: // filename
            {
                if (role == Qt::DisplayRole)
                    return maindata.at(index.row())->Name();
                break;
            }
            case 2: // size
            {
                if (role == Qt::DisplayRole)
                    return maindata.at(index.row())->Size();
                break;
            }
            case 3: // attr
            {
                if (role == Qt::DisplayRole)
                    return maindata.at(index.row())->Attr();
                break;
            }
            case 4: // datetime
            {
                if (role == Qt::DisplayRole)
                    return maindata.at(index.row())->Datetime();
                break;
            }
            default:
                return QVariant(); // any else column denied
            }
        }
        else
            return QVariant(); // index.row and/or index.column are invalid
/*            else if (role == Qt::FontRole)
                return QVariant::fromValue(QFont(maindata.at(index.row())->Font()));
            else if (role == Qt::ForegroundRole)
                return QVariant::fromValue(QColor(maindata.at(index.row())->Color()));
            else if (role == Qt::DecorationRole)

            else if (role == SizeRole)
                return maindata.at(index.row())->Info(index.column());
            else if (role == AttrRole)
                return maindata.at(index.row())->Links(index.column());
            else if (role == HeaderTextRole)
                return maindata.at(index.row())->Header(index.column());
            else if (role == TableNumberRole)
                return maindata.at(index.row())->TableNumber(index.column()); */
    }
    return QVariant();
}

bool FileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
/*    if (index.isValid())
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
    else */
        return false;
}

Qt::ItemFlags FileModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant FileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole || role == Qt::EditRole) && (section < hdr.size()))
        return hdr.at(section);
    return QVariant();
}

QModelIndex FileModel::index(int row, int column, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return createIndex(row, column);
}

bool FileModel::insertColumns(int column, int count, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertColumns(QModelIndex(), column, column+count-1);
    for (int i = 0; i < count; i++)
        hdr.insert(column, "");
    endInsertColumns();
    return true;
}

bool FileModel::insertRows(int row, int count, const QModelIndex &parent)
{
    int i, j;
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row+count-1);
    for (i = 0; i < count; i++)
    {
        FileItem *Item = new FileItem;
        for (j = 0; j < hdr.size(); j++)
            Item->SetName("");
        maindata.insert(row, Item);
    }
    endInsertRows();
    return true;
}

bool FileModel::removeColumns(int column, int count, const QModelIndex &parent)
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

bool FileModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    if ((row+count) > maindata.size())
        return false;
    for (int i = 0; i < count; i++)
    {
        FileItem *Item = maindata.at(row);
        maindata.removeAt(row);
        delete Item;
    }
    endRemoveRows();
    return true;
}

int FileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return maindata.size();
}

bool FileModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
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

void FileModel::GotoFile(QModelIndex idx)
{
    // 1. вытащить ИД кликнутого элемента
    // 2. добавить в стек ИД элемента
    // 3. очистить модель
    // 4. построить новую модель от этого элемента

    // 0
/*    int ClickedRow = idx.row()+1;
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
    } */
}

// установка цвета, шрифта и иконки у последнедобавленного элемента

void FileModel::SetLastItem(QColor FColor, QColor BColor, QFont Font, QIcon Icon, QString AData)
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
//    setData(index(LastIndex, 0, QModelIndex()), AData, CellInfoRole);
}

int FileModel::Setup(QString DirName)
{
    ClearModel();
/*    insertColumns(0,1,QModelIndex());
    QString tmpString;
    char *tmpChar;
    int filepos = 0;
    QFile file;
    file.setFileName(Filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        WARNMSG("Невозможно открыть файл "+Filename);
        return FM_BADSOURCE;
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
//        AddItemToTree(vl);
//        SetLastItem(Colors[0], Qt::transparent, Fonts[0], Icons[0], TM_SIMPLE_ELEMENT);
    }
    file.close(); */
    return FM_OK;
}

int FileModel::PrepareTable(QString Table)
{
/*    RightsFieldNum = -1;
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
    TablesNum++; // количество таблиц увеличиваем */
    return FM_OK;
}

// процедура построения дерева по данному ИД в данной таблице. TableDotId в виде <Table>.<ID>

int FileModel::BuildTree()
{
/*    QStringList TableId = RootIDs.top().split(".");
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
    } */
    return FM_OK;
}

// установка первого элемента в виде раскрытой книги

int FileModel::SetFirstTreeElements()
{
/*    Indentation = 0;
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
    } */
    return FM_OK;
}

int FileModel::SetTable(int Table, QString Id)
{
/*    if (Id == "0") // процедура установки таблицы имеет смысл только для корневого элемента
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
    } */
    return FM_OK;
}

void FileModel::AddItemToTree(QStringList &vl)
{
    int LastIndex = rowCount();
    insertRows(LastIndex, 1);
    for (int column = 0; ((column < vl.size()) && (column < 5)); ++column)
        setData(index(LastIndex, column, QModelIndex()), vl.at(column), Qt::EditRole);
}

void FileModel::ClearModel()
{
    beginResetModel();
    while (rowCount() > 0)
        removeRows(0, 1);
    hdr.clear();
    maindata.clear();
    Folders.clear();
    endResetModel();
}

void FileModel::ClearOnlyData()
{
    beginResetModel();
    while (rowCount() > 0)
        removeRows(0, 1);
    maindata.clear();
    endResetModel();
}

void FileModel::Refresh()
{
    ClearOnlyData();
    if (BuildTree())
        WARNMSG("");
}
