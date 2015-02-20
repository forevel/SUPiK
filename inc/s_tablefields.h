#ifndef S_TABLEFIELDS_H
#define S_TABLEFIELDS_H

#include "../inc/s_sql.h"

// 1. stol - перевод имени в его ИД через строку links в таблице tablefields (для записи значения ИД по имени)
// 2. ltos - перевод ИД через строку links в таблице tablefields в имя (для чтения имени по значению ИД)

class s_tablefields
{
public:
    s_tablefields();

    QStringList toidl (QString tble, QString headers);
    QList<QStringList> tbtovll (QString tble);
    QStringList toid (QString tble, QString headers, QString tbleid);
    QString vtoid_ (QString tble, QString headers, QString value);
    QString vtoid (QString links, QString value);
    QString idtov (QString links, QString id);
    QStringList tablefields (QString tble, QString headers);
    QStringList headers (QString tble);

    int result;
};

extern s_tablefields tfl;

#endif // S_TABLEFIELDS_H
