#include "../inc/s_aitemmodel.h"
#include "../inc/publicclass.h"
#include "../inc/s_sql.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>
#include <QVector>
#include <QColor>
#include <QFont>
#include <QIcon>

s_aitemmodel::s_aitemmodel(QSqlDatabase db1, QString tble1, QSqlDatabase db2, QString tble2, bool linksEnabled, QObject *parent) :
    QAbstractItemModel(parent)
{
    setupUI();
    result = Setup (db1, tble1, db2, tble2, linksEnabled);
}

s_aitemmodel::s_aitemmodel(QList<QStringList> sl, QList<int> setlist, QObject *parent) :
    QAbstractItemModel(parent)
{
    setupUI();
    this->setlist = setlist;
    result = Setup (sl);
}

s_aitemmodel::s_aitemmodel(QStringList sl1, QStringList sl2, QObject *parent) :
    QAbstractItemModel(parent)
{
    setupUI();
    result = Setup (sl1, sl2);
}

s_aitemmodel::~s_aitemmodel()
{
    delete rootItem;
}

int s_aitemmodel::columnCount(const QModelIndex &/*parent*/) const
{
    return rootItem->columnCount();
}

QVariant s_aitemmodel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    s_aitem *item = getItem(index);

    QVariant value = item->data(index.column());

    QFont font = item->font();
    if (role == Qt::FontRole)
    {
        if (rowCount(index) > 0)
            font.setBold(true);
        return QVariant::fromValue(QFont(font));
    }
    else if (role == Qt::ForegroundRole)
        return QVariant::fromValue(QColor(item->color()));
    else if (role == Qt::DisplayRole)
        return item->data(index.column());
    else if ((role == Qt::DecorationRole) && (index.column() == 0))
    {
        if (rowCount(index) != 0)
        {
            if (expandedIndexes.contains(index))
                item->setIcon(1);
            else
                item->setIcon(0);
        }
        else
            item->setIcon(2);
        return icons[item->icon()];
    }
    else if ((role == Qt::EditRole) && isEditable)
        return item->data(index.column());
    else
        return QVariant();

}

bool s_aitemmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || !isEditable)
        return false;

    s_aitem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

s_aitem *s_aitemmodel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        s_aitem *item = static_cast<s_aitem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

Qt::ItemFlags s_aitemmodel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if (isEditable)
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    else
        return QAbstractItemModel::flags(index);
}

QVariant s_aitemmodel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex s_aitemmodel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    s_aitem *parentItem = getItem(parent);

    s_aitem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool s_aitemmodel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool s_aitemmodel::insertRows(int position, int rows, QColor color, QFont font, const QModelIndex &parent)
{
    s_aitem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount(), color, font);
    endInsertRows();

    return success;
}

QModelIndex s_aitemmodel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    s_aitem *childItem = getItem(index);
    s_aitem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool s_aitemmodel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    if (!isEditable)
        return false;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool s_aitemmodel::removeRows(int position, int rows, const QModelIndex &parent)
{
    s_aitem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int s_aitemmodel::rowCount(const QModelIndex &parent) const
{
    s_aitem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool s_aitemmodel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

// добавление в список открытых корней дерева по событию expanded от view

void s_aitemmodel::addExpandedIndex(const QModelIndex &index)
{
    if (!expandedIndexes.contains(index))
        expandedIndexes.append(index);
}

// удаление из списка открытых корней дерева по событию collapsed от view

void s_aitemmodel::removeExpandedIndex(const QModelIndex &index)
{
    int idx = expandedIndexes.indexOf(index);
    if (idx != -1)
        expandedIndexes.removeAt(idx);
}

void s_aitemmodel::setupUI()
{
    // color and font sets:
    // 0 = обычный текст (чёрный, нежирный)
    // 1 = тревога (красный, жирный)
    // 2 = средний уровень (синий, жирный)
    // 3 = обычный текст с выделением (тёмно-красный, нежирный)
    // 4 = слабый уровень (зелёный, жирный)
    // 5 = неактивный (серый, нежирный)

    QFont fontB = QFont("MS Sans Serif", -1, QFont::Bold);
    QFont fontN = QFont("MS Sans Serif", -1, QFont::Normal);
    fonts[0] = fonts[3] = fonts[5] = fontN;
    fonts[1] = fonts[2] = fonts[4] = fontB;

    colors[0] = Qt::black;
    colors[1] = Qt::red;
    colors[2] = Qt::blue;
    colors[3] = Qt::darkRed;
    colors[4] = Qt::darkGreen;
    colors[5] = Qt::gray;

    icons[0] = QIcon(":/pic/Pic/book-angle.png");
    icons[1] = QIcon(":/pic/Pic/book-open.png");
    icons[2] = QIcon(":/pic/Pic/hr.png");

    expandedIndexes.clear();
}

// процедура инициализации модели данными из списка и построение таблицы

int s_aitemmodel::Setup(QList<QStringList> sl)
{
    QStringList tmpsl;
    int i, j;
    tmpsl.clear(); //
    for (i = 0; i < sl.size(); i++) //
        tmpsl << " "; //
    rootItem = new s_aitem(tmpsl, QColor(0,0,0), QFont("MS Sans Serif")); // устанавливаем заголовок (корневой элемент)
    parents << rootItem;
    indentations << 0;
    for (i = 0; i < sl.at(0).size(); i++)
    {
        tmpsl.clear();
        for (j = 0; j < sl.size(); j++)
            tmpsl << sl[j].at(i);
        int set = (i < setlist.size())?setlist.at(i):0;
        additemtotable(tmpsl, set);
    }
    return 0;
}

// процедура инициализации модели данными из двух взаимоподчинённых списков и построение таблицы

int s_aitemmodel::Setup(QStringList sl1, QStringList sl2)
{
    QStringList tmpsl;
    tmpsl << " ";
    rootItem = new s_aitem(tmpsl, QColor(0,0,0), QFont("MS Sans Serif")); // устанавливаем заголовок (корневой элемент)
    parents << rootItem;
    indentations << 0;
    int j = 0;
    for (int i = 0; i < sl1.size(); i++)
    {
        additemtotree0(0, sl1.at(i), 0);
        while (sl2.at(j) != "") // разделитель принадлежности к разным родителям из sl1 в списке sl2 - пустая строка
        {
            additemtotree0(1, sl2.at(j), 0);
            j++;
        }
        j++; // переход через пустую строку
    }
    return 0;
}

// процедура инициализации модели данными из таблиц и построение дерева или таблицы в зависимости от наличия полей alias и idalias
// linksEnabled - параметр, отвечающий за представление полей-ссылок значениями по ИД из подчинённой таблицы (true)
// если false -

int s_aitemmodel::Setup(QSqlDatabase db1, QString tble1, QSqlDatabase db2, QString tble2, bool linksEnabled)
{
    this->db1 = db1;
    this->tble1 = tble1;
    if (tble2 != "")
    {
        this->db2 = db2;
        this->tble2 = tble2;
        twodb = true;
    }
    else
        twodb = false;
    int res = getFieldsAndAliases(linksEnabled);
    if (res) return res;
    rootItem = new s_aitem(dbaliases, QColor(0,0,0), QFont("MS Sans Serif")); // устанавливаем заголовок (корневой элемент)
    parents << rootItem;
    indentations << 0;
    // теперь в db1fields содержатся названия колонок в SQL, а в db1aliases - их описания на русском языке
    isEditable = true;// необходимо, чтобы заполнить дерево
    // строим дерево в модели model
    if (isTree) addtreechild1("0"); // начинаем с корневых элементов
    else addtable();
    return 0;
}

// считываем имена полей и поясняющую строку из таблицы справочника. Её отличительная особенность - id<имя_справочника> = 1

int s_aitemmodel::getFieldsAndAliases(bool linksEnabled)
{
    QString tmpString;
    int res = 0;
    position = 0; // текущий уровень иерархии - 0, т.е. корневые элементы
// получаем имена полей в таблице
    db1fields = getfields(db1, tble1, res);
// если нет поля а-ля idsyslist, значит, таблица неполноценная, и работать с ней нельзя (невозможно получить запись №1 о соответствии полей таблицы их реальным наименованиям)
    if (res) return res;
    dbaliases = getaliases(db1fields, db1, tble1, linksEnabled); // вызов процедуры только для того, чтобы убрать лишние столбцы (с полем "" в строке 1)
    // теперь получаем строку соответствия полей их наименованиям. Для случая объединения двух таблиц - только по db2
    if (twodb)
    {
        // получаем имена полей в таблице
            db2fields = getfields(db2, tble2, res);
        // если нет поля а-ля idsyslist, значит, таблица неполноценная, и работать с ней нельзя (невозможно получить запись №1 о соответствии полей таблицы их реальным наименованиям)
            if (res) return (10+res); // идентификатор проблем с таблицей №2
        dbaliases = getaliases(db2fields, db2, tble2, linksEnabled);
    }
    else
        dbaliases = getaliases(db1fields, db1, tble1, linksEnabled);
    if (dbaliases.isEmpty())
        return 21; // отсутствует первая строка (наименований полей)
    // убираем системные колонки из отображения
    dbaliases.removeAll("x");
    dbaliases.removeAll("m");
    // подбираем индексы полей wh,alt,sys,buh (прав доступа), если они имеются
    permissionsfields = db1fields.indexOf("access");
    // если не найдено прав доступа в таблице, стало быть, их и нет, и нечего их проверять потом
    if (permissionsfields == -1)
        noCredentials = true;
    else
        noCredentials = false;
    return 0;
}

// собираем в sl имена полей из выбранной таблицы и проверяем на достоверность и признаки дерева (наличие полей alias и idalias)

QStringList s_aitemmodel::getfields(QSqlDatabase db, QString tble, int &res)
{
    QStringList sl;
    int i;

    sl = sqlc.getcolumnsfromtable(db, tble);
    if (sl.isEmpty())
    {
        res = 1; // ошибка обращения к таблице (нет таблицы)
        return sl;
    }
// убираем лишние имена столбцов
    sl.removeAll("date");
    sl.removeAll("idPers");
    sl.removeAll("deleted");
// если нет поля а-ля idsyslist, значит, таблица неполноценная, и работать с ней нельзя (невозможно получить запись №1 о соответствии полей таблицы их реальным наименованиям)
    if (sl.contains("id" + tble));
    else
    {
        res = 2; // таблица не содержит столбца ИД
        return sl;
    }
// наличие полей alias и idalias свидетельствует о том, что таблица представляет собой дерево
    if (sl.contains("alias") && sl.contains("idalias"))
    {
        isTree = true;
        // перетасовываем столбцы, чтобы первым шёл ИД, вторым alias, а третьим - idalias. Остальные поля - как придётся
        if (sl.at(1) != "alias")
        {
            i = sl.indexOf("alias");
            sl.move(i, 1);
        }
        if (sl.at(2) != "idalias")
        {
            i = sl.indexOf("idalias");
            sl.move(i, 2);
        }
    }
    else if (tble != tble2) // если обрабатываем вторую db, то isTree надо оставить выставленной по первой db
        isTree = false;
    if (sl.at(0) != "id" + tble)
    {
        i = sl.indexOf("id" + tble);
        sl.move(i, 0);
    }
    res = 0;
    return sl;
}

// собираем наименования полей из БД

QStringList s_aitemmodel::getaliases(QStringList &dbfields, QSqlDatabase db, QString tble, bool linksEnabled)
{
    QStringList sl, tmpsl, tmpsl2, al, bl;
    QString tmpString, tmpString2;
    QSqlQuery get_header_from_db(db);
    QSqlQuery tmpQuery;
    QSqlDatabase tmpdb;
    QString tmptble;
    int i, j;

    tmpsl = sqlc.getvaluesfromtablebyid(db, tble, dbfields, "1"); // собираем признаки из первой строки таблицы
    if (tmpsl.isEmpty())
        return tmpsl; // нет строки с номером 1, выход
    tmpsl2 = sqlc.getvaluesfromtablebyid(db, tble, dbfields, "2"); // собираем наименования полей из второй строки таблицы
    if (tmpsl2.isEmpty())
        return tmpsl2; // нет строки с номером 2, выход
    // записываем наименования полей в список db1aliases, игнорируя по пути те столбцы, для которых не задано наименование (пустая строка)
    i = 0;
    j = 0;
    sl.clear();
    while (i < dbfields.size())
    {
        tmpString = tmpsl.at(j);
        if (tmpString != "")
        {
            // проверка на ссылку на другую таблицу
            if (tmpString.mid(0, 3) == "pc.")
            {
                if (linksEnabled)
                    sl.insert(i, tmpString);
                else
                {
                    dbfields.removeAt(i);
                    i--;
                }
            }
            else
                sl.insert(i, tmpString);
            i++;
        }
        else
            dbfields.removeAt(i);
        j++;
    }
    sl.replace(0, "ИД"); // Первая колонка всегда идентификатор
    return sl;
}

void s_aitemmodel::addtreechild1(QString idparent)
{
    QString tmpString;
    QStringList tmpStringlist;
    int i;
    int set = (twodb) ? 2 : 0;

    // считываем все данные из таблицы
    QSqlQuery get_child_from_db1 (db1);
    tmpString = "SELECT ";
    for (i = 0; i < db1fields.count(); i++)
        tmpString += "`" + db1fields[i] + "`,";
    tmpString = tmpString.left(tmpString.size()-1); // убираем запятую
    tmpString += " FROM `" + tble1 + "` WHERE `idalias`=\"" + idparent + "\" AND `id" + tble1 + "`>2 AND `deleted`=0 ORDER BY `id" + tble1 + "` ASC;";
    get_child_from_db1.exec(tmpString);

// увеличиваем уровень дерева
    position++;
    if (idparent == "0") position = 0; // для корневых элементов position д.б. равен нулю
// строим дерево в модели model
    while (get_child_from_db1.next())
    {
        if ((noCredentials) || (get_child_from_db1.value(permissionsfields).toString().toLong(0, 16) & pc.access))
        {
            tmpStringlist.clear();
            for (i = 0; i < db1fields.size(); i++)
            {
                if (db1fields.at(i)!="access")
                {
                    tmpString = db1fields.at(i);
                    if (tmpString.mid(0, 3) == "pc.")
                    {
                        QSqlDatabase db = sqlc.getdb(tmpString.mid(3, 3));
                        if (db.isValid())
                        {
                            tmpString = sqlc.getvaluefromtablebyid(db, tmpString.right(tmpString.size()-7), \
                                    "alias", get_child_from_db1.value(i).toString());
                            tmpStringlist << tmpString;
                        }
                    }
                    else
                        tmpStringlist << get_child_from_db1.value(i).toString();
                }
            }
            tmpStringlist.replace(0,QString("%1").arg(tmpStringlist.value(0).toInt(0), 7, 10, QChar('0')));
            additemtotree(position, tmpStringlist, set);
            addtreechild1 (get_child_from_db1.value(0).toString()); // в качестве аргумента функции используется индекс поля idalias
        }
    }

    if (twodb) addtreechild2(position, idparent); // добавляем таблицу из подчинённой таблицы
    position--; // после добавления всех детишек уровень понижается
}

// в подчиненной таблице не должно быть деревьев

void s_aitemmodel::addtreechild2(int position, QString idparent)
{
    QString tmpString;
    int i;
    QStringList tmpStringlist;
    // считываем все данные из таблицы
    QSqlQuery get_child_from_db2 (db2);
    tmpString = "SELECT ";
    for (i = 0; i < db2fields.count(); i++)
        tmpString += "`" + db2fields[i] + "`,";
    tmpString = tmpString.left(tmpString.size()-1); // убираем запятую
    tmpString += " FROM `" + tble2 + "` WHERE `id" + tble1 + "`=\"" + idparent + "\" AND `deleted`=0 ORDER BY `id" + tble1 + "` ASC;";
    get_child_from_db2.exec(tmpString);

    while (get_child_from_db2.next())
    {
        tmpStringlist.clear();
        for (i = 0; i < db2fields.size(); i++)
        {
            if (db2fields.at(i)!="access")
            {
                tmpString = db2fields.at(i);
                if (tmpString.mid(0, 3) == "pc.")
                {
                    QSqlDatabase db = sqlc.getdb(tmpString.mid(3, 3));
                    if (db.isValid())
                    {
                        tmpString = sqlc.getvaluefromtablebyid(db, tmpString.right(tmpString.size()-7), \
                                "alias", get_child_from_db2.value(i).toString());
                        tmpStringlist << tmpString;
                    }
                }
                else
                    tmpStringlist << get_child_from_db2.value(i).toString();
            }
        }
        tmpStringlist.replace(0,QString("%1").arg(tmpStringlist.value(0).toInt(0), 7, 10, QChar('0')));
        additemtotree(position, tmpStringlist, 3);
    }
}

void s_aitemmodel::additemtotree(int position, QStringList sl, int set)
{
    QColor color;
    QFont font;

    if (!set)
    {
//        color = (sl.at(0).toInt() < 2000) ? colors[2] : colors[0];
//        font = (sl.at(0).toInt() < 2000) ? fonts[2] : fonts[0];
    }
    else
    {
        color = colors[set];
        font = fonts[set];
    }

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

    s_aitem *parent = parents.last();
    parent->insertChildren(parent->childCount(), 1, rootItem->columnCount(), color, font);
    for (int column = 0; column < sl.size(); ++column)
        parent->child(parent->childCount() - 1)->setData(column, sl[column]);
}

void s_aitemmodel::addtable()
{
    QString tmpString;
    int i;
    QStringList tmpStringlist;

    // считываем все данные из таблицы
    QSqlQuery get_child_from_db1 (db1);
    tmpString = "SELECT ";
    for (i = 0; i < db1fields.count(); i++)
        tmpString += "`" + db1fields[i] + "`,";
    tmpString = tmpString.left(tmpString.size()-1); // убираем запятую
    tmpString += " FROM `" + tble1 + "` WHERE `id" + tble1 + "`>2 AND `deleted`=0 ORDER BY `id" + tble1 + "` ASC;";
    get_child_from_db1.exec(tmpString);

    // строим таблицу
    while (get_child_from_db1.next())
    {
        if ((noCredentials) || (get_child_from_db1.value(permissionsfields).toString().toLong(0, 16) & pc.access))
        {
            tmpStringlist.clear();
            for (i = 0; i < db1fields.count(); i++)
            {
                if (db1fields.at(i)!="access")
                {
                    tmpString = db1fields.at(i);
                    if (tmpString.mid(0, 3) == "pc.")
                    {
                        QSqlDatabase db = sqlc.getdb(tmpString.mid(3, 3));
                        if (db.isValid())
                        {
                            tmpString = sqlc.getvaluefromtablebyid(db, tmpString.right(tmpString.size()-7), \
                                    "alias", get_child_from_db1.value(i).toString());
                            tmpStringlist << tmpString;
                        }
                    }
                    else
                        tmpStringlist << get_child_from_db1.value(i).toString();
                }
            }
            tmpStringlist.replace(db1fields.indexOf("id"+tble1),QString("%1").arg(tmpStringlist.value(db1fields.indexOf("id"+tble1)).toInt(0), 7, 10, QChar('0')));
            additemtotable(tmpStringlist, 0);
        }
    }

    if (twodb)
    {
        // считываем все данные из таблицы
        QSqlQuery get_child_from_db2 (db2);
        tmpString = "SELECT ";
        for (i = 0; i < db2fields.count(); i++)
            tmpString += "`" + db2fields[i] + "`,";
        tmpString = tmpString.left(tmpString.size()-1); // убираем запятую
        tmpString += " FROM `" + tble2 + "` WHERE `id" + tble2 + "`>2 AND `deleted`=0 ORDER BY `id" + tble2 + "` ASC;";
        get_child_from_db2.exec(tmpString);

        // строим таблицу
        while (get_child_from_db2.next())
        {
            tmpStringlist.clear();
            for (i = 0; i < db2fields.count(); i++)
            {
                if (db2fields.at(i)!="access")
                {
                    tmpString = db2fields.at(i);
                    if (tmpString.mid(0, 3) == "pc.")
                    {
                        QSqlDatabase db = sqlc.getdb(tmpString.mid(3, 3));
                        if (db.isValid())
                        {
                            tmpString = sqlc.getvaluefromtablebyid(db, tmpString.right(tmpString.size()-7), \
                                    "alias", get_child_from_db2.value(i).toString());
                            tmpStringlist << tmpString;
                        }
                    }
                    else
                        tmpStringlist << get_child_from_db2.value(i).toString();
                }
            }
            tmpStringlist.replace(db2fields.indexOf("id"+tble2), QString("%1").arg(tmpStringlist.value(db2fields.indexOf("id"+tble2)).toInt(0), 7, 10, QChar('0')));
            additemtotable(tmpStringlist, 0);
        }
    }
}

void s_aitemmodel::additemtotable(QStringList sl, int set)
{
    QColor color;
    QFont font;

    if (!set)
    {
        color = (sl.at(0).toInt() < 2000) ? colors[2] : colors[0];
        font = (sl.at(0).toInt() < 2000) ? fonts[2] : fonts[0];
    }
    else
    {
        color = colors[set];
        font = fonts[set];
    }

    s_aitem *parent = parents.last();
    parent->insertChildren(parent->childCount(), 1, rootItem->columnCount(), color, font);
    for (int column = 0; column < sl.size(); ++column)
        parent->child(parent->childCount() - 1)->setData(column, sl[column]);
}

void s_aitemmodel::additemtotree0(int position, QString str, int set)
{
    QColor color;
    QFont font;

    if (!set)
    {
//        color = (sl.at(0).toInt() < 2000) ? colors[2] : colors[0];
//        font = (sl.at(0).toInt() < 2000) ? fonts[2] : fonts[0];
    }
    else
    {
        color = colors[set];
        font = fonts[set];
    }

    position++;
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

    s_aitem *parent = parents.last();
    parent->insertChildren(parent->childCount(), 1, rootItem->columnCount(), color, font);
    parent->child(parent->childCount() - 1)->setData(0, str);
}
