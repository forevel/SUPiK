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

    void GetValuesByColumn (const QString &tble, const QString &header, QStringList &out); // взять все значения по полю header таблицы tble
    void GetValuesByColumnAndField (const QString &tble, const QString &header, const QString &cheader, const QString &value, QStringList &out); // взять все значения по полю header таблицы tble, где поле cheader этой же таблицы равно value
    void GetValuesByColumnAndFields(const QString &tble, const QString &header, QStringList &cmpfl, QStringList &cmpvl, QStringList &out);
    void GetAllValuesFromTable (const QString &tble, QList<QStringList> &out); // взять все значения по всем полям таблицы tble
    void tov(const QString &tble, const QString &header, const QString &tbleid, QString &out); // взять значение по id=tbleid из table.tablefields для заданного поля header таблицы tble
    void toid(const QString &tble, const QString &header, const QString &value, QString &out); // взять ИД из поля header таблицы tble, для которого значение = value
    void vtoid(PublicClass::ValueStruct &value, QString &out); // взять один ИД по ссылке и значению
    void idtov(const QString &links, const QString &id, PublicClass::ValueStruct &out); // взять одно значение по ссылке и ид
    void idtovl(const QString &links, QStringList &out); // взять все значения по ссылке в зависимости от типа
    void Update (const QString &tble, QStringList &header, QStringList &values);
    void Updatev (const QString &tble, QStringList &headers, QStringList &values); // update headers in the tble with values
    void Insert(const QString tble, QString &out);
    void NewID(const QString &tble, QString &out);
    void Remove(const QString &tble, const QString &id); // "удаление" элемента из таблицы
    void Delete(const QString &tble, const QString &id); // реальное удаление из таблицы
    bool Check(const QString &tble, const QString &cmpfield, const QString &cmpvalue); // проверка наличия записи в таблице
    void tablefields(const QString &tble, const QString &header, QStringList &out);
    void tableheaders(const QString &tble, QStringList &out);
    void tablelinks(const QString &tble, QStringList &out);
    bool tableistree (const QString &tble);
    void valuesbyfield(const QString &tble, QStringList &fl, const QString &cmpfield, const QString &cmpvalue, QStringList &out, bool Warn=true);
    void valuesbyfields(const QString &tble, QStringList &fl, QStringList &cmpfields, QStringList &cmpvalues, QStringList &out, bool Warn=true);
    void valuesbyfieldsmatrix(const QString &tble, QStringList &fl, QStringList &cmpfields, QStringList &cmpvalues, QList<QStringList> &out);
    void TableColumn(const QString &tble, const QString &field, QStringList &out); // взять значения поля field, где tablename=tble

    int result;
};

extern s_tablefields tfl;

#endif // S_TABLEFIELDS_H
