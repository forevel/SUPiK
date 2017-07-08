#ifndef WH_EDITOR_H
#define WH_EDITOR_H

#include <QDialog>
#include <QStack>
#include <QPixmap>
#include <QPointer>
#include <QVBoxLayout>

#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqwidget.h"
#include "../../widgets/s_tqscrollarea.h"

#define MAXROWS 2
#define MAXCOLS 6
#define MAXPRIORITY -1 // "корневой" приоритет, "выше только звёзды"
#define MAXPLACES   999999

#define WRONGNUM    -1

class WhPlacesModel : public QObject
{
    Q_OBJECT
public:
    WhPlacesModel(QObject *parent=0);

    struct WhPlacesItem
    {
        QString Id; // ИД размещения, копия key
        QString Name; // обозначение размещения в документах
        QString Description; // описание размещения
        int PictureIndex; // номер фотографии ёмкости размещения по Pictures (QMap)
        int Row; // ряд размещения в вышестоящем
        int Column; // столбец размещения
        int Rows; // количество рядов в размещении
        int Columns; // количество столбцов в размещении
        int Priority; // приоритет места размещения
    };

    WhPlacesItem Item(int index);
    WhPlacesItem Item(int row, int column);
    void SetItem(int index, WhPlacesItem &value);
    void InsertItem(WhPlacesItem &value);
    int DeleteItem();
    int SetupModel(int rootid);
    int Save(); // сохранить информацию о месте размещения в БД
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

private:
    QStack<WhPlacesModel::WhPlacesItem> ItemsStack;
    int Wh;
    QPointer<WhPlacesModel> WhModel;
    int CurID;
    bool SomethingChanged;
    QMap<int, QPixmap *> Pictures; // картинки мест размещения в соответствии с их ИД в БД
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
    void OpenSpace(); // открыть место размещения и отобразить его содержимое, параметры места размещения - на вершине стека ItemsStack
    void CloseSpace(); // перейти на уровень выше по стеку ItemsStack
    void SaveSpace();
    void EditSpace(); // редактировать информацию о месте размещения
    void CheckChanges(); // проверка наличия внесённых изменений и запись в БД при необходимости
    s_tqScrollArea *SetupCells();

    void UpdateWhComboBox();
    void Disband(int ID); // расформирование единицы размещения
    void ClearLayout ();
    bool CheckPriorities (QString PlaceName);
    void PushItemStack(int row, int column);

private slots:
    void AddNewPlace();
    void ChangePlace(QVariant PlaceName);
    void GoToPlace();
    void GoBack();
    void WriteAndClose();
    void CancelAndClose();
    void ChangeWh(QString);
    void AddNewWh();
    void EditPlace();
    void EditWarehouse(); // редактировать информацию о складе

protected:
    void paintEvent(QPaintEvent *);
};

#endif // WH_EDITOR_H
