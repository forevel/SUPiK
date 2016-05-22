#ifndef S_SQL_H
#define S_SQL_H

#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>

#define SQLWARN WARNMSG(PublicClass::ER_SQL, __LINE__, exec_db.lastError().text())

#define SQLC_OK     0
#define SQLC_EMPTY  1
#define SQLC_FAILED 2

class s_sql : public QObject
{
    Q_OBJECT
public:
    s_sql(QObject *parent=0);

    // вспомогательные процедуры
    QSqlDatabase GetDB (QString dbname); // вернуть ссылку на БД по её имени
    QString FromDB (QSqlDatabase db); // вернуть имя БД по ссылке на неё
//    QSqlDatabase GetDBByTableName (QString tble); // поиск таблицы в базах данных и вернуть ссылку на БД, которая её содержит

    // стандартные операции с БД
    QStringList GetDBFromSQL(); // вернуть все БД, к которым есть доступ
    QStringList GetTablesFromDB(QString db); // вернуть все таблицы, содержащиеся в БД
    QStringList GetColumnsFromTable (QString db, QString tble); // вернуть все поля из таблицы tble БД db
    bool CheckForTable (QString db, QString tble); // проверить таблицу на существование
    void CreateTable(QString db, QString tble, QStringList fl, bool Simple=false); // создать таблицу с заданными в списке строк fl полями
    void AlterTable(QString db, QString tble, QStringList DeleteList, QStringList AddList); // удалить поля по списку DeleteList и добавить поля по списку AddList
    void DropTable(QString db, QString tble); // удалить таблицу из БД
    void AddColumn(QString db, QString tble, QString col, QString def = "NULL"); // добавить поле col в таблицу tble БД db со значением по умолчанию def

    // условные операции
    QString InsertValuesToTable (QString db, QString tble, QStringList fl, QStringList vl); // вставить в таблицу tble значения vl по полям fl
    QString InsertValuesSimple (QString db, QString tble, QStringList fl, QStringList vl); // добавление новой пустой записи и возврат нового ИД
    int UpdateValuesInTable (QString db, QString tble, QStringList fl, QStringList vl, QString field, QString value); // обновить данные в полях fl значениями vl, где field = value
    int DeleteFromDB (QString db, QString tble, QString field, QString value); // "удалить" (поставить "deleted"=1) запись, где field=value
    QList<QStringList> SearchInTableLike (QString db, QString tble, QString field, QString regexpstr); // вернуть список из записей, для которых field похож на regexpstr

    // условные запросы
    QString GetValueFromTableByID (QString db, QString tble, QString field, QString id); // взять значение по полю field для записи с ИД=id
    QString GetValueFromTableByField (QString db, QString tble, QString field, QString cmpfield, QString cmpvalue);
    QString GetValueFromTableByFields (QString db, QString tble, QString field, QStringList cmpfields, QStringList cmpvalues);
    QString GetLastValueFromTableByField (QString db, QString tble, QString field, QString cmpfield, QString cmpvalue); // взять из таблицы последнюю запись по полю field, где cmpfield=cmpvalue
    QStringList GetValuesFromTableByID (QString db, QString tble, QStringList fields, QString id); // взять значения по полям fields для записи с ИД=id
    QStringList GetValuesFromTableByIDSimple (QString db, QString tble, QStringList fields, QString id); // взять значения по полям fields для записи с ИД=id
    QStringList GetValuesFromTableByField (QString db, QString tble, QStringList fields, QString cmpfield, QString cmpvalue);
    QStringList GetValuesFromTableByFields(QString db, QString tble, QStringList fields, QStringList cmpfields, QStringList cmpvalues, \
                                            QString orderby="", bool asc=true);
    QStringList GetValuesFromTableByColumn (QString db, QString tble, QString column, QString orderby="", bool asc=true);
    QList<QStringList> GetValuesFromTableByColumns (QString db, QString tble, QStringList columns);
    QStringList GetValuesFromTableByColumnAndField(QString db, QString tble, QString field, QString cmpfield, QString cmpvalue, \
                                                   QString orderby="", bool asc=true);
    QList<QStringList> GetMoreValuesFromTableByFields (QString db, QString tble, QStringList fields, QStringList cmpfields, QStringList cmpvalues, \
                                                      QString orderby="", bool asc=true);

    // специфические операции
    int GetNextFreeIndex(QString db, QString tble); // вернуть ИД первого попавшегося свободного места, в порядке по возрастанию ИД, в таблице поле ИД идёт как id<tble>
    int GetNextFreeIndexSimple(QString db, QString tble); // вернуть ИД первого попавшегося свободного места, в порядке по возрастанию ИД, в таблице поле ИД идёт как id
    QString GetFullPathToChild(QString db, QString tble, QString idalias);
    int CheckDBForEmptyFields (QString db, QString tble, QString field, QStringList &probid);
    int RealDeleteFromDB (QString db, QString tble, QStringList fields, QStringList values); // процедура реально удаляет строку, для которой field равно value

    int result;
    QString LastError;
    QList<QStringList> QueryResult;

private:

private slots:
    void AppendToQueryResult(QStringList &sl);
};

extern s_sql sqlc;

#endif // S_SQL_H
