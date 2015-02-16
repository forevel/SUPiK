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

// процедура возвращает ИД по значению value для заданной таблицы tble и поля headers

QStringList s_tablefields::GetIdByValue(QString tble, QString headers, QString value)
{
    QStringList tmpStringList;
    QStringList fl = QStringList() << "table" << "tablefields" << "links";
    QStringList cmpfl = QStringList() << "tablename" << "headers";
    QStringList cmpvl = QStringList() << tble << headers;
    QStringList sl = sqlc.getvaluesfromtablebyfields(sqlc.getdb("sup"), "tablefields", fl, cmpfl, cmpvl);
    if (sqlc.result)
    {
        result = 0x41;
        return QStringList();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    tmpStringList << sqlc.getlastvaluefromtablebyfield(sqlc.getdb(db), tble, "id"+tble, sl.at(1), value);
    tmpStringList << sl.at(2);
    if (sqlc.result)
    {
        result = sqlc.result + 0x45;
        return QStringList();
    }
    result = 0;
    return tmpStringList;
}

// перевод ИД через строку links в таблице tablefields в имя
// id - ИД элемента в таблице tablefields:table/tablefields
// links - строка из поля tablefields:links

QString s_tablefields::idtov(QString links, QString id)
{
    QStringList tmpsl;
    QString tmps, outs;
    int tmpi;
    PublicClass::fieldformat ff = pc.getFFfromLinks(links);
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        tmpsl = GetOneValueByOneRowAndId(ff.link.at(0), "Наименование", id);
        if (result)
            return QString();
        outs=tmpsl.at(0);
        break;
    }
    case FW_LINK:
    {
        tmpsl = GetOneValueByOneRowAndId(ff.link.at(0), ff.link.at(1), id);
        if (result)
            return QString();
        outs=tmpsl.at(0);
        break;
    }
    case FW_AUTONUM:
    case FW_NUMBER:
    case FW_PLAIN:
    case FW_MASKED:
    case FW_EQUAT:
    {
        outs = id;
        break;
    }
    case FW_DLINK:
    {
        if (id.at(1) == '_') // символ подчёркивания во второй позиции ИД означает, что номер таблицы надо брать из первого символа
        {
            tmpi = id.at(0).digitValue();
            if (tmpi*2+1 > ff.link.size()) // нет в перечислении links таблицы с таким номером
            {
                result = 0x51;
                return QString();
            }
            id.remove(0, 2);
        }
        else
            tmpi = 0;
        tmpsl = GetOneValueByOneRowAndId(ff.link.at(tmpi*2), ff.link.at(tmpi*2+1), id);
        if (result)
            return QString();
        outs=tmpsl.at(0);
        break;
    }
    }
    return outs;
}

// перевод имени в его ИД через строку links в таблице tablefields (поиск в таблице по имени его ИД)

QString s_tablefields::vtoid(QString links, QString value)
{
    QStringList tmpsl;
    QString outs;
    int tmpi;
    PublicClass::fieldformat ff = pc.getFFfromLinks(links);
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        outs = GetIdByValue(ff.link.at(0), "Наименование", value);
        break;
    }
    case FW_LINK:
    {
        outs = GetIdByValue(ff.link.at(0), ff.link.at(1), value);
        break;
    }
    case FW_AUTONUM:
    case FW_NUMBER:
    case FW_PLAIN:
    case FW_MASKED:
    case FW_EQUAT:
    {
        outs = value;
        break;
    }
    case FW_DLINK:
    {
        if (value.at(1) == '_') // символ подчёркивания во второй позиции ИД означает, что номер таблицы надо брать из первого символа
        {
            tmpi = value.at(0).digitValue();
            if (tmpi*2+1 > ff.link.size()) // нет в перечислении links таблицы с таким номером
            {
                result = 0x51;
                return QString();
            }
            value.remove(0, 2);
        }
        else
            tmpi = 0;
        outs = GetIdByValue(ff.link.at(tmpi*2), ff.link.at(tmpi*2+1), value);
        break;
    }
    }
    if (result)
        return QString();
    return outs;
}
