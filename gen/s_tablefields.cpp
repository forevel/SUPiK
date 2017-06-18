#include <QVariant>
#include <QThread>
#include <QCoreApplication>
#include "s_TableFields.h"
#include "client.h"

// класс s_TableFields предназначен для облегчения работы с таблицей TableFields БД supik и содержит в себе высокоуровневые процедуры работы с данной таблицей:
// 1. GetValuesByColumn ([table:header] to value list) - взять все значения по ссылке table:TableFields из таблицы tablename для данного поля header
// 2. toid - то же, что и 1, но возвращает только одно значение по заданному id<tble>
// 3. tbtovll - взять все значения по всем полям таблицы tble и вернуть их в QList<QStringList> - полезно для заполнения таблиц twocoldialog
// 4. idtov - по заданному ИД и строке links вытащить значение
// 5. vtoid - найти по имени и строке links ИД записи
// id во всех процедурах имеет смысл значения, получаемого по полю table:TableFields, т.е. значения ИД, получаемого по данному полю, которое ссылается на id записи другой таблицы, указанной в поле links
// vl - это значение в таблице, указанной в links, для данного id

s_TableFields tfl;

s_TableFields::s_TableFields()
{
}

// процедура для данного поля header таблицы tble берёт все возможные значения
// и передаёт их в QStringList tmpsl

void s_TableFields::GetValuesByColumn(const QString &tble, const QString &header, QStringList &out)
{
    QStringList sl;
    out.clear();
    result = TFRESULT_NOERROR;
    if (pc.AutonomousMode)
    {
        TableFields(tble, header, sl); // sl.at(0) = <table>, sl.at(1) = <TableFields>
        if (result)
        {
            WARNMSG("");
            return;
        }
        QString db = sl.at(0).split(".").at(0); // table = <db>.<tble>
        QString table = sl.at(0).split(".").at(1);
        out = sqlc.GetValuesFromTableByColumn(db, table, sl.at(1),"id"+table,true);
        if (sqlc.result == 2) // ошибка
        {
            result = TFRESULT_ERROR;
            WARNMSG(sqlc.LastError);
            return;
        }
    }
    else
    {
        sl << tble << header;
        int res = Cli->SendAndGetResult(T_GVSBC, sl);
        if (res == Client::CLIER_EMPTY)
            result = TFRESULT_EMPTY;
        else if ((res != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
            result = TFRESULT_ERROR;
        else
            out = Cli->Result.at(0);
    }
}

// GetValuesByColumnAndField - это процедура GetValuesByColumn с дополнительным условием
// берёт все значения по полю header таблицы tble, где поле cheader этой же таблицы равно value

void s_TableFields::GetValuesByColumnAndField(const QString &tble, const QString &header, const QString &cheader, const QString &value, QStringList &out)
{
    GetValuesByColumnAndFields(tble, header, QStringList(cheader), QStringList(value), out);
}

void s_TableFields::GetValuesByColumnAndFields(const QString &tble, const QString &header, QStringList &cmpfl, QStringList &cmpvl, QStringList &out)
{
    result = TFRESULT_NOERROR;
    out.clear();
    if ((cmpfl.size() != cmpvl.size()) || (cmpfl.size() == 0))
    {
        result = TFRESULT_ERROR;
        DBGMSG;
        return;
    }
    result = TFRESULT_NOERROR;
    if (pc.AutonomousMode)
    {
        QStringList realcmpfl, tmpsl;
        // взяли все реальные названия полей сравнения
        for (int i=0; i<cmpfl.size(); i++)
        {
            QString field = cmpfl.at(i);
            TableFields(tble, field, tmpsl);
            if (result)
            {
                WARNMSG("");
                return;
            }
            realcmpfl << tmpsl.at(1);
        }
        QStringList tmpsl2;
        TableFields(tble,header,tmpsl2);
        if (result)
        {
            WARNMSG("");
            return;
        }
        QString realheader = tmpsl2.at(1);
        QString cmpdb = tmpsl.at(0).split(".").at(0); // реальное имя БД
        QString cmptble = tmpsl.at(0).split(".").at(1); // реальное название таблицы
        out = sqlc.GetValuesFromTableByColumnAndFields(cmpdb,cmptble,realheader,realcmpfl,cmpvl);
        if (sqlc.result)
        {
            WARNMSG(sqlc.LastError);
            result = TFRESULT_ERROR;
            return;
        }
    }
    else
    {
        int pairs_num = cmpfl.size();
        QStringList sl = QStringList() << QString::number(pairs_num) << tble << header;
        for (int i=0; i<pairs_num; i++)
        {
            sl << cmpfl.at(i);
            sl << cmpvl.at(i);
        }
        int res = Cli->SendAndGetResult(T_GVSBCF, sl);
        if (res == Client::CLIER_EMPTY)
            result = TFRESULT_EMPTY;
        else if ((res != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
            result = TFRESULT_ERROR;
        else
            out = Cli->Result.at(0);
    }
}

// процедура возвращает список из списков значений, взятых по полям для таблицы tble
// нужна для организации списка выбора из таблиц а-ля "*_сокращ"

void s_TableFields::GetAllValuesFromTable(const QString &tble, QStringList &out)
{
    QStringList tmpsl, sl;
    out.clear();
    result = TFRESULT_NOERROR;
    if (pc.AutonomousMode)
    {
/*        tablelinks(tble, sl); // берём links
        int i;
        if (result)
        {
            WARNMSG("");
            return;
        }
        out.append(sl);
        TableHeaders(tble, sl); // берём header
        if (result)
        {
            WARNMSG("");
            return;
        }
        out.insert(0, sl); // на поз. 0 заголовки, на поз. 1 - links */
        for (int i = 0; i < sl.size(); i++)
        {
            QString field = sl.at(i);
            GetValuesByColumn(tble, field, tmpsl);
            out.append(tmpsl);
        }
    }
    else
    {
        sl << tble;
        int res = Cli->SendAndGetResult(T_GFT, sl);
        if (res == Client::CLIER_EMPTY)
            result = TFRESULT_EMPTY;
        else if ((res != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
            result = TFRESULT_ERROR;
        else
            out = Cli->Result.at(0);
    }
}

// процедура возвращает значение по ссылке tbleid из table.TableFields для заданной таблицы tble и поля header

void s_TableFields::tov(const QString &tble, const QString &header, const QString &tbleid, QString &out)
{
    QStringList sl;
    result = TFRESULT_NOERROR;
    out.clear();
    if (pc.AutonomousMode)
    {
        TableFields(tble, header, sl); // sl.at(0) = <table>, sl.at(1) = <TableFields>
        if (result != TFRESULT_NOERROR)
        {
            WARNMSG("");
            return;
        }
        QString db = sl.at(0).split(".").at(0);
        QString tmptble = sl.at(0).split(".").at(1);
        out = sqlc.GetValueFromTableByID(db, tmptble, sl.at(1), tbleid);
        if (sqlc.result == 2) // если ошибка в запросе SQL
        {
            result = TFRESULT_ERROR;
            WARNMSG(sqlc.LastError);
            return;
        }
    }
    else
    {
        sl << tble << header << tbleid;
        int res = Cli->SendAndGetResult(T_TV, sl);
        if (res == Client::CLIER_EMPTY)
        {
            INFOMSG("tov: Empty result");
            result = TFRESULT_EMPTY;
        }
        else if (res != Client::CLIER_NOERROR)
        {
            WARNMSG("tov: Error result");
            result = TFRESULT_ERROR;
        }
        else
            out = Cli->ResultStr;
    }
}

// перевод ИД через строку links в таблице TableFields в имя
// id - ИД элемента в таблице TableFields:table/TableFields
// links - строка из поля TableFields:links

void s_TableFields::idtov(const QString &links, const QString &id, QString &out)
{
    QStringList sl;
    PublicClass::FieldFormat ff;
//    if (pc.AutonomousMode)
//    {
        QString table, header, tmpid;
        out = ""; // значение по умолчанию - пустая строка
        tmpid = id;
        pc.getFFfromLinks(links, ff);
        switch (ff.ftype)
        {
        case FW_ALLINK:
        {
            if (!id.toInt()) // корневой элемент
            {
                out = "<Корневой элемент>";
                break;
            }
            sl = id.split(".");
            if (sl.size() > 1)
                tmpid = sl.at(1);
            table = ff.link.at(0);
            header = "Наименование";
            tov(table, header, tmpid, out);
            if (result)
                WARNMSG("");
            break;
        }
        case FW_LINK:
        case FW_MAXLINK:
        {
            sl = id.split(".");
            if (sl.size() > 1)
                tmpid = sl.at(1);
            table = ff.link.at(0);
            header = ff.link.at(1);
            tov(table, header, tmpid, out);
            if (result == TFRESULT_ERROR)
                WARNMSG("");
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
            out = id;
            break;
        }
        case FW_DATE:
        case FW_DATETIME:
        {
/*            QDateTime tmpd = QDateTime::fromString(id, "dd-MM-yyyy hh:mm:ss");
            if (tmpd.isValid())
                out.Value = tmpd.toString("dd-MM-yyyy");
            else
            {
                tmpd = QDateTime::fromString(id, "dd-MM-yyyy");
                if (tmpd.isValid())
                    out.Value = tmpd.toString("dd-MM-yyyy");
                else
                    out.Value = "N/A";
            } */
            out = id;
            break;
        }
        case FW_DLINK:
        {
            sl = id.split("."); // номер таблицы до точки (нумерация с нуля), id - после точки
            int tmpi = sl.at(0).toInt();
            if (tmpi*2+1 > ff.link.size()) // нет в перечислении links таблицы с таким номером
            {
                result = TFRESULT_ERROR;
                WARNMSG("");
                return;
            }
            table = ff.link.at(tmpi*2);
            header = ff.link.at(tmpi*2+1);
            QString value = sl.at(1);
            tov(table, header, value, out);
            QString tmps = "_"+sl.at(0); // _ - признак того, что в ячейку надо сохранить доп. информацию о номере таблицы
            out.insert(0, tmps);
            if (result)
                WARNMSG("");
            break;
        }
        case FW_RIGHTS:
        {
            bool ok;
            quint32 tmpui = id.toUInt(&ok, 16); // перевод прав в шестнадцатиричную систему
            if (!ok)
            {
                result = TFRESULT_ERROR;
                DBGMSG;
                return;
            }
            out.clear();
            int j = 0;
            while ((tmpui) && (j < ACC_NUM))
            {
                switch (j%2)
                {
                case 0:
                {
                    out.insert(0, (tmpui & 0x0001) ? "ч" : ".");
                    break;
                }
                case 1:
                {
                    out.insert(0, (tmpui & 0x0001) ? "з" : ".");
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
        case FW_BOOL:
        {
            bool ok;
            int tmpb = id.toInt(&ok);
            if (!ok)
            {
                result = TFRESULT_ERROR;
                DBGMSG;
                return;
            }
            out = (tmpb == 0) ? "0" : "1";
            break;
        }
        case FW_CRYPT:
        {
            if (pc.AutonomousMode)
            {

            }
            else
            {
                sl << links << id;
                int res = Cli->SendAndGetResult(T_IDTV, sl);
                if ((res != Client::CLIER_NOERROR) || Cli->Result.isEmpty())
                {
                    result = 1;
                    return;
                }
                if (Cli->Result.at(0).size() < 2)
                {
                    result = 1;
                    return;
                }
                sl = Cli->Result.at(0); // sl[0] - <id>, sl[1] - <links>
                out = sl.at(0);
                result = 0;
            }
            break;
        }
        default:
        {
            out = id;
            break;
        }
        }
/*    }
    else
    {
        sl << links << id;
        int res = Cli->SendAndGetResult(T_IDTV, sl);
        if ((res != Client::CLIER_NOERROR) || Cli->Result.isEmpty())
        {
            result = 1;
            return;
        }
        if (Cli->Result.at(0).size() < 3)
        {
            result = 1;
            return;
        }
        sl = Cli->Result.at(0);
        out.Links = sl.at(2);
        out.Type = sl.at(0).toInt();
        out.Value = sl.at(1);
        result = 0;
    } */
}

// взять все значения по ссылке links в зависимости от типа ссылки

void s_TableFields::idtovl(const QString &links, QStringList &out)
{
//    if (pc.AutonomousMode)
//    {
        QString table, header, value;
        PublicClass::FieldFormat ff;
        out.clear();
        result = TFRESULT_NOERROR;
        pc.getFFfromLinks(links, ff);
        switch (ff.ftype)
        {
        case FW_ALLINK:
        {
            QString id;
            table = ff.link.at(0);
            header = "Наименование";
            value = ff.link.at(1);
            toid(table, header, value, id); // взять ИД элемента, по ИД_а которого отбирать список элементов
            if (result)
            {
                WARNMSG("");
                return;
            }
            value = "ИД_а";
            GetValuesByColumnAndField(table, header, value, id, out);
            if (result)
            {
                WARNMSG("");
                return;
            }
            break;
        }
        case FW_MAXLINK:
        case FW_LINK:
        {
            table = ff.link.at(0);
            header = ff.link.at(1);
            GetValuesByColumn(table, header, out);
            if (result)
            {
                WARNMSG("");
                return;
            }
            break;
        }
        default:
            break;
        }
/*    }
    else
    {
        QStringList sl;
        sl << links;
        int res = Cli->SendAndGetResult(T_IDTVL, sl);
        if ((res != Client::CLIER_NOERROR) || Cli->Result.isEmpty())
        {
            result = 1;
            return;
        }
        out = Cli->Result.at(0);
    }*/
}

// перевод имени в его ИД через строку links в таблице TableFields (поиск в таблице по имени его ИД)
// links = 2.9, value = "ч..ч..чузчузч"

void s_TableFields::vtoid(QString &links, QString &in, QString &out)
{
//    if (pc.AutonomousMode)
//    {
        QString table, header;
        result = TFRESULT_NOERROR;
        out.clear();
        if (in.isEmpty()) // пустая строка вполне имеет право на запись
            return;
        PublicClass::FieldFormat ff;
        pc.getFFfromLinks(links, ff);
        switch (ff.ftype)
        {
        case FW_ALLINK:
        {
            if (in == "<Корневой элемент>")
                out = "0";
            else
            {
                table = ff.link.at(0);
                header = "Наименование";
                toid(table, header, in, out);
            }
            if (result)
                WARNMSG("");
            break;
        }
        case FW_LINK:
        case FW_MAXLINK:
        {
            table = ff.link.at(0);
            header = ff.link.at(1);
            toid(table, header, in, out);
            if (result)
                WARNMSG("");
            break;
        }
        case FW_NUMBER:
        case FW_FNUMBER:
        case FW_PLAIN:
        {
            if (ff.delegate == FD_SPIN) // для спина отдельная обработка, ибо дурит как хочет
            {
                int tmpInt = ff.link.at(0).count("n", Qt::CaseSensitive);
                int tmpInt2 = ff.link.at(0).count("d", Qt::CaseSensitive);
                QStringList sl = in.split(".");
                if (sl.size() > 0)
                {
                    out = sl.at(0).right(tmpInt);
                    for (int i=out.size(); i<tmpInt; ++i)
                        out.insert(0, '0');
                }
                if ((sl.size() > 1) && (tmpInt2 > 0))
                    out += "." + sl.at(1).left(tmpInt2);
                break;
            }
            out = in;
            break;
        }
        case FW_AUTONUM:
        case FW_MASKED:
        case FW_EQUAT:
        case FW_ID:
        case FW_SPECIAL:
        case FW_FLINK:
        case FW_ILINK:
        case FW_PIXTE:
        {
            out = in;
            break;
        }
        case FW_DATE:
        case FW_DATETIME:
        {
/*            QDateTime tmpd = QDateTime::fromString(vl.Value, "dd/MM/yyyy");
            if (!tmpd.isValid())
                out = "N/A";
            else
                out = tmpd.toString("dd-MM-yyyy hh:mm:ss"); */
            out = in;
            break;
        }
        case FW_DLINK:
        {
            int tmpi;
            if (in.at(0) == '_') // символ подчёркивания в первой позиции ИД означает, что номер таблицы надо брать из второго символа
            {
                tmpi = in.at(1).digitValue();
                if (tmpi*2+1 > ff.link.size()) // нет в перечислении links таблицы с таким номером
                {
                    result = TFRESULT_ERROR;
                    WARNMSG("");
                    return;
                }
                in.remove(0, 2);
            }
            else
                tmpi = 0;
            table = ff.link.at(tmpi*2);
            header = ff.link.at(tmpi*2+1);
            toid(table, header, in, out);
            if (result)
                WARNMSG("");
            break;
        }
        case FW_RIGHTS:
        {
            quint32 outui=0, tmpui = 0x0001;
            int j = 0;
            while ((!in.isEmpty()) && (j < ACC_NUM)) // пока в строке есть что-нибудь и находимся в пределах битовой ширины прав
            {
                switch (j%2)
                {
                case 0:
                {
                    int tmpi = in.size()-1;
                    QChar tmpc = in.at(tmpi);
                    if (tmpc == QChar(1095)) // "ч"
                        outui |= tmpui;
                    break;
                }
                case 1:
                {
                    int tmpi = in.size()-1;
                    QChar tmpc = in.at(tmpi);
                    if (tmpc == QChar(1079)) // "з"
                        outui |= tmpui;
                    break;
                }
                default:
                    break;
                }
                in.chop(1);
                j++;
                tmpui <<= 1;
            }
            out = QString::number(outui, 16).toUpper();
            break;
        }
        case FW_CRYPT:
        {
            if (pc.AutonomousMode)
            {

            }
            else
            {
                QStringList sl;
                sl << in << links;
                int res = Cli->SendAndGetResult(T_VTID, sl);
                if (res == Client::CLIER_EMPTY)
                    result = TFRESULT_EMPTY;
                else if (res != Client::CLIER_NOERROR)
                    result = TFRESULT_ERROR;
                else
                    out = Cli->ResultStr;
            }
            break;
        }
        case FW_BOOL:
        {
            out = in;
            break;
        }
        default:
        {
            WARNMSG("");
            break;
        }
        }
}

// вспомогательная процедура возвращает ИД по значению value для заданной таблицы tble и поля header

void s_TableFields::toid(const QString &tble, const QString &header, const QString &value, QString &out)
{
    QStringList sl;
    QString tmptble;
    result = TFRESULT_NOERROR;
    out.clear();
    if (pc.AutonomousMode)
    {
        TableFields(tble, header, sl); // sl.at(0) = <table>, sl.at(1) = <TableFields>
        if (result)
        {
            WARNMSG("");
            return;
        }
        QString db = sl.at(0).split(".").at(0);
        tmptble = sl.at(0).split(".").at(1);
        out = sqlc.GetLastValueFromTableByField(db, tmptble, "id"+tmptble, sl.at(1), value);
        if (sqlc.result)
        {
            result = 1;
            WARNMSG(sqlc.LastError);
            return;
        }
    }
    else
    {
        sl << tble << header << value;
        int res = Cli->SendAndGetResult(T_TID, sl);
        if (res == Client::CLIER_EMPTY)
            result = TFRESULT_EMPTY;
        else if (res != Client::CLIER_NOERROR)
            result = TFRESULT_ERROR;
        else
            out = Cli->ResultStr;
    }
}

// Update - процедура обновления записи values по адресу table:TableFields для таблицы tble
// ИД записи, по которой обновляются значения, хранятся в values.at(headers.indexof("ИД"))

void s_TableFields::Update(const QString &tble, QStringList &headers, QStringList &values)
{
    result = TFRESULT_NOERROR;
    if (headers.size() != values.size())
    {
        result = TFRESULT_ERROR;
        DBGMSG;
        return;
    }
    int dateidx = headers.indexOf("Дата");
    int idpersidx = headers.indexOf("ИДПольз");
    if (pc.AutonomousMode)
    {
        // Сначала найдём для данной таблицы ключевое поле ("v") и по нему вытащим реальную БД, таблицу и наименование поля
        QStringList fl = QStringList() << "table" << "TableFields" << "header";
        QStringList cmpfl = QStringList() << "tablename" << "keyfield";
        QStringList cmpvl = QStringList() << tble << "v";
        QStringList keydbtble = sqlc.GetValuesFromTableByFields("sup", "TableFields", fl, cmpfl, cmpvl);
        if (sqlc.result)
        {
            result = TFRESULT_ERROR;
            WARNMSG(sqlc.LastError);
            return;
        }
        QStringList tmpsl = keydbtble.at(0).split(".");
        keydbtble.insert(1, tmpsl.at(1)); // в первом элементе будет tble
        keydbtble.replace(0, tmpsl.at(0)); // в нулевом элементе оставляем только db
        int ididx = headers.indexOf(keydbtble.at(3)); // вытаскиваем индекс ключевого поля
        if (ididx == -1)
        {
            WARNMSG("Не найдено ключевое поле "+keydbtble.at(3)+" в таблице "+keydbtble.at(1));
            result = TFRESULT_ERROR;
            return;
        }
        QString keyid;
        if (ididx < values.size())
            keyid = values.at(ididx); // id - ИД элемента в ключевом поле, используется при записи всех остальных полей
        else
        {
            result = TFRESULT_ERROR;
            WARNMSG("Не найдено ключевое поле "+keydbtble.at(3)+" в таблице "+keydbtble.at(1));
            return;
        }
        keyid = QString::number(keyid.toInt()); // убираем незначащие нули
        QStringList tmpTableFields, tmpvalues;
        QString tmpdb, tmptble, tmpdbtble;
        int i;
        while (headers.size() > 0)
        {
            QStringList dbtble;
            QString header = headers.at(0);
            TableFields(tble, header, dbtble);
            if (dbtble.at(0) != "-") // если знак дефиса, поле не пишется в базу
            {
                tmpdbtble = dbtble.at(0);
                tmpvalues = QStringList() << values.at(0); // кладём первое значение в выходной буфер значений
                tmpTableFields = QStringList() << dbtble.at(1);
                headers.removeAt(0);
                values.removeAt(0);
                i = 0;
                while (i < headers.size())
                {
                    header = headers.at(i);
                    TableFields(tble, header, dbtble);
                    if (dbtble.size() == 0)
                    {
                        WARNMSG("");
                        result = TFRESULT_ERROR;
                        return;
                    }
                    if (dbtble.at(0) == tmpdbtble) // здесь проверка на дефис не нужна, т.к. сравнение ведётся с tmpdbtble, который уже проверен ранее
                    {
                        tmpTableFields << dbtble.at(1);
                        tmpvalues << values.at(i);
                        headers.removeAt(i);
                        values.removeAt(i);
                    }
                    else
                        i++;
                }
                tmpdb = tmpdbtble.split(".").at(0);
                tmptble = tmpdbtble.split(".").at(1);
                if (dateidx == -1)
                {
                    tmpTableFields << "date";
                    tmpvalues << pc.DateTime;
                }
                if (idpersidx == -1)
                {
                    tmpTableFields << "idpers";
                    tmpvalues << QString::number(pc.idPers);
                }
                result = sqlc.UpdateValuesInTable(tmpdb, tmptble, tmpTableFields, tmpvalues, keydbtble.at(2), keyid);
                if (result)
                    WARNMSG(sqlc.LastError);
            }
        }
    }
    else
    {
        QStringList sl;
        sl << tble;
        for (int i=0; i<headers.size(); ++i)
        {
            sl << headers.at(i);
            if (i < values.size())
                sl << values.at(i);
        }
        int res = Cli->SendAndGetResult(T_UPD, sl);
        if (res != Client::CLIER_NOERROR)
            result = TFRESULT_ERROR;
    }
}

// Updatev - процедура обновления записей headers для таблицы tble значениями values
// ИД записи, по которой обновляются значения, хранятся в values.at(headers.indexof("ИД"))

void s_TableFields::Updatev(const QString &tble, QStringList &headers, QStringList &values)
{
    result = TFRESULT_NOERROR;
    if (headers.size() != values.size())
    {
        result = TFRESULT_ERROR;
        DBGMSG;
        return;
    }
/*    if (pc.AutonomousMode)
    {
        // Сначала найдём для данной таблицы ключевое поле ("v") и по нему вытащим реальную БД, таблицу и наименование поля
        QStringList fl = QStringList() << "table" << "TableFields" << "header";
        QStringList cmpfl = QStringList() << "tablename" << "keyfield";
        QStringList cmpvl = QStringList() << tble << "v";
        QStringList keydbtble = sqlc.GetValuesFromTableByFields("sup", "TableFields", fl, cmpfl, cmpvl);
        if (sqlc.result)
        {
            result = TFRESULT_ERROR;
            WARNMSG(sqlc.LastError);
            return;
        }
        QStringList tmpsl = keydbtble.at(0).split(".");
        keydbtble.insert(1, tmpsl.at(1)); // в первом элементе будет tble
        keydbtble.replace(0, tmpsl.at(0)); // в нулевом элементе оставляем только db
        int ididx = headers.indexOf(keydbtble.at(3)); // вытаскиваем индекс ключевого поля
        if (ididx == -1)
        {
            WARNMSG("Не найдено ключевое поле "+keydbtble.at(3)+" в таблице "+keydbtble.at(1));
            result = TFRESULT_ERROR;
            return;
        }
        QString keyid;
        if (ididx < values.size())
            keyid = values.at(ididx); // id - ИД элемента в ключевом поле, используется при записи всех остальных полей
        else
        {
            result = TFRESULT_ERROR;
            WARNMSG("Не найдено ключевое поле "+keydbtble.at(3)+" в таблице "+keydbtble.at(1));
            return;
        }
        keyid = QString::number(keyid.toInt()); // убираем незначащие нули
        QStringList tmpTableFields, tmpvalues;
        QString tmpdb, tmptble, tmpdbtble;
        int i;
        while (headers.size() > 0)
        {
            QStringList dbtble;
            QString header = headers.at(0);
            TableFields(tble, header, dbtble);
            if (dbtble.at(0) != "-") // если знак дефиса, поле не пишется в базу
            {
                tmpdbtble = dbtble.at(0);
                tmpvalues = QStringList() << values.at(0); // кладём первое значение в выходной буфер значений
                tmpTableFields = QStringList() << dbtble.at(1);
                headers.removeAt(0);
                values.removeAt(0);
                i = 0;
                while (i < headers.size())
                {
                    header = headers.at(i);
                    TableFields(tble, header, dbtble);
                    if (dbtble.size() == 0)
                    {
                        WARNMSG("");
                        result = TFRESULT_ERROR;
                        return;
                    }
                    if (dbtble.at(0) == tmpdbtble) // здесь проверка на дефис не нужна, т.к. сравнение ведётся с tmpdbtble, который уже проверен ранее
                    {
                        tmpTableFields << dbtble.at(1);
                        tmpvalues << values.at(i);
                        headers.removeAt(i);
                        values.removeAt(i);
                    }
                    else
                        i++;
                }
                tmpdb = tmpdbtble.split(".").at(0);
                tmptble = tmpdbtble.split(".").at(1);
                tmpTableFields << "date" << "idpers";
                tmpvalues << pc.DateTime << QString::number(pc.idPers);
                result = sqlc.UpdateValuesInTable(tmpdb, tmptble, tmpTableFields, tmpvalues, keydbtble.at(2), keyid);
                if (result)
                    WARNMSG(sqlc.LastError);
            }
        }
    }
    else
    { */
        QStringList sl;
        sl << tble;
        for (int i=0; i<headers.size(); ++i)
        {
            sl << headers.at(i);
            if (i < values.size())
                sl << values.at(i);
        }
        int res = Cli->SendAndGetResult(T_UPDV, sl);
        if (res != Client::CLIER_NOERROR)
            result = TFRESULT_ERROR;
//    }
}

// insert - вставка новой записи в таблицу tble
// возвращает индекс новой строки

void s_TableFields::Insert(const QString tble, QString &out)
{
    out.clear();
    result = TFRESULT_NOERROR;
    if (pc.AutonomousMode)
    {
        QStringList cmpfl = QStringList() << "tablename" << "keyfield";
        QStringList cmpvl = QStringList() << tble << "v";
        QString keydbtble = sqlc.GetValueFromTableByFields("sup", "TableFields", "table", cmpfl, cmpvl);
        if (sqlc.result)
        {
            result = TFRESULT_ERROR;
            WARNMSG(sqlc.LastError);
            return;
        }
        out = sqlc.InsertValuesToTable(keydbtble.split(".").at(0), keydbtble.split(".").at(1), QStringList(), QStringList()); // вставка новой пустой строки
        if (sqlc.result)
        {
            result = TFRESULT_ERROR;
            WARNMSG(sqlc.LastError);
            return;
        }
    }
    else
    {
        QStringList sl;
        sl << tble;
        int res = Cli->SendAndGetResult(T_INS, sl);
        if (res == Client::CLIER_EMPTY)
            result = TFRESULT_EMPTY;
        else if (res != Client::CLIER_NOERROR)
            result = TFRESULT_ERROR;
        else
            out = QString::number(Cli->ResultInt);
    }
}

// newid - найти первый свободный ИД в таблице
// возвращает индекс новой строки

void s_TableFields::NewID(const QString &tble, QString &out)
{
    out.clear();
    result = TFRESULT_NOERROR;
    if (pc.AutonomousMode)
    {
        QStringList cmpfl = QStringList() << "tablename" << "keyfield";
        QStringList cmpvl = QStringList() << tble << "v";
        QString keydbtble = sqlc.GetValueFromTableByFields("sup", "TableFields", "table", cmpfl, cmpvl);
        if (sqlc.result)
        {
            result = TFRESULT_ERROR;
            WARNMSG(sqlc.LastError);
            return;
        }
        int newid = sqlc.GetNextFreeIndex(keydbtble.split(".").at(0), keydbtble.split(".").at(1));
        if (sqlc.result)
        {
            result = TFRESULT_ERROR;
            WARNMSG(sqlc.LastError);
            return;
        }
        out = QString::number(newid);
    }
    else
    {
        QStringList sl;
        sl << tble;
        int res = Cli->SendAndGetResult(T_GID, sl);
        if (res == Client::CLIER_EMPTY)
            result = TFRESULT_EMPTY;
        else if (res != Client::CLIER_NOERROR)
            result = TFRESULT_ERROR;
        else
            out = QString::number(Cli->ResultInt);
    }
}

// remove - "удаление" записи с индексом id из таблицы tble
// важно: здесь не удаляются ссылки на данную запись, которая будет удалена, проверку "дохлых" ссылок на записи, у которых deleted=1, необходимо
// проводить и исправлять при старте СУПиКа или при "обновлении проблем"

void s_TableFields::Remove(const QString &tble, const QString &id)
{
    result = TFRESULT_NOERROR;
    if (pc.AutonomousMode)
    {
        QStringList fl = QStringList() << "table" << "TableFields";
        QStringList cmpfl = QStringList() << "tablename" << "keyfield";
        QStringList cmpvl = QStringList() << tble << "v";
        QStringList keydbtble = sqlc.GetValuesFromTableByFields("sup", "TableFields", fl, cmpfl, cmpvl);
        if (sqlc.result)
        {
            WARNMSG(sqlc.LastError);
            result = TFRESULT_ERROR;
            return;
        }
        sqlc.DeleteFromDB(keydbtble.at(0).split(".").at(0), keydbtble.at(0).split(".").at(1), keydbtble.at(1), id);
        if (sqlc.result)
        {
            result = TFRESULT_ERROR;
            WARNMSG(sqlc.LastError);
        }
    }
    else
    {
        QStringList sl;
        sl << tble << id;
        int res = Cli->SendAndGetResult(T_DEL, sl);
        if (res != Client::CLIER_NOERROR)
            result = TFRESULT_ERROR;
    }
}

// remove - реальное удаление записи с индексом id из таблицы tble
// важно: здесь не удаляются ссылки на данную запись, которая будет удалена, проверку "дохлых" ссылок на отсутствующие записи необходимо
// проводить и исправлять при старте СУПиКа или при "обновлении проблем"

void s_TableFields::Delete(const QString &tble, const QString &id)
{
    result = TFRESULT_NOERROR;
    if (pc.AutonomousMode)
    {
        QStringList fl = QStringList() << "table" << "TableFields";
        QStringList cmpfl = QStringList() << "tablename" << "keyfield";
        QStringList cmpvl = QStringList() << tble << "v";
        QStringList keydbtble = sqlc.GetValuesFromTableByFields("sup", "TableFields", fl, cmpfl, cmpvl);
        if (sqlc.result)
        {
            WARNMSG(sqlc.LastError);
            result = TFRESULT_ERROR;
            return;
        }
        sqlc.RealDeleteFromDB(keydbtble.at(0).split(".").at(0), keydbtble.at(0).split(".").at(1), QStringList(keydbtble.at(1)), QStringList(id));
        if (sqlc.result)
        {
            result = TFRESULT_ERROR;
            WARNMSG(sqlc.LastError);
        }
    }
    else
    {
        QStringList sl;
        sl << tble << id;
        int res = Cli->SendAndGetResult(T_RDEL, sl);
        if (res != Client::CLIER_NOERROR)
        {
            result = TFRESULT_ERROR;
            return;
        }
    }
}

bool s_TableFields::Check(const QString &tble, const QString &cmpfield, const QString &cmpvalue)
{
    result = TFRESULT_NOERROR;
    QStringList sl;
    QString realcmpfield;
    if (pc.AutonomousMode)
    {
        tfl.TableFields(tble,cmpfield, sl);
        if (result)
        {
            WARNMSG("");
            return false;
        }
        QString cmpdb = sl.at(0).split(".").at(0); // реальное имя БД
        QString cmptble = sl.at(0).split(".").at(1); // реальное название таблицы
        realcmpfield = sl.at(1); // реальное название поля сравнения
        sl = sqlc.GetColumnsFromTable(cmpdb,cmptble);
        sl = sqlc.GetValuesFromTableByField(cmpdb,cmptble,sl,realcmpfield,cmpvalue);
        if (sqlc.result)
        {
            result = TFRESULT_ERROR;
            return false;
        }
        return true;
    }
    else
    {
        QStringList sl;
        sl << tble << cmpfield << cmpvalue;
        int res = Cli->SendAndGetResult(T_C, sl);
        if (res == Client::CLIER_EMPTY)
            result = TFRESULT_EMPTY;
        else if (res != Client::CLIER_NOERROR)
            result = TFRESULT_ERROR;
        else
        {
            if (Cli->ResultStr == "0")
                return false;
            else
                return true;
        }
        return false;
    }
}

void s_TableFields::valuesbyfield(const QString &tble, QStringList &fl, const QString &cmpfield, const QString &cmpvalue, QStringList &out, bool Warn)
{
    valuesbyfields(tble, fl, QStringList(cmpfield), QStringList(cmpvalue), out, Warn);
/*    QStringList sl;
    out.clear();
    result = TFRESULT_NOERROR;
    if (pc.AutonomousMode)
    {
        tfl.TableFields(tble,cmpfield, sl);
        if (result)
        {
            WARNMSG("");
            return;
        }
        QString cmpdb = sl.at(0).split(".").at(0); // реальное имя БД
        QString cmptble = sl.at(0).split(".").at(1); // реальное название таблицы
        QString realcmpfield = sl.at(1); // реальное название поля сравнения
        for (int i = 0; i < fl.size(); i++)
        {
            QString field = fl.at(i);
            TableFields(tble,field, sl);
            if (result)
            {
                WARNMSG("");
                return;
            }
            fl.replace(i, sl.at(1)); // заменяем русское наименование поля на его реальное название
        }
        out = sqlc.GetValuesFromTableByField(cmpdb,cmptble,fl,realcmpfield,cmpvalue);
        if ((sqlc.result) && (Warn))
        {
            WARNMSG(sqlc.LastError);
            result = TFRESULT_ERROR;
            return;
        }
    }
    else
    {
        sl << QString::number(fl.size()) << "1" << tble << fl << cmpfield << cmpvalue;
        int res = Cli->SendAndGetResult(T_GVSBFS, sl);
        if (res == Client::CLIER_EMPTY)
            result = TFRESULT_EMPTY;
        else if ((res != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
            result = TFRESULT_ERROR;
        else
            out = Cli->Result.at(0);
    } */
}

void s_TableFields::valuesbyfields(const QString &tble, QStringList &fl, QStringList &cmpfields, QStringList &cmpvalues, QStringList &out, bool Warn)
{
    Q_UNUSED(Warn);
    QList<QStringList> lsl;
    valuesbyfieldsmatrix(tble, fl, cmpfields, cmpvalues, lsl);
    if (result == TFRESULT_NOERROR)
        out = lsl.at(0);
}

void s_TableFields::valuesbyfieldsmatrix(const QString &tble, QStringList &fl, QStringList &cmpfields, QStringList &cmpvalues, QList<QStringList> &out)
{
    QStringList cmpfl, sl;
    out.clear();
    result = TFRESULT_NOERROR;
    if ((cmpfields.size() != cmpvalues.size()) || (cmpfields.size() == 0) || (fl.size() == 0))
    {
        result = 1;
        DBGMSG;
        return;
    }
    if (pc.AutonomousMode)
    {
        // взяли все реальные названия полей сравнения
        for (int i=0; i<cmpfields.size(); i++)
        {
            QString cmpfield = cmpfields.at(i);
            TableFields(tble,cmpfield, sl);
            if (result)
            {
                WARNMSG("");
                return;
            }
            cmpfl << sl.at(1);
        }
        for (int i = 0; i < fl.size(); i++)
        {
            QString field = fl.at(i);
            TableFields(tble,field, sl);
            if (result)
            {
                WARNMSG("");
                return;
            }
            fl.replace(i, sl.at(1)); // заменяем русское наименование поля на его реальное название
        }
        QString cmpdb = sl.at(0).split(".").at(0); // реальное имя БД
        QString cmptble = sl.at(0).split(".").at(1); // реальное название таблицы
        out = sqlc.GetMoreValuesFromTableByFields(cmpdb,cmptble,fl,cmpfl,cmpvalues);
//        if ((sqlc.result) && (Warn))
        if (sqlc.result)
        {
            WARNMSG(sqlc.LastError);
            result = TFRESULT_ERROR;
            return;
        }
    }
    else
    {
        int i;
        int fields_num = fl.size();
        int pairs_num = cmpfields.size();
        sl << QString::number(fields_num) << QString::number(pairs_num) << tble;
        for (i=0; i<fields_num; i++)
            sl << fl.at(i);
        for (i=0; i<pairs_num; i++)
        {
            sl << cmpfields.at(i);
            sl << cmpvalues.at(i);
        }
        int res = Cli->SendAndGetResult(T_GVSBFS, sl);
        if (res == Client::CLIER_EMPTY)
            result = TFRESULT_EMPTY;
        else if ((res != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
            result = TFRESULT_ERROR;
        else
            out = Cli->Result;
    }
}

void s_TableFields::TableColumn(const QString &tble, const QString &field, QStringList &out)
{
    out = sqlc.GetValuesFromTableByColumnAndFields("sup", "TableFields", field, QStringList("tablename"), QStringList(tble), "fieldsorder");
    if (sqlc.result)
    {
        WARNMSG(sqlc.LastError);
        result = TFRESULT_ERROR;
        out.clear();
        return;
    }
    result = TFRESULT_NOERROR;
}

void s_TableFields::TableFields(const QString &tble, const QString &header, QStringList &out)
{
    QStringList fl = QStringList() << "table" << "TableFields" << "links";
    QStringList cmpfl = QStringList() << "tablename" << "header";
    QStringList cmpvl = QStringList() << tble << header;
    out = sqlc.GetValuesFromTableByFields("sup", "TableFields", fl, cmpfl, cmpvl);
    if ((sqlc.result) || (out.isEmpty()))
    {
        WARNMSG(sqlc.LastError);
        result = TFRESULT_ERROR;
        out.clear();
        return;
    }
    result = TFRESULT_NOERROR;
}

void s_TableFields::TableHeaders(const QString &tble, QStringList &out)
{
    out = sqlc.GetValuesFromTableByColumnAndFields("sup", "TableFields", "header", QStringList("tablename"), QStringList(tble), "fieldsorder", true);
    if ((sqlc.result) || (out.isEmpty()))
    {
        WARNMSG(sqlc.LastError);
        result = TFRESULT_ERROR;
        out.clear();
        return;
    }
    result = TFRESULT_NOERROR;
}

void s_TableFields::tablelinks(const QString &tble, QStringList &out)
{
    out = sqlc.GetValuesFromTableByColumnAndFields("sup", "TableFields", "links", QStringList("tablename"), QStringList(tble), "fieldsorder", true);
    if ((sqlc.result) || (out.isEmpty()))
    {
        WARNMSG(sqlc.LastError);
        result = TFRESULT_ERROR;
        out.clear();
        return;
    }
    result = TFRESULT_NOERROR;
}

bool s_TableFields::tableistree(const QString &tble)
{
    QStringList tmpfl = QStringList() << "TableFields" << "tablename";
    QStringList tmpvl = QStringList() << "alias" << tble;
    sqlc.GetValueFromTableByFields("sup","TableFields","TableFields",tmpfl,tmpvl);
    if (sqlc.result)
        return false;
    else
        return true;
}
