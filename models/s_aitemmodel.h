#ifndef S_AITEMMODEL_H
#define S_AITEMMODEL_H

#include <QAbstractItemModel>
#include <QSqlDatabase>
#include <QStringList>
#include <QIcon>

#include "s_aitem.h"

class s_aitemmodel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit s_aitemmodel(QSqlDatabase db1, QString tble1, QSqlDatabase db2, QString tble2, bool linksEnabled, QObject *parent = 0);
//    explicit s_aitemmodel(QStringList sl, QObject *parent = 0);
    explicit s_aitemmodel(QList<QStringList> sl, QList<int> setlist = QList<int>(), QObject *parent = 0);
    explicit s_aitemmodel(QStringList sl1, QStringList sl2, QObject *parent = 0);
    ~s_aitemmodel();

    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &/*parent = QModelIndex()*/) const;
    s_aitem *getItem(const QModelIndex &index) const;
    QModelIndex parent(const QModelIndex &index) const;
    bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows, QColor color, QFont font, const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool isEditable; // признак, показывающий, можно ли редактировать модель
    int result; // результат выполнения инициализации модели

    int getFieldsAndAliases(bool linksEnabled);
    bool twodb, isTree, noCredentials;
    QStringList db1fields,  db2fields, dbaliases;
    QSqlDatabase db1, db2;
    QString tble1, tble2;
    QList<QPersistentModelIndex> expandedIndexes;

signals:

public slots:

    void addExpandedIndex(const QModelIndex &index);
    void removeExpandedIndex(const QModelIndex &index);

private:
    int position;
    int permissionsfields;
    QColor colors[6];
    QFont fonts[6];
    QIcon icons[3]; // // 0 = закрытое состояние корня дерева, 1 = открытое состояние, 2 = элемент дерева

    s_aitem *rootItem; // корень сделан публичным для того, чтобы была возможность изменить заголовок дерева через rootItem->setItemdata
    QList<s_aitem*> parents;
    QList<int> indentations, setlist;

    int Setup (QSqlDatabase db1, QString tble1, QSqlDatabase db2, QString tble2, bool linksEnabled);
//    int Setup (QStringList sl);
    int Setup (QList<QStringList> sl);
    int Setup (QStringList sl1, QStringList sl2);
    void setupUI();
    void addtreechild1(QString idparent);
    void addtreechild2(int position, QString);
    void addtable();
    void additemtotable(QStringList sl, int set);
    void additemtotree(int position, QStringList sl, int set);
    void additemtotree0(int position, QString sl, int set);
    QStringList getfields(QSqlDatabase db, QString tble, int &result);
    QStringList getaliases(QStringList &dbfields, QSqlDatabase db, QString tble, bool linksEnabled);
};

#endif // S_AITEMMODEL_H
