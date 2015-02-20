#include "../inc/s_tablefields.h"
#include "../inc/publicclass.h"

// класс s_tablefields предназначен для облегчения работы с таблицей tablefields БД supik и содержит в себе высокоуровневые процедуры работы с данной таблицей:
// 1. toidl ([table:headers] to id stringlist) - взять все значения по ссылке table:tablefields из таблицы tablename для данного поля headers
// 2. toid - то же, что и 1, но возвращает только одно значение по заданному id<tble>
// 3. tbtovll - взять все значения по всем полям таблицы tble и вернуть их в QList<QStringList> - полезно для заполнения таблиц s_2cdialog, s_ncdialog
// 4. idtov - по заданному ИД и строке links вытащить значение
// 5. vtoid - найти по имени и строке links ИД записи
// id во всех процедурах имеет смысл значения, получаемого по полю table:tablefields, т.е. значения ИД, получаемого по данному полю, которое ссылается на id записи другой таблицы, указанной в поле links
// vl - это значение в таблице, указанной в links, для данного id

s_tablefields tfl;

s_tablefields::s_tablefields()
{
}

// процедура для данного поля headers таблицы tble берёт все возможные значения
// и передаёт их в QStringList tmpsl следующим образом:
// tmpsl.at(0) = links для данного поля
// tmpsl.at(1)-... = значения

QStringList s_tablefields::toidl(QString tble, QString headers)
{
    QStringList sl = tablefields(tble, headers); // sl.at(0) = <table>, sl.at(1) = <tablefields>, sl.at(2) = <links>
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

// процедура возвращает список из списков значений, взятых по полям для таблицы tble

QList<QStringList> s_tablefields::tbtovll(QString tble)
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
        tmpsl = toidl(tble, sl.at(i));
        tmpsl.removeAt(0); // убираем links, который в этой функции уже оприходован
        lsl.append(tmpsl);
    }
    return lsl;
}

// процедура возвращает список из двух значений:
// 0 - значение по ссылке table.tablefields для заданной таблицы tble и поля headers
// 1 - значение links для той же таблицы и того же headers

QStringList s_tablefields::toid(QString tble, QString headers, QString tbleid)
{
    QStringList tmpStringList;
    QStringList sl = tablefields(tble, headers); // sl.at(0) = <table>, sl.at(1) = <tablefields>, sl.at(2) = <links>
    if (sqlc.result)
    {
        result = 0x31;
        return QStringList();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    tmpStringList << sqlc.getvaluefromtablebyid(sqlc.getdb(db), tble, sl.at(1), tbleid);
    tmpStringList << sl.at(2);
    if (sqlc.result)
    {
        result = sqlc.result + 0x35;
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
        tmpsl = toid(ff.link.at(0), "Наименование", id);
        if (result)
            return QString();
        outs=tmpsl.at(0);
        break;
    }
    case FW_LINK:
    case FW_MAXLINK:
    {
        tmpsl = toid(ff.link.at(0), ff.link.at(1), id);
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
        tmpsl = toid(ff.link.at(tmpi*2), ff.link.at(tmpi*2+1), id);
        if (result)
            return QString();
        outs=tmpsl.at(0);
        break;
    }
    case FW_RIGHTS:
    {
        tmpi = id.toInt(0, 16); // перевод прав в шестнадцатиричную систему
        outs.clear();
        int j = 0;
        while (tmpi)
        {
            switch (j%3)
            {
            case 0:
            {
                outs += (tmpi & 0x0001) ? "у" : ".";
                break;
            }
            case 1:
            {
                outs += (tmpi & 0x0001) ? "з" : ".";
                break;
            }
            case 2:
            {
                outs += (tmpi & 0x0001) ? "ч" : ".";
                break;
            }
            default:
                break;
            }
            j++;
            tmpi >>= 1;
        }
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
        outs = vtoid_(ff.link.at(0), "Наименование", value);
        break;
    }
    case FW_LINK:
    case FW_MAXLINK:
    {
        outs = vtoid_(ff.link.at(0), ff.link.at(1), value);
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
        outs = vtoid_(ff.link.at(tmpi*2), ff.link.at(tmpi*2+1), value);
        break;
    }
    }
    if (result)
        return QString();
    return outs;
}

// вспомогательная процедура возвращает ИД по значению value для заданной таблицы tble и поля headers

QString s_tablefields::vtoid_(QString tble, QString headers, QString value)
{
    QString tmpString;
    QStringList sl = tablefields(tble, headers); // sl.at(0) = <table>, sl.at(1) = <tablefields>, sl.at(2) = <links>
    if (sqlc.result)
    {
        result = 0x41;
        return QString();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    tmpString = sqlc.getlastvaluefromtablebyfield(sqlc.getdb(db), tble, "id"+tble, sl.at(1), value);
    if (sqlc.result)
    {
        result = sqlc.result + 0x45;
        return QString();
    }
    result = 0;
    return tmpString;
}

QStringList s_tablefields::tablefields(QString tble, QString headers)
{
    QStringList fl = QStringList() << "table" << "tablefields" << "links";
    QStringList cmpfl = QStringList() << "tablename" << "headers";
    QStringList cmpvl = QStringList() << tble << headers;
    QStringList sl = sqlc.getvaluesfromtablebyfields(sqlc.getdb("sup"), "tablefields", fl, cmpfl, cmpvl);
    if (sqlc.result)
    {
        result = sqlc.result + 0x55;
        return QStringList();
    }
    result = 0;
    return sl;
}

QStringList s_tablefields::headers(QString tble)
{
    QStringList sl = sqlc.getvaluesfromtablebycolumnandfield(sqlc.getdb("sup"), "tablefields", "headers", "tablename", tble);
    if (sqlc.result)
    {
        result = sqlc.result + 0x65;
        return QStringList();
    }
    result = 0;
    return sl;
}
