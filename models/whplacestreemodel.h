#ifndef WHPLACESTREEMODEL_H
#define WHPLACESTREEMODEL_H

#include <QAbstractItemModel>
#include "whplacestreeitem.h"

class WhPlacesTreeModel
{
    Q_OBJECT
public:
    WhPlacesTreeModel(QObject *parent=0);
    ~WhPlacesTreeModel();

    WhPlacesTreeItem Data(int Index);
    int SetData(int Index, WhPlacesTreeItem Value);
    int Index(QString Name);
    int InsertChild(int ParentIndex, WhPlacesTreeItem Value);
    int Load (QString Table, int Index);
    int Save (QString Table);
    void ClearModel();

private:
    QMap<int, WhPlacesTreeItem*> Items;
    int position, vlsize, vlidalpos;
    QStringList catlist;

    int BuildTree (int Index);
    WhPlacesTreeItem *addTreeCat (QString catlistid);
    void additemtotree(int position, QStringList sl);
};

#endif // WHPLACESTREEMODEL_H
