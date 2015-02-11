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
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    QStringList tmpsl = sqlc.getvaluesfromtablebycolumn(sqlc.getdb(db), tble, sl.at(1));
    if (sqlc.result)
    {
        result = sqlc.result + 0x15;
        return QStringList();
    }
    tmpsl.insert(0, sl.at(2));
    result = 0;
    return tmpsl;
}

QList<QStringList> s_tablefields::GetAllValues(QString tble)
{
    QList<QStringList> lsl;
    QStringList tmpsl;
    QStringList sl = sqlc.getvaluesfromtablebycolumnandfield(sqlc.getdb("sup"), "tablefields", "links", "tablename", tble); // берём links
    if (sqlc.result)
    {
        result = 0x21;
        return QList<QStringList>();
    }
    lsl.append(sl);
    sl = sqlc.getvaluesfromtablebycolumnandfield(sqlc.getdb("sup"), "tablefields", "headers", "tablename", tble); // берём headers
    if (sqlc.result)
    {
        result = 0x22;
        return QList<QStringList>();
    }
    lsl.insert(0, sl); // на поз. 0 заголовки, на поз. 1 - links
    for (int i = 0; i < sl.size(); i++)
    {
        tmpsl = GetAllValuesByOneRow(tble, sl.at(i));
        tmpsl.removeAt(0); // убираем links, который в этой функции уже оприходован
        lsl.append(tmpsl);
    }
    return lsl;
}

// процедура возвращает список из двух значений:
// 0 - значение по ссылке table.tablefields для заданной таблицы tble и поля headers
// 1 - значение links для той же таблицы и того же headers

QStringList s_tablefields::GetOneValueByOneRowAndId(QString tble, QString headers, QString id)
{
    QStringList tmpStringList;
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
    tmpStringList << sqlc.getvaluefromtablebyid(sqlc.getdb(db), tble, sl.at(1), id);
    tmpStringList << sl.at(2);
    if (sqlc.result)
    {
        result = sqlc.result + 0x35;
        return QStringList();
    }
    result = 0;
    return tmpStringList;
}
