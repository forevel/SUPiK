#include "../inc/s_tablefields.h"
#include "../inc/publicclass.h"

s_tablefields tfl;

s_tablefields::s_tablefields()
{
}

QStringList s_tablefields::GetAllValuesByOneRow(QString tble, QString headers)
{
    QStringList fl = QStringList() << "table" << "tablefields" << "links";
    QStringList cmpfl = QStringList() << "tablename" << "headers";
    QStringList cmpvl = QStringList() << tble << headers;
    QStringList sl = sqlc.getvaluesfromtablebyfields(sqlc.getdb("sup"), "tablefields", fl, cmpfl, cmpvl);
    if (sqlc.result)
    {
        result = 0x11;
        return QStringList();
    }
//    fl.clear();
//    fl << sl.at(1);
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    sl = sqlc.getvaluesfromtablebycolumn(sqlc.getdb(db), tble, sl.at(1));
    if (sqlc.result)
    {
        result = sqlc.result + 0x15;
        return QStringList();
    }
    result = 0;
    return sl;
/*    PublicClass::fieldformat ff = pc.getFFfromLinks(sl.at(2));
    switch (ff.ftype)
    {
    case
    } */
}

QList<QStringList> s_tablefields::GetAllValues(QString tble)
{
    QStringList tmpsl;
    QStringList sl = sqlc.getvaluesfromtablebycolumnandfield(sqlc.getdb("sup"), "tablefields", "headers", "tablename", tble); // берём headers
    if (sqlc.result)
    {
        result = 0x21;
        return QList<QStringList>();
    }
    QList<QStringList> lsl;
    for (int i = 0; i < sl.size(); i++)
    {
        tmpsl = GetAllValuesByOneRow(tble, sl.at(i));
        lsl.append(tmpsl);
    }
    return lsl;
}

QString s_tablefields::GetOneValueByOneRowAndId(QString tble, QString headers, QString id)
{
    QString tmpString;
    QStringList fl = QStringList() << "table" << "tablefields" << "links";
    QStringList cmpfl = QStringList() << "tablename" << "headers";
    QStringList cmpvl = QStringList() << tble << headers;
    QStringList sl = sqlc.getvaluesfromtablebyfields(sqlc.getdb("sup"), "tablefields", fl, cmpfl, cmpvl);
    if (sqlc.result)
    {
        result = 0x31;
        return QStringList();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    tmpString = sqlc.getvaluefromtablebyid(sqlc.getdb(db), tble, sl.at(1));
    if (sqlc.result)
    {
        result = sqlc.result + 0x35;
        return QString();
    }
    result = 0;
    return tmpString;
}
