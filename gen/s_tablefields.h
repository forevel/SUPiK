#ifndef S_TABLEFIELDS_H
#define S_TABLEFIELDS_H

#define TFWARN(a)   WARNMSG(PublicClass::ER_TFIELD,__LINE__,a)
#define TFDBG       DBGMSG(PublicClass::ER_TFIELD,__LINE__)

#include "s_sql.h"
#include "publicclass.h"

class s_tablefields
{
public:
    s_tablefields();

    QStringList htovl (QString tble, QString header); // взять все значения по полю header таблицы tble
    QStringList htovlc (QString tble, QString header, QString cheader, QString value); // взять все значения по полю header таблицы tble, где поле cheader этой же таблицы равно value
    QList<QStringList> tbvll (QString tble); // взять все значения по всем полям таблицы tble
    QString tov(QString tble, QString header, QString tbleid); // взять значение по id=tbleid из table.tablefields для заданного поля header таблицы tble
    QString toid(QString tble, QString header, QString value); // взять ИД из поля header таблицы tble, для которого значение = value
    QString vtoid (PublicClass::ValueStruct value); // взять один ИД по ссылке и значению
    PublicClass::ValueStruct idtov(QString links, QString id); // взять одно значение по ссылке и ид
    QStringList idtovl (QString links); // взять все значения по ссылке в зависимости от типа
    void idtois (QString tble, QStringList header, QStringList values);
    QString insert(QString tble);
    QString NewID(QString tble);
    void remove(QString tble, QString id); // "удаление" элемента из таблицы
    void Delete(QString tble, QString id); // реальное удаление из таблицы
    bool Check(QString tble, QString cmpfield, QString cmpvalue); // проверка наличия записи в таблице
    QStringList tablefields (QString tble, QString header);
    QStringList tableheaders (QString tble);
    QStringList tablelinks (QString tble);
    bool tableistree (QString tble);
    QStringList valuesbyfield(QString tble, QStringList fl, QString cmpfield, QString cmpvalue, bool Warn=true);
    QStringList valuesbyfields(QString tble, QStringList fl, QStringList cmpfields, QStringList cmpvalues, bool Warn=true);
    QStringList HeaderByFields(QString &tble, QString &header, QStringList &cmpfl, QStringList &cmpvl);
    QStringList TableColumn(QString tble, QString field); // взять значения поля field, где tablename=tble

    int result;
};

extern s_tablefields tfl;

#endif // S_TABLEFIELDS_H
