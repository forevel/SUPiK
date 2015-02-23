#ifndef S_TABLEFIELDS_H
#define S_TABLEFIELDS_H

#include "../inc/s_sql.h"

// 1. stol - перевод имени в его ИД через строку links в таблице tablefields (для записи значения ИД по имени)
// 2. ltos - перевод ИД через строку links в таблице tablefields в имя (для чтения имени по значению ИД)

class s_tablefields
{
public:
    s_tablefields();

    QStringList toidl (QString tble, QString headers); // взять все значения по полю headers таблицы tble
    QStringList toidlc (QString tble, QString headers, QString cheaders, QString value); // взять все значения по полю headers таблицы tble, где поле cheaders этой же таблицы равно value
    QList<QStringList> tbtovll (QString links); // взять все значения по всем полям таблицы tble
    QStringList toid (QString tble, QString headers, QString tbleid);
    QString vtoid_ (QString tble, QString headers, QString value); // взять ИД из поля headers таблицы tble, для которого значение = value
    QString vtoid (QString links, QString value); // взять один ИД по ссылке и значению
    QString idtov (QString links, QString id); // взять одно значение по ссылке и ид
    QStringList idtovl (QString links); // взять все значения по ссылке в зависимости от типа
    int idtois (QString tble, QStringList headers, QStringList values);
    QString insert(QString tble);
    int remove(QString tble, QString id); // "удаление" элемента из таблицы
    QStringList tablefields (QString tble, QString headers);
    QStringList headers (QString tble);
    bool tableistree (QString tble);

    int result;
};

extern s_tablefields tfl;

#endif // S_TABLEFIELDS_H
