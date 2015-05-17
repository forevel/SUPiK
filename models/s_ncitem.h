#ifndef S_NCITEM_H
#define S_NCITEM_H

#include <QStringList>
#include <QColor>
#include <QFont>
#include <QIcon>

class s_ncitem
{
public:
    explicit s_ncitem(s_ncitem *parent = 0);

    QString data(int column) const;
    QString linksdata(int column) const;
    void setData(int column, const QString &value);
    void setLinksData(int column, const QString &value);
    bool isEmpty(QList<int> columnsToCheck);
    void setColor(int column, QColor color);
    void setFont(int column, QFont font);
    void setIcon(int column, QIcon icon);
    void setAData(QString str);
    void setTData(int column, QString str);
    QColor color(int column);
    QFont font(int column);
    QIcon icon(int column);
    QString AData();
    QString TData(int column);

private:
    QStringList itemData; // содержимое строки
    QStringList linksData; // содержимое вспомогательного поля
    QString aData; // вспомогательное содержимое для всего итема
    QStringList tDataList; // вспомогательное содержимое для каждой ячейки итема
    QList<QColor> itemColor;
    QList<QFont> itemFont;
    QList<QIcon> itemIcon;
};

#endif // S_NCITEM_H
