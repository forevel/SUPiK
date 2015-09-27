#include "s_sql.h"
#include "publicclass.h"
#include <QVariant>
#include <QDebug>

s_sql sqlc;

s_sql::s_sql()
{
}

// процедура возвращает базу данных по её имени в dbname

QSqlDatabase s_sql::GetDB(QString dbname)
{
    QSqlDatabase tmpdb;
    if (pc.db.keys().contains(dbname))
        tmpdb = pc.db[dbname];
    else
        return QSqlDatabase();
    return tmpdb;
}

// процедура возвращает имя БД по её экземпляру

QString s_sql::FromDB(QSqlDatabase db)
{
    if (db.databaseName() == "supik") return "sup";
    else if (db.databaseName() == "enterprise") return "ent";
    else if (db.databaseName() == "altium") return "alt";
    else if (db.databaseName() == "solidworks") return "sol";
    else if (db.databaseName() == "devices") return "dev";
    else if (db.databaseName() == "schemagee") return "sch";
    else if (db.databaseName() == "constructives") return "con";
/*    if (pc.db.values().contains(db))
        return pc.db.key(db);*/
    else
        return QString();
}

// процедура возвращает базу данных по имени таблицы, которая в БД должна содержаться

QSqlDatabase s_sql::GetDBByTableName(QString tble)
{
    QSqlDatabase tmpdb;
    QStringList dbs = GetDBFromSQL();
    for (int i = 0; i < dbs.size(); i++)
    {
        tmpdb = GetDB(dbs.at(i).left(3));
        QStringList tbles = GetTablesFromDB(tmpdb);
        while (!tbles.isEmpty())
        {
            QString tmpString = tbles.takeFirst();
            if (tble == tmpString)
                return tmpdb;
        }
    }
    return QSqlDatabase();
}

// процедура возвращает все имеющиеся БД (кроме системных)

QStringList s_sql::GetDBFromSQL()
{
    QStringList sl;
    sl.clear();
    QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", "ALL"));
    db->setDatabaseName("supik");
    db->setUserName("supik");
    db->setHostName(pc.SQLPath);
    db->setPassword("sysupik");
    if (!db->open())
        return sl;
    QSqlQuery exec_db(*db);
    exec_db.exec("SHOW DATABASES;");
    while (exec_db.next())
        sl << exec_db.value(0).toString();
    exec_db.finish();
    if (db->isOpen())
        db->close();
    delete db;
    QSqlDatabase::removeDatabase("ALL");
    sl.removeAll("mysql");
    sl.removeAll("information_schema");
    sl.removeAll("performance_schema");
    sl.removeAll("test");
    return sl;
}

// процедура возвращает список таблиц из БД db

QStringList s_sql::GetTablesFromDB(QSqlDatabase db)
{
    QStringList sl;
    QSqlQuery exec_db (db);

    sl.clear();
    exec_db.exec("SHOW TABLES;");
    while (exec_db.next())
        sl << exec_db.value(0).toString();
    return sl;
}

// процедура возвращает список полей из db:tble

QStringList s_sql::GetColumnsFromTable(QSqlDatabase db, QString tble)
{
    QStringList sl;
    QSqlQuery exec_db (db);

    sl.clear();
    exec_db.exec("SHOW COLUMNS FROM `" + tble + "`;");
    if (!exec_db.isActive())
    {
        result = 1;
        SQLWARN;
        return QStringList();
    }
    while (exec_db.next())
        sl << exec_db.value(0).toString();
    result=0;
    return sl;
}

// процедура создаёт новую таблицу, в fl имена столбцов

void s_sql::CreateTable(QSqlDatabase db, QString tble, QStringList fl)
{
    QSqlQuery exec_db(db);
    QString tmpString = "CREATE TABLE `"+tble+"` (`id"+tble+"` int(11) NOT NULL,";
    for (int i = 0; i < fl.size(); i++)
        tmpString += "`"+fl.at(i)+"` varchar(128) DEFAULT NULL,";
    tmpString += "`idpers` varchar(128) DEFAULT NULL,`date` VARCHAR(128) DEFAULT NULL,`deleted` int(1) NOT NULL DEFAULT '0',"
            "PRIMARY KEY (`id"+tble+"`),UNIQUE KEY `id"+tble+"_UNIQUE` (`id"+tble+"`)) ENGINE=MyISAM DEFAULT CHARSET=utf8;";
    exec_db.exec(tmpString);
    if (exec_db.isActive())
    {
        result = 0;
        return;
    }
    result = 1;
    SQLWARN;
    return;
}

// удалить поля по списку DeleteList и добавить поля по списку AddList

void s_sql::AlterTable(QSqlDatabase db, QString tble, QStringList DeleteList, QStringList AddList)
{
    QSqlQuery exec_db(db);
    QString tmpString = "ALTER TABLE `"+tble+"` ";
    QStringList lastcol = sqlc.GetColumnsFromTable(db, tble);
    QString After = lastcol.last();
    if (!AddList.isEmpty())
    {
        tmpString += "ADD COLUMN ";
        while (!AddList.isEmpty())
        {
            QString tmps = AddList.first();
            tmpString += "`"+tmps+"` VARCHAR(128) NULL AFTER `" + After + "`,";
            After = tmps;
            AddList.removeFirst();
        }
        tmpString.chop(1); // последняя запятая
    }
    if (!DeleteList.isEmpty())
    {
        tmpString += " DROP COLUMN ";
        while (!DeleteList.isEmpty())
        {
            QString tmps = DeleteList.first();
            tmpString += "`"+tmps+"`,";
            DeleteList.removeFirst();
        }
    }
    tmpString += ";";
    exec_db.exec(tmpString);
    if (exec_db.isActive())
    {
        result = 0;
        return;
    }
    result = 1;
    SQLWARN;
    return;
}

// процедура удаляет таблицу из БД

void s_sql::DropTable(QSqlDatabase db, QString tble)
{
    QSqlQuery exec_db (db);
    exec_db.exec("DROP TABLE IF EXISTS `"+tble+"`;");
    if (exec_db.isActive())
    {
        result = 0;
        return;
    }
    result = 1;
    SQLWARN;
    return;
}

// процедура добавляет столбец к таблице

void s_sql::AddColumn(QSqlDatabase db, QString tble, QString col, QString def)
{
    QStringList lastcol = sqlc.GetColumnsFromTable(db, tble);
    QSqlQuery exec_db(db);
    QString tmpString = (def == "NULL")?"":"DEFAULT '"+def+"'";
    tmpString = "ALTER TABLE `"+tble+"` ADD COLUMN `"+col+"` VARCHAR(128) NULL "+tmpString+" AFTER `"+lastcol.last()+"`;";
    exec_db.exec(tmpString);
    if (exec_db.isActive())
        result = 0;
    else
    {
        SQLWARN;
        result = 1;
    }
    return;
}

// взять следующий свободный индекс из БД db, таблицы tble и вернуть его int-ом

int s_sql::GetNextFreeIndex(QSqlDatabase db, QString tble)
{
    long i = 1;
    QSqlQuery exec_db (db);
    QString tmpString = "SELECT `id" + tble + "` FROM `" + tble + "` ORDER BY `id" + tble + "` ASC;";
    exec_db.exec(tmpString); // индексы сортируем по возрастанию
    if (exec_db.isActive())
    {
        while ((exec_db.next()) && (exec_db.value(0).toInt(0) == i)) // пока нет пропусков
            i++;
        result = 0;
        return i;
    }
    else
    {
        result = 1;
        SQLWARN;
        return -1;
    }
}

// взять следующий свободный индекс из БД db, таблицы tble и вернуть его int-ом. id - без имени таблицы

int s_sql::GetNextFreeIndexSimple(QSqlDatabase db, QString tble)
{
    long i = 1;
    QSqlQuery exec_db (db);
    QString tmpString = "SELECT `id` FROM `" + tble + "` ORDER BY `id` ASC;";
    exec_db.exec(tmpString); // индексы сортируем по возрастанию
    if (exec_db.isActive())
    {
        while ((exec_db.next()) && (exec_db.value(0).toInt(0) == i)) // пока нет пропусков
            i++;
        result = 0;
        return i;
    }
    else
    {
        result = 1;
        SQLWARN;
        return -1;
    }
}

// процедура берёт из таблицы запись № id<tble> по полям fields и возвращает значения полей

QStringList s_sql::GetValuesFromTableByID(QSqlDatabase db, QString tble, QStringList fields, QString id)
{
    QStringList vl;

    vl = GetValuesFromTableByField(db, tble, fields, "id"+tble, id);
    return vl;
}

// процедура берёт из таблицы запись № id<tble> по полю field и возвращает значение поля

QString s_sql::GetValueFromTableByID(QSqlDatabase db, QString tble, QString field, QString id)
{
    QString vl;

    vl = GetValueFromTableByField(db, tble, field, "id"+tble, id);
    return vl;
}

// процедура берёт из таблицы первую запись, в которой cmpfield=cmpvalue, по полям fields и возвращает значения полей

QStringList s_sql::GetValuesFromTableByField(QSqlDatabase db, QString tble, QStringList fields, QString cmpfield, QString cmpvalue)
{
    QString tmpString;
    QStringList vl;
    int i;
    QSqlQuery exec_db (db);

    tmpString = "SELECT ";
    for (i = 0; i < fields.size(); i++)
        tmpString += "`" + fields.at(i) + "`,";
    tmpString = tmpString.left(tmpString.size()-1); // удаляем последнюю запятую
    tmpString += " FROM `" + tble + "` WHERE `" + cmpfield + "`=\"" + cmpvalue + "\" AND `deleted`=0;";
    exec_db.exec(tmpString);
    exec_db.next();
    if (exec_db.isValid())
    {
        vl.clear();
        i = 0;
        while (i < fields.size())
            vl << exec_db.value(i++).toString();
        result=0;
        return vl;
    }
    result=1;
    SQLWARN;
    return QStringList();
}

// процедура берёт из таблицы все значения по столбцу column для всех строк

QStringList s_sql::GetValuesFromTableByColumn(QSqlDatabase db, QString tble, QString column, QString orderby, bool asc)
{
    QString tmpString;
    QStringList vl;
    QSqlQuery exec_db (db);

    tmpString = "SELECT `" + column + "` FROM `" + tble + "` WHERE `deleted`=0";
    if (!orderby.isEmpty())
    {
        tmpString += " ORDER BY `"+orderby+"` ";
        if (asc)
            tmpString += "ASC";
        else
            tmpString += "DESC";
    }
    tmpString += ";";
    exec_db.exec(tmpString);
    vl.clear();
    while (exec_db.next())
        vl << exec_db.value(0).toString();
    if (vl.isEmpty())
    {
        result=1;
        SQLWARN;
        return QStringList();
    }
    result=0;
    return vl;
}

// процедура берёт из таблицы все значения по столбцам columns

QList<QStringList> s_sql::GetValuesFromTableByColumns(QSqlDatabase db, QString tble, QStringList columns)
{
    QString tmpString;
    QList<QStringList> vl;
    QStringList tmpStringList;
    QSqlQuery exec_db (db);
    int i;

    for (i = 0; i < columns.size(); i++)
    {
        tmpString = "SELECT `" + columns.at(i) + "` FROM `" + tble + "` WHERE `deleted`=0;";
        exec_db.exec(tmpString);
        tmpStringList.clear();
        while (exec_db.next())
            tmpStringList.append(exec_db.value(0).toString());
        vl.append(tmpStringList);
    }
    if (vl.isEmpty())
    {
        result=1;
        SQLWARN;
        return QList<QStringList>();
    }
    else
    {
        result = 0;
        return vl;
    }
}

// процедура берёт из таблицы только те значения по столбцу column, для которых поле str1 равно значению str2

QStringList s_sql::GetValuesFromTableByColumnAndField(QSqlDatabase db, QString tble, QString column, QString cmpfield, QString cmpvalue,\
                                                      QString orderby, bool asc)
{
    QString tmpString;
    QStringList vl;
    QSqlQuery exec_db (db);

    tmpString = "SELECT `" + column + "` FROM `" + tble + "` WHERE `deleted`=0 AND `" + cmpfield + "`=\"" + cmpvalue + "\"";
    if (!orderby.isEmpty())
    {
        tmpString += " ORDER BY `"+orderby+"` ";
        if (asc)
            tmpString += "ASC";
        else
            tmpString += "DESC";
    }
    tmpString += ";";
    exec_db.exec(tmpString);
    if (!exec_db.isActive())
    {
        result = 1;
        SQLWARN;
        return QStringList();
    }
    vl.clear();
    while (exec_db.next())
        vl << exec_db.value(0).toString();
    result=0;
    return vl;
}

// процедура берёт из таблицы первую запись, в которой cmpfield=cmpvalue, по полю field и возвращает значение поля

QString s_sql::GetValueFromTableByField (QSqlDatabase db, QString tble, QString field, QString cmpfield, QString cmpvalue)
{
    QString tmpString;
    QString vl;
    QSqlQuery exec_db (db);

    tmpString = "SELECT `" + field + "` FROM `" + tble + "` WHERE `" + cmpfield + "`=\"" + cmpvalue + "\" AND `deleted`=0;";
    exec_db.exec(tmpString);
    if (!exec_db.isActive())
    {
        result = 2;
        SQLWARN;
        return QString();
    }
    exec_db.next();
    if (exec_db.isValid())
    {
        vl = exec_db.value(0).toString();
        result = 0;
        return vl;
    }
    result = 1;
    return QString();
}

// процедура берёт из таблицы первую запись, в которой соответствующие поля из cmpfields=cmpvalues, по полю field и возвращает значение поля

QString s_sql::GetValueFromTableByFields (QSqlDatabase db, QString tble, QString field, QStringList cmpfields, QStringList cmpvalues)
{
    QString tmpString;
    QString vl;
    QSqlQuery exec_db (db);
    int i;

    if (cmpfields.isEmpty())
    {
        result = 4;
        WARNMSG(PublicClass::ER_SQL, __LINE__, "Переданный список сравнения пуст");
        return QString();
    }
    if (cmpfields.size() != cmpvalues.size())
    {
        result = 5;
        WARNMSG(PublicClass::ER_SQL, __LINE__, "Длина списка полей сравнения не совпадает с длиной списка значений");
        return QString();
    }

    tmpString = "SELECT `" + field + "` FROM `" + tble + "` WHERE ";
    for (i = 0; i < cmpfields.size(); i++)
        tmpString += "`" + cmpfields.at(i) + "`=\""+cmpvalues.at(i)+"\" AND ";
    tmpString = tmpString.left(tmpString.size()-5); // удаляем последний AND
    tmpString +=  " AND `deleted`=0;";
    exec_db.exec(tmpString);
    if (!exec_db.isActive())
    {
        result = 2;
        SQLWARN;
        return QString();
    }
    exec_db.next();
    if (exec_db.isValid())
    {
        vl = exec_db.value(0).toString();
        result = 0;
        return vl;
    }
    result = 1;
    return QString();
}

// процедура берёт из таблицы поля fields для строки, в которой поля cmpfield равны cmpvalues

QStringList s_sql::GetValuesFromTableByFields (QSqlDatabase db, QString tble, QStringList fields, QStringList cmpfields, QStringList cmpvalues)
{
    QString tmpString;
    QStringList vl;
    QSqlQuery exec_db (db);
    int i;

    if (cmpfields.isEmpty())
    {
        result = 4;
        WARNMSG(PublicClass::ER_SQL, __LINE__, "Переданный список сравнения пуст");
        return QStringList();
    }
    if (cmpfields.size() != cmpvalues.size())
    {
        result = 5;
        WARNMSG(PublicClass::ER_SQL, __LINE__, "Длина списка полей сравнения не совпадает с длиной списка значений");
        return QStringList();
    }
    tmpString = "SELECT ";
    for (i = 0; i < fields.size(); i++)
        tmpString += "`" + fields.at(i) + "`,";
    tmpString = tmpString.left(tmpString.size()-1); // удаляем последнюю запятую
    tmpString += " FROM `" + tble + "` WHERE ";
    for (i = 0; i < cmpfields.size(); i++)
        tmpString += "`" + cmpfields.at(i) + "`=\""+cmpvalues.at(i)+"\" AND ";
    tmpString = tmpString.left(tmpString.size()-5); // удаляем последний AND
    tmpString +=  " AND `deleted`=0;";
    exec_db.exec(tmpString);
    if (!exec_db.isActive())
    {
        result = 2;
        SQLWARN;
        return QStringList();
    }
    exec_db.next();
    if (exec_db.isValid())
    {
        for (i = 0; i < fields.size(); i++)
            vl << exec_db.value(i).toString();
        result = 0;
        return vl;
    }
    result = 1;
    SQLWARN;
    return QStringList();
}

// процедура берёт из таблицы последнюю запись, в которой cmpfield=cmpvalue, по полю field и возвращает значение поля

QString s_sql::GetLastValueFromTableByField (QSqlDatabase db, QString tble, QString field, QString cmpfield, QString cmpvalue)
{
    QString tmpString;
    QString vl;
    QSqlQuery exec_db (db);

    tmpString = "SELECT `" + field + "` FROM `" + tble + "` WHERE `" + cmpfield + "`=\"" + cmpvalue + "\" AND `deleted`=0 ORDER BY `id"+tble+"` DESC;";
    exec_db.exec(tmpString);
    exec_db.next();
    if (exec_db.isValid())
    {
        vl = exec_db.value(0).toString();
        result = 0;
        return vl;
    }
    result = 1;
    SQLWARN;
    return QString();
}

// процедура вставляет новую запись с первым свободным индексом в db:tble, используя имена полей из fl и значения из vl

QString s_sql::InsertValuesToTable(QSqlDatabase db, QString tble, QStringList fl, QStringList vl)
{
    int i;

    if (fl.size() != vl.size())
    {
        result=1; // кол-во полей и кол-во значений не равны друг другу, ошибка
        WARNMSG(PublicClass::ER_SQL, __LINE__, "Длина списка полей сравнения не совпадает с длиной списка значений");
        return QString(); // проблемы с записью
    }
    QSqlQuery exec_db (db);
    QString newID;
    newID = QString::number(GetNextFreeIndex(db, tble));
    QString tmpString = "INSERT INTO `" + tble + "` (`id" + tble + "`";
    for (i = 0; i < fl.size(); i++)
        tmpString += ",`" + fl.at(i) + "`";
    tmpString += ") VALUES(\"" + newID + "\"";
    for (i = 0; i < vl.size(); i++)
        tmpString += ",\"" + vl.at(i) + "\"";
    tmpString += ");";
    exec_db.exec(tmpString);
    if (exec_db.isActive())
    {
        result = 0;
        return newID; // всё ок
    }
    SQLWARN;
    result=2;
    return QString(); // проблемы с записью
}

// процедура обновления данных в таблице db:tble в полях fl значениями vl, в строке, где field = value

int s_sql::UpdateValuesInTable(QSqlDatabase db, QString tble, QStringList fl, QStringList vl, QString field, QString value)
{
    int i;

    if (fl.size() != vl.size())
        return 1; // кол-во полей и кол-во значений не равны друг другу, ошибка
    QSqlQuery exec_db (db);
    QString tmpString = "UPDATE `" + tble + "` SET ";
    for (i = 0; i < fl.size(); i++)
        tmpString += "`" + fl.at(i) + "`=\"" + vl.at(i) + "\",";
    tmpString = tmpString.left(tmpString.size()-1);
    tmpString += " WHERE `" + field + "`=\"" + value + "\";";
    exec_db.exec(tmpString);
    if (exec_db.isActive())
        return 0; // всё ок
    result = 2;
    SQLWARN;
    return 2; // проблемы с записью
}

// процедура восстанавливает в path полный путь до потомка, ИД которого ссылается на db:tble:idalias

QString s_sql::GetFullPathToChild(QSqlDatabase db, QString tble, QString idalias)
{
    QString path;
    if (idalias == "0")
    {
        path = "<root>";
        return 0;
    }
    result = 0;
    path.clear();
    if (idalias == "") // для новых элементов
        return QString();
    QStringList field, values;

    values = sqlc.GetColumnsFromTable(db, tble);
    if ((values.indexOf("alias") == -1) || (values.indexOf("idalias") == -1))
    {
        path = GetValueFromTableByID(db, tble, tble, idalias);
        if (path.isEmpty())
        {
            result = 3;
            return QString();
        }
        return path;
    }
    field << "alias" << "idalias";
    values << "temp" << "temp"; // для выполнения следующего сравнения

    while (values.at(1) != "0") // пока не дошли до корня дерева
    {
        values = GetValuesFromTableByID(db, tble, field, idalias);
        if (values.isEmpty())
        {
            result = 2;
            return QString();
        }
        path.insert(0, ">" + values.at(0));
        idalias = values.at(1);
    }
    if (path.isEmpty())
    {
        result = 1;
        return QString(); // некорректный запрос
    }
    path.remove(0, 1); // убираем первый знак "-"
    return path;
}

// процедура ставит признак deleted равным 1 для записей, у которых field равен value

int s_sql::DeleteFromDB(QSqlDatabase db, QString tble, QString field, QString value)
{
    QSqlQuery exec_db(db);
    exec_db.exec("UPDATE `"+tble+"` SET `deleted`=1 WHERE `"+field+"`=\""+value+"\";");
    if (exec_db.isActive())
    {
        result = 0;
        return 0;
    }
    else
    {
        result = 1;
        SQLWARN;
        return 1;
    }
}

// процедура реально удаляет строку, для которой field равно value

int s_sql::RealDeleteFromDB(QSqlDatabase db, QString tble, QString field, QString value)
{
    QSqlQuery exec_db(db);
    exec_db.exec("DELETE FROM `"+tble+"` WHERE `"+field+"`=\""+value+"\";");
    if (exec_db.isActive())
    {
        result = 0;
        return 0;
    }
    else
    {
        result = 1;
        SQLWARN;
        return 1;
    }
}

// процедура ищет записи с пустыми полями fields и записывает их в список probid, при этом возвращая ненулевой результат

int s_sql::CheckDBForEmptyFields(QSqlDatabase db, QString tble, QStringList fields, QStringList &probid)
{
    int i;
    probid.clear();

    QSqlQuery exec_db (db);
    for (i = 0; i < fields.size(); i++)
    {
        QString tmpString = "SELECT `id" + tble + "` FROM `" + tble + "` WHERE `" + fields.at(i) + "` IS NULL AND `deleted`=0;";
        exec_db.exec(tmpString);
        if (!exec_db.isActive())
            SQLWARN;
        while (exec_db.next())
            probid << exec_db.value(0).toString();
    }
    if (probid.isEmpty())
        return 0;
    return 1;
}

// вернуть список из записей, для которых field похож на regexpstr

QList<QStringList> s_sql::SearchInTableLike(QSqlDatabase db, QString tble, QString field, QString regexpstr)
{
    QList<QStringList> sl;
    QStringList tmpsl;
    QSqlQuery exec_db(db);
    int i;
    sl.clear();
    QStringList col = GetColumnsFromTable(db, tble);
    if (result)
        return QList<QStringList>();
    QString tmpString = "SELECT ";
    for (i = 0; i < col.size(); i++)
        tmpString += "`"+col.at(i)+"`,";
    tmpString.chop(1);
    tmpString +=  "FROM `"+tble+"` WHERE `"+field+"` RLIKE '"+regexpstr+"' AND `deleted`=0;";
    exec_db.exec(tmpString);
    while (exec_db.next())
    {
        tmpsl.clear();
        for (i=0; i<col.size(); i++)
            tmpsl.append(exec_db.value(i).toString());
        sl.append(tmpsl);
    }
    if (sl.isEmpty())
    {
        result = 1;
        SQLWARN;
        return sl;
    }
    result = 0;
    return sl;
}

// процедура возвращает набор строк из таблицы db.tble, значения cmpfield которых равны значению cmpvalue

QList<QStringList> s_sql::GetMoreValuesFromTableByField(QSqlDatabase db, QString tble, QStringList fields, QString cmpfield, QString cmpvalue, \
                                                        QString orderby, bool asc)
{
    QList<QStringList> sl;
    QStringList tmpsl;
    QString tmpString;
    int i;
    QSqlQuery exec_db (db);

    tmpString = "SELECT ";
    for (i = 0; i < fields.size(); i++)
        tmpString += "`" + fields.at(i) + "`,";
    tmpString = tmpString.left(tmpString.size()-1); // удаляем последнюю запятую
    tmpString += " FROM `" + tble + "` WHERE `" + cmpfield + "`=\"" + cmpvalue + "\" AND `deleted`=0";
    if (!orderby.isEmpty())
    {
        tmpString += " ORDER BY `"+orderby+"` ";
        if (asc)
            tmpString += "ASC";
        else
            tmpString += "DESC";
    }
    tmpString += ";";
    exec_db.exec(tmpString);
    while (exec_db.next())
    {
        tmpsl.clear();
        for (i = 0; i < fields.size(); i++)
            tmpsl << exec_db.value(i).toString();
        sl.append(tmpsl);
    }
    if (sl.isEmpty())
    {
        result=1;
        return QList<QStringList>();
    }
    else
    {
        result = 0;
        return sl;
    }
}
