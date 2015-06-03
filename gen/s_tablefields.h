#ifndef S_TABLEFIELDS_H
#define S_TABLEFIELDS_H

#include "s_sql.h"

class s_tablefields
{
public:
    s_tablefields();

    QStringList htovl (QString tble, QString header); // взять все значения по полю header таблицы tble
    QStringList htovlc (QString tble, QString header, QString cheader, QString value); // взять все значения по полю header таблицы tble, где поле cheader этой же таблицы равно value
    QList<QStringList> tbvll (QString tble); // взять все значения по всем полям таблицы tble
    QString tov(QString tble, QString header, QString tbleid); // взять значение по id=tbleid из table.tablefields для заданного поля header таблицы tble
    QString toid(QString tble, QString header, QString value); // взять ИД из поля header таблицы tble, для которого значение = value
    QString vtoid (QString links, QString value); // взять один ИД по ссылке и значению
    QString idtov (QString links, QString id); // взять одно значение по ссылке и ид
    QStringList idtovl (QString links); // взять все значения по ссылке в зависимости от типа
    int idtois (QString tble, QStringList header, QStringList values);
    QString insert(QString tble);
    int remove(QString tble, QString id); // "удаление" элемента из таблицы
    QStringList tablefields (QString tble, QString header);
    QStringList tableheaders (QString tble);
    QStringList tablelinks (QString tble);
    bool tableistree (QString tble);

    int result;
};

extern s_tablefields tfl;

#endif // S_TABLEFIELDS_H