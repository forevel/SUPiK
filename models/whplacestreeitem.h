#ifndef WHPLACESTREEITEM_H
#define WHPLACESTREEITEM_H

#include <QStringList>

class WhPlacesTreeItem
{
public:
    WhPlacesTreeItem(WhPlacesTreeItem *parent=0);
    ~WhPlacesTreeItem();

    QString Alias; // наименование размещения
    int IdAlias; // ссылка на корневой элемент размещения (-1 -> нет размещения)
    QString Name; // обозначение размещения в документах
    QString Description; // описание размещения
    int WhNum; // номер размещения на складе
    int WhPlaceTypeID; // тип размещения по "Склады типы размещения"

private:
};

#endif // WHPLACESTREEITEM_H
