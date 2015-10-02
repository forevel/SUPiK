#ifndef S_SQL_H
#define S_SQL_H

#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>

#define SQLWARN WARNMSG(PublicClass::ER_SQL, __LINE__, exec_db.lastError().text())
class s_sql
{
public:
    s_sql();

    // вспомогательные процедуры
    QSqlDatabase GetDB (QString dbname); // вернуть ссылку на БД по её имени
    QString FromDB (QSqlDatabase db); // вернуть имя БД по ссылке на неё
    QSqlDatabase GetDBByTableName (QString tble); // поиск таблицы в базах данных и вернуть ссылку на БД, которая её содержит

    // стандартные операции с БД
    QStringList GetDBFromSQL(); // вернуть все БД, к которым есть доступ
    QStringList GetTablesFromDB(QSqlDatabase db); // вернуть все таблицы, содержащиеся в БД
    QStringList GetColumnsFromTable (QSqlDatabase db, QString tble); // вернуть все поля из таблицы tble БД db
    void CreateTable(QSqlDatabase db, QString tble, QStringList fl); // создать таблицу с заданными в списке строк fl полями
    void AlterTable(QSqlDatabase db, QString tble, QStringList DeleteList, QStringList AddList); // удалить поля по списку DeleteList и добавить поля по списку AddList
    void DropTable(QSqlDatabase db, QString tble); // удалить таблицу из БД
    void AddColumn(QSqlDatabase db, QString tble, QString col, QString def = "NULL"); // добавить поле col в таблицу tble БД db со значением по умолчанию def

    // условные операции
    QString InsertValuesToTable (QSqlDatabase db, QString tble, QStringList fl, QStringList vl); // вставить в таблицу tble значения vl по полям fl
    int UpdateValuesInTable (QSqlDatabase db, QString tble, QStringList fl, QStringList vl, QString field, QString value); // обновить данные в полях fl значениями vl, где field = value
    int DeleteFromDB (QSqlDatabase db, QString tble, QString field, QString value); // "удалить" (поставить "deleted"=1) запись, где field=value
    QList<QStringList> SearchInTableLike (QSqlDatabase db, QString tble, QString field, QString regexpstr); // вернуть список из записей, для которых field похож на regexpstr

    // условные запросы
    QString GetValueFromTableByID (QSqlDatabase db, QString tble, QString field, QString id); // взять значение по полю field для записи с ИД=id
    QString GetValueFromTableByField (QSqlDatabase db, QString tble, QString field, QString cmpfield, QString cmpvalue);
    QString GetValueFromTableByFields (QSqlDatabase db, QString tble, QString field, QStringList cmpfields, QStringList cmpvalues);
    QString GetLastValueFromTableByField (QSqlDatabase db, QString tble, QString field, QString cmpfield, QString cmpvalue); // взять из таблицы последнюю запись по полю field, где cmpfield=cmpvalue
    QStringList GetValuesFromTableByID (QSqlDatabase db, QString tble, QStringList fields, QString id); // взять значения по полям fields для записи с ИД=id
    QStringList GetValuesFromTableByField (QSqlDatabase db, QString tble, QStringList fields, QString cmpfield, QString cmpvalue);
    QStringList GetValuesFromTableByFields (QSqlDatabase db, QString tble, QStringList fields, QStringList cmpfields, QStringList cmpvalues);
    QStringList GetValuesFromTableByColumn (QSqlDatabase db, QString tble, QString column, QString orderby="", bool asc=true);
    QList<QStringList> GetValuesFromTableByColumns (QSqlDatabase db, QString tble, QStringList columns);
    QStringList GetValuesFromTableByColumnAndField(QSqlDatabase db, QString tble, QString field, QString cmpfield, QString cmpvalue, \
                                                   QString orderby="", bool asc=true);
    QList<QStringList> GetMoreValuesFromTableByField (QSqlDatabase db, QString tble, QStringList fields, QString cmpfield, QString cmpvalue,\
                                                      QString orderby="", bool asc=true);

    // специфические операции
    int GetNextFreeIndex(QSqlDatabase db, QString tble); // вернуть ИД первого попавшегося свободного места, в порядке по возрастанию ИД, в таблице поле ИД идёт как id<tble>
    int GetNextFreeIndexSimple(QSqlDatabase db, QString tble); // вернуть ИД первого попавшегося свободного места, в порядке по возрастанию ИД, в таблице поле ИД идёт как id
    QString GetFullPathToChild(QSqlDatabase db, QString tble, QString idalias);
    int CheckDBForEmptyFields (QSqlDatabase db, QString tble, QStringList fields, QStringList &probid);
    int RealDeleteFromDB (QSqlDatabase db, QString tble, QStringList fields, QStringList values); // процедура реально удаляет строку, для которой field равно value

    int result;
};

extern s_sql sqlc;

#endif // S_SQL_H
