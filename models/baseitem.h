#ifndef BASEITEM_H
#define BASEITEM_H

#include <QStringList>
#include <QColor>
#include <QFont>
#include <QIcon>

class BaseItem
{
public:
    BaseItem(int columns=1, BaseItem *parent=0);
    ~BaseItem();

    QString Data(int column) const;
    QString Header(int column) const;
    QString ToolTip(int column) const;
    bool SetData(int column, const QString &Value);
    bool SetToolTip(int column, const QString &Value);
    bool SetHeader(int column, const QString &Value);
    void SetColor(int column, QColor Color);
    void SetFont(int column, QFont Font);
    void SetIcon(int column, QIcon Icon);
    QColor Color(int column);
    QFont Font(int column);
    QIcon Icon(int column);

private:
    QStringList ItemData, ToolTipData, Headers; // вспомогательные поля
    QList<QColor> ItemColor;
    QList<QFont> ItemFont;
    QList<QIcon> ItemIcon;
};
#endif // BASEITEM_H
