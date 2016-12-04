#ifndef S_TABLEFIELDS_H
#define S_TABLEFIELDS_H

#include "s_sql.h"
#include "publicclass.h"

enum Results
{
    TFRESULT_NOERROR,
    TFRESULT_EMPTY,
    TFRESULT_ERROR
};

class s_tablefields
{
public:
    s_tablefields();

    void htovl (QString &tble, QString &header, QStringList &out); // взять все значения по полю header таблицы tble
    void htovlc (QString &tble, QString &header, QString &cheader, QString &value, QStringList &out); // взять все значения по полю header таблицы tble, где поле cheader этой же таблицы равно value
    void tbvll (QString &tble, QList<QStringList> &out); // взять все значения по всем полям таблицы tble
    void tov(QString &tble, QString &header, QString &tbleid, QString &out); // взять значение по id=tbleid из table.tablefields для заданного поля header таблицы tble
    void toid(QString &tble, QString &header, QString &value, QString &out); // взять ИД из поля header таблицы tble, для которого значение = value
    void vtoid(PublicClass::ValueStruct &value, QString &out); // взять один ИД по ссылке и значению
    void idtov(QString links, QString id, PublicClass::ValueStruct &out); // взять одно значение по ссылке и ид
    void idtovl(QString &links, QStringList &out); // взять все значения по ссылке в зависимости от типа
    void idtois (QString &tble, QStringList &header, QStringList &values);
    void idtoisv (QString &tble, QStringList &headers, QStringList &values); // update headers in the tble with values
    void insert(QString tble, QString &out);
    void NewID(QString &tble, QString &out);
    void remove(QString &tble, QString &id); // "удаление" элемента из таблицы
    void Delete(QString &tble, QString &id); // реальное удаление из таблицы
    bool Check(QString &tble, QString &cmpfield, QString &cmpvalue); // проверка наличия записи в таблице
    void tablefields(const QString &tble, const QString &header, QStringList &out);
    void tableheaders(QString &tble, QStringList &out);
    void tablelinks(QString &tble, QStringList &out);
    bool tableistree (QString &tble);
    void valuesbyfield(const QString &tble, QStringList &fl, const QString &cmpfield, const QString &cmpvalue, QStringList &out, bool Warn=true);
    void valuesbyfields(QString &tble, QStringList &fl, QStringList &cmpfields, QStringList &cmpvalues, QStringList &out, bool Warn=true);
    void HeaderByFields(QString &tble, QString &header, QStringList &cmpfl, QStringList &cmpvl, QStringList &out);
    void TableColumn(QString &tble, QString &field, QStringList &out); // взять значения поля field, где tablename=tble

    int result;
};

extern s_tablefields tfl;

#endif // S_TABLEFIELDS_H
