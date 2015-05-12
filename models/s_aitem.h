#ifndef S_AITEM_H
#define S_AITEM_H

#include <QList>
#include <QVariant>
#include <QColor>
#include <QFont>

class s_aitem
{
public:
    explicit s_aitem(const QList<QVariant> &data, QColor color, QFont font, s_aitem *parent = 0);
    explicit s_aitem(const QStringList &list, QColor color, QFont font, s_aitem *parent = 0);
    ~s_aitem();

    s_aitem *child(int row);
    int childCount() const;
    int columnCount() const;
    int childNumber() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns, QColor color, QFont font);
    bool insertColumns(int position, int columns);
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    bool setData(int column, const QVariant &value);
//    void setItemdata(QList<QVariant> &data);
    int row() const;
    int icon(); // взять иконку для данной строки
    void setIcon(int iconindex);
    QColor color();
    QFont font();
    s_aitem *parent();

private:
    QList<QVariant> itemData;
    QList<s_aitem*> childItems;
    QColor itemColor;
    QFont itemFont;
    int item0icon; // 0 = закрытое состояние корня дерева, 1 = открытое состояние, 2 = элемент дерева. Иконки определяются моделью
    s_aitem *parentItem;
};

#endif // S_AITEM_H
