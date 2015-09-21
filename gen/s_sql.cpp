#include "s_sql.h"
#include "publicclass.h"
#include <QVariant>
#include <QDebug>

s_sql sqlc;

s_sql::s_sql()
{
}

// процедура возвращает базу данных по её имени в dbname

QSqlDatabase s_sql::getdb(QString dbname)
{
    QSqlDatabase tmpdb;
    if (pc.db.keys().contains(dbname))
        tmpdb = pc.db[dbname];
    else
        return QSqlDatabase();
    return tmpdb;
}

// процедура возвращает имя БД по её экземпляру

QString s_sql::fromdb(QSqlDatabase db)
{
/*    if (db.databaseName() == "supik") return "sup";
    else if (db.databaseName() == "enterprise") return "ent";
    else if (db.databaseName() == "altium") return "alt";
    else if (db.databaseName() == "solidworks") return "sol";
    else if (db.databaseName() == "devices") return "dev";
    else if (db.databaseName() == "schemagee") return "sch";
    else if (db.databaseName() == "constructives") return "con";*/
    if (pc.db.values().contains(db))
        return pc.db.key(db);
    else
        return QString();
}

// процедура возвращает базу данных по имени таблицы, которая в БД должна содержаться

QSqlDatabase s_sql::getdbbytablename(QString tble)
{
    QSqlDatabase tmpdb;
    QStringList dbs = getdbfromsql();
    for (int i = 0; i < dbs.size(); i++)
    {
        tmpdb = getdb(dbs.at(i).left(3));
        QStringList tbles = gettablesfromdb(tmpdb);
        while (!tbles.isEmpty())
        {
            QString tmpString = tbles.takeFirst();
            if (tble == tmpString)
                return tmpdb;
        }
    }
    return QSqlDatabase();
}

// процедура возвращает список полей из db:tble

QStringList s_sql::getcolumnsfromtable(QSqlDatabase db, QString tble)
{
    QStringList sl;
    QSqlQuery get_fields_from_db (db);

    sl.clear();
    get_fields_from_db.exec("SHOW COLUMNS FROM `" + tble + "`;");
    if (!get_fields_from_db.isActive())
    {
        result = 1;
        return QStringList();
    }
    while (get_fields_from_db.next())
        sl << get_fields_from_db.value(0).toString();
    result=0;
    return sl;
}

// процедура берёт из таблицы запись № id<tble> по полям fields и возвращает значения полей

QStringList s_sql::getvaluesfromtablebyid(QSqlDatabase db, QString tble, QStringList fields, QString id)
{
    QStringList vl;

    vl = getvaluesfromtablebyfield(db, tble, fields, "id"+tble, id);
//    result=0;
    return vl;
}

// процедура берёт из таблицы запись № id<tble> по полю field и возвращает значение поля

QString s_sql::getvaluefromtablebyid(QSqlDatabase db, QString tble, QString field, QString id)
{
    QString vl;

    vl = getvaluefromtablebyfield(db, tble, field, "id"+tble, id);
    result=0;
    return vl;
}

// процедура берёт из таблицы первую запись, в которой cmpfield=cmpvalue, по полям fields и возвращает значения полей

QStringList s_sql::getvaluesfromtablebyfield(QSqlDatabase db, QString tble, QStringList fields, QString cmpfield, QString cmpvalue)
{
    QString tmpString;
    QStringList vl;
    int i;
    QSqlQuery get_fields_from_db (db);

    tmpString = "SELECT ";
    for (i = 0; i < fields.size(); i++)
        tmpString += "`" + fields.at(i) + "`,";
    tmpString = tmpString.left(tmpString.size()-1); // удаляем последнюю запятую
    tmpString += " FROM `" + tble + "` WHERE `" + cmpfield + "`=\"" + cmpvalue + "\" AND `deleted`=0;";
    get_fields_from_db.exec(tmpString);
    get_fields_from_db.next();
    if (get_fields_from_db.isValid())
    {
        vl.clear();
        i = 0;
        while (i < fields.size())
            vl << get_fields_from_db.value(i++).toString();
        result=0;
        return vl;
    }
    result=1;
    return QStringList();
}

// процедура берёт из таблицы все значения по столбцу column для всех строк

QStringList s_sql::getvaluesfromtablebycolumn(QSqlDatabase db, QString tble, QString column, QString orderby, bool asc)
{
    QString tmpString;
    QStringList vl;
    QSqlQuery get_fields_from_db (db);

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
    get_fields_from_db.exec(tmpString);
    vl.clear();
    while (get_fields_from_db.next())
        vl << get_fields_from_db.value(0).toString();
    if (vl.isEmpty())
    {
        result=1;
        return QStringList();
    }
    result=0;
    return vl;
}

// процедура берёт из таблицы все значения по столбцам columns

QList<QStringList> s_sql::getvaluesfromtablebycolumns(QSqlDatabase db, QString tble, QStringList columns)
{
    QString tmpString;
    QList<QStringList> vl;
    QStringList tmpStringList;
    QSqlQuery get_fields_from_db (db);
    int i;

    for (i = 0; i < columns.size(); i++)
    {
        tmpString = "SELECT `" + columns.at(i) + "` FROM `" + tble + "` WHERE `deleted`=0;";
        get_fields_from_db.exec(tmpString);
        tmpStringList.clear();
        while (get_fields_from_db.next())
            tmpStringList.append(get_fields_from_db.value(0).toString());
        vl.append(tmpStringList);
    }
    if (vl.isEmpty())
    {
        result=1;
        return QList<QStringList>();
    }
    else
    {
        result = 0;
        return vl;
    }
}

// процедура берёт из таблицы только те значения по столбцу column, для которых поле str1 равно значению str2

QStringList s_sql::getvaluesfromtablebycolumnandfield(QSqlDatabase db, QString tble, QString column, QString cmpfield, QString cmpvalue)
{
    QString tmpString;
    QStringList vl;
    QSqlQuery get_fields_from_db (db);

    tmpString = "SELECT `" + column + "` FROM `" + tble + "` WHERE `deleted`=0 AND `" + cmpfield + "`=\"" + cmpvalue + "\";";
    get_fields_from_db.exec(tmpString);
    vl.clear();
    while (get_fields_from_db.next())
        vl << get_fields_from_db.value(0).toString();
    result=0;
    return vl;
}

// процедура берёт из таблицы первую запись, в которой cmpfield=cmpvalue, по полю field и возвращает значение поля

QString s_sql::getvaluefromtablebyfield (QSqlDatabase db, QString tble, QString field, QString cmpfield, QString cmpvalue)
{
    QString tmpString;
    QString vl;
    QSqlQuery get_fields_from_db (db);

    tmpString = "SELECT `" + field + "` FROM `" + tble + "` WHERE `" + cmpfield + "`=\"" + cmpvalue + "\" AND `deleted`=0;";
    get_fields_from_db.exec(tmpString);
    if (!get_fields_from_db.isActive())
    {
        result = 2;
        return QString();
    }
    get_fields_from_db.next();
    if (get_fields_from_db.isValid())
    {
        vl = get_fields_from_db.value(0).toString();
        result = 0;
        return vl;
    }
    result = 1;
    return QString();
}

// процедура берёт из таблицы первую запись, в которой соответствующие поля из cmpfields=cmpvalues, по полю field и возвращает значение поля

QString s_sql::getvaluefromtablebyfields (QSqlDatabase db, QString tble, QString field, QStringList cmpfields, QStringList cmpvalues)
{
    QString tmpString;
    QString vl;
    QSqlQuery get_fields_from_db (db);
    int i;

    if (cmpfields.isEmpty())
        return 4;
    if (cmpfields.size() != cmpvalues.size())
        return 5;

    tmpString = "SELECT `" + field + "` FROM `" + tble + "` WHERE ";
    for (i = 0; i < cmpfields.size(); i++)
        tmpString += "`" + cmpfields.at(i) + "`=\""+cmpvalues.at(i)+"\" AND ";
    tmpString = tmpString.left(tmpString.size()-5); // удаляем последний AND
    tmpString +=  " AND `deleted`=0;";
    get_fields_from_db.exec(tmpString);
    if (!get_fields_from_db.isActive())
    {
        result = 2;
        return QString();
    }
    get_fields_from_db.next();
    if (get_fields_from_db.isValid())
    {
        vl = get_fields_from_db.value(0).toString();
        result = 0;
        return vl;
    }
    result = 1;
    return QString();
}

// процедура берёт из таблицы поля fields для строки, в которой поля cmpfield равны cmpvalues

QStringList s_sql::getvaluesfromtablebyfields (QSqlDatabase db, QString tble, QStringList fields, QStringList cmpfields, QStringList cmpvalues)
{
    QString tmpString;
    QStringList vl;
    QSqlQuery get_fields_from_db (db);
    int i;

    if (cmpfields.isEmpty())
    {
        result = 4;
        return QStringList();
    }
    if (cmpfields.size() != cmpvalues.size())
    {
        result = 5;
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
    get_fields_from_db.exec(tmpString);
    if (!get_fields_from_db.isActive())
    {
        result = 2;
        return QStringList();
    }
    get_fields_from_db.next();
    if (get_fields_from_db.isValid())
    {
        for (i = 0; i < fields.size(); i++)
            vl << get_fields_from_db.value(i).toString();
        result = 0;
        return vl;
    }
    result = 1;
    return QStringList();
}

// процедура возвращает значение по ссылке из поля tablefields таблицы tablefields, для которой table.header = tablenheaders
// значение берётся для id<tablefields.at(1)>=<keyfieldid>
// ЕСТЬ ОШИБКА В СТРОКЕ 3 - tablenheaders - ЭТО НЕ СПИСОК СТРОК, А ПРОСТО СТРОКА!!!

QString s_sql::getvaluefromtablebytablefields(QString tablenheaders, QString keyfield, QString keyfieldid)
{
    QString ftext;
    QStringList cmpfl = QStringList() << "tablename" << "header";
    QStringList cmpvl = QStringList() << tablenheaders.at(0) << tablenheaders.at(1);
    ftext = getvaluefromtablebyfields(sqlc.getdb("sup"),"tablefields","table",cmpfl,cmpvl);
    ftext += getvaluefromtablebyfields(sqlc.getdb("sup"),"tablefields","tablefields",cmpfl,cmpvl);
    if (result)
        return QString();
    // далее cmpfl используется как временная переменная, дабы не плодить переменных
    cmpfl.clear();
    cmpfl=ftext.split(".");
    ftext = getvaluefromtablebyfield(sqlc.getdb(cmpfl.at(0)), cmpfl.at(1), cmpfl.at(2), keyfield, keyfieldid);
    if (result)
        return QString();
    else
        return ftext;
}
// процедура берёт из таблицы последнюю запись, в которой cmpfield=cmpvalue, по полю field и возвращает значение поля

QString s_sql::getlastvaluefromtablebyfield (QSqlDatabase db, QString tble, QString field, QString cmpfield, QString cmpvalue)
{
    QString tmpString;
    QString vl;
    QSqlQuery get_fields_from_db (db);

    tmpString = "SELECT `" + field + "` FROM `" + tble + "` WHERE `" + cmpfield + "`=\"" + cmpvalue + "\" AND `deleted`=0 ORDER BY `id"+tble+"` DESC;";
    get_fields_from_db.exec(tmpString);
    get_fields_from_db.next();
    if (get_fields_from_db.isValid())
    {
        vl = get_fields_from_db.value(0).toString();
        result = 0;
        return vl;
    }
    result = 1;
    return QString();
}

// процедура вставляет новую запись с первым свободным индексом в db:tble, используя имена полей из fl и значения из vl

QString s_sql::insertvaluestotable(QSqlDatabase db, QString tble, QStringList fl, QStringList vl, int id)
{
    int i;

    if (fl.size() != vl.size())
    {
        result=1; // кол-во полей и кол-во значений не равны друг другу, ошибка
        return QString(); // проблемы с записью
    }
    QSqlQuery insert_new (db);
    QString newID;
    if (id != 0)
        newID = QString::number(id);
    else
        newID = QString::number(getnextfreeindex(db, tble));
    QString tmpString = "INSERT INTO `" + tble + "` (`id" + tble + "`";
    for (i = 0; i < fl.size(); i++)
        tmpString += ",`" + fl.at(i) + "`";
    tmpString += ") VALUES(\"" + newID + "\"";
    for (i = 0; i < vl.size(); i++)
        tmpString += ",\"" + vl.at(i) + "\"";
    tmpString += ");";
    insert_new.exec(tmpString);
    if (insert_new.isActive())
    {
        result = 0;
        return newID; // всё ок
    }
    tmpString = insert_new.lastError().text();
    result=2;
    return QString(); // проблемы с записью
}

// процедура обновления данных в таблице db:tble в полях fl значениями vl, в строке, где field = value

int s_sql::updatevaluesintable(QSqlDatabase db, QString tble, QStringList fl, QStringList vl, QString field, QString value)
{
    int i;

    if (fl.size() != vl.size())
        return 1; // кол-во полей и кол-во значений не равны друг другу, ошибка
    QSqlQuery update_db (db);
    QString tmpString = "UPDATE `" + tble + "` SET ";
    for (i = 0; i < fl.size(); i++)
        tmpString += "`" + fl.at(i) + "`=\"" + vl.at(i) + "\",";
    tmpString = tmpString.left(tmpString.size()-1);
    tmpString += " WHERE `" + field + "`=\"" + value + "\";";
    update_db.exec(tmpString);
    if (update_db.isActive())
        return 0; // всё ок
    return 2; // проблемы с записью
}

// взять следующий свободный индекс из БД db, таблицы tble и вернуть его int-ом

int s_sql::getnextfreeindex(QSqlDatabase db, QString tble)
{
    long i = 1;
    QSqlQuery get_indexes (db);
    QString tmpString = "SELECT `id" + tble + "` FROM `" + tble + "` ORDER BY `id" + tble + "` ASC;";
    get_indexes.exec(tmpString); // индексы сортируем по возрастанию
    if (get_indexes.isActive())
    {
        while ((get_indexes.next()) && (get_indexes.value(0).toInt(0) == i)) // пока нет пропусков
            i++;
        result = 0;
        return i;
    }
    else
    {
        result = 1;
        return -1;
    }
}

// взять следующий свободный индекс из БД db, таблицы tble и вернуть его int-ом. id - без имени таблицы

int s_sql::getnextfreeindexsimple(QSqlDatabase db, QString tble)
{
    long i = 1;
    QSqlQuery get_indexes (db);
    QString tmpString = "SELECT `id` FROM `" + tble + "` ORDER BY `id` ASC;";
    get_indexes.exec(tmpString); // индексы сортируем по возрастанию
    if (get_indexes.isActive())
    {
        while ((get_indexes.next()) && (get_indexes.value(0).toInt(0) == i)) // пока нет пропусков
            i++;
        result = 0;
        return i;
    }
    else
    {
        result = 1;
        return -1;
    }
}

// процедура восстанавливает в path полный путь до потомка, ИД которого ссылается на db:tble:idalias

int s_sql::getfullpathtochild(QSqlDatabase db, QString tble, QString idalias, QString &path)
{
    if (idalias == "0")
    {
        path = "<root>";
        return 0;
    }

    path.clear();
    if (idalias == "") // для новых элементов
        return 0;
    QStringList field, values;

    values = sqlc.getcolumnsfromtable(db, tble);
    if ((values.indexOf("alias") == -1) || (values.indexOf("idalias") == -1))
    {
        path = getvaluefromtablebyid(db, tble, tble, idalias);
        if (path.isEmpty())
            return 3;
        return 0;
    }
    field << "alias" << "idalias";
    values << "temp" << "temp"; // для выполнения следующего сравнения

    while (values.at(1) != "0") // пока не дошли до корня дерева
    {
        values = getvaluesfromtablebyid(db, tble, field, idalias);
        if (values.isEmpty())
            return 2;
        path.insert(0, ">" + values.at(0));
        idalias = values.at(1);
    }
    if (path.isEmpty())
        return 1; // некорректный запрос
    path.remove(0, 1); // убираем первый знак "-"
    return 0;
}

// процедура возвращает список таблиц из БД db

QStringList s_sql::gettablesfromdb(QSqlDatabase db)
{
    QStringList sl;
    QSqlQuery get_tables_from_db (db);

    sl.clear();
    get_tables_from_db.exec("SHOW TABLES;");
    while (get_tables_from_db.next())
        sl << get_tables_from_db.value(0).toString();
    return sl;
}

// процедура возвращает все имеющиеся БД (кроме системных)

QStringList s_sql::getdbfromsql()
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
    QSqlQuery get_tables_from_db(*db);
    get_tables_from_db.exec("SHOW DATABASES;");
    while (get_tables_from_db.next())
        sl << get_tables_from_db.value(0).toString();
    get_tables_from_db.finish();
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

// процедура ставит признак deleted равным 1 для записей, у которых field равен value

int s_sql::deletefromdb(QSqlDatabase db, QString tble, QString field, QString value)
{
    QSqlQuery deletefromdb(db);
    deletefromdb.exec("UPDATE `"+tble+"` SET `deleted`=1 WHERE `"+field+"`=\""+value+"\";");
    if (deletefromdb.isActive())
        return 0;
    else
        return 1;
}

// процедура реально удаляет строку, для которой field равно value

int s_sql::RealDeleteFromDB(QSqlDatabase db, QString tble, QString field, QString value)
{
    QSqlQuery RealDelete(db);
    RealDelete.exec("DELETE FROM `"+tble+"` WHERE `"+field+"`=\""+value+"\";");
    if (RealDelete.isActive())
        return 0;
    else
        return 1;
}

// процедура ищет записи с пустыми полями fields и записывает их в список probid, при этом возвращая ненулевой результат

int s_sql::checkdbforemptyfields(QSqlDatabase db, QString tble, QStringList fields, QStringList &probid)
{
    int i;
    probid.clear();

    QSqlQuery check_db (db);
    for (i = 0; i < fields.size(); i++)
    {
        QString tmpString = "SELECT `id" + tble + "` FROM `" + tble + "` WHERE `" + fields.at(i) + "` IS NULL AND `deleted`=0;";
        check_db.exec(tmpString);
        if (!check_db.isActive())
            tmpString = check_db.lastError().text();
        while (check_db.next())
            probid << check_db.value(0).toString();
    }
    if (probid.isEmpty())
        return 0;
    return 1;
}

// процедура формирует в sl1 и sl2 дерево, состоящее из имён БД и таблиц

void s_sql::prepareslsfortree(QStringList &sl1, QStringList &sl2)
{
    QStringList tmpsl;

    sl2.empty();
    sl1 = sqlc.getdbfromsql();
    for (int i = 0; i < sl1.size(); i++)
    {
        QString tmpString = sl1.at(i);
        tmpsl = sqlc.gettablesfromdb(sqlc.getdb(tmpString.left(3)));
        sl2.append(tmpsl);
        sl2.append("");
    }
}

// процедура удаляет таблицу из БД

int s_sql::dropTable(QSqlDatabase db, QString tble)
{
    QSqlQuery drop_db (db);
    drop_db.exec("DROP TABLE IF EXISTS `"+tble+"`;");
    if (drop_db.isActive())
        return 0;
    else
        return 1;
}

// процедура создаёт новую таблицу, в fl имена столбцов

int s_sql::createTable(QSqlDatabase db, QString tble, QStringList fl)
{
    QSqlQuery create_db(db);
    QString tmpString = "CREATE TABLE `"+tble+"` (`id"+tble+"` int(11) NOT NULL,";
    for (int i = 0; i < fl.size(); i++)
        tmpString += "`"+fl.at(i)+"` varchar(128) DEFAULT NULL,";
    tmpString += "`idpers` varchar(128) DEFAULT NULL,`date` VARCHAR(128) DEFAULT NULL,`deleted` int(1) NOT NULL DEFAULT '0',"
            "PRIMARY KEY (`id"+tble+"`),UNIQUE KEY `id"+tble+"_UNIQUE` (`id"+tble+"`)) ENGINE=MyISAM DEFAULT CHARSET=utf8;";
    create_db.exec(tmpString);
    if (create_db.isActive())
        return 0;
    else
        return 1;
}

// процедура добавляет столбец к таблице

void s_sql::addColumn(QSqlDatabase db, QString tble, QString col, QString def)
{
    QStringList lastcol = sqlc.getcolumnsfromtable(db, tble);
    QSqlQuery addcol(db);
    QString tmpString = (def == "NULL")?"":"DEFAULT '"+def+"'";
    tmpString = "ALTER TABLE `"+tble+"` ADD COLUMN `"+col+"` VARCHAR(128) NULL "+tmpString+" AFTER `"+lastcol.last()+"`;";
    addcol.exec(tmpString);
    if (addcol.isActive())
        result = 0;
    else
        result = 1;
    return;
}

// процедура возвращает список значений рядов из столбцов col таблицы db.tble, значения field которых удовлетворяют условию regexpstr

QList<QStringList> s_sql::searchintablefieldlike(QSqlDatabase db, QString tble, QStringList col, QString field, QString regexpstr)
{
    QList<QStringList> sl;
    QStringList tmpsl;
    QSqlQuery search_db(db);
    int i;
    sl.clear();
    for (i = 0; i < col.size(); i++)
    {
        tmpsl.clear();
        QString tmpString = "SELECT `"+col.at(i)+"` FROM `"+tble+"` WHERE `"+field+"` RLIKE '"+regexpstr+"' AND `deleted`=0;";
        search_db.exec(tmpString);
        int j = 0;
        while (search_db.next())
        {
            tmpsl.insert(j, search_db.value(0).toString());
            j++;
        }
        sl.insert(i, tmpsl);
    }
    if (sl.isEmpty())
    {
        result = 1;
        return sl;
    }
    result = 0;
    return sl;
}

// процедура возвращает набор строк из таблицы db.tble, значения cmpfield которых равны значению cmpvalue

QList<QStringList> s_sql::getmorevaluesfromtablebyfield(QSqlDatabase db, QString tble, QStringList fields, QString cmpfield, QString cmpvalue, \
                                                        QString orderby, bool asc)
{
    QList<QStringList> sl;
    QStringList tmpsl;
    QString tmpString;
    int i;
    QSqlQuery get_fields_from_db (db);

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
    get_fields_from_db.exec(tmpString);
    while (get_fields_from_db.next())
    {
        tmpsl.clear();
        for (i = 0; i < fields.size(); i++)
            tmpsl << get_fields_from_db.value(i).toString();
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

// процедура делает новую запись в таблицу tble (имя в tablefields)

void s_sql::bytablefieldsinsert(QString tble, QStringList headers, QStringList values)
{
    QList<QStringList> lsl;
    QStringList fl;
    QStringList tmpvl, vl, ls;
    fl << "table" << "tablefields" << "header" << "links" << "keyfield";
    lsl = sqlc.getmorevaluesfromtablebyfield(pc.sup, "tablefields", fl, "tablename", tble, "fieldsorder", true);
    if (result)
        return;
    int i;
    while (lsl.size() > 0)
    {
        QString curtble = lsl.at(0).at(0);
        if (curtble == "-")
        {
            lsl.removeAt(0);
            continue;
        }
        tmpvl.clear();
        vl.clear();
        ls.clear();
        QString tmpString;
        fl.clear();
        int ididx = -1;
        for (i = 0; i < lsl.size(); i++)
        {
            if (lsl.at(i).at(0) == curtble)
            {
                if (lsl.at(i).at(4) == "v")
                    ididx = i;
                int hdindex = headers.indexOf(lsl.at(i).at(2));
                if (hdindex == -1)
                    continue;
                if (hdindex < values.size())
                    tmpvl << values.at(hdindex);
                else
                    continue;
                fl << lsl.at(i).at(1);
                ls << lsl.at(i).at(3);
                lsl.removeAt(i); // в StringList-ах находятся только те поля, которые относятся к текущей таблице
            }
        }
        if (ididx != -1)
        {
            fl.swap(0, ididx);
            ls.swap(0, ididx);
            tmpvl.swap(0, ididx);
        }
        // теперь надо отделить мух от котлет - "живые" значения от ссылочных
        for (i = 0; i < tmpvl.size(); i++)
        {
            QStringList ls_splitted = ls.at(i).split(".");
            switch (ls_splitted.at(1).toInt())
            {
            case FW_AUTONUM:
            case FW_NUMBER:
            case FW_MASKED:
            case FW_EQUAT:
            case FW_PLAIN:
            case FW_RIGHTS:
            case FW_DATE:
            case FW_TLINK:
            {
                vl << tmpvl.at(i);
                break;
            }
            case FW_DLINK:
            {
/*                if (tmpvl.at(i).left(2) == "q_") // при вызове функции в значение для DLINK необходимо в начале подставить "q_"
                {
                    tmpString = tmpvl.at(i).right(tmpvl.at(i).size()-2);
                    ls_splitted.at()
                }
                vl << () ?  : tmpvl.at(i); */
            }
            case FW_LINK:
            case FW_ALLINK:
            {
                QStringList tmpsl = QStringList() << "table" << "tablefields";
                QStringList cmpfl = QStringList() << "tablename" << "header";
                QStringList cmpvl = QStringList() << ls_splitted.at(3) << ls_splitted.at(4);
                QStringList tmpfl = getvaluesfromtablebyfields(pc.sup, "tablefields", tmpsl, cmpfl, cmpvl);
                if (result)
                    return;
                QString tmpid = getvaluefromtablebyfield(getdb(tmpfl.at(0).split(".").at(0)), tmpfl.at(0).split(".").at(1), tmpfl.at(1), tmpfl.at(1).right(tmpfl.at(1).size()-2), tmpvl.at(i));
                if (result)
                {
                    if (ls_splitted.at(0).toInt() == FD_CHOOSE_X)
                    {
                        // записать в карантинную таблицу (если такая есть или, если нет, то в обычную) значение tmpvl.at(i) и использовать его как tmpid
                    }
                    else
                        return;
                }
                vl << tmpid;
                break;
            }
            case FW_MAXLINK:
            {
/*                QStringList tmpsl = QStringList << "table" << "tablefields" << "links";
                QStringList cmpfl = QStringList << "tablename" << "header";
                QStringList cmpvl = QStringList << ls_splitted.at(3) << ls_splitted.at(4);
                QStringList tmpfl = getvaluesfromtablebyfields(pc.sup, "tablefields", tmpsl, cmpfl, cmpvl);
                if (result)
                    return;
                bool ok;
                int tmpInt = tmpfl.at(2).toInt(&ok);
                if (ok) // в позиции находится число => ссылка на ячейку таблицы ордера
                {
                    tmpString = sqlc.getlastvaluefromtablebyfield(getdb(tmpfl.at(0).split(".").at(0)), tmpfl.at(0).split(".").at(1), tmpfl.at(1), "id"+tmpfl.at(2), tmpsl.at(3),\
                                                              mainmodel->data(mainmodel->index(j, tmpInt), Qt::DisplayRole).toString());
                    if (tmpString.isEmpty())
                    {
                        QMessageBox::warning(this, "warning!", "Ошибка в таблице flowfields по полю" + FlowFields.at(i));
                        return;
                    }
                }
                else // в поз. 4 находится имя поля из таблицы tmpsl.at(1).tmpsl.at(2).
                {
                    tmpString = sqlc.getlastvaluefromtablebyfield(sqlc.getdb(tmpsl.at(1)), tmpsl.at(2), "id"+tmpsl.at(2), tmpsl.at(3),\
                                                              tmpsl.at(4));
                    if (tmpString.isEmpty())
                    {
                        QMessageBox::warning(this, "warning!", "Ошибка в таблице flowfields по полю" + FlowFields.at(i));
                        return;
                    }
                }
                vl << tmpString; */
                break;
            }
            default:
                break;
            }
        }
        QStringList dbtble = curtble.split(".");
        tmpString = getvaluefromtablebyfield(getdb(dbtble.at(0)), dbtble.at(1), "id"+dbtble.at(1), fl.at(0), vl.at(0));
        if (result == 2)
            return;
        else if (result == 0)
            updatevaluesintable(getdb(dbtble.at(0)), dbtble.at(1), fl, vl, "id"+dbtble.at(1), tmpString);
        else
            insertvaluestotable(getdb(dbtble.at(0)), dbtble.at(1), fl, vl);
        if (result)
            return;
    }
}

QString s_sql::bytablefieldsgetvalue(QString tble, QString headers)
{
    return QString();
}
