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
    QString Info(int Column) const;
    QString TableNumber(int Column) const;
    QString Links(int Column) const;
    QString Header(int Column) const;
    bool SetData(int Column, const QString &Value);
    bool SetInfo(int Column, const QString &Value);
    bool SetLinks(int Column, const QString &Value);
    bool SetHeader(int Column, const QString &Value);
    bool SetTableNumber(int Column, const QString &Value);

//    int ColumnCount() const;
//    bool RemoveColumns(int Position, int Columns);
//    int Row() const;
    void SetColor(int Column, QColor Color);
    void SetFont(int Column, QFont Font);
    void SetIcon(int Column, QIcon Icon);
    QColor Color(int Column);
    QFont Font(int Column);
    QIcon Icon(int Column);

private:
    QStringList ItemData;
    QStringList LinksData, TableNumbers, Headers, Infos; // вспомогательные поля
/*    TreeItem *ParentItem;
    QList<TreeItem*> ChildItems; */
    QList<QColor> ItemColor;
    QList<QFont> ItemFont;
    QList<QIcon> ItemIcon;
};
#endif // TREEITEM_H
