#ifndef WHPLACESTREEMODEL_H
#define WHPLACESTREEMODEL_H

#include <QAbstractItemModel>
#include "whplacestreeitem.h"

class WhPlacesTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    WhPlacesTreeModel(QObject *parent=0);
    ~WhPlacesTreeModel();

    QModelIndex parent(const QModelIndex &index) const;
    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
//    bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    void addRow(int position, QModelIndex &parent = QModelIndex());
    QModelIndex getIndex(WhPlacesTreeItem *item) const;
    WhPlacesTreeItem *getItem(const QModelIndex &index) const;
    int Setup (QString table, int index);
    void ClearModel();

private:
    WhPlacesTreeItem *rootItem;
    QList<WhPlacesTreeItem*> parents;
    QList<int> indentations;
    QList<QStringList> vl;
    int position, vlsize, vlidalpos;
    QStringList catlist;

    int BuildTree (QString id);
    WhPlacesTreeItem *addTreeCat (QString catlistid);
    void additemtotree(int position, QStringList sl);

};

#endif // WHPLACESTREEMODEL_H
