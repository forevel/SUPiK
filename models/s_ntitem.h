#ifndef S_NTITEM_H
#define S_NTITEM_H

#include <QStringList>
#include <QColor>
#include <QFont>
#include <QIcon>

class s_ntitem
{
public:
    s_ntitem(s_ntitem *parent=0);
    ~s_ntitem();

    QString data(int column) const;
    QString linksdata(int column) const;
    bool setData(int column, const QString &value);
    bool setLinksData(int column, const QString &value);

    s_ntitem *parent();
    s_ntitem *child(int row);
    int childCount() const;
//    int childNumber() const;
    int columnCount() const;
    s_ntitem *insertChild(int position, int columns);
//    bool insertColumns(int position, int columns);
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int row() const;
    void setColor(int column, QColor color);
    void setFont(int column, QFont font);
    void setIcon(int column, QIcon icon);
    QColor color(int column);
    QFont font(int column);
    QIcon icon(int column);

private:
    QStringList itemData;
    s_ntitem *parentItem;
    QList<s_ntitem*> childItems;
    QStringList linksData; // содержимое вспомогательного поля
    QList<QColor> itemColor;
    QList<QFont> itemFont;
    QList<QIcon> itemIcon;
};

#endif // S_NTITEM_H
