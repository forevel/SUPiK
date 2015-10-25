#include "whplacestreemodel.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"

#include <QSqlRecord>

WhPlacesTreeModel::WhPlacesTreeModel()
{
    Items[0] = NULL;
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
    Items[Index]->Id = Index;
}

void WhPlacesTreeModel::Update(int Index, WhPlacesTreeItem *Value)
{
    if (Items.keys().contains(Index))
    {
        Items[Index] = Value;
        Items[Index]->Id = Index;
    }
    else
        SetData(Index, Value);
}

// добавляем элемент с новым ИД в модель

int WhPlacesTreeModel::Insert(WhPlacesTreeItem *Value)
{
    // сначала ищем первый свободный ИД в таблице Склады размещение
    QString NewID = tfl.insert(WHPLACES);
    if (tfl.result)
        return -1;
    // затем добавляем элемент
    int tmpi = NewID.toInt();
    SetData(tmpi, Value);
    return tmpi;
}

int WhPlacesTreeModel::DeleteNew()
{
    WhPlacesTreeItem *item;
    Find(0x80, QStringList(QString::number(WHP_CREATENEW)));
    while ((item = Next()) != NULL)
    {
        tfl.Delete(WHPLACES, QString::number(item->Id));
        if (tfl.result)
            return 1;
    }
    Find(0x80, QStringList(QString::number(WHP_UPDATENEW)));
    while ((item = Next()) != NULL)
    {
        tfl.Delete(WHPLACES, QString::number(item->Id));
        if (tfl.result)
            return 1;
    }
    return 0;
}

// процедура инициализации модели данными из таблицы table в tablefields и построение дерева по полям alias и idalias
// index - ИД записи по таблице whplaces

int WhPlacesTreeModel::Load(int Index)
{
    ClearModel();
    RootIndexID = Index;
    QStringList fl = QStringList() << "table" << "tablefields";
    vl = sqlc.GetMoreValuesFromTableByField(sqlc.GetDB("sup"), "tablefields", fl, "tablename", WHPLACES, "fieldsorder", true);
    if (sqlc.result)
        return 1;
    if (vl.at(2).at(1) != "idalias")
        return 1;
    if (vl.at(1).at(1) != "alias")
        return 1;
    catlist = vl.at(0).at(0).split("."); // catlist - таблица, из которой брать категории
    vlsize = vl.size();
    int res = Build(Index, true);
    if (res)
        return 1;
    return 0;
}

// процедура построения дерева
// на входе catlist (ссылка на таблицу категорий с полем idalias) и slvtble (название таблицы в chooselists, из которой брать записи категорий)

int WhPlacesTreeModel::Build(int Index, bool root)
{
    int res;
    QStringList tmpStringList;
    QString tmpString;
    // считываем все данные из таблицы
    QSqlQuery get_child_from_db1 (sqlc.GetDB(catlist.at(0)));
    tmpString = "SELECT ";
    for (int i=0; i<vlsize; i++)
        tmpString += "`" + vl.at(i).at(1) + "`,";
    tmpString.chop(1);
    if (root)
        tmpString += " FROM `"+catlist.at(1)+"` WHERE `id"+catlist.at(1)+"`=\""+QString::number(Index)+"\" AND `deleted`=0 ORDER BY `id"+catlist.at(1)+"` ASC;";
    else
        tmpString += " FROM `"+catlist.at(1)+"` WHERE `idalias`=\""+QString::number(Index)+"\" AND `deleted`=0 ORDER BY `id"+catlist.at(1)+"` ASC;";
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
            int tmpi = get_child_from_db1.value(0).toInt();
            if (tmpi != -1)
                res = Build(tmpi, false); // в качестве аргумента функции используется индекс поля idalias
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
    item->UpdIns = WHP_UNCHANGED;
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
    // получить список заголовков по таблице WHPLACES
    QStringList fl = tfl.tableheaders(WHPLACES);
    if (tfl.result)
        return 1;
    // для каждого элемента из Items
    for (int i=0; i<Items.keys().size(); i++)
    {
        int CurKey = Items.keys().at(i);
        WhPlacesTreeItem *item = Data(CurKey);
        // если его UpdIns содержит NEW, то элемент всё равно уже есть в БД, т.к. записан процедурой Insert
        if ((item->UpdIns == WHP_CREATENEW) || (item->UpdIns == WHP_UPDATENEW) || (item->UpdIns == WHP_UPDATE))
        {
            QStringList vl = QStringList() << QString::number(item->Id) << item->Alias << QString::number(item->IdAlias) << \
                                              item->Description << item->Name << QString::number(item->WhID) << QString::number(item->WhNum) << \
                                              QString::number(item->WhPlaceTypeID);
            tfl.idtois(WHPLACES, fl, vl);
            if (tfl.result)
                return 1;
        }
    }
    // иначе ничего делать не надо
    return 0;
}

QList<int> WhPlacesTreeModel::Children(int Index)
{
    QList<int> sl;
    for (int i=0; i<Items.size(); i++)
    {
        int tmpi = Items.keys().at(i);
        if (Items.value(tmpi)->IdAlias == Index)
            sl << tmpi;
    }
    return sl;
}

int WhPlacesTreeModel::Find(quint8 mask, QStringList cmpvl)
{
    if ((cmpvl.isEmpty()) || !(mask & 0xFF))
        return 1;
    this->mask = mask & 0xFF;
    CmpValues = cmpvl;
    CurIndex = 0;
    return 0;
}

WhPlacesTreeModel::WhPlacesTreeItem *WhPlacesTreeModel::Next()
{
    while (CurIndex < Items.size())
    {
        int tmpi = Items.keys().at(CurIndex);
        CurIndex++;
        int cmpvlindex = 0;
        if (mask & 0x01)
        {
            if (Items.value(tmpi)->Alias == CmpValues.at(cmpvlindex));
            else
                continue;
            cmpvlindex++;
        }
        if (mask & 0x02)
        {
            if (Items.value(tmpi)->IdAlias == CmpValues.at(cmpvlindex).toInt());
            else
                continue;
            cmpvlindex++;
        }
        if (mask & 0x04)
        {
            if (Items.value(tmpi)->Name == CmpValues.at(cmpvlindex));
            else
                continue;
            cmpvlindex++;
        }
        if (mask & 0x08)
        {
            if (Items.value(tmpi)->Description == CmpValues.at(cmpvlindex));
            else
                continue;
            cmpvlindex++;
        }
        if (mask & 0x10)
        {
            if (Items.value(tmpi)->WhID == CmpValues.at(cmpvlindex).toInt());
            else
                continue;
            cmpvlindex++;
        }
        if (mask & 0x20)
        {
            if (Items.value(tmpi)->WhNum == CmpValues.at(cmpvlindex).toInt());
            else
                continue;
            cmpvlindex++;
        }
        if (mask & 0x40)
        {
            if (Items.value(tmpi)->WhPlaceTypeID == CmpValues.at(cmpvlindex).toInt());
            else
                continue;
        }
        if (mask & 0x80)
        {
            if (Items.value(tmpi)->UpdIns == CmpValues.at(cmpvlindex).toInt());
            else
                continue;
        }
        return Items.value(tmpi);
    }
    return NULL;
}
