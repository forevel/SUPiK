#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QColor>
#include <QFont>
#include <QIcon>
#include <QStack>
#include "treeitem.h"
#include "../gen/publicclass.h"

#define TM_SIMPLE_ELEMENT           "0"
#define TM_ELEMENT_WITH_CHILDREN    "1"

class TreeModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    explicit TreeModel(QObject *parent = 0);
    ~TreeModel();

    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QModelIndex index(int row, int column, const QModelIndex &index = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertColumns(int column, int count, const QModelIndex &parent);
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    int Setup(QStringList Tables, int Type=TT_TYPE1, bool Cond = false);
    int Setup(QString Table, bool Cond = false);
    int SetupFile(QString Filename, QString StringToFind);
//    int SetupRawComp(QString db, QString tble);
    int SetupRaw(QString db, QString tble, QStringList fields = QStringList());
    void Refresh();
    bool HaveChildren(int row);
    void SetLastItem(QColor FColor, QColor BColor, QFont Font, QIcon Icon, QString AData="0");
    void ClearModel();
    void AddItemToTree(QStringList &Fields);

    // роли для моделей
    enum Roles
    {
        LinksRole = Qt::UserRole,
        HeaderTextRole = Qt::UserRole+1,
        TableNumberRole = Qt::UserRole+2,
        CellInfoRole = Qt::UserRole+3
    };

    enum TreeTypes
    {
        TT_SIMPLE, // простое дерево из одной таблицы по полям idalias
        TT_TYPE1,  // дерево типа 1, построенное по двум таблицам - первое с idalias, второе - ссылками id<tble> на первую таблицу
        TT_TYPE2,  // дерево типа 2, построенное след. образом: 1 уровень - таблица №1 с полями id<tble1>, 2 уровень - таблица №2 с такими полями id<tble2>, которые равны
                   // полям id<tble2> в таблице 3, причём из таблицы 3 берутся элементы, для которых id<tble1> равны id<tble1> выбранного в уровне 1 элемента
                   // 3 уровень - элементы таблицы 3, для которых id<tble1>=id<tble1> уровня 1 И id<tble2>=id<tble2> выбранного в уровне 2
        TT_TABLE   // таблица, не дерево (нет поля idalias)
    };

    enum Stage2Types
    {
        TM_STR,     // SetTree
        TM_STT,     // SetTable
        TM_SNTR,    // SetNextTree
        TM_SNTT     // SetNextTable
    };

    int TreeType; // тип дерева (см. определение enum TreeType)

signals:

public slots:
    void GoIntoIndex(QModelIndex idx);

private:
    QColor Colors[7]; // определение набора цветов шрифта
    QFont Fonts[7]; // определение набора шрифтов
    QIcon Icons[7]; // определение набора иконок
    QList<TreeItem *> maindata;
    QStringList hdr;
    QList<QStringList> TableHeaders;
    QList<QStringList> TableLinks;
    QStringList DBs;
    QStringList Tables;
    QList<bool> TableIsTree; // список признаков, дерево это (есть поля idalias и alias) или таблица
    int TablesNum; // количество таблиц, участвующих в работе
    QStack<QString> RootIDs; // элементы записываются в виде: <номер_таблицы>.<ИД>
    int Indentation;
    bool IsEditable, IsRaw, IsConditional; // IsConditional - условное дерево/таблица, где видимость элементов зависит от поля "Права доступа"
    int RightsFieldNum; // номер поля "Права доступа" в tablefields

    int Build();
    int SetFirstTreeElements();
    int Set(int table, QString &id, int type);
    int PrepareTable(QString Table); // подготовить очередные TableHeaders, DBs и Tables по таблице Table
    void ClearOnlyData();
};

#endif // TREEMODEL_H
