#include "s_sql.h"
#include "publicclass.h"
#include <QVariant>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include "client.h"

s_sql sqlc;

s_sql::s_sql(QObject *parent) : QObject(parent)
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
/*
QSqlDatabase s_sql::GetDBByTableName(QString tble)
{
    QStringList dbs = GetDBFromSQL();
    for (int i = 0; i < dbs.size(); i++)
    {
        QStringList tbles = GetTablesFromDB(dbs.at(i).left(3));
        while (!tbles.isEmpty())
        {
            QString tmpString = tbles.takeFirst();
            if (tble == tmpString)
                return tmpdb;
        }
    }
    return QSqlDatabase();
}
*/
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

QStringList s_sql::GetTablesFromDB(QString db)
{
    QStringList sl;
    QSqlQuery exec_db (GetDB(db));

    sl.clear();
    exec_db.exec("SHOW TABLES;");
    while (exec_db.next())
        sl << exec_db.value(0).toString();
    return sl;
}

// процедура возвращает список полей из db:tble

QStringList s_sql::GetColumnsFromTable(QString db, QString tble)
{
    QStringList sl;
    QSqlQuery exec_db (GetDB(db));

    sl.clear();
    exec_db.exec("SHOW COLUMNS FROM `" + tble + "`;");
    if (!exec_db.isActive())
    {
        result = 1;
        LastError = exec_db.lastError().text();
        return QStringList();
    }
    while (exec_db.next())
        sl << exec_db.value(0).toString();
    result=0;
    return sl;
}

// процедура возвращает true в случае наличия таблицы в БД, иначе false

bool s_sql::CheckForTable(QString db, QString tble)
{
    QSqlQuery exec_db (GetDB(db));
    exec_db.exec("SELECT * FROM `" + tble + "`;");
    if (!exec_db.isActive())
    {
        result = 1;
        return false;
    }
    result=0;
    return true;
}

// процедура создаёт новую таблицу, в fl имена столбцов

void s_sql::CreateTable(QString db, QString tble, QStringList fl, bool Simple)
{
    QSqlQuery exec_db(GetDB(db));
    QString tmpString;
    if (Simple)
        tmpString = "CREATE TABLE `"+tble+"` (`id` int(11) NOT NULL,";
    else
        tmpString = "CREATE TABLE `"+tble+"` (`id"+tble+"` int(11) NOT NULL,";
    for (int i = 0; i < fl.size(); i++)
        tmpString += "`"+fl.at(i)+"` varchar(128) DEFAULT NULL,";
    if (Simple)
        tmpString += "`idpers` varchar(128) DEFAULT NULL,`date` VARCHAR(128) DEFAULT NULL,`deleted` int(1) NOT NULL DEFAULT '0',"
                "PRIMARY KEY (`id`),UNIQUE KEY `id_UNIQUE` (`id`)) ENGINE=MyISAM DEFAULT CHARSET=utf8;";
    else
        tmpString += "`idpers` varchar(128) DEFAULT NULL,`date` VARCHAR(128) DEFAULT NULL,`deleted` int(1) NOT NULL DEFAULT '0',"
                "PRIMARY KEY (`id"+tble+"`),UNIQUE KEY `id"+tble+"_UNIQUE` (`id"+tble+"`)) ENGINE=MyISAM DEFAULT CHARSET=utf8;";
    exec_db.exec(tmpString);
    if (exec_db.isActive())
    {
        result = 0;
        return;
    }
    result = 1;
    LastError = exec_db.lastError().text();
    return;
}

// удалить поля по списку DeleteList и добавить поля по списку AddList. Добавить id<tble>, если требуется

void s_sql::AlterTable(QString db, QString tble, QStringList DeleteList, QStringList AddList)
{
    QSqlQuery exec_db(GetDB(db));
    // сначала проверим наличие полей id<tble>,idpers,date,deleted
    QStringList sl = GetColumnsFromTable(db, tble);
    bool IdExist = (sl.indexOf("id"+tble) != -1);
    bool DateExist = (sl.indexOf("date") != -1);
    bool IdpersExist = (sl.indexOf("idpers") != -1);
    bool DeletedExist = (sl.indexOf("deleted") != -1);
    if (result)
    {
        LastError = exec_db.lastError().text();
        return;
    }
    QString tmpString = "ALTER TABLE `"+tble+"` ";
    if ((!AddList.isEmpty()) || !IdExist || !DateExist || !IdpersExist || !DeletedExist)
        // нет одного из стандартных полей или есть что-то в списке на добавление
    {
        if (!IdExist)
            tmpString += "ADD COLUMN `id"+tble+"` int(11) NOT NULL,";
        if (!DateExist)
            tmpString += "ADD COLUMN `date` VARCHAR(128) DEFAULT NULL,";
        if (!IdpersExist)
            tmpString += "ADD COLUMN `idpers` VARCHAR(128) DEFAULT NULL,";
        if (!DeletedExist)
            tmpString += "ADD COLUMN `deleted` int(1) NOT NULL DEFAULT '0',";
        while (!AddList.isEmpty())
        {
            QString tmps = AddList.first();
            tmpString += "ADD COLUMN `"+tmps+"` VARCHAR(128) NULL,";
            AddList.removeFirst();
        }
        tmpString.chop(1); // последняя запятая
        tmpString.append(",");
    }
    if (!DeleteList.isEmpty())
    {
        while (!DeleteList.isEmpty())
        {
            QString tmps = DeleteList.first();
            tmpString += "DROP COLUMN `"+tmps+"`,";
            DeleteList.removeFirst();
        }
    }
    tmpString.chop(1);
    tmpString += ";";
    exec_db.exec(tmpString);
    if (exec_db.isActive())
    {
        result = 0;
        return;
    }
    result = 1;
    LastError = exec_db.lastError().text();
    return;
}

// процедура удаляет таблицу из БД

void s_sql::DropTable(QString db, QString tble)
{
    QSqlQuery exec_db (GetDB(db));
    exec_db.exec("DROP TABLE IF EXISTS `"+tble+"`;");
    if (exec_db.isActive())
    {
        result = 0;
        return;
    }
    result = 1;
    LastError = exec_db.lastError().text();
    return;
}

// процедура добавляет столбец к таблице

void s_sql::AddColumn(QString db, QString tble, QString col, QString def)
{
    QStringList lastcol = sqlc.GetColumnsFromTable(db, tble);
    QSqlQuery exec_db(GetDB(db));
    QString tmpString = (def == "NULL")?"":"DEFAULT '"+def+"'";
    tmpString = "ALTER TABLE `"+tble+"` ADD COLUMN `"+col+"` VARCHAR(128) NULL "+tmpString+" AFTER `"+lastcol.last()+"`;";
    exec_db.exec(tmpString);
    if (exec_db.isActive())
        result = 0;
    else
    {
        LastError = exec_db.lastError().text();
        result = 1;
    }
    return;
}

// взять следующий свободный индекс из БД db, таблицы tble и вернуть его int-ом

int s_sql::GetNextFreeIndex(QString db, QString tble)
{
    long i = 1;
    QSqlQuery exec_db (GetDB(db));
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
        LastError = exec_db.lastError().text();
        return -1;
    }
}

// взять следующий свободный индекс из БД db, таблицы tble и вернуть его int-ом. id - без имени таблицы

int s_sql::GetNextFreeIndexSimple(QString db, QString tble)
{
    long i = 1;
    QSqlQuery exec_db (GetDB(db));
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
        LastError = exec_db.lastError().text();
        return -1;
    }
}

// процедура берёт из таблицы запись № id<tble> по полям fields и возвращает значения полей

QStringList s_sql::GetValuesFromTableByID(QString db, QString tble, QStringList fields, QString id)
{
    QStringList vl;

    vl = GetValuesFromTableByField(db, tble, fields, "id"+tble, id);
    return vl;
}

// процедура берёт из таблицы запись № id по полям fields и возвращает значения полей

QStringList s_sql::GetValuesFromTableByIDSimple(QString db, QString tble, QStringList fields, QString id)
{
    QStringList vl;

    vl = GetValuesFromTableByField(db, tble, fields, "id", id);
    return vl;
}

// процедура берёт из таблицы запись № id<tble> по полю field и возвращает значение поля

QString s_sql::GetValueFromTableByID(QString db, QString tble, QString field, QString id)
{
    QString vl;

    vl = GetValueFromTableByField(db, tble, field, "id"+tble, QString::number(id.toInt()));
    return vl;
}

// процедура берёт из таблицы первую запись, в которой cmpfield=cmpvalue, по полям fields и возвращает значения полей

QStringList s_sql::GetValuesFromTableByField(QString db, QString tble, QStringList fields, QString cmpfield, QString cmpvalue)
{
    QString tmpString;
    QStringList vl;
    int i;
    QSqlQuery exec_db (GetDB(db));

    tmpString = "SELECT ";
    for (i = 0; i < fields.size(); i++)
        tmpString += "`" + fields.at(i) + "`,";
    tmpString = tmpString.left(tmpString.size()-1); // удаляем последнюю запятую
    tmpString += " FROM `" + tble + "` WHERE `" + cmpfield + "`=\"" + cmpvalue + "\" AND `deleted`=0;";
    exec_db.exec(tmpString);
    if (exec_db.isActive())
    {
        while (exec_db.next())
        {
            vl.clear();
            i = 0;
            while (i < fields.size())
                vl << exec_db.value(i++).toString();
        }
        result=0;
        return vl;
    }
    result=1;
    LastError = exec_db.lastError().text();
    return QStringList();
}

// процедура берёт из таблицы все значения по столбцу column для всех строк

QStringList s_sql::GetValuesFromTableByColumn(QString db, QString tble, QString column, QString orderby, bool asc)
{
    QString tmpString;
    QStringList vl;
    QSqlQuery exec_db (GetDB(db));

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
    if (!exec_db.isActive())
    {
        result = 2;
        LastError = exec_db.lastError().text();
        return QStringList();
    }
    vl.clear();
    while (exec_db.next())
        vl << exec_db.value(0).toString();
    if (vl.isEmpty())
    {
        result=1;
        return QStringList();
    }
    result=0;
    return vl;
}

// процедура берёт из таблицы все значения по столбцам columns

QList<QStringList> s_sql::GetValuesFromTableByColumns(QString db, QString tble, QStringList columns)
{
    QString tmpString;
    QList<QStringList> vl;
    QStringList tmpStringList;
    QSqlQuery exec_db (GetDB(db));
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
        LastError = exec_db.lastError().text();
        return QList<QStringList>();
    }
    else
    {
        result = 0;
        return vl;
    }
}

// процедура берёт из таблицы только те значения по столбцу column, для которых поле str1 равно значению str2

QStringList s_sql::GetValuesFromTableByColumnAndField(QString db, QString tble, QString column, QString cmpfield, QString cmpvalue,\
                                                      QString orderby, bool asc)
{
    QString tmpString;
    QStringList vl;
    QSqlQuery exec_db (GetDB(db));

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
        LastError = exec_db.lastError().text();
        return QStringList();
    }
    vl.clear();
    while (exec_db.next())
        vl << exec_db.value(0).toString();
    result=0;
    return vl;
}

// процедура берёт из таблицы первую запись, в которой cmpfield=cmpvalue, по полю field и возвращает значение поля

QString s_sql::GetValueFromTableByField (QString db, QString tble, QString field, QString cmpfield, QString cmpvalue)
{
    QString tmpString;
    QString vl;
    if (pc.AutonomousMode)
    {
        QSqlDatabase sqldb = GetDB(db);
        if (sqldb.isValid())
        {
            QSqlQuery exec_db (db);
            tmpString = "SELECT `" + field + "` FROM `" + tble + "` WHERE `" + cmpfield + "`=\"" + cmpvalue + "\" AND `deleted`=0;";
            exec_db.exec(tmpString);
            if (!exec_db.isActive())
            {
                result = SQLC_FAILED;
                LastError = exec_db.lastError().text();
                return QString();
            }
            exec_db.next();
            if (exec_db.isValid())
            {
                vl = exec_db.value(0).toString();
                result = SQLC_OK;
                return vl;
            }
            result = SQLC_EMPTY;
            return QString();
        }
        else
        {
            result = SQLC_FAILED;
            LastError = "Невозможно открыть БД "+db;
            return QString();
        }
    }
    else // server mode
    {
        QStringList sl = QStringList() << "1" << "1" << db << tble << field << cmpfield << cmpvalue;
        QueryResult.clear();
        connect(Cli,SIGNAL(DataReady(QStringList&)),this,SLOT(AppendToQueryResult(QStringList&)));
        Cli->SendCmd(Client::CMD_GVSBFS, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        disconnect(Cli,SIGNAL(DataReady(QStringList&)),this,0);
        if (QueryResult.size() > 0)
        {
            sl = QueryResult.at(0);
            if (sl.size() > 0)
                return QueryResult.at(0).at(0);
        }
        return QString();
    }
}

// слот, добавляющий к списку списков строк QueryResult значения из канала связи с сервером

void s_sql::AppendToQueryResult(QStringList &sl)
{
    QueryResult.append(sl);
}

// процедура берёт из таблицы первую запись, в которой соответствующие поля из cmpfields=cmpvalues, по полю field и возвращает значение поля

QString s_sql::GetValueFromTableByFields (QString db, QString tble, QString field, QStringList cmpfields, QStringList cmpvalues)
{
    QString tmpString;
    QString vl;
    QSqlQuery exec_db (GetDB(db));
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
        result =SQLC_FAILED;
        LastError = exec_db.lastError().text();
        return QString();
    }
    exec_db.next();
    if (exec_db.isValid())
    {
        vl = exec_db.value(0).toString();
        result = SQLC_OK;
        return vl;
    }
    result = SQLC_EMPTY;
    return QString();
}

// процедура берёт из таблицы поля fields для строки, в которой поля cmpfield равны cmpvalues

QStringList s_sql::GetValuesFromTableByFields (QString db, QString tble, QStringList fields, QStringList cmpfields, QStringList cmpvalues, QString orderby, bool asc)
{
    QList<QStringList> lsl = GetMoreValuesFromTableByFields(db,tble,fields,cmpfields,cmpvalues,orderby,asc);
    if (lsl.size())
        return lsl.at(0);
    else
        return QStringList();
}

// процедура берёт из таблицы последнюю запись, в которой cmpfield=cmpvalue, по полю field и возвращает значение поля

QString s_sql::GetLastValueFromTableByField (QString db, QString tble, QString field, QString cmpfield, QString cmpvalue)
{
    QString tmpString;
    QString vl;
    QSqlQuery exec_db (GetDB(db));

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
    LastError = exec_db.lastError().text();
    return QString();
}

// добавление новой пустой записи и возврат нового ИД для БД Altium

QString s_sql::InsertValuesSimple(QString db, QString tble, QStringList fl, QStringList vl)
{
    int i;
    QSqlQuery exec_db (GetDB(db));
    QString newID = QString::number(GetNextFreeIndexSimple(db, tble));
    QString tmpString = "INSERT INTO `" + tble + "` (`id`";
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
    LastError = exec_db.lastError().text();
    result=2;
    return QString(); // проблемы с записью
}

// процедура вставляет новую запись с первым свободным индексом в db:tble, используя имена полей из fl и значения из vl

QString s_sql::InsertValuesToTable(QString db, QString tble, QStringList fl, QStringList vl)
{
    int i;

    if (fl.size() != vl.size())
    {
        result=1; // кол-во полей и кол-во значений не равны друг другу, ошибка
        WARNMSG(PublicClass::ER_SQL, __LINE__, "Длина списка полей сравнения не совпадает с длиной списка значений");
        return QString(); // проблемы с записью
    }
    QSqlQuery exec_db (GetDB(db));
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
    LastError = exec_db.lastError().text();
    result=2;
    return QString(); // проблемы с записью
}

// процедура обновления данных в таблице db:tble в полях fl значениями vl, в строке, где field = value

int s_sql::UpdateValuesInTable(QString db, QString tble, QStringList fl, QStringList vl, QString field, QString value)
{
    int i;

    if (fl.size() != vl.size())
        return 1; // кол-во полей и кол-во значений не равны друг другу, ошибка
    QSqlQuery exec_db (GetDB(db));
    QString tmpString = "UPDATE `" + tble + "` SET ";
    for (i = 0; i < fl.size(); i++)
        tmpString += "`" + fl.at(i) + "`=\"" + vl.at(i) + "\",";
    tmpString = tmpString.left(tmpString.size()-1);
    tmpString += " WHERE `" + field + "`=\"" + value + "\";";
    exec_db.exec(tmpString);
    if (exec_db.isActive())
        return 0; // всё ок
    result = 2;
    LastError = exec_db.lastError().text();
    return 2; // проблемы с записью
}

// процедура восстанавливает в path полный путь до потомка, ИД которого ссылается на db:tble:idalias

QString s_sql::GetFullPathToChild(QString db, QString tble, QString idalias)
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
    if ((values.indexOf(tble) == -1) || (values.indexOf("idalias") == -1))
    {
        path = GetValueFromTableByID(db, tble, tble, idalias);
        if (path.isEmpty())
        {
            result = 3;
            return QString();
        }
        return path;
    }
    field << tble << "idalias";
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

int s_sql::DeleteFromDB(QString db, QString tble, QString field, QString value)
{
    QSqlQuery exec_db(GetDB(db));
    exec_db.exec("UPDATE `"+tble+"` SET `deleted`=1 WHERE `"+field+"`=\""+value+"\";");
    if (exec_db.isActive())
    {
        result = 0;
        return 0;
    }
    else
    {
        result = 1;
        LastError = exec_db.lastError().text();
        return 1;
    }
}

// процедура реально удаляет строку, для которой field равно value

int s_sql::RealDeleteFromDB(QString db, QString tble, QStringList fields, QStringList values)
{
    QSqlQuery exec_db(GetDB(db));
    if (fields.size() != values.size())
    {
        LastError = exec_db.lastError().text();
        result = 1;
        return 1;
    }
    QString tmps = "DELETE FROM `"+tble+"` WHERE ";
    for (int i=0; i<fields.size(); i++)
        tmps += "`"+fields.at(i)+"`=\""+values.at(i)+"\" AND ";
    tmps.chop(5); // убрали " AND "
    tmps += ";";
    exec_db.exec(tmps);
    if (exec_db.isActive())
    {
        result = 0;
        return 0;
    }
    else
    {
        result = 1;
        LastError = exec_db.lastError().text();
        return 1;
    }
}

// процедура ищет записи с пустыми полями fields и записывает их ИД в список probid, при этом возвращая ненулевой результат

int s_sql::CheckDBForEmptyFields(QString db, QString tble, QString field, QStringList &probid)
{
    probid.clear();

    QSqlQuery exec_db (GetDB(db));
    QString tmpString = "SELECT `id" + tble + "` FROM `" + tble + "` WHERE `" + field + "` IS NULL AND `deleted`=0;";
    exec_db.exec(tmpString);
    if (!exec_db.isActive())
        LastError = exec_db.lastError().text();
    while (exec_db.next())
        probid << exec_db.value(0).toString();
    if (probid.isEmpty())
        return 0;
    return 1;
}

// вернуть список из записей, для которых field похож на regexpstr

QList<QStringList> s_sql::SearchInTableLike(QString db, QString tble, QString field, QString regexpstr)
{
    QList<QStringList> sl;
    QStringList tmpsl;
    QSqlQuery exec_db(GetDB(db));
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
    if (!exec_db.isActive())
    {
        LastError = exec_db.lastError().text();
        result = 2;
        return QList<QStringList>();
    }
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
        LastError = exec_db.lastError().text();
        return sl;
    }
    result = 0;
    return sl;
}

// процедура возвращает набор строк из таблицы db.tble, значения cmpfield которых равны значению cmpvalue

QList<QStringList> s_sql::GetMoreValuesFromTableByFields(QString db, QString tble, QStringList fields, QStringList cmpfields, QStringList cmpvalues, \
                                                        QString orderby, bool asc)
{
    QString tmpString;
    QList<QStringList> lsl;
    QStringList vl;
    if (pc.AutonomousMode)
    {
        QSqlQuery exec_db (GetDB(db));
        int i;

        if (cmpfields.isEmpty())
        {
            result = 4;
            WARNMSG(PublicClass::ER_SQL, __LINE__, "Переданный список сравнения пуст");
            return QList<QStringList>();
        }
        if (cmpfields.size() != cmpvalues.size())
        {
            result = 5;
            WARNMSG(PublicClass::ER_SQL, __LINE__, "Длина списка полей сравнения не совпадает с длиной списка значений");
            return QList<QStringList>();
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
            LastError = exec_db.lastError().text();
            return QList<QStringList>();
        }
        while (exec_db.next())
        {
            vl.clear();
            for (i = 0; i < fields.size(); i++)
                vl << exec_db.value(i).toString();
            lsl.append(vl);
        }
        result = 0;
        return lsl;
    }
    else
    {
        int i;
        int fields_num = fields.size();
        int pairs_num = cmpfields.size();
        QStringList sl = QStringList() << QString::number(fields_num) << QString::number(pairs_num) << db << tble;
        for (i=0; i<fields_num; i++)
            sl << fields.at(i);
        for (i=0; i<pairs_num; i++)
        {
            sl << cmpfields.at(i);
            sl << cmpvalues.at(i);
        }
        if (!orderby.isEmpty())
        {
            sl << orderby;
            if (asc)
                sl << "ASC";
            else
                sl << "DESC";
        }
        QueryResult.clear();
        connect(Cli,SIGNAL(DataReady(QStringList&)),this,SLOT(AppendToQueryResult(QStringList&)));
        Cli->SendCmd(Client::CMD_GVSBFS, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        disconnect(Cli,SIGNAL(DataReady(QStringList&)),this,0);
        return QueryResult;
    }
}
