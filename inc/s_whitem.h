#ifndef S_WHITEM_H
#define S_WHITEM_H

#include <QStringList>

class s_whitem
{
public:
    explicit s_whitem(QStringList data, s_whitem *parent = 0);
    ~s_whitem();

    QString data(int column) const;
    bool setData(int column, const QString &value);
    bool isEmpty(QList<int> columnsToCheck);

private:
    QStringList itemData; // содержимое строки
};

#endif // S_WHITEM_H
