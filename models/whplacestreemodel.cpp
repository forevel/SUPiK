#include "whplacestreemodel.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"

#include <QSqlRecord>

WhPlacesTreeModel::WhPlacesTreeModel()
{

}

WhPlacesTreeModel::~WhPlacesTreeModel()
{
    ClearModel();
}

WhPlacesTreeModel::WhPlacesTreeItem *WhPlacesTreeModel::Data(int Index)
{
    if (Items.keys().contains(Index))
        return Items.value(Index);
    else
        return NULL;
}

void WhPlacesTreeModel::SetData(int Index, WhPlacesTreeItem *Value)
{
    if (Items.keys().contains(Index))
    {
        WhPlacesTreeItem *item = Items.value(Index);
        delete item;
    }
    Items[Index] = Value;
}

int WhPlacesTreeModel::Index(QString Alias)
{
    for (int i=0; i<Items.size(); i++)
    {
        if (Items.value(Items.keys().at(i))->Alias == Alias)
            return i;
    }
    return -1;
}

/*int WhPlacesTreeModel::InsertChild(int ParentIndex, WhPlacesTreeItem Value)
{

}
*/
// процедура инициализации модели данными из таблицы table в tablefields и построение дерева по полям alias и idalias

int WhPlacesTreeModel::Load(int Index)
{
    ClearModel();
    QStringList fl = QStringList() << "table" << "tablefields";
    vl = sqlc.GetMoreValuesFromTableByField(sqlc.GetDB("sup"), "tablefields", fl, "tablename", "Склады размещение_полн", "fieldsorder", true);
    if (sqlc.result)
        return 1;
    if (vl.at(2).at(1) != "idalias")
        return 1;
    if (vl.at(1).at(1) != "alias")
        return 1;
    catlist = vl.at(2).at(0).split("."); // catlist - таблица, из которой брать категории
    vlsize = vl.at(1).size();
    int res = Build(Index);
    if (res)
        return 1;
    return 0;
}

// процедура построения дерева
// на входе catlist (ссылка на таблицу категорий с полем idalias) и slvtble (название таблицы в chooselists, из которой брать записи категорий)

int WhPlacesTreeModel::Build(int Index)
{
    int res;
    QStringList tmpStringList;
    QString tmpString;
    // считываем все данные из таблицы
    QSqlQuery get_child_from_db1 (sqlc.GetDB(catlist.at(0)));
    tmpString = "SELECT ";
    for (int i=0; i<vlsize; i++)
        tmpString += "`" + vl.at(1).at(i) + "`,";
    tmpString += "` FROM `"+catlist.at(1)+"` WHERE `idalias`=\""+Index+"\" AND `deleted`=0 ORDER BY `id"+catlist.at(1)+"` ASC;";
    get_child_from_db1.exec(tmpString);
    // строим дерево в модели model
    while (get_child_from_db1.next())
    {
        tmpStringList.clear();
        for (int i=0; i<get_child_from_db1.record().count(); i++)
            tmpStringList << get_child_from_db1.value(i).toString();
        AddItem(tmpStringList);
        if (get_child_from_db1.record().count() > 1) // просто чтобы не нарваться на exception
        {
            int tmpi = get_child_from_db1.value(2).toInt();
            if (tmpi != -1)
                res = Build(tmpi); // в качестве аргумента функции используется индекс поля idalias
        }
        if (res)
            return 1;
    }
    return 0;
}

void WhPlacesTreeModel::AddItem(QStringList sl)
{
    if (sl.size() < WHITEMSIZE)
        return;
    WhPlacesTreeItem *item = new WhPlacesTreeItem;
    item->Alias = sl.at(1);
    item->Description = sl.at(3);
    item->IdAlias = sl.at(2).toInt();
    item->Name = sl.at(4);
    item->WhNum = sl.at(6).toInt();
    item->WhPlaceTypeID = sl.at(7).toInt();
    item->WhID = sl.at(5).toInt();
    SetData(sl.at(0).toInt(), item);
}

void WhPlacesTreeModel::ClearModel()
{
    while (!Items.isEmpty())
    {
        int key = Items.keys().at(0);
        WhPlacesTreeItem *item = Items.value(key);
        delete item;
        Items.remove(key);
    }
}

int WhPlacesTreeModel::Save()
{
    // аналогично Load пробегаем по всем ветвям
    // ищем сначала элемент с такими
    return 0;
}

QList<int> WhPlacesTreeModel::Children(int Index)
{
    QList<int> sl;
    for (int i=0; i<Items.size(); i++)
    {
        int tmpi = Items.keys.at(i);
        if (Items.value(tmpi)->IdAlias == Index)
            sl << tmpi;
    }
    return sl;
}
