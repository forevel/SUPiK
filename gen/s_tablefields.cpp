#include <QVariant>
#include "s_tablefields.h"

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
    QStringList tmpsl = sqlc.GetValuesFromTableByColumn(sqlc.GetDB(db), tble, sl.at(1),"id"+tble,true);
    if (sqlc.result == 2) // ошибка
    {
        result = 1;
        TFWARN(sqlc.LastError);
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
        TFWARN("");
        return QStringList();
    }
    QStringList cl = tablefields(tble, cheader); // cl.at(1) = <tablefields>
    if (result)
    {
        TFWARN("");
        return QStringList();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    QStringList tmpsl = sqlc.GetValuesFromTableByColumnAndField(sqlc.GetDB(db), tble, sl.at(1), cl.at(1), value);
    if (sqlc.result) // || tmpsl.isEmpty())
    {
        result = 1;
        TFWARN(sqlc.LastError);
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
        TFWARN("");
        return QList<QStringList>();
    }
    lsl.append(sl);
    sl = tableheaders(tble); // берём header
    if (result)
    {
        TFWARN("");
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
        TFWARN("");
        return QString();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    tmpString = sqlc.GetValueFromTableByID(sqlc.GetDB(db), tble, sl.at(1), tbleid);
    if (sqlc.result == 2) // если ошибка в запросе SQL
    {
        result = 1;
        TFWARN(sqlc.LastError);
        return QString();
    }
    result = 0;
    return tmpString;
}

// перевод ИД через строку links в таблице tablefields в имя
// id - ИД элемента в таблице tablefields:table/tablefields
// links - строка из поля tablefields:links

PublicClass::ValueStruct s_tablefields::idtov(QString links, QString id)
{
    PublicClass::ValueStruct vl;
    vl.Type = VS_STRING; // тип по умолчанию - простая строка
    vl.Value = ""; // значение по умолчанию - пустая строка
    vl.Links = links;
    PublicClass::FieldFormat ff = pc.getFFfromLinks(links);
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        if (!id.toInt()) // корневой элемент
        {
            vl.Value = "<Корневой элемент>";
            return vl;
        }
        QStringList tmpsl = id.split(".");
        if (tmpsl.size() > 1)
            id = tmpsl.at(1);
        vl.Value = tov(ff.link.at(0), "Наименование", id);
        if (result)
            TFWARN("");
        break;
    }
    case FW_LINK:
    case FW_MAXLINK:
    {
        QStringList tmpsl = id.split(".");
        if (tmpsl.size() > 1)
            id = tmpsl.at(1);
        vl.Value = tov(ff.link.at(0), ff.link.at(1), id);
        if (result)
            TFWARN("");
        break;
    }
    case FW_AUTONUM:
    case FW_NUMBER:
    case FW_FNUMBER:
    case FW_PLAIN:
    case FW_MASKED:
    case FW_EQUAT:
    case FW_ID:
    {
        vl.Value = id;
        break;
    }
    case FW_DLINK:
    {
        QStringList sl = id.split("."); // номер таблицы до точки (нумерация с нуля), id - после точки
        int tmpi = sl.at(0).toInt();
        if (tmpi*2+1 > ff.link.size()) // нет в перечислении links таблицы с таким номером
        {
            result = 1;
            TFWARN("");
            return vl;
        }
        vl.Value = "_"+sl.at(0)+tov(ff.link.at(tmpi*2), ff.link.at(tmpi*2+1), sl.at(1)); // _ - признак того, что в ячейку надо сохранить доп. информацию о номере таблицы
        if (result)
            TFWARN("");
        break;
    }
    case FW_RIGHTS:
    {
        bool ok;
        quint32 tmpui = id.toUInt(&ok, 16); // перевод прав в шестнадцатиричную систему
        if (!ok)
        {
            result = 1;
            TFDBG;
            return vl;
        }
        QString outs;
        int j = 0;
        while ((tmpui) && (j < ACC_NUM))
        {
            switch (j%2)
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
            default:
                break;
            }
            j++;
            tmpui >>= 1;
        }
        vl.Value = outs;
        break;
    }
    case FW_BOOL:
    {
        bool ok;
        int tmpb = id.toInt(&ok);
        if (!ok)
        {
            result = 1;
            TFDBG;
            return vl;
        }
        vl.Type = VS_ICON;
        if (tmpb == 0)
            vl.Value = ":/res/cross.png";
        else
            vl.Value = ":/res/ok.png";
    }
    default:
    {
        vl.Value = id;
        break;
    }
    }
    return vl;
}

// взять все значения по ссылке links в зависимости от типа ссылки

QStringList s_tablefields::idtovl(QString links)
{
    QStringList tmpsl;
    PublicClass::FieldFormat ff = pc.getFFfromLinks(links);
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        QString id = toid(ff.link.at(0), "Наименование", ff.link.at(1)); // взять ИД элемента, по ИД_а которого отбирать список элементов
        if (result)
        {
            TFWARN("");
            return QStringList();
        }
        tmpsl = htovlc(ff.link.at(0), "Наименование", "ИД_а", id);
        if (result)
        {
            TFWARN("");
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
            TFWARN("");
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

QString s_tablefields::vtoid(PublicClass::ValueStruct vl)
{
    QString outs;
    result = 0;
    if ((vl.Type == VS_STRING) && (vl.Value == "")) // пустая строка вполне имеет право на запись
        return QString();
    PublicClass::FieldFormat ff = pc.getFFfromLinks(vl.Links);
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        if (vl.Type == VS_STRING)
        {
            if (vl.Value == "<Корневой элемент>")
                outs = "0";
            else
                outs = toid(ff.link.at(0), "Наименование", vl.Value);
            if (result)
                TFWARN("");
        }
        else
            TFWARN("");
        break;
    }
    case FW_LINK:
    case FW_MAXLINK:
    {
        outs = toid(ff.link.at(0), ff.link.at(1), vl.Value);
        if (result)
            TFWARN("");
        break;
    }
    case FW_AUTONUM:
    case FW_NUMBER:
    case FW_FNUMBER:
    case FW_PLAIN:
    case FW_MASKED:
    case FW_EQUAT:
    case FW_ID:
    case FW_SPECIAL:
    case FW_FLINK:
    case FW_ILINK:
    {
        outs = vl.Value;
        break;
    }
    case FW_DLINK:
    {
        QString tmps = vl.Value;
        int tmpi;
        if (tmps.at(0) == '_') // символ подчёркивания в первой позиции ИД означает, что номер таблицы надо брать из второго символа
        {
            tmpi = tmps.at(1).digitValue();
            if (tmpi*2+1 > ff.link.size()) // нет в перечислении links таблицы с таким номером
            {
                result = 1;
                TFWARN("");
                return QString();
            }
            tmps.remove(0, 2);
        }
        else
            tmpi = 0;
        outs = toid(ff.link.at(tmpi*2), ff.link.at(tmpi*2+1), tmps);
        if (result)
            TFWARN("");
        break;
    }
    case FW_RIGHTS:
    {
        QString tmps = vl.Value;
        quint32 outui=0, tmpui = 0x0001;
        int j = 0;
        while ((!tmps.isEmpty()) && (j < ACC_NUM)) // пока в строке есть что-нибудь и находимся в пределах битовой ширины прав
        {
            switch (j%2)
            {
            case 0:
            {
                int tmpi = tmps.size()-1;
                QChar tmpc = tmps.at(tmpi);
                if (tmpc == QChar(1095)) // "ч"
                    outui |= tmpui;
                break;
            }
            case 1:
            {
                int tmpi = tmps.size()-1;
                QChar tmpc = tmps.at(tmpi);
                if (tmpc == QChar(1079)) // "з"
                    outui |= tmpui;
                break;
            }
            default:
                break;
            }
            tmps.chop(1);
            j++;
            tmpui <<= 1;
        }
        outs = QString::number(outui, 16).toUpper();
        break;
    }
    case FW_BOOL:
    {
        if (vl.Type == VS_ICON)
        {
            if (vl.Value == ":/res/ok.png")
                outs = "1";
            else if (vl.Value == ":/res/cross.png")
                outs = "0";
            else
                TFWARN("");
        }
        else
            TFWARN("");
        break;
    }
    default:
    {
        TFWARN("");
        break;
    }
    }
    return outs;
}

// вспомогательная процедура возвращает ИД по значению value для заданной таблицы tble и поля header

QString s_tablefields::toid(QString tble, QString header, QString value)
{
    QString tmpString;
    QStringList sl = tablefields(tble, header); // sl.at(0) = <table>, sl.at(1) = <tablefields>
    if (result)
    {
        TFWARN("");
        return QString();
    }
    QString db = sl.at(0).split(".").at(0);
    tble = sl.at(0).split(".").at(1);
    tmpString = sqlc.GetLastValueFromTableByField(sqlc.GetDB(db), tble, "id"+tble, sl.at(1), value);
    if (sqlc.result)
    {
        result = 1;
        TFWARN(sqlc.LastError);
        return QString();
    }
    result = 0;
    return tmpString;
}

// idtois - процедура обновления записи values по адресу table:tablefields для таблицы tble
// ИД записи, по которой обновляются значения, хранятся в values.at(headers.indexof("ИД"))

void s_tablefields::idtois(QString tble, QStringList headers, QStringList values)
{
    if (headers.size() != values.size())
    {
        result = 1;
        TFDBG;
        return;
    }
    // Сначала найдём для данной таблицы ключевое поле ("v") и по нему вытащим реальную БД, таблицу и наименование поля
    QStringList fl = QStringList() << "table" << "tablefields" << "header";
    QStringList cmpfl = QStringList() << "tablename" << "keyfield";
    QStringList cmpvl = QStringList() << tble << "v";
    QStringList keydbtble = sqlc.GetValuesFromTableByFields(sqlc.GetDB("sup"), "tablefields", fl, cmpfl, cmpvl);
    if (sqlc.result)
    {
        result = 1;
        TFWARN(sqlc.LastError);
        return;
    }
    QStringList tmpsl = keydbtble.at(0).split(".");
    keydbtble.insert(1, tmpsl.at(1)); // в первом элементе будет tble
    keydbtble.replace(0, tmpsl.at(0)); // в нулевом элементе оставляем только db
    int ididx = headers.indexOf(keydbtble.at(3)); // вытаскиваем индекс ключевого поля
    if (ididx == -1)
    {
        TFWARN("Не найдено ключевое поле "+keydbtble.at(3)+" в таблице "+keydbtble.at(1));
        result = 1;
        return;
    }
    QString keyid;
    if (ididx < values.size())
        keyid = values.at(ididx); // id - ИД элемента в ключевом поле, используется при записи всех остальных полей
    else
    {
        result = 1;
        TFWARN("Не найдено ключевое поле "+keydbtble.at(3)+" в таблице "+keydbtble.at(1));
        return;
    }
    keyid = QString::number(keyid.toInt()); // убираем незначащие нули
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
                if (dbtble.size() == 0)
                {
                    TFWARN("");
                    return;
                }
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
            result = sqlc.UpdateValuesInTable(sqlc.GetDB(tmpdb), tmptble, tmptablefields, tmpvalues, keydbtble.at(2), keyid);
            if (result)
                TFWARN(sqlc.LastError);
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
    QString keydbtble = sqlc.GetValueFromTableByFields(sqlc.GetDB("sup"), "tablefields", "table", cmpfl, cmpvl);
    if (sqlc.result)
    {
        result = 1;
        TFWARN(sqlc.LastError);
        return QString();
    }
    QString newid = sqlc.InsertValuesToTable(sqlc.GetDB(keydbtble.split(".").at(0)), keydbtble.split(".").at(1), QStringList(), QStringList()); // вставка новой пустой строки
    if (sqlc.result)
    {
        result = 1;
        TFWARN(sqlc.LastError);
        return QString();
    }
    result = 0;
    return newid;
}

// newid - найти первый свободный ИД в таблице
// возвращает индекс новой строки

QString s_tablefields::NewID(QString tble)
{
    QStringList cmpfl = QStringList() << "tablename" << "keyfield";
    QStringList cmpvl = QStringList() << tble << "v";
    QString keydbtble = sqlc.GetValueFromTableByFields(sqlc.GetDB("sup"), "tablefields", "table", cmpfl, cmpvl);
    if (sqlc.result)
    {
        result = 1;
        TFWARN(sqlc.LastError);
        return QString();
    }
    int newid = sqlc.GetNextFreeIndex(sqlc.GetDB(keydbtble.split(".").at(0)), keydbtble.split(".").at(1));
    if (sqlc.result)
    {
        result = 1;
        TFWARN(sqlc.LastError);
        return QString();
    }
    result = 0;
    return QString::number(newid);
}

// remove - "удаление" записи с индексом id из таблицы tble
// важно: здесь не удаляются ссылки на данную запись, которая будет удалена, проверку "дохлых" ссылок на записи, у которых deleted=1, необходимо
// проводить и исправлять при старте СУПиКа или при "обновлении проблем"

void s_tablefields::remove(QString tble, QString id)
{
    QStringList fl = QStringList() << "table" << "tablefields";
    QStringList cmpfl = QStringList() << "tablename" << "keyfield";
    QStringList cmpvl = QStringList() << tble << "v";
    QStringList keydbtble = sqlc.GetValuesFromTableByFields(sqlc.GetDB("sup"), "tablefields", fl, cmpfl, cmpvl);
    if (sqlc.result)
    {
        TFWARN(sqlc.LastError);
        result = 1;
        return;
    }
    result = sqlc.DeleteFromDB(sqlc.GetDB(keydbtble.at(0).split(".").at(0)), keydbtble.at(0).split(".").at(1), keydbtble.at(1), id);
    if (result)
        TFWARN(sqlc.LastError);
}

// remove - реальное удаление записи с индексом id из таблицы tble
// важно: здесь не удаляются ссылки на данную запись, которая будет удалена, проверку "дохлых" ссылок на отсутствующие записи необходимо
// проводить и исправлять при старте СУПиКа или при "обновлении проблем"

void s_tablefields::Delete(QString tble, QString id)
{
    QStringList fl = QStringList() << "table" << "tablefields";
    QStringList cmpfl = QStringList() << "tablename" << "keyfield";
    QStringList cmpvl = QStringList() << tble << "v";
    QStringList keydbtble = sqlc.GetValuesFromTableByFields(sqlc.GetDB("sup"), "tablefields", fl, cmpfl, cmpvl);
    if (sqlc.result)
    {
        TFWARN(sqlc.LastError);
        result = 1;
        return;
    }
    result = sqlc.RealDeleteFromDB(sqlc.GetDB(keydbtble.at(0).split(".").at(0)), keydbtble.at(0).split(".").at(1), QStringList(keydbtble.at(1)), QStringList(id));
    if (result)
        TFWARN(sqlc.LastError);
}

bool s_tablefields::Check(QString tble, QString cmpfield, QString cmpvalue)
{
    QStringList sl = tfl.tablefields(tble,cmpfield);
    if (result)
    {
        TFWARN("");
        return false;
    }
    QString cmpdb = sl.at(0).split(".").at(0); // реальное имя БД
    QString cmptble = sl.at(0).split(".").at(1); // реальное название таблицы
    cmpfield = sl.at(1); // реальное название поля сравнения
    sl = sqlc.GetColumnsFromTable(sqlc.GetDB(cmpdb),cmptble);
    sl = sqlc.GetValuesFromTableByField(sqlc.GetDB(cmpdb),cmptble,sl,cmpfield,cmpvalue);
    if (sqlc.result)
    {
        result = 1;
        return false;
    }
    result = 0;
    return true;
}

QStringList s_tablefields::valuesbyfield(QString tble, QStringList fl, QString cmpfield, QString cmpvalue, bool Warn)
{
    QStringList sl = tfl.tablefields(tble,cmpfield);
    if (result)
    {
        TFWARN("");
        return QStringList();
    }
    QString cmpdb = sl.at(0).split(".").at(0); // реальное имя БД
    QString cmptble = sl.at(0).split(".").at(1); // реальное название таблицы
    cmpfield = sl.at(1); // реальное название поля сравнения
    for (int i = 0; i < fl.size(); i++)
    {
        sl = tablefields(tble,fl.at(i));
        if (result)
        {
            TFWARN("");
            return QStringList();
        }
        fl.replace(i, sl.at(1)); // заменяем русское наименование поля на его реальное название
    }
    sl = sqlc.GetValuesFromTableByField(sqlc.GetDB(cmpdb),cmptble,fl,cmpfield,cmpvalue);
    if ((sqlc.result) && (Warn))
    {
        TFWARN(sqlc.LastError);
        result = 1;
        return QStringList();
    }
    result = 0;
    return sl;
}

QStringList s_tablefields::valuesbyfields(QString tble, QStringList fl, QStringList cmpfields, QStringList cmpvalues, bool Warn)
{
    if ((cmpfields.size() != cmpvalues.size()) || (cmpfields.size() == 0) || (fl.size() == 0))
    {
        result = 1;
        TFDBG;
        return QStringList();
    }
    QStringList cmpfl, tmpsl;
    // взяли все реальные названия полей сравнения
    for (int i=0; i<cmpfields.size(); i++)
    {
        tmpsl = tablefields(tble,cmpfields.at(i));
        if (result)
        {
            TFWARN("");
            return QStringList();
        }
        cmpfl << tmpsl.at(1);
    }
    for (int i = 0; i < fl.size(); i++)
    {
        QStringList sl = tablefields(tble,fl.at(i));
        if (result)
        {
            TFWARN("");
            return QStringList();
        }
        fl.replace(i, sl.at(1)); // заменяем русское наименование поля на его реальное название
    }
    QString cmpdb = tmpsl.at(0).split(".").at(0); // реальное имя БД
    QString cmptble = tmpsl.at(0).split(".").at(1); // реальное название таблицы
    QStringList tmps = sqlc.GetValuesFromTableByFields(sqlc.GetDB(cmpdb),cmptble,fl,cmpfl,cmpvalues);
    if ((sqlc.result) && (Warn))
    {
        TFWARN(sqlc.LastError);
        result = 1;
        return QStringList();
    }
    result = 0;
    return tmps;
}

QStringList s_tablefields::TableColumn(QString tble, QString field)
{
    QStringList sl = sqlc.GetValuesFromTableByColumnAndField(sqlc.GetDB("sup"), "tablefields", field, "tablename", tble, "fieldsorder");
    if (sqlc.result)
    {
        TFWARN(sqlc.LastError);
        result = 1;
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
    QStringList sl = sqlc.GetValuesFromTableByFields(sqlc.GetDB("sup"), "tablefields", fl, cmpfl, cmpvl);
    if ((sqlc.result) || (sl.isEmpty()))
    {
        TFWARN(sqlc.LastError);
        result = 1;
        return QStringList();
    }
    result = 0;
    return sl;
}

QStringList s_tablefields::tableheaders(QString tble)
{
    QStringList sl = sqlc.GetValuesFromTableByColumnAndField(sqlc.GetDB("sup"), "tablefields", "header", "tablename", tble, "fieldsorder", true);
    if ((sqlc.result) || (sl.isEmpty()))
    {
        TFWARN(sqlc.LastError);
        result = 1;
        return QStringList();
    }
    result = 0;
    return sl;
}

QStringList s_tablefields::tablelinks(QString tble)
{
    QStringList sl = sqlc.GetValuesFromTableByColumnAndField(sqlc.GetDB("sup"), "tablefields", "links", "tablename", tble, "fieldsorder", true);
    if ((sqlc.result) || (sl.isEmpty()))
    {
        TFWARN(sqlc.LastError);
        result = 1;
        return QStringList();
    }
    result = 0;
    return sl;
}

bool s_tablefields::tableistree(QString tble)
{
    QStringList tmpfl = QStringList() << "tablefields" << "tablename";
    QStringList tmpvl = QStringList() << "alias" << tble;
    sqlc.GetValueFromTableByFields(sqlc.GetDB("sup"),"tablefields","tablefields",tmpfl,tmpvl);
    if (sqlc.result)
        return false;
    else
        return true;
}
