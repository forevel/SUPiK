#ifndef WH_EDITOR_H
#define WH_EDITOR_H

#include <QDialog>
#include <QStack>
#include <QPixmap>
#include <QVBoxLayout>

#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqlabel.h"

#define MAXROWS 2
#define MAXCOLS 6

#define MAXPLACES   999999

class WhPlacesModel
{
public:
    WhPlacesModel();

    struct WhPlacesItem
    {
        int Id; // ИД размещения, копия key
        QString Alias; // наименование размещения
//        int IdAlias; // ссылка на корневой элемент размещения (-1 -> нет размещения)
        QString Name; // обозначение размещения в документах
        QString Description; // описание размещения
//        int WhID; // ИД склада по wh
//        int WhNum; // номер размещения на складе
        int WhPlaceTypeID; // тип размещения по "Склады типы размещения"
        int UpdIns; // признак того, что элемент менялся (=1), был создан (=3), был создан и изменён (но не записан в БД, =2) или без изменений (=0)
    };

    WhPlacesItem Item(int index);
    void SetItem(int index, WhPlacesItem value);
    void InsertItem(WhPlacesItem value);
    int DeleteItem();
    int SetupModel(int rootid);
//    int Load (int Index);
//    int Save ();
    void ClearModel();
//    QList<int> Children(int Index); // выдать индексы всех дочерних элементов для данного элемента
//    int Find(quint8 mask, QStringList cmpvl); // инициировать поиск элемента, у которого элементы с номерами по маске mask равны элементам списка cmpvl. Mask начинается с элемента Alias (не с Id!)
//    WhPlacesTreeItem *Next(); // продолжить поиск после Find (или перебрать все элементы с начала)

private:
    QList <WhPlacesItem> Items;
    int RootID;

    int Build();
//    void AddItem (QStringList sl);
};

class Wh_Editor : public QDialog
{
    Q_OBJECT
public:
    explicit Wh_Editor(QWidget *parent=0);

signals:
    void CloseAllWidgets();

private:
    QStack<int> IDs;
    int Wh;
    WhPlacesModel *WhModel;
    int CurID;
    bool SomethingChanged;
    struct IDProperties
    {
        QString Picture;
        QString ChoosePlaceString;
        int PlaceType;
        QString PlacePrefix;
        int Rows;
        int Columns;
    };
    IDProperties CurIDProperties;

    void SetupUI();
    void UpdateWhComboBox();
    void SetCells(QWidget *w);
    void BuildWorkspace(int ID, bool IsWarehouse); // отобразить рабочее поле (размещения внутри размещения с данным ID). IsWarehouse - признак "корня"
    void UpdatePlace();
    void Disband(int ID); // расформирование единицы размещения
    QStringList NameAndPicture(int ID); // вытащить картинку по ИД размещения
    void ClearLayout (QLayout *lyout);
    bool CheckPriorities (QString PlaceName);

private slots:
    void AddNewPlace();
    void ChangePlace(QVariant PlaceName);
    void GoToPlace();
    void GoBack();
    void WriteAndClose();
    void CancelAndClose();
    void ChangeWh(QString);
    void AddNewWh();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // WH_EDITOR_H
