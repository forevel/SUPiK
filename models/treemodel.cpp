#include "treemodel.h"
#include "../gen/publicclass.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"
#include <QSqlDatabase>
#include <QSqlQuery>
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
            else if (role == SetRole)
                return maindata.at(index.row())->AData(index.column());
/*            else if (role == Qt::UserRole)
                return maindata.at(index.row())->linksdata(index.column());
            else if (role == Qt::UserRole+1)
                return maindata.at(index.row())->AData();
            else if (role == Qt::UserRole+2)
                return maindata.at(index.row())->TData(index.column()); */
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
        else if (role == SetRole)
        {
            maindata.at(index.row())->SetAData(index.column(), value.toString());
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
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;// | Qt::ItemIsEditable;
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
//            Item->setLinksData(j, "");
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
    if ((idx.row() == 0) && (RootIDs.size() != 1)) // кликнули по первой строке, значит, надо свернуть на один уровень
    {
        // 2
        RootIDs.pop();
        // 3
        ClearOnlyData();
        // 4
        if (BuildTree(RootIDs.top()))
            TMODELWARN;
        return;
    }
    if (data(index(idx.row(),0,QModelIndex()),SetRole) == TM_SIMPLE_ELEMENT) // если простой элемент
        return;
    // 1
    QString IndexID = data(index(idx.row(),0,QModelIndex()),Qt::DisplayRole).toString(); // ИД должен быть в нулевом столбце
//    IndexID = QString::number(IndexID.toInt());
    // 2
    RootIDs.push(IndexID);
    // 3
    ClearOnlyData();
    // 4
    if (BuildTree(IndexID))
        TMODELWARN;
}

// слот, вызываемый по закрытию элемента

void TreeModel::GoOut(QModelIndex index)
{
    Q_UNUSED(index);
    // 1. проверить, если в стеке остался только один элемент ("0"), выйти
    // 2. выкинуть из стека один элемент
    // 3. очистить модель
    // 4. построить новую модель от последнего элемента в стеке

    // 1
    if (RootIDs.size() == 1)
        return;
    // 2
    RootIDs.pop();
    // 3
    ClearOnlyData();
    // 4
    if (BuildTree(RootIDs.top()))
    {
        TMODELWARN;
        return;
    }
}

// установка цвета, шрифта и иконки у последнедобавленного элемента

void TreeModel::SetLastItem(QColor Color, QFont Font, QIcon Icon, QString AData)
{
    if (rowCount() == 0)
        return;
    int LastIndex = rowCount()-1;
    for (int i=0; i<columnCount(); i++)
    {
        setData(index(LastIndex, i, QModelIndex()), Color, Qt::ForegroundRole);
        setData(index(LastIndex, i, QModelIndex()), Font, Qt::FontRole);
    }
    setData(index(LastIndex, 0, QModelIndex()), Icon, Qt::DecorationRole);
    setData(index(LastIndex, 0, QModelIndex()), AData, SetRole);
}

// процедура инициализации модели данными из таблицы table и построение дерева по полям alias и idalias

int TreeModel::Setup(QString Table)
{
    QStringList sl = QStringList() << Table;
    TreeType = TT_SIMPLE;
    return Setup(sl);
}

int TreeModel::Setup(QStringList Tables, int Type)
{
    ClearModel();

    // 1. взять столбцы tablefields из tablefields, где tablename=table
    // 2. найти среди них столбцы <db>.<tble>.alias и <db>.<tble>.idalias. Если нет - это не дерево, выход
    // 3. взять значения столбцов alias и idalias из таблицы <db>.<tble>
    // 4. построить по ним дерево

    // 1
    if (Tables.isEmpty())
    {
        TMODELWARN;
        return 1;
    }
    TablesNum = 0;
    int i;
    for (i=0; i<Tables.size(); i++)
    {
        if (PrepareTable(Tables.at(i)))
            return 1;
    }
    switch (Type)
    {
    case 1:
        TreeType = TT_TYPE1;
        break;
    case 2:
        TreeType = TT_TYPE2;
        break;
    default:
        break;
    }
    if (BuildTree("0.0")) // элементы записываются в виде: <номер_таблицы>.<ИД>
    {
        TMODELWARN;
        return 1;
    }
    return 0;
}

int TreeModel::PrepareTable(QString Table)
{
    if (Table.isEmpty())
    {
        TMODELWARN;
        return 1;
    }
    QStringList fl = QStringList() << "table" << "tablefields" << "header";
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByField(sqlc.GetDB("sup"), "tablefields", fl, "tablename", Table, "fieldsorder", true);
    if (sqlc.result)
    {
        TMODELWARN;
        return 1;
    }
    int i;
    int idalpos = -1;
    int idpos = -1;
    bool IsIdAliasExist = false, IsAliasExist = false;
    QStringList TableHeadersSl;

    // ищем ключевые поля - idalias, alias и id<tble>
    for (i = 0; i < vl.size(); i++)
    {
        if (vl.at(i).size()<3)
        {
            TMODELWARN;
            return 1;
        }
        TableHeadersSl.append(vl.at(i).at(1));
        if (vl.at(i).at(1) == "idalias")
        {
            idalpos = i;
            IsIdAliasExist = true;
        }
        if (vl.at(i).at(1) == "alias")
            IsAliasExist = true;
        if (vl.at(i).at(2) == "ИД")
            idpos = i;
    }
    if (idpos == -1)
    {
        TMODELINFO("Не найдены ключевые поля в таблице "+Table);
        return 1;
    }
    if (!IsIdAliasExist)
    {
        if (TreeType == TT_SIMPLE)
        {
//        TMODELINFO("Не найдено поле idalias в таблице "+Table);
//        return 1;
            TreeType = TT_TABLE;
        }
        TableIsTree.append(false);
    }
    else if (!IsAliasExist) // если есть idalias, но нет alias - это не дело
    {
        TMODELINFO("Не найдено поле alias в таблице "+Table);
        return 1;
    }
    else
        TableIsTree.append(true);

    // обновляем списки заголовков, БД и таблиц
    QStringList tmpsl = vl.at(idpos).at(0).split("."); // tmpsl - таблица в виде db.tble
    if (idpos != -1)
        TableHeadersSl.removeAt(idpos); // убираем поле ИД, т.к. оно при построении дерева всё равно будет добавлено
    if (idalpos != -1)
        TableHeadersSl.removeAt(idalpos); // убираем поле ИД_а, т.к. это вспомогательное поле, и его показывать не нужно
    TableHeaders.append(TableHeadersSl); // список полей, которые необходимо отображать в дереве
    int ColumnNeeded = TableHeadersSl.size()+1; // +1 на ИД
    if (columnCount()<ColumnNeeded) // если число столбцов в дереве меньше числа полей в очередной таблице
    {
        for (i = columnCount(); i < ColumnNeeded; i++) // добавим нехватающее количество столбцов в общую модель
            insertColumns(i,1,QModelIndex());
    }
    if (tmpsl.size()<2)
    {
        TMODELWARN;
        return 1;
    }
    DBs.append(tmpsl.at(0));
    Tables.append(tmpsl.at(1));
    TablesNum++; // количество таблиц увеличиваем
    return 0;
}

// процедура построения дерева по данному ИД в данной таблице. TableDotId в виде <Table>.<ID>

int TreeModel::BuildTree(QString TableDotId)
{
    QStringList TableId = TableDotId.split(".");
    if (TableId.size()<2)
        return 1;
    int Table = TableId.at(0).toInt();
    QString Id = QString::number(TableId.at(1).toInt()); // убираем лишние нули
    if (SetFirstTreeElement(Table, Id)) // нарисовать первый элемент его самого с открытой книгой
    {
        TMODELWARN;
        return 1;
    }
    if (TableIsTree.at(Table))
    {
        if (SetTree(Table, Id))
        {
            TMODELWARN;
            return 1;
        }
    }
    else
    {
        if (SetTable(Table, Id))
        {
            TMODELWARN;
            return 1;
        }
    }
    // построили по текущему уровню и текущей таблице, надо проверить следующую по списку таблицу на наличие элементов, связанных с текущим Id
    if (TableDotId != "0.0")
        Table++; // переходим на новый уровень дерева
    else
        return 0;
    if (Table >= TablesNum) // больше нет таблиц
        return 0; // выход
    if (TableIsTree.at(Table))
    {
        if (SetNextTree(Table, Id))
        {
            TMODELWARN;
            return 1;
        }
    }
    else
    {
        if (SetNextTable(Table, Id))
        {
            TMODELWARN;
            return 1;
        }
    }
    return 0;
}

// установка первого элемента в виде раскрытой книги

int TreeModel::SetFirstTreeElement(int Table, QString Id)
{
    if (Id == "0") // корневой элемент, выход
        return 0;
    QStringList tmpsl = sqlc.GetValuesFromTableByID(sqlc.GetDB(DBs.at(Table)), Tables.at(Table), TableHeaders.at(Table), Id);
    if (sqlc.result)
    {
        TMODELWARN;
        return 1;
    }
    tmpsl.insert(0,QString("%1").arg(Id.toInt(0), 7, 10, QChar('0')));
    AddItemToTree(tmpsl);
    SetLastItem(Colors[4], Fonts[4], Icons[4], TM_ELEMENT_WITH_CHILDREN); // раскрытая книга
    return 0;
}

int TreeModel::SetTree(int Table, QString Id)
{
    QStringList tmpsl = TableHeaders.at(Table);
//    tmpsl.removeAll("id"+Tables.at(Table)); // смысл этих двух строк - принудительно поставить ИД на первое место, чтобы в него добавлять нули
    tmpsl.insert(0, "id"+Tables.at(Table));
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByField(sqlc.GetDB(DBs.at(Table)), Tables.at(Table), tmpsl, "idalias", Id, "id"+Tables.at(Table));
    if (sqlc.result)
    {
        TMODELWARN;
        return 1;
    }
    for (int i=0; i<vl.size(); i++)
    {
        tmpsl = vl.at(i);
        QString RootId = tmpsl.at(0);
        tmpsl.replace(0, QString::number(Table)+"."+QString("%1").arg(tmpsl.at(0).toInt(0), 7, 10, QChar('0'))); // добавка нулей
        AddItemToTree(tmpsl);
        // проверка наличия потомков у элемента
        QString tmps = sqlc.GetValueFromTableByField(sqlc.GetDB(DBs.at(Table)), Tables.at(Table), "alias", "idalias", RootId);
        if (sqlc.result == SQLC_FAILED)
        {
            TMODELWARN;
            return 1;
        }
        // если есть хотя бы один потомок, надо ставить "книжку"
        if (tmps.isEmpty()) // нет потомков
            SetLastItem(Colors[0],Fonts[0],Icons[0],TM_SIMPLE_ELEMENT);
        else
            SetLastItem(Colors[4],Fonts[4],Icons[3],TM_ELEMENT_WITH_CHILDREN); // закрытая книга
    }
    return 0;
}

int TreeModel::SetTable(int Table, QString Id)
{
    if (Id == "0") // процедура установки таблицы имеет смысл только для корневого элемента
    {
        QString MainTable = Tables.at(Table);
        QStringList tmpsl = TableHeaders.at(Table);
        tmpsl.insert(0, "id"+MainTable);
        QList<QStringList> vl = sqlc.GetMoreValuesFromTableByField(sqlc.GetDB(DBs.at(Table)), MainTable, tmpsl, "deleted", "0", "id"+MainTable);
        if (sqlc.result)
        {
            TMODELWARN;
            return 1;
        }
        int NewTable = Table+1;
        bool NewTableExist = (NewTable < TablesNum);
        for (int i=0; i<vl.size(); i++)
        {
            tmpsl = vl.at(i);
            QString RootId = tmpsl.at(0);
            tmpsl.replace(0, QString::number(Table)+"."+QString("%1").arg(RootId.toInt(),7, 10, QChar('0'))); // добавка нулей
            AddItemToTree(tmpsl);
            // проверка наличия потомков у элемента
            if (NewTableExist) // если дальше ещё есть таблицы
            {
                QString tmps = sqlc.GetValueFromTableByField(sqlc.GetDB(DBs.at(NewTable)), Tables.at(NewTable), "id"+Tables.at(NewTable), "id"+MainTable, RootId);
                if (sqlc.result == SQLC_FAILED)
                {
                    TMODELWARN;
                    return 1;
                }
                // если есть хотя бы один потомок, надо ставить "книжку"
                if (tmps.isEmpty()) // нет потомков
                    SetLastItem(Colors[0],Fonts[0],Icons[0],TM_SIMPLE_ELEMENT);
                else
                    SetLastItem(Colors[4],Fonts[4],Icons[3],TM_ELEMENT_WITH_CHILDREN); // закрытая книга
            }
            else
                SetLastItem(Colors[0],Fonts[0],Icons[0],TM_SIMPLE_ELEMENT);
        }
    }
    return 0;
}

int TreeModel::SetNextTree(int Table, QString Id)
{
    QStringList tmpsl = TableHeaders.at(Table);
    tmpsl.insert(0, "id"+Tables.at(Table));
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByField(sqlc.GetDB(DBs.at(Table)), Tables.at(Table), tmpsl, "id"+Tables.at(Table-1), Id, "id"+Tables.at(Table));
    if (sqlc.result)
    {
        TMODELWARN;
        return 1;
    }
    for (int i=0; i<vl.size(); i++)
    {
        tmpsl = vl.at(i);
        QString RootId = tmpsl.at(0);
        tmpsl.replace(0, QString::number(Table)+"."+QString("%1").arg(tmpsl.at(0).toInt(0), 7, 10, QChar('0'))); // добавка нулей
        AddItemToTree(tmpsl);
        // проверка наличия потомков у элемента
        QString tmps = sqlc.GetValueFromTableByField(sqlc.GetDB(DBs.at(Table)), Tables.at(Table), "alias", "idalias", RootId);
        if (sqlc.result == SQLC_FAILED)
        {
            TMODELWARN;
            return 1;
        }
        // если есть хотя бы один потомок, надо ставить "книжку"
        if (tmps.isEmpty()) // нет потомков
            SetLastItem(Colors[0],Fonts[0],Icons[0],TM_SIMPLE_ELEMENT);
        else
            SetLastItem(Colors[4],Fonts[4],Icons[3],TM_ELEMENT_WITH_CHILDREN); // закрытая книга
    }
    return 0;
}

int TreeModel::SetNextTable(int Table, QString Id)
{
    QString MainTable = Tables.at(Table);
    QStringList tmpsl = TableHeaders.at(Table);
    tmpsl.insert(0, "id"+Tables.at(Table));
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByField(sqlc.GetDB(DBs.at(Table)), Tables.at(Table), tmpsl, "id"+Tables.at(Table-1), Id, "id"+Tables.at(Table));
    if (sqlc.result)
    {
        TMODELWARN;
        return 1;
    }
    int NewTable = Table+1;
    bool NewTableExist = (NewTable < TablesNum);
    for (int i=0; i<vl.size(); i++)
    {
        tmpsl = vl.at(i);
        QString RootId = tmpsl.at(0);
        tmpsl.replace(0, QString::number(Table)+"."+QString("%1").arg(tmpsl.at(0).toInt(0), 7, 10, QChar('0'))); // добавка нулей
        AddItemToTree(tmpsl);
        if (NewTableExist) // если дальше ещё есть таблицы
        {
            QString tmps = sqlc.GetValueFromTableByField(sqlc.GetDB(DBs.at(NewTable)), Tables.at(NewTable), "id"+Tables.at(NewTable), "id"+MainTable, RootId);
            if (sqlc.result == SQLC_FAILED)
            {
                TMODELWARN;
                return 1;
            }
            // если есть хотя бы один потомок, надо ставить "книжку"
            if (tmps.isEmpty()) // нет потомков
                SetLastItem(Colors[0],Fonts[0],Icons[0],TM_SIMPLE_ELEMENT);
            else
                SetLastItem(Colors[4],Fonts[4],Icons[3], TM_ELEMENT_WITH_CHILDREN); // закрытая книга
        }
    }
    return 0;
}

void TreeModel::AddItemToTree(QStringList sl)
{
    int LastIndex = rowCount();
    insertRows(LastIndex, 1);
    for (int column = 0; column < sl.size(); ++column)
        setData(index(LastIndex, column, QModelIndex()), sl.at(column), Qt::EditRole);
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
    TableIsTree.clear();
    endResetModel();
}

void TreeModel::ClearOnlyData()
{
    beginResetModel();
    while (rowCount() > 0)
        removeRows(0, 1);
    maindata.clear();
/*    RootIDs.clear();
    RootIDs.push("0"); */
    endResetModel();
}
