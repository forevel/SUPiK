#ifndef WHPLACESTREEMODEL_H
#define WHPLACESTREEMODEL_H

#include <QAbstractItemModel>

#define WHITEMSIZE  8

#define WHPLACES    "Склады размещение_полн"

#define WHP_CREATENEW   3
#define WHP_UPDATENEW   2
#define WHP_UPDATE      1
#define WHP_UNCHANGED   0

class WhPlacesTreeModel
{
public:
    WhPlacesTreeModel();
    ~WhPlacesTreeModel();

    struct WhPlacesTreeItem
    {
        int Id; // ИД размещения, копия key
        QString Alias; // наименование размещения
        int IdAlias; // ссылка на корневой элемент размещения (-1 -> нет размещения)
        QString Name; // обозначение размещения в документах
        QString Description; // описание размещения
        int WhID; // ИД склада по wh
        int WhNum; // номер размещения на складе
        int WhPlaceTypeID; // тип размещения по "Склады типы размещения"
        int UpdIns; // признак того, что элемент менялся (=1), был создан (=3), был создан и изменён (но не записан в БД, =2) или без изменений (=0)
    };

    WhPlacesTreeItem *Data(int Index);
    void SetData(int Index, WhPlacesTreeItem *Value);
    int Insert(WhPlacesTreeItem *Value);
    int DeleteNew();
    int Load (int Index);
    int Save ();
    void Update(int Index, WhPlacesTreeItem *Value);
    void ClearModel();
    QList<int> Children(int Index); // выдать индексы всех дочерних элементов для данного элемента
    int Find(quint8 mask, QStringList cmpvl); // инициировать поиск элемента, у которого элементы с номерами по маске mask равны элементам списка cmpvl. Mask начинается с элемента Alias (не с Id!)
    WhPlacesTreeItem *Next(); // продолжить поиск после Find (или перебрать все элементы с начала)

private:
    QMap<int,WhPlacesTreeItem *> Items;
    int vlsize;
    quint8 mask;
    QStringList catlist, CmpValues;
    QList<QStringList> vl;
    int CurIndex; // индекс элемента, с которого надо продолжать поиск в функции Next

    int Build (int Index);
    void AddItem (QStringList sl);
};

#endif // WHPLACESTREEMODEL_H
