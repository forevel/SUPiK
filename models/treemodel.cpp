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
    RootIDs.push("0");
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
        BuildTree(RootIDs.top(), false);
        return;
    }
    // 1
    QString IndexID = data(index(idx.row(),0,QModelIndex()),Qt::DisplayRole).toString(); // ИД должен быть в нулевом столбце
    IndexID = QString::number(IndexID.toInt());
    // 2
    RootIDs.push(IndexID);
    // 3
    ClearOnlyData();
    // 4
    BuildTree(IndexID, false);
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
    BuildTree(RootIDs.top(), false);
}

// установка цвета, шрифта и иконки у последнедобавленного элемента

void TreeModel::SetLastItem(QColor Color, QFont Font, QIcon Icon)
{
    if (rowCount() == 0)
        return;
    int LastIndex = rowCount()-1;
    for (int i=0; i<columnCount(); i++)
    {
        setData(index(LastIndex, i, QModelIndex()), Color, Qt::ForegroundRole);
        setData(index(LastIndex, i, QModelIndex()), Font, Qt::FontRole);
    }
    setData(index(rowCount()-1, 0, QModelIndex()), Icon, Qt::DecorationRole);
}

// процедура инициализации модели данными из таблицы table = supik.chooselists и построение дерева по полям alias и idalias

int TreeModel::Setup(QString Table)
{
    ClearModel();
    // 1. взять столбцы tablefields из tablefields, где tablename=table
    // 2. найти среди них столбцы <db>.<tble>.alias и <db>.<tble>.idalias. Если нет - это не дерево, выход
    // 3. взять значения столбцов alias и idalias из таблицы <db>.<tble>
    // 4. построить по ним дерево

    // 1
    int i;
    QStringList fl = QStringList() << "table" << "tablefields" << "header";
    QList<QStringList> vl = sqlc.GetMoreValuesFromTableByField(sqlc.GetDB("sup"), "tablefields", fl, "tablename", Table, "fieldsorder", true);
    if (sqlc.result)
    {
        TMODELWARN;
        return 1;
    }
    // 2
    int idalpos=-1;
    bool IsIdAliasExist = false, IsAliasExist = false;
    TableHeaders.clear();
    for (i = 0; i < vl.size(); i++)
    {
        TableHeaders << vl.at(i).at(1); // формирование списка полей
        if (vl.at(i).at(1) == "idalias")
        {
            idalpos = i;
            IsIdAliasExist = true;
        }
        if (vl.at(i).at(1) == "alias")
            IsAliasExist = true;
    }
    if (!IsIdAliasExist)
        return PublicClass::ER_TMODEL; // не найдено поле idalias
    if (!IsAliasExist)
    {
        TMODELINFO("Не найдено поле alias в таблице "+Table);
        return PublicClass::ER_TMODEL;
    }
    // 3
    QStringList tmpsl = vl.at(idalpos).at(0).split("."); // tmpsl - таблица в виде db.tble
    vl.removeAt(idalpos); // не включать ИД_а в заголовок
    TableHeaders.removeAt(idalpos); // не включать ИД_а в список полей отображения
    for (i = 0; i < vl.size(); i++)
    {
        insertColumns(i,1,QModelIndex());
        setHeaderData(i, Qt::Horizontal, vl.at(i).at(2), Qt::EditRole);
    }
    // 4
    if (tmpsl.size()<2)
    {
        TMODELWARN;
        return 1;
    }
    MainDb = tmpsl.at(0);
    MainTable = tmpsl.at(1);
    TableHeaders.removeAll("id"+MainTable); // убираем из списка полей ИД, т.к. он всегда добавляется в BuildTree на первое место
    int res = BuildTree("0", false);
    if (res)
    {
        TMODELWARN;
        return 1;
    }
    return 0;
}

/*
// процедура инициализации модели данными из двух таблиц и построение дерева
// из catdb.cattble берутся "родители", из slvtble - "дети", причём в slvtble должно присутствовать
// поле `id<cattble>`. Slvtble = имя таблицы в supik.models

int TreeModel::Setup(QString Maintble, QString Slvtble)
{
    ClearModel();
    // 1. взять столбцы tablefields из tablefields, где tablename=table и headers=ИД_а
    // 4. построить дерево

    // 1
    int i;
    QStringList tmpsl = tfl.tablefields(Maintble, "ИД_а"); // взять table,tablefields,links из tablefields, где таблица maintble и заголовок ИД_а
    if (tfl.result) // нет поля idalias в таблице - это не дерево!
    {
        TMODELWARN;
        return 1;
    }
    catlist = tmpsl.at(0).split("."); // catlist - таблица, из которой брать категории
    QStringList headers = tfl.tableheaders(Slvtble);
    if (tfl.result)
    {
        TMODELWARN;
        return 1;
    }
    if (headers.isEmpty()) // нет заголовков в подчинённой таблице
    {
        TMODELWARN;
        return 1;
    }
    slvtblefields.clear();
    slvtblelinks.clear();
    for (i = 0; i < headers.size(); i++)
    {
        setHeaderData(i, Qt::Horizontal, headers.at(i), Qt::EditRole);
        tmpsl = tfl.tablefields(Slvtble, headers.at(i)); // взяли table,tablefields,links из tablefields для подчинённой таблицы и данного заголовка
        if (tfl.result) // что-то не так с подчинённой таблицей нет такого заголовка
        {
            TMODELWARN;
            return 1;
        }
        if (tmpsl.size() > 2)
        {
            slvtblelinks << tmpsl.at(2);
            slvtblefields << tmpsl.at(1);
        }
        else // нет почему-то в возвращённом результате второго элемента
        {
            TMODELWARN;
            return 1;
        }
    }
    this->slvtble = tmpsl.at(0); // имя таблицы - в переменную
    // 4
    FirstPass = true;
    int res = BuildTree("0", true);
    if (res)
    {
        TMODELWARN;
        return 1;
    }
    return 0;
}
*/
// процедура построения дерева
// на входе catlist (ссылка на таблицу категорий с полем idalias) и slvtble (название таблицы в chooselists, из которой брать записи категорий)

int TreeModel::BuildTree(QString id, bool twodb)
{
    Q_UNUSED(twodb);
    QStringList tmpsl;
    QString tmps;
    if (id == "0"); // для корневых элементов
    else // для некорневого надо нарисовать первый элемент его самого с открытой книгой
    {
        QSqlQuery get_child_from_db0 (sqlc.GetDB(MainDb));
        tmps = "SELECT ";
        for (int i=0; i<TableHeaders.size(); i++)
            tmps += "`"+TableHeaders.at(i)+"`,";
        tmps.chop(1); // последняя запятая долой
        tmps += " FROM `"+MainTable+"` WHERE `id"+MainTable+"`=\""+id+"\" AND `deleted`=0 LIMIT 1;";
//        tmps = "SELECT `alias` FROM `"+MainTable+"` WHERE `id"+MainTable+"`=\""+id+"\" AND `deleted`=0 LIMIT 1;";
        get_child_from_db0.exec(tmps);
        get_child_from_db0.next();
        tmpsl = QStringList() << QString("%1").arg(id.toInt(0), 7, 10, QChar('0'));
        for (int i=0; i<TableHeaders.size(); i++)
            tmpsl << get_child_from_db0.value(i).toString();
        AddItemToTree(tmpsl);
        SetLastItem(Colors[4], Fonts[4], Icons[4]); // раскрытая книга
    }
    QSqlQuery get_child_from_db1 (sqlc.GetDB(MainDb));
    tmps = "SELECT `id"+MainTable+"`,";
    for (int i=0; i<TableHeaders.size(); i++)
        tmps += "`"+TableHeaders.at(i)+"`,";
    tmps.chop(1); // последняя запятая долой
    tmps += " FROM `"+MainTable+"` WHERE `idalias`=\""+id+"\" AND `deleted`=0 ORDER BY `id"+MainTable+"` ASC;";
    get_child_from_db1.exec(tmps);
    while (get_child_from_db1.next())
    {
        QString RootId = get_child_from_db1.value(0).toString();
        tmpsl = QStringList() << QString("%1").arg(RootId.toInt(0), 7, 10, QChar('0'));
        for (int i=0; i<TableHeaders.size(); i++)
            tmpsl << get_child_from_db1.value(i+1).toString();
        AddItemToTree(tmpsl);
        QSqlQuery get_child_from_db2 (sqlc.GetDB(MainDb));
        tmps = "SELECT `alias` FROM `"+MainTable+"` WHERE `idalias`=\""+RootId+"\" AND `deleted`=0 LIMIT 1;"; // если есть хотя бы один потомок, надо ставить "книжку"
        get_child_from_db2.exec(tmps);
        if (get_child_from_db2.next()) // есть потомки
            SetLastItem(Colors[4],Fonts[4],Icons[3]); // закрытая книга
        else
            SetLastItem(Colors[0],Fonts[0],Icons[0]);
    }
/*    if (twodb)
    {
        res = addTreeSlvItem(position, id); // добавляем таблицу из подчинённой таблицы
        if (!res)
            HaveChildren = true;
        else if (res != -1) // если не нет потомков, а просто ошибка
        {
            TMODELWARN;
            return 1;
        }
    } */
    return 0;
}

// процедура добавления к модели записи
// в slvtble находится имя подчинённой таблицы
// в position - уровень, в который добавляем, в id - ИД элемента, по которому отбираются данные (id<tble>)

/*int TreeModel::addTreeSlvItem(int position, QString id)
{
    int i;
    QString tmps;
    QStringList tmpsl;
    bool HaveChildren = false;

    tmpsl = slvtble.split(".");
    // считываем все данные из таблицы
    QSqlQuery get_child_from_db2 (sqlc.GetDB(tmpsl.at(0)));
    tmps = "SELECT ";
    for (i = 0; i < slvtblefields.count(); i++)
        tmps += "`" + slvtblefields.at(i) + "`,";
    tmps = tmps.left(tmps.size()-1); // убираем запятую
    tmps += " FROM `"+tmpsl.at(1)+"` WHERE `id"+catlist.at(1)+"`=\""+id+"\" AND `deleted`=0 ORDER BY `id"+tmpsl.at(1)+"` ASC;";
    get_child_from_db2.exec(tmps);

    while (get_child_from_db2.next())
    {
        HaveChildren = true;
        tmpsl.clear();
        for (i = 0; i < slvtblefields.size(); i++)
            tmpsl << tfl.idtov(slvtblelinks.at(i), get_child_from_db2.value(i).toString()); // установка элемента дерева в соответствии с links
        tmpsl.replace(0,QString("%1").arg(tmpsl.value(0).toInt(0), 7, 10, QChar('0')));
        additemtotree(position, tmpsl, 0);
    }
    if (HaveChildren)
        return 0;
    else
        return -1;
}*/

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
    RootIDs.push("0");
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
