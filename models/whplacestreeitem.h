#ifndef WHPLACESTREEITEM_H
#define WHPLACESTREEITEM_H

#include <QStringList>

class WhPlacesTreeItem
{
public:
    WhPlacesTreeItem(WhPlacesTreeItem *parent=0);
    ~WhPlacesTreeItem();

    QString data(int column) const;
    bool setData(int column, const QString &value);
    WhPlacesTreeItem *parent();
    WhPlacesTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    WhPlacesTreeItem *insertChild(int position, int columns);
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int row() const;

private:
    QStringList itemData;
    WhPlacesTreeItem *parentItem;
    QList<WhPlacesTreeItem *> childItems;
};

#endif // WHPLACESTREEITEM_H
