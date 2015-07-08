#include "s_tablefields.h"
#include "publicclass.h"

// класс s_tablefields предназначен для облегчения работы с таблицей tablefields БД supik и содержит в себе высокоуровневые процедуры работы с данной таблицей:
// 1. htovl ([table:header] to value list) - взять все значения по ссылке table:tablefields из таблицы tablename для данного поля header
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

// процедура для данного поля header таблицы tble берёт все возможные значения
// и передаёт их в QStringList tmpsl

QStringList s_tablefields::htovl(QString tble, QString header)
{
    QStringList sl = tablefields(tble, header); // sl.at(0) = <table>, sl.at(1) = <tablefields>
    if (result)
        return QStringList();
    QString db = sl.at(0).split(".").at(0); // table = <db>.<tble>
    tble = sl.at(0).split(".").at(1);
    QStringList tmpsl = sqlc.getvaluesfromtablebycolumn(sqlc.getdb(db), tble, sl.at(1),"id"+tble,true);
    if (sqlc.result)
    {
        result = sqlc.result + 0x03 + ER_TFIELD;
        return QStringList();
    }
    result = 0;
    return tmpsl;
}

// htovlc - это процедура htovl с дополнительным условием
// берёт все значения по полю header таблицы tble, где поле cheader этой же таблицы равно value

QStringList s_tablefields::htovlc(QString tble, QString header, QString cheader, QString value)
{
    QStringList sl = tablefields(tble, header); // sl.at(0) = <table>, sl.at(1) = <tablefields>
    if (result)
    {
        result += 0x11;
        return QStringList();
    }
    QStringList cl = tablefields(tble, cheader); // cl.at(1) = <tablefields>
    if (result)
    {
        result += 0x15;
        return QStringList();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    QStringList tmpsl = sqlc.getvaluesfromtablebycolumnandfield(sqlc.getdb(db), tble, sl.at(1), cl.at(1), value);
    if ((sqlc.result) || tmpsl.isEmpty())
    {
        result = sqlc.result + 0x1A + ER_TFIELD;
        return QStringList();
    }
    result = 0;
    return tmpsl;
}

// процедура возвращает список из списков значений, взятых по полям для таблицы tble
// нужна для организации списка выбора из таблиц а-ля "*_сокращ"

QList<QStringList> s_tablefields::tbvll(QString tble)
{
    QList<QStringList> lsl;
    QStringList tmpsl;
    QStringList sl = tablelinks(tble); // берём links
    int i;
    if (result)
    {
        result += 0x21;
        return QList<QStringList>();
    }
    lsl.append(sl);
    sl = tableheaders(tble); // берём header
    if (result)
    {
        result += 0x24;
        return QList<QStringList>();
    }
    lsl.insert(0, sl); // на поз. 0 заголовки, на поз. 1 - links
    for (i = 0; i < sl.size(); i++)
    {
        tmpsl = htovl(tble, sl.at(i));
        lsl.append(tmpsl);
    }
    result = 0;
    return lsl;
}

// процедура возвращает значение по ссылке tbleid из table.tablefields для заданной таблицы tble и поля header

QString s_tablefields::tov(QString tble, QString header, QString tbleid)
{
    QString tmpString;
    QStringList sl = tablefields(tble, header); // sl.at(0) = <table>, sl.at(1) = <tablefields>
    if (result)
    {
        result = result + 0x31;
        return QString();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    tmpString = sqlc.getvaluefromtablebyid(sqlc.getdb(db), tble, sl.at(1), tbleid);
    if (sqlc.result)
    {
        result = sqlc.result + 0x34 + ER_TFIELD;
        return QString();
    }
    result = 0;
    return tmpString;
}

// перевод ИД через строку links в таблице tablefields в имя
// id - ИД элемента в таблице tablefields:table/tablefields
// links - строка из поля tablefields:links

QString s_tablefields::idtov(QString links, QString id)
{
    QString outs;
    int tmpi;
    PublicClass::fieldformat ff = pc.getFFfromLinks(links);
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        if (!id.toInt()) // корневой элемент
        {
            outs = "<Корневой элемент>";
            break;
        }
        outs = tov(ff.link.at(0), "Наименование", id);
        if (result)
        {
            result += 0x41 + ER_TFIELD;
            return QString();
        }
        break;
    }
    case FW_LINK:
    case FW_MAXLINK:
    {
        outs = tov(ff.link.at(0), ff.link.at(1), id);
        if (result)
        {
            result += 0x44 + ER_TFIELD;
            return QString();
        }
        break;
    }
    case FW_AUTONUM:
    case FW_NUMBER:
    case FW_PLAIN:
    case FW_MASKED:
    case FW_EQUAT:
    case FW_ID:
    {
        outs = id;
        break;
    }
    case FW_DLINK:
    {
        QStringList sl = id.split("."); // номер таблицы до точки (нумерация с нуля), id - после точки
        tmpi = sl.at(0).toInt();
        if (tmpi*2+1 > ff.link.size()) // нет в перечислении links таблицы с таким номером
        {
            result = 0x48 + ER_TFIELD;
            return QString();
        }
        outs = "_"+sl.at(0)+tov(ff.link.at(tmpi*2), ff.link.at(tmpi*2+1), sl.at(1)); // _ - признак того, что в ячейку надо сохранить доп. информацию о номере таблицы
        if (result)
        {
            result += 0x4A + ER_TFIELD;
            return QString();
        }
        break;
    }
    case FW_RIGHTS:
    {
        bool ok;
        quint32 tmpui = id.toUInt(&ok, 16); // перевод прав в шестнадцатиричную систему
        if (!ok)
            return 0x4D + ER_TFIELD;
        outs.clear();
        int j = 0;
        while ((tmpui) && (j < ACC_NUM))
        {
            switch (j%3)
            {
            case 0:
            {
                outs.insert(0, (tmpui & 0x0001) ? "ч" : ".");
                break;
            }
            case 1:
            {
                outs.insert(0, (tmpui & 0x0001) ? "з" : ".");
                break;
            }
            case 2:
            {
                outs.insert(0, (tmpui & 0x0001) ? "у" : ".");
                break;
            }
            default:
                break;
            }
            j++;
            tmpui >>= 1;
        }
        break;
    }
    default:
    {
        outs = id;
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
        QString id = toid(ff.link.at(0), "Наименование", ff.link.at(1)); // взять ИД элемента, по ИД_а которого отбирать список элементов
        if (result)
        {
            result += 0x51;
            return QStringList();
        }
        tmpsl = htovlc(ff.link.at(0), "Наименование", "ИД_а", id);
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
        tmpsl = htovl(ff.link.at(0), ff.link.at(1));
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
// links = 2.9, value = "ч..ч..чузчузч"

QString s_tablefields::vtoid(QString links, QString value)
{
    QString outs;
    result = 0;
    if (value == "") // пустая строка вполне имеет право на запись
        return QString();
    int tmpi;
    PublicClass::fieldformat ff = pc.getFFfromLinks(links);
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        if (value == "<Корневой элемент>")
            outs = "0";
        else
            outs = toid(ff.link.at(0), "Наименование", value);
        if (result)
            result += 0x61;
        break;
    }
    case FW_LINK:
    case FW_MAXLINK:
    {
        outs = toid(ff.link.at(0), ff.link.at(1), value);
        if (result)
            result += 0x64;
        break;
    }
    case FW_AUTONUM:
    case FW_NUMBER:
    case FW_PLAIN:
    case FW_MASKED:
    case FW_EQUAT:
    case FW_ID:
    case FW_SPECIAL:
    {
        outs = value;
        break;
    }
    case FW_DLINK:
    {
        if (value.at(0) == '_') // символ подчёркивания в первой позиции ИД означает, что номер таблицы надо брать из второго символа
        {
            tmpi = value.at(1).digitValue();
            if (tmpi*2+1 > ff.link.size()) // нет в перечислении links таблицы с таким номером
            {
                result = 0x5A + ER_TFIELD;
                return QString();
            }
            value.remove(0, 2);
        }
        else
            tmpi = 0;
        outs = toid(ff.link.at(tmpi*2), ff.link.at(tmpi*2+1), value);
        if (result)
            result += 0x67;
        break;
    }
    case FW_RIGHTS:
    {
        quint32 outui=0, tmpui = 0x0001;
        int j = 0;
        while ((!value.isEmpty()) && (j < ACC_NUM)) // пока в строке есть что-нибудь и находимся в пределах битовой ширины прав
        {
            switch (j%3)
            {
            case 0:
            {
                int tmpi = value.size()-1;
                QChar tmpc = value.at(tmpi);
                if (tmpc == QChar(1095)) // "ч"
                    outui |= tmpui;
                break;
            }
            case 1:
            {
                int tmpi = value.size()-1;
                QChar tmpc = value.at(tmpi);
                if (tmpc == QChar(1079)) // "з"
                    outui |= tmpui;
                break;
            }
            case 2:
            {
                int tmpi = value.size()-1;
                QChar tmpc = value.at(tmpi);
                if (tmpc == QChar(1091)) // "у"
                    outui |= tmpui;
                break;
            }
            default:
                break;
            }
            value.chop(1);
            j++;
            tmpui <<= 1;
        }
        outs = QString::number(outui, 16).toUpper();
        break;
    }
    default:
    {
        result = ER_TFIELD + 0x5D;
        break;
    }
    }
    if (result)
        return QString();
    return outs;
}

// вспомогательная процедура возвращает ИД по значению value для заданной таблицы tble и поля header

QString s_tablefields::toid(QString tble, QString header, QString value)
{
    QString tmpString;
    QStringList sl = tablefields(tble, header); // sl.at(0) = <table>, sl.at(1) = <tablefields>
    if (result)
    {
        result += 0x61;
        return QString();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    tmpString = sqlc.getlastvaluefromtablebyfield(sqlc.getdb(db), tble, "id"+tble, sl.at(1), value);
    if (sqlc.result)
    {
        result = sqlc.result + 0x65 + ER_TFIELD;
        return QString();
    }
    result = 0;
    return tmpString;
}

// idtois - процедура обновления записи values по адресу table:tablefields для таблицы tble
// ИД записи, по которой обновляются значения, хранятся в values.at(header.indexof("ИД"))

void s_tablefields::idtois(QString tble, QStringList headers, QStringList values)
{
    if (headers.size() != values.size())
    {
        result = 0x71 + ER_TFIELD;
        return;
    }
    QStringList fl = QStringList() << "table" << "tablefields" << "header";
    QStringList cmpfl = QStringList() << "tablename" << "keyfield";
    QStringList cmpvl = QStringList() << tble << "v";
    QStringList keydbtble = sqlc.getvaluesfromtablebyfields(sqlc.getdb("sup"), "tablefields", fl, cmpfl, cmpvl);
    if (sqlc.result)
    {
        result = 0x72 + ER_TFIELD;
        return;
    }
    keydbtble.insert(1, keydbtble.at(0).split(".").at(1)); // в первом элементе будет tble
    QString tmpString = keydbtble.at(0);
    tmpString.remove(3, tmpString.size()-3);
    keydbtble.replace(0, tmpString); // в нулевом элементе оставляем только db
    int ididx = headers.indexOf(keydbtble.at(3)); // вытаскиваем индекс ключевого поля
    if (ididx == -1)
    {
        result = 0x73 + ER_TFIELD;
        return;
    }
    QString keyid;
    if (ididx < values.size())
        keyid = values.at(ididx); // id - ИД элемента в ключевом поле, используется при записи всех остальных полей
    else
    {
        result = 0x74 + ER_TFIELD;
        return;
    }
    QStringList tmptablefields, tmpvalues;
    QString tmpdb, tmptble, tmpdbtble;
    int i;
    while (headers.size() > 0)
    {
        QStringList dbtble = tablefields(tble, headers.at(0));
        if (dbtble.at(0) != "-") // если знак дефиса, поле не пишется в базу
        {
            tmpdbtble = dbtble.at(0);
            tmpvalues = QStringList() << values.at(0); // кладём первое значение в выходной буфер значений
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
                result += 0x7D + ER_TFIELD;
        }
    }
    return;
}

// insert - вставка новой записи в таблицу tble
// возвращает индекс новой строки

QString s_tablefields::insert(QString tble)
{
    QStringList cmpfl = QStringList() << "tablename" << "keyfield";
    QStringList cmpvl = QStringList() << tble << "v";
    QString keydbtble = sqlc.getvaluefromtablebyfields(sqlc.getdb("sup"), "tablefields", "table", cmpfl, cmpvl);
    if (sqlc.result)
    {
        result = sqlc.result + 0x81 + ER_TFIELD;
        return QString();
    }
    QString newid = sqlc.insertvaluestotable(sqlc.getdb(keydbtble.split(".").at(0)), keydbtble.split(".").at(1), QStringList(), QStringList()); // вставка новой пустой строки
    if (sqlc.result)
    {
        result = sqlc.result + 0x84 + ER_TFIELD;
        return QString();
    }
    result = 0;
    return newid;
}

// remove - "удаление" записи с индексом id из таблицы tble
// важно: здесь не удаляются ссылки на данную запись, которая будет удалена, проверку "дохлых" ссылок на записи, у которых deleted=1, необходимо
// проводить и исправлять при старте СУПиКа или при "обновлении проблем"

void s_tablefields::remove(QString tble, QString id)
{
    QStringList fl = QStringList() << "table" << "tablefields";
    QStringList cmpfl = QStringList() << "tablename" << "keyfield";
    QStringList cmpvl = QStringList() << tble << "v";
    QStringList keydbtble = sqlc.getvaluesfromtablebyfields(sqlc.getdb("sup"), "tablefields", fl, cmpfl, cmpvl);
    if (sqlc.result)
    {
        result = sqlc.result + 0x91 + ER_TFIELD;
        return;
    }
    result = sqlc.deletefromdb(sqlc.getdb(keydbtble.at(0).split(".").at(0)), keydbtble.at(0).split(".").at(1), keydbtble.at(1), id);
    if (result)
        result += 0x94 + ER_TFIELD;
}

QStringList s_tablefields::valuesbyfield(QString tble, QStringList fl, QString cmpfield, QString cmpvalue)
{
    QStringList sl = tfl.tablefields(tble,cmpfield);
    if (result)
        return QStringList();
    QString cmpdb = sl.at(0).split(".").at(0); // реальное имя БД
    QString cmptble = sl.at(0).split(".").at(1); // реальное название таблицы
    cmpfield = sl.at(1); // реальное название поля сравнения
    for (int i = 0; i < fl.size(); i++)
    {
        sl = tablefields(tble,fl.at(i));
        if (result)
            return QStringList();
        fl.replace(i, sl.at(1)); // заменяем русское наименование поля на его реальное название
    }
    sl = sqlc.getvaluesfromtablebyfield(sqlc.getdb(cmpdb),cmptble,fl,cmpfield,cmpvalue);
    if (sqlc.result)
    {
        result = sqlc.result + ER_TFIELD + 0xA1;
        return QStringList();
    }
    result = 0;
    return sl;
}

QStringList s_tablefields::tablefields(QString tble, QString header)
{
    QStringList fl = QStringList() << "table" << "tablefields" << "links";
    QStringList cmpfl = QStringList() << "tablename" << "header";
    QStringList cmpvl = QStringList() << tble << header;
    QStringList sl = sqlc.getvaluesfromtablebyfields(sqlc.getdb("sup"), "tablefields", fl, cmpfl, cmpvl);
    if ((sqlc.result) || (sl.isEmpty()))
    {
        result = sqlc.result + 0xB1 + ER_TFIELD;
        return QStringList();
    }
    result = 0;
    return sl;
}

QStringList s_tablefields::tableheaders(QString tble)
{
    QStringList sl = sqlc.getvaluesfromtablebycolumnandfield(sqlc.getdb("sup"), "tablefields", "header", "tablename", tble);
    if ((sqlc.result) || (sl.isEmpty()))
    {
        result = sqlc.result + 0xC1 + ER_TFIELD;
        return QStringList();
    }
    result = 0;
    return sl;
}

QStringList s_tablefields::tablelinks(QString tble)
{
    QStringList sl = sqlc.getvaluesfromtablebycolumnandfield(sqlc.getdb("sup"), "tablefields", "links", "tablename", tble);
    if ((sqlc.result) || (sl.isEmpty()))
    {
        result = sqlc.result + 0xD1 + ER_TFIELD;
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
