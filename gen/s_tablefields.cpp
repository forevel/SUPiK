#include <QVariant>
#include <QThread>
#include <QCoreApplication>
#include "s_tablefields.h"
#include "client.h"

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

void s_tablefields::htovl(QString &tble, QString &header, QStringList &out)
{
    QStringList sl;
    if (pc.AutonomousMode)
    {
        tablefields(tble, header, sl); // sl.at(0) = <table>, sl.at(1) = <tablefields>
        if (result)
        {
            out.clear();
            return;
        }
        QString db = sl.at(0).split(".").at(0); // table = <db>.<tble>
        tble = sl.at(0).split(".").at(1);
        out = sqlc.GetValuesFromTableByColumn(db, tble, sl.at(1),"id"+tble,true);
        if (sqlc.result == 2) // ошибка
        {
            result = 1;
            TFWARN(sqlc.LastError);
            out.clear();
            return;
        }
        result = 0;
    }
    else
    {
        sl << tble << header;
        Cli->SendCmd(T_GVSBC, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
        {
            result = 1;
            out.clear();
        }
        else
        {
            out = Cli->Result.at(0);
            result = 0;
            return;
        }
    }
}

// htovlc - это процедура htovl с дополнительным условием
// берёт все значения по полю header таблицы tble, где поле cheader этой же таблицы равно value

void s_tablefields::htovlc(QString &tble, QString &header, QString &cheader, QString &value, QStringList &out)
{
    QStringList sl, cl;
    if (pc.AutonomousMode)
    {
        tablefields(tble, header, sl); // sl.at(0) = <table>, sl.at(1) = <tablefields>
        if (result)
        {
            TFWARN("");
            out.clear();
            return;
        }
        tablefields(tble, cheader, cl); // cl.at(1) = <tablefields>
        if (result)
        {
            TFWARN("");
            out.clear();
            return;
        }
        QString db = sl.at(0).split(".").at(0);
        tble = sl.at(0).split(".").at(1);
        out = sqlc.GetValuesFromTableByColumnAndFields(db, tble, sl.at(1), QStringList(cl.at(1)), QStringList(value));
        if (sqlc.result) // || tmpsl.isEmpty())
        {
            result = 1;
            TFWARN(sqlc.LastError);
            out.clear();
            return;
        }
        result = 0;
    }
    else
    {
        sl << tble << header << cheader << value;
        Cli->SendCmd(T_GVSBCF, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
        {
            result = 1;
            out.clear();
        }
        else
        {
            out = Cli->Result.at(0);
            result = 0;
            return;
        }
    }
}

// процедура возвращает список из списков значений, взятых по полям для таблицы tble
// нужна для организации списка выбора из таблиц а-ля "*_сокращ"

void s_tablefields::tbvll(QString &tble, QList<QStringList> &out)
{
    QStringList tmpsl, sl;
    if (pc.AutonomousMode)
    {
        tablelinks(tble, sl); // берём links
        int i;
        if (result)
        {
            TFWARN("");
            out.clear();
            return;
        }
        out.append(sl);
        tableheaders(tble, sl); // берём header
        if (result)
        {
            TFWARN("");
            out.clear();
            return;
        }
        out.insert(0, sl); // на поз. 0 заголовки, на поз. 1 - links
        for (i = 0; i < sl.size(); i++)
        {
            QString field = sl.at(i);
            htovl(tble, field, tmpsl);
            out.append(tmpsl);
        }
        result = 0;
    }
    else
    {
        sl << tble;
        Cli->SendCmd(T_GFT, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if (Cli->DetectedError != Client::CLIER_NOERROR)
        {
            result = 1;
            out.clear();
            return;
        }
        out = Cli->Result;
        result = 0;
    }
}

// процедура возвращает значение по ссылке tbleid из table.tablefields для заданной таблицы tble и поля header

void s_tablefields::tov(QString &tble, QString &header, QString &tbleid, QString &out)
{
    QStringList sl;
    if (pc.AutonomousMode)
    {
        tablefields(tble, header, sl); // sl.at(0) = <table>, sl.at(1) = <tablefields>
        if (result)
        {
            TFWARN("");
            out.clear();
            return;
        }
        QString db = sl.at(0).split(".").at(0);
        tble = sl.at(0).split(".").at(1);
        out = sqlc.GetValueFromTableByID(db, tble, sl.at(1), tbleid);
        if (sqlc.result == 2) // если ошибка в запросе SQL
        {
            result = 1;
            TFWARN(sqlc.LastError);
            out.clear();
            return;
        }
        result = 0;
    }
    else
    {
        sl << tble << header << tbleid;
        Cli->SendCmd(T_TV, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || Cli->Result.isEmpty())
        {
            result = 1;
            out.clear();
            return;
        }
        if (Cli->Result.at(0).isEmpty())
        {
            result = 1;
            out.clear();
            return;
        }
        out = Cli->Result.at(0).at(0);
        result = 0;
    }
}

// перевод ИД через строку links в таблице tablefields в имя
// id - ИД элемента в таблице tablefields:table/tablefields
// links - строка из поля tablefields:links

void s_tablefields::idtov(QString links, QString id, PublicClass::ValueStruct &out)
{
    QStringList sl;
    PublicClass::FieldFormat ff;
    if (pc.AutonomousMode)
    {
        QString table, header;
        out.Type = VS_STRING; // тип по умолчанию - простая строка
        out.Value = ""; // значение по умолчанию - пустая строка
        out.Links = links;
        pc.getFFfromLinks(links, ff);
        switch (ff.ftype)
        {
        case FW_ALLINK:
        {
            if (!id.toInt()) // корневой элемент
            {
                out.Value = "<Корневой элемент>";
            }
            sl = id.split(".");
            if (sl.size() > 1)
                id = sl.at(1);
            table = ff.link.at(0);
            header = "Наименование";
            tov(table, header, id, out.Value);
            if (result)
                TFWARN("");
            break;
        }
        case FW_LINK:
        case FW_MAXLINK:
        {
            sl = id.split(".");
            if (sl.size() > 1)
                id = sl.at(1);
            table = ff.link.at(0);
            header = ff.link.at(1);
            tov(table, header, id, out.Value);
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
            out.Value = id;
            break;
        }
        case FW_DLINK:
        {
            sl = id.split("."); // номер таблицы до точки (нумерация с нуля), id - после точки
            int tmpi = sl.at(0).toInt();
            if (tmpi*2+1 > ff.link.size()) // нет в перечислении links таблицы с таким номером
            {
                result = 1;
                TFWARN("");
                return;
            }
            table = ff.link.at(tmpi*2);
            header = ff.link.at(tmpi*2+1);
            QString value = sl.at(1);
            tov(table, header, value, out.Value);
            QString tmps = "_"+sl.at(0); // _ - признак того, что в ячейку надо сохранить доп. информацию о номере таблицы
            out.Value.insert(0, tmps);
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
                return;
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
            out.Value = outs;
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
                return;
            }
            out.Type = VS_ICON;
            if (tmpb == 0)
                out.Value = ":/res/cross.png";
            else
                out.Value = ":/res/ok.png";
        }
        default:
        {
            out.Value = id;
            break;
        }
        }
    }
    else
    {
        sl << links << id;
        Cli->SendCmd(T_IDTV, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || Cli->Result.isEmpty())
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
    }
}

// взять все значения по ссылке links в зависимости от типа ссылки

void s_tablefields::idtovl(QString &links, QStringList &out)
{
    if (pc.AutonomousMode)
    {
        QString table, header, value;
        PublicClass::FieldFormat ff;
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
                TFWARN("");
                out.clear();
                return;
            }
            value = "ИД_а";
            htovlc(table, header, value, id, out);
            if (result)
            {
                TFWARN("");
                out.clear();
                return;
            }
            break;
        }
        case FW_MAXLINK:
        case FW_LINK:
        {
            table = ff.link.at(0);
            header = ff.link.at(1);
            htovl(table, header, out);
            if (result)
            {
                TFWARN("");
                out.clear();
                return;
            }
            break;
        }
        default:
            break;
        }
    }
    else
    {
        QStringList sl;
        sl << links;
        Cli->SendCmd(T_IDTVL, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || Cli->Result.isEmpty())
        {
            result = 1;
            return;
        }
        out = Cli->Result.at(0);
    }
    result = 0;
}

// перевод имени в его ИД через строку links в таблице tablefields (поиск в таблице по имени его ИД)
// links = 2.9, value = "ч..ч..чузчузч"

void s_tablefields::vtoid(PublicClass::ValueStruct &vl, QString &out)
{
    if (pc.AutonomousMode)
    {
        QString table, header;
        result = 0;
        if ((vl.Type == VS_STRING) && (vl.Value == "")) // пустая строка вполне имеет право на запись
        {
            out.clear();
            return;
        }
        PublicClass::FieldFormat ff;
        pc.getFFfromLinks(vl.Links, ff);
        switch (ff.ftype)
        {
        case FW_ALLINK:
        {
            if (vl.Type == VS_STRING)
            {
                if (vl.Value == "<Корневой элемент>")
                    out = "0";
                else
                {
                    table = ff.link.at(0);
                    header = "Наименование";
                    toid(table, header, vl.Value, out);
                }
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
            table = ff.link.at(0);
            header = ff.link.at(1);
            toid(table, header, vl.Value, out);
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
            if (ff.delegate == FD_SPIN) // для спина отдельная обработка, ибо дурит как хочет
            {
                int tmpInt = ff.link.at(0).count("n", Qt::CaseSensitive);
                int tmpInt2 = ff.link.at(0).count("d", Qt::CaseSensitive);
                QStringList sl = vl.Value.split(".");
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
            out = vl.Value;
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
                    out.clear();
                    return;
                }
                tmps.remove(0, 2);
            }
            else
                tmpi = 0;
            table = ff.link.at(tmpi*2);
            header = ff.link.at(tmpi*2+1);
            toid(table, header, tmps, out);
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
            out = QString::number(outui, 16).toUpper();
            break;
        }
        case FW_BOOL:
        {
            if (vl.Type == VS_ICON)
            {
                if (vl.Value == ":/res/ok.png")
                    out = "1";
                else if (vl.Value == ":/res/cross.png")
                    out = "0";
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
    }
    else
    {
        QStringList sl;
        sl << QString::number(vl.Type) << vl.Value << vl.Links;
        Cli->SendCmd(T_VTID, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || Cli->Result.isEmpty())
        {
            result = 1;
            out.clear();
            return;
        }
        if (Cli->Result.at(0).isEmpty())
        {
            result = 1;
            out.clear();
            return;
        }
        out = Cli->Result.at(0).at(0);
        result = 0;
    }
}

// вспомогательная процедура возвращает ИД по значению value для заданной таблицы tble и поля header

void s_tablefields::toid(QString &tble, QString &header, QString &value, QString &out)
{
    QStringList sl;
    if (pc.AutonomousMode)
    {
        tablefields(tble, header, sl); // sl.at(0) = <table>, sl.at(1) = <tablefields>
        if (result)
        {
            TFWARN("");
            out.clear();
            return;
        }
        QString db = sl.at(0).split(".").at(0);
        tble = sl.at(0).split(".").at(1);
        out = sqlc.GetLastValueFromTableByField(db, tble, "id"+tble, sl.at(1), value);
        if (sqlc.result)
        {
            result = 1;
            TFWARN(sqlc.LastError);
            out.clear();
            return;
        }
    }
    else
    {
        sl << tble << header << value;
        Cli->SendCmd(T_TID, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || Cli->Result.isEmpty())
        {
            result = 1;
            out.clear();
            return;
        }
        if (Cli->Result.at(0).isEmpty())
        {
            result = 1;
            out.clear();
            return;
        }
        out = Cli->Result.at(0).at(0);
    }
    result = 0;
}

// idtois - процедура обновления записи values по адресу table:tablefields для таблицы tble
// ИД записи, по которой обновляются значения, хранятся в values.at(headers.indexof("ИД"))

void s_tablefields::idtois(QString &tble, QStringList &headers, QStringList &values)
{
    if (headers.size() != values.size())
    {
        result = 1;
        TFDBG;
        return;
    }
    if (pc.AutonomousMode)
    {
        // Сначала найдём для данной таблицы ключевое поле ("v") и по нему вытащим реальную БД, таблицу и наименование поля
        QStringList fl = QStringList() << "table" << "tablefields" << "header";
        QStringList cmpfl = QStringList() << "tablename" << "keyfield";
        QStringList cmpvl = QStringList() << tble << "v";
        QStringList keydbtble = sqlc.GetValuesFromTableByFields("sup", "tablefields", fl, cmpfl, cmpvl);
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
            QStringList dbtble;
            QString header = headers.at(0);
            tablefields(tble, header, dbtble);
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
                    header = headers.at(i);
                    tablefields(tble, header, dbtble);
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
                result = sqlc.UpdateValuesInTable(tmpdb, tmptble, tmptablefields, tmpvalues, keydbtble.at(2), keyid);
                if (result)
                    TFWARN(sqlc.LastError);
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
        Cli->SendCmd(T_UPD, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
        {
            result = 1;
            return;
        }
        result = 0;
    }
}

// insert - вставка новой записи в таблицу tble
// возвращает индекс новой строки

void s_tablefields::insert(QString tble, QString &out)
{
    if (pc.AutonomousMode)
    {
        QStringList cmpfl = QStringList() << "tablename" << "keyfield";
        QStringList cmpvl = QStringList() << tble << "v";
        QString keydbtble = sqlc.GetValueFromTableByFields("sup", "tablefields", "table", cmpfl, cmpvl);
        if (sqlc.result)
        {
            result = 1;
            TFWARN(sqlc.LastError);
            out.clear();
            return;
        }
        out = sqlc.InsertValuesToTable(keydbtble.split(".").at(0), keydbtble.split(".").at(1), QStringList(), QStringList()); // вставка новой пустой строки
        if (sqlc.result)
        {
            result = 1;
            TFWARN(sqlc.LastError);
            out.clear();
            return;
        }
    }
    else
    {
        QStringList sl;
        sl << tble;
        Cli->SendCmd(T_INS, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || Cli->Result.isEmpty())
        {
            result = 1;
            out.clear();
            return;
        }
        if (Cli->Result.at(0).isEmpty())
        {
            result = 1;
            out.clear();
            return;
        }
        out = Cli->Result.at(0).at(0);
    }
    result = 0;
}

// newid - найти первый свободный ИД в таблице
// возвращает индекс новой строки

void s_tablefields::NewID(QString &tble, QString &out)
{
    if (pc.AutonomousMode)
    {
        QStringList cmpfl = QStringList() << "tablename" << "keyfield";
        QStringList cmpvl = QStringList() << tble << "v";
        QString keydbtble = sqlc.GetValueFromTableByFields("sup", "tablefields", "table", cmpfl, cmpvl);
        if (sqlc.result)
        {
            result = 1;
            TFWARN(sqlc.LastError);
            out.clear();
            return;
        }
        int newid = sqlc.GetNextFreeIndex(keydbtble.split(".").at(0), keydbtble.split(".").at(1));
        if (sqlc.result)
        {
            result = 1;
            TFWARN(sqlc.LastError);
            out.clear();
            return;
        }
        out = QString::number(newid);
    }
    else
    {
        QStringList sl;
        sl << tble;
        Cli->SendCmd(T_GID, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || Cli->Result.isEmpty())
        {
            result = 1;
            out.clear();
            return;
        }
        if (Cli->Result.at(0).isEmpty())
        {
            result = 1;
            out.clear();
            return;
        }
        out = Cli->Result.at(0).at(0);
    }
    result = 0;
}

// remove - "удаление" записи с индексом id из таблицы tble
// важно: здесь не удаляются ссылки на данную запись, которая будет удалена, проверку "дохлых" ссылок на записи, у которых deleted=1, необходимо
// проводить и исправлять при старте СУПиКа или при "обновлении проблем"

void s_tablefields::remove(QString &tble, QString &id)
{
    if (pc.AutonomousMode)
    {
        QStringList fl = QStringList() << "table" << "tablefields";
        QStringList cmpfl = QStringList() << "tablename" << "keyfield";
        QStringList cmpvl = QStringList() << tble << "v";
        QStringList keydbtble = sqlc.GetValuesFromTableByFields("sup", "tablefields", fl, cmpfl, cmpvl);
        if (sqlc.result)
        {
            TFWARN(sqlc.LastError);
            result = 1;
            return;
        }
        result = sqlc.DeleteFromDB(keydbtble.at(0).split(".").at(0), keydbtble.at(0).split(".").at(1), keydbtble.at(1), id);
        if (result)
            TFWARN(sqlc.LastError);
    }
    else
    {
        QStringList sl;
        sl << tble << id;
        Cli->SendCmd(T_DEL, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
            result = 1;
        else
            result = 0;
    }
}

// remove - реальное удаление записи с индексом id из таблицы tble
// важно: здесь не удаляются ссылки на данную запись, которая будет удалена, проверку "дохлых" ссылок на отсутствующие записи необходимо
// проводить и исправлять при старте СУПиКа или при "обновлении проблем"

void s_tablefields::Delete(QString &tble, QString &id)
{
    if (pc.AutonomousMode)
    {
        QStringList fl = QStringList() << "table" << "tablefields";
        QStringList cmpfl = QStringList() << "tablename" << "keyfield";
        QStringList cmpvl = QStringList() << tble << "v";
        QStringList keydbtble = sqlc.GetValuesFromTableByFields("sup", "tablefields", fl, cmpfl, cmpvl);
        if (sqlc.result)
        {
            TFWARN(sqlc.LastError);
            result = 1;
            return;
        }
        result = sqlc.RealDeleteFromDB(keydbtble.at(0).split(".").at(0), keydbtble.at(0).split(".").at(1), QStringList(keydbtble.at(1)), QStringList(id));
        if (result)
            TFWARN(sqlc.LastError);
    }
    else
    {
        QStringList sl;
        sl << tble << id;
        Cli->SendCmd(T_RDEL, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
            result = 1;
        else
            result = 0;
    }
}

bool s_tablefields::Check(QString &tble, QString &cmpfield, QString &cmpvalue)
{
    QStringList sl;
    if (pc.AutonomousMode)
    {
        tfl.tablefields(tble,cmpfield, sl);
        if (result)
        {
            TFWARN("");
            return false;
        }
        QString cmpdb = sl.at(0).split(".").at(0); // реальное имя БД
        QString cmptble = sl.at(0).split(".").at(1); // реальное название таблицы
        cmpfield = sl.at(1); // реальное название поля сравнения
        sl = sqlc.GetColumnsFromTable(cmpdb,cmptble);
        sl = sqlc.GetValuesFromTableByField(cmpdb,cmptble,sl,cmpfield,cmpvalue);
        if (sqlc.result)
        {
            result = 1;
            return false;
        }
        result = 0;
        return true;
    }
    else
    {
        QStringList sl;
        sl << tble << cmpfield << cmpvalue;
        Cli->SendCmd(T_C, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
        {
            result = 1;
            return false;
        }
        if (Cli->Result.at(0).isEmpty())
        {
            result = 1;
            return false;
        }
        result = 0;
        if (Cli->Result.at(0).at(0) == "0")
            return false;
        else
            return true;
    }
}

void s_tablefields::valuesbyfield(QString &tble, QStringList &fl, QString &cmpfield, QString &cmpvalue, QStringList &out, bool Warn)
{
    QStringList sl;
    if (pc.AutonomousMode)
    {
        tfl.tablefields(tble,cmpfield, sl);
        if (result)
        {
            TFWARN("");
            out.clear();
            return;
        }
        QString cmpdb = sl.at(0).split(".").at(0); // реальное имя БД
        QString cmptble = sl.at(0).split(".").at(1); // реальное название таблицы
        cmpfield = sl.at(1); // реальное название поля сравнения
        for (int i = 0; i < fl.size(); i++)
        {
            QString field = fl.at(i);
            tablefields(tble,field, sl);
            if (result)
            {
                TFWARN("");
                out.clear();
                return;
            }
            fl.replace(i, sl.at(1)); // заменяем русское наименование поля на его реальное название
        }
        out = sqlc.GetValuesFromTableByField(cmpdb,cmptble,fl,cmpfield,cmpvalue);
        if ((sqlc.result) && (Warn))
        {
            TFWARN(sqlc.LastError);
            result = 1;
            out.clear();
            return;
        }
    }
    else
    {
        sl << QString::number(fl.size()) << "1" << tble << fl << cmpfield << cmpvalue;
        Cli->SendCmd(T_GVSBFS, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || Cli->Result.isEmpty())
        {
            result = 1;
            return;
        }
        out = Cli->Result.at(0);
    }
    result = 0;
}

void s_tablefields::valuesbyfields(QString &tble, QStringList &fl, QStringList &cmpfields, QStringList &cmpvalues, QStringList &out, bool Warn)
{
    QStringList cmpfl, sl;
    if ((cmpfields.size() != cmpvalues.size()) || (cmpfields.size() == 0) || (fl.size() == 0))
    {
        result = 1;
        TFDBG;
        out.clear();
        return;
    }
    if (pc.AutonomousMode)
    {
        // взяли все реальные названия полей сравнения
        for (int i=0; i<cmpfields.size(); i++)
        {
            QString cmpfield = cmpfields.at(i);
            tablefields(tble,cmpfield, sl);
            if (result)
            {
                TFWARN("");
                out.clear();
                return;
            }
            cmpfl << sl.at(1);
        }
        for (int i = 0; i < fl.size(); i++)
        {
            QString field = fl.at(i);
            tablefields(tble,field, sl);
            if (result)
            {
                TFWARN("");
                out.clear();
                return;
            }
            fl.replace(i, sl.at(1)); // заменяем русское наименование поля на его реальное название
        }
        QString cmpdb = sl.at(0).split(".").at(0); // реальное имя БД
        QString cmptble = sl.at(0).split(".").at(1); // реальное название таблицы
        out = sqlc.GetValuesFromTableByFields(cmpdb,cmptble,fl,cmpfl,cmpvalues);
        if ((sqlc.result) && (Warn))
        {
            TFWARN(sqlc.LastError);
            result = 1;
            out.clear();
            return;
        }
        result = 0;
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
        Cli->SendCmd(T_GVSBFS, sl);
        while (Cli->Busy)
        {
            QThread::msleep(10);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        if ((Cli->DetectedError != Client::CLIER_NOERROR) || (Cli->Result.size() == 0))
        {
            result = 1;
            out.clear();
            return;
        }
        out = Cli->Result.at(0);
        result = 0;
    }
}

void s_tablefields::TableColumn(QString &tble, QString &field, QStringList &out)
{
    out = sqlc.GetValuesFromTableByColumnAndFields("sup", "tablefields", field, QStringList("tablename"), QStringList(tble), "fieldsorder");
    if (sqlc.result)
    {
        TFWARN(sqlc.LastError);
        result = 1;
        out.clear();
        return;
    }
    result = 0;
}

void s_tablefields::tablefields(QString &tble, QString &header, QStringList &out)
{
    QStringList fl = QStringList() << "table" << "tablefields" << "links";
    QStringList cmpfl = QStringList() << "tablename" << "header";
    QStringList cmpvl = QStringList() << tble << header;
    out = sqlc.GetValuesFromTableByFields("sup", "tablefields", fl, cmpfl, cmpvl);
    if ((sqlc.result) || (out.isEmpty()))
    {
        TFWARN(sqlc.LastError);
        result = 1;
        out.clear();
        return;
    }
    result = 0;
}

void s_tablefields::tableheaders(QString &tble, QStringList &out)
{
    out = sqlc.GetValuesFromTableByColumnAndFields("sup", "tablefields", "header", QStringList("tablename"), QStringList(tble), "fieldsorder", true);
    if ((sqlc.result) || (out.isEmpty()))
    {
        TFWARN(sqlc.LastError);
        result = 1;
        out.clear();
        return;
    }
    result = 0;
}

void s_tablefields::tablelinks(QString &tble, QStringList &out)
{
    out = sqlc.GetValuesFromTableByColumnAndFields("sup", "tablefields", "links", QStringList("tablename"), QStringList(tble), "fieldsorder", true);
    if ((sqlc.result) || (out.isEmpty()))
    {
        TFWARN(sqlc.LastError);
        result = 1;
        out.clear();
        return;
    }
    result = 0;
}

bool s_tablefields::tableistree(QString &tble)
{
    QStringList tmpfl = QStringList() << "tablefields" << "tablename";
    QStringList tmpvl = QStringList() << "alias" << tble;
    sqlc.GetValueFromTableByFields("sup","tablefields","tablefields",tmpfl,tmpvl);
    if (sqlc.result)
        return false;
    else
        return true;
}

void s_tablefields::HeaderByFields(QString &tble, QString &header, QStringList &cmpfl, QStringList &cmpvl, QStringList &out)
{
    if ((cmpfl.size() != cmpvl.size()) || (cmpfl.size() == 0))
    {
        result = 1;
        TFDBG;
        out.clear();
        return;
    }
    QStringList realcmpfl, tmpsl;
    // взяли все реальные названия полей сравнения
    for (int i=0; i<cmpfl.size(); i++)
    {
        QString field = cmpfl.at(i);
        tablefields(tble, field, tmpsl);
        if (result)
        {
            TFWARN("");
            out.clear();
            return;
        }
        realcmpfl << tmpsl.at(1);
    }
    QStringList tmpsl2;
    tablefields(tble,header,tmpsl2);
    if (result)
    {
        TFWARN("");
        out.clear();
        return;
    }
    QString realheader = tmpsl2.at(1);
    QString cmpdb = tmpsl.at(0).split(".").at(0); // реальное имя БД
    QString cmptble = tmpsl.at(0).split(".").at(1); // реальное название таблицы
    out = sqlc.GetValuesFromTableByColumnAndFields(cmpdb,cmptble,realheader,realcmpfl,cmpvl);
    if (sqlc.result)
    {
        TFWARN(sqlc.LastError);
        result = 1;
        out.clear();
        return;
    }
    result = 0;
}
