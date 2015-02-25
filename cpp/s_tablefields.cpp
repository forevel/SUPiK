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
    if (result)
        return QStringList();
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    QStringList tmpsl = sqlc.getvaluesfromtablebycolumn(sqlc.getdb(db), tble, sl.at(1));
    if (sqlc.result)
    {
        result = sqlc.result;
        return QStringList();
    }
    tmpsl.insert(0, sl.at(2));
    result = 0;
    return tmpsl;
}

// toidlc - это процедура toidl с дополнительным условием
// берёт все значения по полю headers таблицы tble, где поле cheaders этой же таблицы равно value

QStringList s_tablefields::toidlc(QString tble, QString headers, QString cheaders, QString value)
{
    QStringList sl = tablefields(tble, headers); // sl.at(0) = <table>, sl.at(1) = <tablefields>, sl.at(2) = <links>
    if (result)
    {
        result += 0x11;
        return QStringList();
    }
    QStringList cl = tablefields(tble, cheaders); // cl.at(1) = <tablefields>
    if (result)
    {
        result += 0x15;
        return QStringList();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    QStringList tmpsl = sqlc.getvaluesfromtablebycolumnandfield(sqlc.getdb(db), tble, sl.at(1), cl.at(1), value);
    if (sqlc.result)
    {
        result = sqlc.result + 0x1A;
        return QStringList();
    }
    tmpsl.insert(0, sl.at(2));
    result = 0;
    return tmpsl;
}

// процедура возвращает список из списков значений, взятых по полям для таблицы tble
// нужна для организации списка выбора из таблиц а-ля "*_сокращ"

QList<QStringList> s_tablefields::tbtovll(QString links)
{
    QList<QStringList> lsl;
    QStringList tmpsl;
    PublicClass::fieldformat ff = pc.getFFfromLinks(links);
    QString tble = ff.link.at(0);
    QStringList sl = sqlc.getvaluesfromtablebycolumnandfield(sqlc.getdb("sup"), "tablefields", "links", "tablename", tble); // берём links
    int i;
    if (sqlc.result)
    {
        result = sqlc.result + 0x21;
        return QList<QStringList>();
    }
    lsl.append(sl);
    sl = sqlc.getvaluesfromtablebycolumnandfield(sqlc.getdb("sup"), "tablefields", "headers", "tablename", tble); // берём headers
    if (sqlc.result)
    {
        result = sqlc.result + 0x24;
        return QList<QStringList>();
    }
    lsl.insert(0, sl); // на поз. 0 заголовки, на поз. 1 - links
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        QString id = vtoid_(ff.link.at(0), "Наименование", ff.link.at(1)); // взять ИД элемента, по ИД_а которого отбирать список элементов
        if (result)
        {
            result += 0x51;
            return QList<QStringList>();
        }
        for (i = 0; i < sl.size(); i++)
        {
            tmpsl = toidlc(ff.link.at(0), sl.at(i), "ИД_а", id);
            if (result)
            {
                result += 0x54;
                return QList<QStringList>();
            }
            lsl.append(tmpsl);
        }
        break;
    }
    case FW_MAXLINK:
    case FW_LINK:
    {
        for (i = 0; i < sl.size(); i++)
        {
            tmpsl = toidl(tble, sl.at(i));
            tmpsl.removeAt(0); // убираем links, который в этой функции уже оприходован
            lsl.append(tmpsl);
        }
        break;
    }
    default:
        return QList<QStringList>();
    }
    result = 0;
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
        result = sqlc.result + 0x31;
        return QStringList();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    tmpStringList << sqlc.getvaluefromtablebyid(sqlc.getdb(db), tble, sl.at(1), tbleid);
    tmpStringList << sl.at(2);
    if (sqlc.result)
    {
        result = sqlc.result + 0x34;
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
    QString outs;
    int tmpi;
    PublicClass::fieldformat ff = pc.getFFfromLinks(links);
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        tmpsl = toid(ff.link.at(0), "Наименование", id);
        if (result)
        {
            result += 0x41;
            return QString();
        }
        outs=tmpsl.at(0);
        break;
    }
    case FW_LINK:
    case FW_MAXLINK:
    {
        tmpsl = toid(ff.link.at(0), ff.link.at(1), id);
        if (result)
        {
            result += 0x44;
            return QString();
        }
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
                result = 0x48;
                return QString();
            }
            id.remove(0, 2);
        }
        else
            tmpi = 0;
        tmpsl = toid(ff.link.at(tmpi*2), ff.link.at(tmpi*2+1), id);
        if (result)
        {
            result += 0x4A;
            return QString();
        }
        outs=tmpsl.at(0);
        break;
    }
    case FW_RIGHTS:
    {
        bool ok;
        tmpi = id.toInt(&ok, 16); // перевод прав в шестнадцатиричную систему
        if (!ok)
            return 0x4D;
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

// взять все значения по ссылке links в зависимости от типа ссылки

QStringList s_tablefields::idtovl(QString links)
{
    QStringList tmpsl;
    PublicClass::fieldformat ff = pc.getFFfromLinks(links);
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        QString id = vtoid_(ff.link.at(0), "Наименование", ff.link.at(1)); // взять ИД элемента, по ИД_а которого отбирать список элементов
        if (result)
        {
            result += 0x51;
            return QStringList();
        }
        tmpsl = toidlc(ff.link.at(0), "Наименование", "ИД_а", id);
        if (result)
        {
            result += 0x54;
            return QStringList();
        }
        break;
    }
    case FW_MAXLINK:
    case FW_LINK:
    {
        tmpsl = toidl(ff.link.at(0), ff.link.at(1));
        if (result)
        {
            result += 0x57;
            return QStringList();
        }
        break;
    }
    default:
        break;
    }
    result = 0;
    return tmpsl;
}

// перевод имени в его ИД через строку links в таблице tablefields (поиск в таблице по имени его ИД)

QString s_tablefields::vtoid(QString links, QString value)
{
    QString outs;
    if (value == "") // пустая строка вполне имеет право на запись
    {
        result = 0;
        return QString();
    }
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
    case FW_ID:
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

// idtois - процедура добавления новой записи values по адресу table:tablefields для таблицы tble
// ИД записи, по которой добавляются значения, хранятся в values.at(headers.indexof("ИД"))

int s_tablefields::idtois(QString tble, QStringList headers, QStringList values)
{
    if (headers.size() != values.size())
        return 0x01;
    QStringList fl = QStringList() << "table" << "tablefields" << "headers";
    QStringList cmpfl = QStringList() << "tablename" << "keyfield";
    QStringList cmpvl = QStringList() << tble << "v";
    QStringList keydbtble = sqlc.getvaluesfromtablebyfields(sqlc.getdb("sup"), "tablefields", fl, cmpfl, cmpvl);
    if (sqlc.result)
        return 0x02;
    keydbtble.insert(1, keydbtble.at(0).split(".").at(1)); // в первом элементе будет tble
    QString tmpString = keydbtble.at(0);
    tmpString.remove(3, tmpString.size()-3);
    keydbtble.replace(0, tmpString); // в нулевом элементе оставляем только db
    int ididx = headers.indexOf(keydbtble.at(3)); // вытаскиваем индекс ключевого поля
    if (ididx == -1)
        return 0x03;
    QString keyid;
    if (ididx < values.size())
        keyid = values.at(ididx); // id - ИД элемента в ключевом поле, используется при записи всех остальных полей
    else
        return 0x04;
    QStringList tmptablefields, tmpvalues;
    QString tmpdb, tmptble, tmpdbtble;
    int i;
    while (headers.size() > 0)
    {
        QStringList dbtble = tablefields(tble, headers.at(0));
        if (result)
            return result + 0x04;
        if (dbtble.at(0) != "-") // если знак дефиса, поле не пишется в базу
        {
            tmpdbtble = dbtble.at(0);
            tmpvalues = QStringList() << values.at(0); // кладём первое значение в выходной буфер значений
            if (result)
                return result + 0x07;
            tmptablefields = QStringList() << dbtble.at(1);
            headers.removeAt(0);
            values.removeAt(0);
            i = 0;
            while (i < headers.size())
            {
                dbtble = tablefields(tble, headers.at(i));
                if (dbtble.at(0) == tmpdbtble) // здесь проверка на дефис не нужна, т.к. сравнение ведётся с tmpdbtble, который уже проверен ранее
                {
                    tmptablefields << dbtble.at(1);
                    tmpvalues << values.at(i);
                    if (result)
                        return result + 0x0A;
                    headers.removeAt(i);
                    values.removeAt(i);
                }
                else
                    i++;
            }
            tmpdb = tmpdbtble.split(".").at(0);
            tmptble = tmpdbtble.split(".").at(1);
            tmptablefields << "date" << "idpers";
            tmpvalues << pc.DateTime << QString::number(pc.idPers);
            result = sqlc.updatevaluesintable(sqlc.getdb(tmpdb), tmptble, tmptablefields, tmpvalues, keydbtble.at(2), keyid);
            if (result)
                return result + 0x0D;
        }
    }
    return 0;
}

// insert - вставка новой записи в таблицу tble
// возвращает индекс новой строки

QString s_tablefields::insert(QString tble)
{
    QStringList cmpfl = QStringList() << "tablename" << "keyfield";
    QStringList cmpvl = QStringList() << tble << "v";
    QString keydbtble = sqlc.getvaluefromtablebyfields(sqlc.getdb("sup"), "tablefields", "table", cmpfl, cmpvl);
    QString newid = sqlc.insertvaluestotable(sqlc.getdb(keydbtble.split(".").at(0)), keydbtble.split(".").at(1), QStringList(), QStringList()); // вставка новой пустой строки
    if (sqlc.result)
    {
        result = 1;
        return QString();
    }
    result = 0;
    return newid;
}

// remove - "удаление" записи с индексом id из таблицы tble
// важно: здесь не удаляются ссылки на данную запись, которая будет удалена, проверку "дохлых" ссылок на записи, у которых deleted=1, необходимо
// проводить и исправлять при старте СУПиКа или при "обновлении проблем"

int s_tablefields::remove(QString tble, QString id)
{
    QStringList fl = QStringList() << "table" << "tablefields";
    QStringList cmpfl = QStringList() << "tablename" << "keyfield";
    QStringList cmpvl = QStringList() << tble << "v";
    QStringList keydbtble = sqlc.getvaluesfromtablebyfields(sqlc.getdb("sup"), "tablefields", fl, cmpfl, cmpvl);
    int res = sqlc.deletefromdb(sqlc.getdb(keydbtble.at(0).split(".").at(0)), keydbtble.at(0).split(".").at(1), keydbtble.at(1), id);
    return res;
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

bool s_tablefields::tableistree(QString tble)
{
    QStringList tmpfl = QStringList() << "tablefields" << "tablename";
    QStringList tmpvl = QStringList() << "alias" << tble;
    sqlc.getvaluefromtablebyfields(sqlc.getdb("sup"),"tablefields","tablefields",tmpfl,tmpvl);
    if (sqlc.result)
        return false;
    else
        return true;
}
