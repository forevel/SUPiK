#ifndef WHPLACESTREEMODEL_H
#define WHPLACESTREEMODEL_H

#include <QAbstractItemModel>

#define WHITEMSIZE  8

class WhPlacesTreeModel
{
public:
    WhPlacesTreeModel();
    ~WhPlacesTreeModel();

    struct WhPlacesTreeItem
    {
        QString Alias; // наименование размещения
        int IdAlias; // ссылка на корневой элемент размещения (-1 -> нет размещения)
        QString Name; // обозначение размещения в документах
        QString Description; // описание размещения
        int WhID; // ИД склада по wh
        int WhNum; // номер размещения на складе
        int WhPlaceTypeID; // тип размещения по "Склады типы размещения"
    };

    WhPlacesTreeItem *Data(int Index);
    void SetData(int Index, WhPlacesTreeItem *Value);
    int Index(QString Alias);
//    int InsertChild(int ParentIndex, WhPlacesTreeItem Value);
    int Load (int Index);
    int Save ();
    void ClearModel();
    QList<int> Children(int Index); // выдать индексы всех дочерних элементов для данного элемента

private:
    QMap<int,WhPlacesTreeItem *> Items;
    int vlsize;
    QStringList catlist;
    QList<QStringList> vl;

    int Build (int Index);
    void AddItem (QStringList sl);
};

#endif // WHPLACESTREEMODEL_H
