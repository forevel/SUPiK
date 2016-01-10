#ifndef TREEITEM_H
#define TREEITEM_H

#include <QStringList>
#include <QColor>
#include <QFont>
#include <QIcon>

class TreeItem
{
public:
    TreeItem(TreeItem *parent=0);
    ~TreeItem();

    QString Data(int Column) const;
    QString AData(int Column) const;
//    QString linksdata(int column) const;
    bool SetData(int Column, const QString &Value);
    bool SetAData(int Column, const QString &Value);
//    bool setLinksData(int column, const QString &value);

    int ColumnCount() const;
    bool RemoveColumns(int Position, int Columns);
    int Row() const;
    void SetColor(int Column, QColor Color);
    void SetFont(int Column, QFont Font);
    void SetIcon(int Column, QIcon Icon);
    QColor Color(int Column);
    QFont Font(int Column);
    QIcon Icon(int Column);

private:
    QStringList ItemData;
    QStringList ItemAData;
    TreeItem *ParentItem;
    QList<TreeItem*> ChildItems;
//    QStringList linksData; // содержимое вспомогательного поля
    QList<QColor> ItemColor;
    QList<QFont> ItemFont;
    QList<QIcon> ItemIcon;
};
#endif // TREEITEM_H
