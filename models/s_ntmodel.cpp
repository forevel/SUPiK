#include "s_ntmodel.h"
#include "../gen/publicclass.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>

s_ntmodel::s_ntmodel(QObject *parent) :
    QAbstractItemModel(parent)
{
    // color and font sets:
    // 0 = обычный текст (чёрный, нежирный)
    // 1 = тревога (красный, жирный)
    // 2 = средний уровень (синий, жирный)
    // 3 = обычный текст с выделением (тёмно-красный, нежирный)
    // 4 = слабый уровень (зелёный, жирный)
    // 5 = неактивный (серый, нежирный)
    // 6 = совсем слабый уровень (зелёный, нежирный)

    colors[0] = Qt::black;
    colors[1] = Qt::red;
    colors[2] = Qt::blue;
    colors[3] = Qt::darkRed;
    colors[4] = Qt::darkGreen;
    colors[5] = Qt::gray;
    colors[6] = Qt::darkGreen;
    QFont fontB = QFont("MS Sans Serif", -1, QFont::Bold);
    QFont fontN = QFont("MS Sans Serif", -1, QFont::Normal);
    fonts[0] = fonts[3] = fonts[5] = fonts[6] = fontN;
    fonts[1] = fonts[2] = fonts[4] = fontB;
    icons[0] = QIcon(":/res/hr.png");
    icons[1] = QIcon(":/res/ok.png");
    icons[2] = QIcon(":/res/cross.png");
    icons[3] = QIcon(":/res/book-angle.png");
    icons[4] = QIcon(":/res/book-open.png");
    rootItem = new s_ntitem;
    parents.clear();
    indentations.clear();
    position = 0;
    parents << rootItem;
    indentations << 0;
    expandedIndexes.clear();
}

s_ntmodel::~s_ntmodel()
{
    delete rootItem;
}

int s_ntmodel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

QVariant s_ntmodel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QString();
    s_ntitem *item=getItem(index);
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
        return item->data(index.column());
    else if (role == Qt::ForegroundRole)
        return QVariant::fromValue(QColor(item->color(index.column())));
    else if (role == Qt::FontRole)
    {
        QFont font = item->font(index.column());
        return QVariant::fromValue(QFont(font));
    }
    else if ((role == Qt::DecorationRole) && (index.column() == 0))
        return QVariant::fromValue(QIcon(item->icon(index.column())));
    else
        return QVariant();

}

bool s_ntmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        s_ntitem *item=getItem(index);
        if (role == Qt::EditRole)
        {
            bool result = item->setData(index.column(), value.toString());
            if (result)
                emit dataChanged(index, index);
            return result;
        }
        else if (role == Qt::ForegroundRole)
        {
            item->setColor(index.column(), value.value<QColor>());
            return true;
        }
        else if (role == Qt::FontRole)
        {
            item->setFont(index.column(), value.value<QFont>());
            return true;
        }
/*        else if (role == Qt::UserRole)
        {
            bool result = item->setLinksData(index.column(), value.toString());
            return result;
        } */
        else
            return false;
    }
    else
        return false;
}

Qt::ItemFlags s_ntmodel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
/*    s_ntitem *item=getItem(index);
    fieldformat ff = getFFfromLinks(item->linksdata(index.column()));
    if ((ff.delegate == FD_SIMPLE) || (ff.delegate == FD_DISABLED))
        return Qt::NoItemFlags;
    else */
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;// | Qt::ItemIsEditable;
}

QVariant s_ntmodel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}

QModelIndex s_ntmodel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
    s_ntitem *parentItem=getItem(parent);
    s_ntitem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

/*bool s_ntmodel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;
    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();
    return success;
}*/

bool s_ntmodel::insertRows(int position, int rows, const QModelIndex &parent)
{
    bool success;
    s_ntitem *parentItem=getItem(parent);
    beginInsertRows(parent, position, position + rows - 1);
    for (int i = 0; i < rows; i++)
    {
        s_ntitem *child = parentItem->insertChild(position, rootItem->columnCount());
        if (child)
        {
            parentItem->setIcon(0, icons[3]); // у родителя появился потомок - стало быть устанавливаем "закрытую книгу" в значке
            child->setIcon(0, icons[0]); // у потомка - просто линия, пока он не стал сам родителем
            success = true;
        }
        else
            success = false;
    }
    endInsertRows();
    return success;
}

QModelIndex s_ntmodel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();
    s_ntitem *childItem=getItem(index);
    s_ntitem *parentItem = childItem->parent();
    if (parentItem == rootItem)
        return QModelIndex();
    return createIndex(parentItem->row(), 0, parentItem);
}

bool s_ntmodel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;
    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();
    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());
    return success;
}

bool s_ntmodel::removeRows(int position, int rows, const QModelIndex &parent)
{
    s_ntitem *parentItem=getItem(parent);
    bool success = true;
    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();
    return success;
}

int s_ntmodel::rowCount(const QModelIndex &parent) const
{
    s_ntitem *parentItem=getItem(parent);
    return parentItem->childCount();
}

bool s_ntmodel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;
    bool result = rootItem->setData(section, value.toString());
    if (result)
        emit headerDataChanged(orientation, section, section);
    return result;
}

// свои методы

QModelIndex s_ntmodel::getIndex(s_ntitem *item) const
{
    if (item == rootItem)
        return QModelIndex();
    QModelIndex parentIdx;
    s_ntitem* parentItem = item->parent();
    if (parentItem == rootItem)
        parentIdx = QModelIndex();
    else
        parentIdx = index(parentItem->row(), 0, QModelIndex());
    return index(item->row(), 0, parentIdx);
}

s_ntitem *s_ntmodel::getItem(const QModelIndex &index) const
{
    s_ntitem *item;
    if (index.isValid())
        item = static_cast<s_ntitem*>(index.internalPointer());
    else
        item = rootItem;
    return item;
}

void s_ntmodel::addRow(int position, int cfset, QModelIndex &parent)
{
    insertRows(position, 1, parent);
    s_ntitem *parentItem = getItem(parent);
    s_ntitem *child = parentItem->child(position);
    child->setColor(0, colors[cfset]);
    child->setFont(0, fonts[cfset]);
}

// добавление в список открытых корней дерева по событию expanded от view

void s_ntmodel::addExpandedIndex(const QModelIndex &index)
{
    if (index.isValid())
    {
        if (!expandedIndexes.contains(index))
        {
            s_ntitem *item = getItem(index);
            if (item->childCount())
            {
                item->setIcon(0, icons[4]);
                expandedIndexes.append(index);
            }
        }
    }
}

// удаление из списка открытых корней дерева по событию collapsed от view

void s_ntmodel::removeExpandedIndex(const QModelIndex &index)
{
    if (index.isValid())
    {
        int idx = expandedIndexes.indexOf(index);
        if (idx != -1)
        {
            expandedIndexes.removeAt(idx);
            s_ntitem *item=getItem(index);
            item->setIcon(0, icons[3]);
        }
    }
}

// процедура инициализации модели данными из таблицы table = supik.chooselists и построение дерева по полям alias и idalias

int s_ntmodel::Setup(QString table)
{
    ClearModel();
    // 1. взять столбцы tablefields из tablefields, где tablename=table
    // 2. найти среди них столбцы <db>.<tble>.alias и <db>.<tble>.idalias. Если нет - это не дерево, выход
    // 3. взять значения столбцов alias и idalias из таблицы <db>.<tble>
    // 4. построить по ним дерево

    // 1
    int i;
    QStringList fl = QStringList() << "table" << "tablefields" << "header";
    vl = sqlc.GetMoreValuesFromTableByField(sqlc.GetDB("sup"), "tablefields", fl, "tablename", table, "fieldsorder", true);
    if (sqlc.result)
    {
        WARNMSG(PublicClass::ER_NTMODEL, __LINE__);
        return 1;
    }
    // 2
    int idalpos=-1;
    for (i = 0; i < vl.size(); i++)
    {
        if (vl.at(i).at(1) == "idalias")
        {
            idalpos = i;
            break;
        }
    }
    if (idalpos == -1)
        return PublicClass::ER_NTMODEL; // не найдено поле idalias
    // 3
    catlist = vl.at(idalpos).at(0).split("."); // catlist - таблица, из которой брать категории
    vl.removeAt(idalpos); // не включать ссылку на категорию в заголовок
    for (i = 0; i < vl.size(); i++)
        setHeaderData(i, Qt::Horizontal, vl.at(i).at(2), Qt::EditRole);
    // 4
    int res = BuildTree("0", false);
    if (res)
    {
        WARNMSG(PublicClass::ER_NTMODEL, __LINE__);
        return 1;
    }
    return 0;
}

// процедура инициализации модели данными из двух таблиц и построение дерева
// из catdb.cattble берутся "родители", из slvtble - "дети", причём в slvtble должно присутствовать
// поле `id<cattble>`. Slvtble = имя таблицы в supik.models

int s_ntmodel::Setup(QString maintble, QString slvtble)
{
    ClearModel();
    // 1. взять столбцы tablefields из tablefields, где tablename=table и headers=ИД_а
    // 4. построить дерево

    // 1
    int i;
    QStringList tmpsl = tfl.tablefields(maintble, "ИД_а"); // взять table,tablefields,links из tablefields, где таблица maintble и заголовок ИД_а
    if (tfl.result) // нет поля idalias в таблице - это не дерево!
    {
        WARNMSG(PublicClass::ER_NTMODEL, __LINE__);
        return 1;
    }
    catlist = tmpsl.at(0).split("."); // catlist - таблица, из которой брать категории
    QStringList headers = tfl.tableheaders(slvtble);
    if (tfl.result)
    {
        WARNMSG(PublicClass::ER_NTMODEL, __LINE__);
        return 1;
    }
    if (headers.isEmpty()) // нет заголовков в подчинённой таблице
    {
        WARNMSG(PublicClass::ER_NTMODEL, __LINE__);
        return 1;
    }
    for (i = 0; i < headers.size(); i++)
    {
        setHeaderData(i, Qt::Horizontal, headers.at(i), Qt::EditRole);
        tmpsl = tfl.tablefields(slvtble, headers.at(i)); // взяли table,tablefields,links из tablefields для подчинённой таблицы и данного заголовка
        if (tfl.result) // что-то не так с подчинённой таблицей нет такого заголовка
        {
            WARNMSG(PublicClass::ER_NTMODEL, __LINE__);
            return 1;
        }
        if (tmpsl.size() > 2)
        {
            slvtblelinks << tmpsl.at(2);
            slvtblefields << tmpsl.at(1);
        }
        else // нет почему-то в возвращённом результате второго элемента
        {
            WARNMSG(PublicClass::ER_NTMODEL, __LINE__);
            return 1;
        }
    }
    this->slvtble = tmpsl.at(0); // имя таблицы - в переменную
    // 4
    int res = BuildTree("0", true);
    if (res)
    {
        WARNMSG(PublicClass::ER_NTMODEL, __LINE__);
        return 1;
    }
    return 0;
}

// процедура построения дерева
// на входе catlist (ссылка на таблицу категорий с полем idalias) и slvtble (название таблицы в chooselists, из которой брать записи категорий)

int s_ntmodel::BuildTree(QString id, bool twodb)
{
    int res;
    QStringList tmpStringList;
    QString tmpString;
    // считываем все данные из таблицы
    QSqlQuery get_child_from_db1 (sqlc.GetDB(catlist.at(0)));
    tmpString = "SELECT `alias`,`id"+catlist.at(1)+"` FROM `"+catlist.at(1)+"` WHERE `idalias`=\""+id+"\" AND `deleted`=0 ORDER BY `id"+catlist.at(1)+"` ASC;";
    get_child_from_db1.exec(tmpString);
    bool HaveChildren = false;
    // увеличиваем уровень дерева
    position++;
    if (id == "0") position = 0; // для корневых элементов position д.б. равен нулю
// строим дерево в модели model
    int set = 4;
    while (get_child_from_db1.next())
    {
        HaveChildren = true;
        tmpStringList.clear();
        tmpStringList << QString("%1").arg(get_child_from_db1.value(1).toInt(0), 7, 10, QChar('0')) << get_child_from_db1.value(0).toString();
        additemtotree(position, tmpStringList, set);
        res = BuildTree(get_child_from_db1.value(1).toString(), twodb); // в качестве аргумента функции используется индекс поля idalias
        if (res)
        {
            WARNMSG(PublicClass::ER_NTMODEL, __LINE__);
            return 1;
        }
    }
    if (twodb)
    {
        res = addTreeSlvItem(position, id); // добавляем таблицу из подчинённой таблицы
        if (!res)
            HaveChildren = true;
        else if (res != -1) // если не нет потомков, а просто ошибка
        {
            WARNMSG(PublicClass::ER_NTMODEL, __LINE__);
            return 1;
        }
    }
    position--; // после добавления всех детишек уровень понижается
    if (HaveChildren);
    else // не имеет потомков
    {
        s_ntitem *item = parents.last()->child(parents.last()->childCount()-1); // текущий элемент
        for (int i=0; i<item->columnCount(); i++)
        {
            item->setColor(i, colors[6]);
            item->setFont(i, fonts[6]);
        }
    }
    return 0;
}

// процедура добавления к модели записи
// в slvtble находится имя подчинённой таблицы
// в position - уровень, в который добавляем, в id - ИД элемента, по которому отбираются данные (id<tble>)

int s_ntmodel::addTreeSlvItem(int position, QString id)
{
    int i;
    QString tmpString;
    QStringList tmpStringlist;
    bool HaveChildren = false;

    tmpStringlist = slvtble.split(".");
    // считываем все данные из таблицы
    QSqlQuery get_child_from_db2 (sqlc.GetDB(tmpStringlist.at(0)));
    tmpString = "SELECT ";
    for (i = 0; i < slvtblefields.count(); i++)
        tmpString += "`" + slvtblefields.at(i) + "`,";
    tmpString = tmpString.left(tmpString.size()-1); // убираем запятую
    tmpString += " FROM `"+tmpStringlist.at(1)+"` WHERE `id"+catlist.at(1)+"`=\""+id+"\" AND `deleted`=0 ORDER BY `id"+tmpStringlist.at(1)+"` ASC;";
    get_child_from_db2.exec(tmpString);

    while (get_child_from_db2.next())
    {
        HaveChildren = true;
        tmpStringlist.clear();
        for (i = 0; i < slvtblefields.size(); i++)
            tmpStringlist << tfl.idtov(slvtblelinks.at(i), get_child_from_db2.value(i).toString()); // установка элемента дерева в соответствии с links
        tmpStringlist.replace(0,QString("%1").arg(tmpStringlist.value(0).toInt(0), 7, 10, QChar('0')));
        additemtotree(position, tmpStringlist, 0);
    }
    if (HaveChildren)
        return 0;
    else
        return -1;
}

void s_ntmodel::additemtotree(int position, QStringList sl, int set)
{
    QColor color=colors[set];
    QFont font=fonts[set];
    if (position > indentations.last()) {
        // The last child of the current parent is now the new parent
        // unless the current parent has no children.

        if (parents.last()->childCount() > 0) {
            parents << parents.last()->child(parents.last()->childCount()-1);
            indentations << position;
        }
    } else {
        while (position < indentations.last() && parents.count() > 0) {
            parents.pop_back();
            indentations.pop_back();
        }
    }
    s_ntitem *parent = parents.last();
    QModelIndex parentIndex = getIndex(parent);
    addRow(parent->childCount(), set, parentIndex);
    for (int column = 0; column < sl.size(); ++column)
    {
        setData(index(parent->childCount()-1, column, parentIndex), sl.at(column), Qt::EditRole);
        setData(index(parent->childCount()-1, column, parentIndex), color, Qt::ForegroundRole);
        setData(index(parent->childCount()-1, column, parentIndex), font, Qt::FontRole);
    }
}

void s_ntmodel::ClearModel()
{
    beginResetModel();
    while (rowCount() > 0)
        removeRows(0, 1);
    endResetModel();

}
