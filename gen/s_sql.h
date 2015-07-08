#ifndef S_SQL_H
#define S_SQL_H

#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>

class s_sql
{
public:
    s_sql();
    QSqlDatabase getdb (QString dbname);
    QString fromdb (QSqlDatabase db);
    QSqlDatabase getdbbytablename (QString tble);
    QStringList getcolumnsfromtable (QSqlDatabase db, QString tble);
    QStringList getvaluesfromtablebyid (QSqlDatabase db, QString tble, QStringList fields, QString id);
    QString getvaluefromtablebyid (QSqlDatabase db, QString tble, QString field, QString id);
    QStringList getvaluesfromtablebyfield (QSqlDatabase db, QString tble, QStringList fields, QString cmpfield, QString cmpvalue);
    QStringList getvaluesfromtablebyfields (QSqlDatabase db, QString tble, QStringList fields, QStringList cmpfields, QStringList cmpvalues);
    QStringList getvaluesfromtablebycolumn (QSqlDatabase db, QString tble, QString column, QString orderby="", bool asc=true);
    QList<QStringList> getvaluesfromtablebycolumns (QSqlDatabase db, QString tble, QStringList columns);
    QStringList getvaluesfromtablebycolumnandfield(QSqlDatabase db, QString tble, QString field, QString cmpfield, QString cmpvalue);
    QString getvaluefromtablebyfield (QSqlDatabase db, QString tble, QString field, QString cmpfield, QString cmpvalue);
    QString getvaluefromtablebyfields (QSqlDatabase db, QString tble, QString field, QStringList cmpfields, QStringList cmpvalues);
    QString getlastvaluefromtablebyfield (QSqlDatabase db, QString tble, QString field, QString cmpfield, QString cmpvalue);
    QString getvaluefromtablebytablefields(QString tablenheaders, QString keyfield, QString keyfieldid);
    QList<QStringList> searchintablefieldlike (QSqlDatabase db, QString tble, QStringList col, QString field, QString regexpstr);
    QList<QStringList> getmorevaluesfromtablebyfield (QSqlDatabase db, QString tble, QStringList fields, QString cmpfield, QString cmpvalue,\
                                                      QString orderby="", bool asc=true);
    QStringList gettablesfromdb(QSqlDatabase db);
    QStringList getdbfromsql();
    QString insertvaluestotable (QSqlDatabase db, QString tble, QStringList fl, QStringList vl, int id = 0);
    int getnextfreeindex(QSqlDatabase db, QString tble);
    int getfullpathtochild(QSqlDatabase db, QString tble, QString idalias, QString &path);
    int updatevaluesintable (QSqlDatabase db, QString tble, QStringList fl, QStringList vl, QString field, QString value);
    int checkdbforemptyfields (QSqlDatabase db, QString tble, QStringList fields, QStringList &probid);
    int deletefromdb (QSqlDatabase db, QString tble, QString field, QString value);
    int dropTable (QSqlDatabase db, QString tble);
    void prepareslsfortree(QStringList &sl1, QStringList &sl2);
    int createTable (QSqlDatabase db, QString tble, QStringList fl);
    void addColumn(QSqlDatabase db, QString tble, QString col, QString def = "NULL");

    void bytablefieldsinsert (QString tble, QStringList headers, QStringList values);
    void bytablefieldssetone (QString tble, QString headers, QString value, QString id);
    QString bytablefieldsgetvalue (QString tble, QString headers);
    QStringList bytablefieldsgetvalues (QString tble, QStringList headers);
    int result;
};

extern s_sql sqlc;

#endif // S_SQL_H
