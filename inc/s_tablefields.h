#ifndef S_TABLEFIELDS_H
#define S_TABLEFIELDS_H

#include "../inc/s_sql.h"

class s_tablefields
{
public:
    s_tablefields();

    QStringList GetAllValuesByOneRow (QString tble, QString headers);
    QList<QStringList> GetAllValues (QString tble);
    QString GetOneValueByOneRowAndId (QString tble, QString headers, QString id);

    int result;
};

extern s_tablefields tfl;

#endif // S_TABLEFIELDS_H
