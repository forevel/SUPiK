#include "publicclass.h"

#include <QVariant>
#include <QColor>
#include <QString>
#include <QSettings>
#include <QDate>
#include <QDateTime>
#include "s_sql.h"

PublicClass pc;

static const QMap<QChar, QString> RUSLAT = PublicClass::ruslat();

PublicClass::PublicClass()
{
    LandP = new QSettings ("EvelSoft","Supik");
    TabColors[TW_PROB] = QColor(153, 153, 153); // GENERAL
    TabColors[TW_SYSBU] = TabColors[TW_SYSRS] = TabColors[TW_SYSST] = TabColors[TW_SYSDIR] = QColor(194, 194, 194); // SYSTEM
    TabColors[TW_SET] = QColor(255, 204, 204); // SETTINGS
    TabColors[TW_COMP] = QColor(153, 204, 153); // COMPONENTS
    TabColors[TW_DIR] = QColor(255, 255, 153); // DIRECTORY
    TabColors[TW_ADM] = QColor(102, 102, 153); // ADMIN
    TabColors[TW_WH] = QColor(204, 204, 51); // WAREHOUSE
    TabColors[TW_DOC] = QColor(204, 204, 153); // DOCUMENTS

    NewNotifyHasArrived = false;
    Acknowledged = false;
    UpdateInProgress = false;
}

PublicClass::~PublicClass()
{
    delete LandP;
}

void PublicClass::InitiatePublicClass()
{
    colors[0] = Qt::black;
    colors[1] = Qt::red;
    colors[2] = Qt::blue;
    colors[3] = Qt::darkRed;
    colors[4] = Qt::darkGreen;
    colors[5] = Qt::gray;
    QFont fontB = QFont("MS Sans Serif", -1, QFont::Bold);
    QFont fontN = QFont("MS Sans Serif", -1, QFont::Normal);
    fonts[0] = fonts[3] = fonts[5] = fontN;
    fonts[1] = fonts[2] = fonts[4] = fontB;
    icons[0] = QIcon(":/res/hr.png");
    icons[1] = QIcon(":/res/ok.png");
    icons[2] = QIcon(":/res/cross.png");
    icons[3] = QIcon(":/res/calend.png");
    icons[4] = QIcon(":/res/refresh.png");
    icons[5] = QIcon(":/res/TN.png");
//    Date = QDate::currentDate().toString("dd/MM/yyyy");
    DateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    SQLPath = LandP->value("settings/SQLPath","localhost").toString();
    PathToLibs = LandP->value("settings/pathtolibs","////FSERVER//PCAD//Altium//Libs//").toString();
    PathToSup = LandP->value("settings/pathtosup","////NS//SUPiK").toString();
    timerperiod = LandP->value("settings/timerperiod","1").toInt();
    openBD(alt, "ALT", "altium", "supik", "sysupik");
    openBD(con, "CON", "constructives", "supik", "sysupik");
    openBD(dev, "DEV", "devices", "supik", "sysupik");
    openBD(ent, "ENT", "enterprise", "supik", "sysupik");
    openBD(sch, "SCH", "schemagee", "supik", "sysupik");
    openBD(sol, "SOL", "solidworks", "supik", "sysupik");
    openBD(sup, "SUP", "supik", "supik", "sysupik");
    db["alt"]=alt;
    db["sup"]=sup;
    db["ent"]=ent;
    db["dev"]=dev;
    db["sol"]=sol;
    db["sch"]=sch;
    db["con"]=con;
    symfind = "LIBREFERENCE=";
    footfind = "PATTERN=";
    idRecord = -1;
}

// открытие БД

void PublicClass::openBD(QSqlDatabase &db, QString dbid, QString dbname, QString login, QString psw)
{
    if (!db.isOpen())
    {
        db = QSqlDatabase::addDatabase("QMYSQL", dbid);
        db.setHostName(SQLPath);
        db.setDatabaseName(dbname);
        db.setUserName(login);
        db.setPassword(psw);
        db.setPort(3306);
    }
}

// проверка целостности основных баз данных

void PublicClass::DBCheck()
{
    sysMessages.clear();
    whMessages.clear();
    altMessages.clear();
    giMessages.clear();
    sadmMessages.clear();
    QStringList fields, values, tmpprob;
    QString tmpString;
    int i, j, res;
    // сначала проверим справочники на пустые поля (alias, idalias, <idtble>, <tble>, idpers, date, deleted)
    QStringList databases, tables;
    databases = db.keys();
    if (access & ACC_SYS_FULL)
    {
        for (i = 0; i < databases.size(); i++)
        {
            tables = sqlc.GetTablesFromDB(db[databases.at(i)]);
            for (j = 0; j < tables.size(); j++)
            {
                values = sqlc.GetColumnsFromTable(db[databases.at(i)], tables.at(j));
                if (sqlc.result) addmessage(sysMessages, "!Невозможно получить данные по столбцам таблицы " + tables.at(j));
                if (values.indexOf("idpers") == -1)
                    addmessage(sysMessages, "!Отсутствует поле idpers в таблице " + tables.at(j));
                if (values.indexOf("deleted") == -1)
                    addmessage(sysMessages, "!Отсутствует поле deleted в таблице " + tables.at(j));
                if (values.indexOf("date") == -1)
                    addmessage(sysMessages, "!Отсутствует поле date в таблице " + tables.at(j));
                fields.clear();
                if (values.indexOf(tables.at(j)) != -1) // есть поле <tble>
                {
                    fields << tables.at(j);
                    res = sqlc.CheckDBForEmptyFields(pc.sup, "syslist", fields, tmpprob);
                    if (res)
                    {
                        while (!tmpprob.isEmpty())
                        {
                            addmessage(sysMessages, "!Не заполнено поле " + tables.at(j) + " в таблице "+tables.at(j)+" по индексу " + tmpprob.at(0));
                            tmpprob.removeAt(0);
                        }
                    }
                }
                else if ((values.indexOf("idalias") != -1) && (values.indexOf("alias") != -1))
                {
                    fields << "alias" << "idalias";
                    res = sqlc.CheckDBForEmptyFields(pc.sup, "syslist", fields, tmpprob);
                    if (res)
                    {
                        while (!tmpprob.isEmpty())
                        {
                            addmessage(sysMessages, "!Не заполнено поле (id)alias в таблице "+tables.at(j)+" по индексу " + tmpprob.at(0));
                            tmpprob.removeAt(0);
                        }
                    }
                }
                else
                    addmessage(sysMessages, "!Отсутствует поле " + tables.at(j) + " или набор полей alias+idalias в таблице " + tables.at(j));
            }
        }
        // затем проверим наличие 1-й и 2-й строк во всех таблицах, кроме альтиумовских
        // затем проверим системные справочники (personel, syslist, dirlist на корректность данных и заполненность полей)
        QSqlQuery dirlist(pc.sup);
        dirlist.exec("SELECT `iddirlist`,`dirlist`,`pc` FROM `dirlist` WHERE `iddirlist`>2;");
        if (dirlist.isActive())
        {
            while (dirlist.next())
            {
                if ((dirlist.value(1).toString() != "") && (dirlist.value(2).toString()!=""))
                {
                    tmpString = dirlist.value(2).toString();
                    QSqlDatabase db = sqlc.GetDB(tmpString.mid(0,3));
                    if (db.isValid())
                    {
                        QSqlQuery tryopen(db);
                        tmpString = tmpString.right(tmpString.size()-4);
                        tryopen.exec("SELECT `"+tmpString+"` FROM `"+tmpString+"`;");
                        if (tryopen.isActive());
                        else
                        {
                            tryopen.exec("SELECT `alias`,`idalias` FROM `"+tmpString+"`;");
                            if (tryopen.isActive());
                            else
                                addmessage(sysMessages, "@В справочнике "+dirlist.value(1).toString()+", на который ссылается строка №" \
                                           +dirlist.value(0).toString()+" dirlist нет необходимых полей или справочник не существует");
                        }
                    }
                    else
                        addmessage(sysMessages, "@Некорректная ссылка в dirlist, строка №"+dirlist.value(0).toString());
                }
                else
                    addmessage(sysMessages, "@Пустые поля в dirlist, строка №"+dirlist.value(0).toString());
            }

        }
        else
            addmessage(sysMessages, "!Невозможно подключиться к БД dirlist");
    // затем проверим справочники на правильность ссылок (наличие таких элементов в таблице, на которую дана ссылка)
    // затем проверим справочники на наличие дублирующихся элементов (по основному полю - alias либо <tble>)
    // затем проверим правильность полей date (дата не позднее текущей)
    }
    if (access & ACC_WH_FULL)
    {
    // затем проверим наличие элементов номенклатуры, у которых не задана группа
    }
}

void PublicClass::addmessage(QStringList &sl, QString mes)
{
    NewNotifyHasArrived = true;
    Acknowledged = false;
//    if (sl.indexOf(mes) == -1)
        sl.append(mes);
}

// функция таймера
// каждые Х минут (задаётся в настройках Settings) проверять наличие новых записей в таблицах qnk и qaltium

void PublicClass::minutetest()
{
    if (pc.access & ACC_ALT_FULL)
    {
        if (notify & PR_Q); // если признак уже висит, то не надо его и менять
        else
        {
            QStringList vl = sqlc.GetValuesFromTableByColumn(sqlc.GetDB("ent"), "qnk", "idqnk","idqnk",true);
            if (!vl.isEmpty())
            {
                notify |= PR_Q;
                addmessage(altMessages, "Добавлены новые элементы в БД номенклатуры");
            }
            vl = sqlc.GetValuesFromTableByColumn(sqlc.GetDB("ent"), "qaltium", "idqaltium","idqaltium",true);
            if (!vl.isEmpty())
            {
                notify |= PR_Q;
                addmessage(altMessages, "Добавлены новые элементы в БД Altium");
            }
            vl = sqlc.GetValuesFromTableByColumn(sqlc.GetDB("ent"), "qschemagee", "idqschemagee","idschemagee",true);
            if (!vl.isEmpty())
            {
                notify |= PR_Q;
                addmessage(altMessages, "Добавлены новые элементы в БД Schemagee");
            }
        }
    }
    // затем проверим, нет ли новых уведомлений об изменениях в каталогах проектов
    // затем проверим, не назначено ли нашему idpers каких-то заданий
}

void PublicClass::fillallprob()
{
    pc.allprobs.clear();
    if (pc.access & ACC_SYS_RO)
        pc.allprobs.append(pc.sysMessages);
    if (pc.access & ACC_WH_RO)
        pc.allprobs.append(pc.whMessages);
    if (pc.access & ACC_ALT_RO)
        pc.allprobs.append(pc.altMessages);
    if (pc.access & ACC_TB_RO)
        pc.allprobs.append(pc.giMessages);
    if (pc.access & ACC_SADM_RO)
        pc.allprobs.append(pc.sadmMessages);
}

QString PublicClass::getTranslit(QString str)
{
    int i;
    QString newstr = "";
    for (i = 0; i < str.size(); i++)
    {
        chr = str.at(i);
        if (RUSLAT.contains(chr))
            newstr += RUSLAT.value(chr);
        else
            newstr += chr;
//        QString tmpString = RUSLAT.value('е');
//        chr = RUSLAT.key("E");
    }
    return newstr;
}

PublicClass::fieldformat PublicClass::getFFfromLinks(QString links) const
{
    QStringList tmpsl = links.split(".");
    fieldformat ff;
    ff.ftype = 8;
    ff.delegate = FD_SIMGRID;
    ff.dependson = -1;
    ff.link.clear();
    if (!tmpsl.size())
        return ff;
    ff.delegate = tmpsl.at(0).toInt();
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    ff.ftype = tmpsl.at(0).toInt();
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    bool ok;
    ff.dependson = tmpsl.at(0).toInt(&ok, 10);
    if (!ok)
        ff.dependson = -1;
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    for (int i = 0; i < tmpsl.size(); i++)
        ff.link << tmpsl.at(i);
    return ff;
}

QString PublicClass::getlinksfromFF(PublicClass::fieldformat ff)
{
    QString tmpString = QString::number(ff.delegate,10)+"."+QString::number(ff.ftype,10)+".";
    if (ff.dependson != -1)
        tmpString += QString::number(ff.dependson,10);
    int i = 0;
    while (i < ff.link.size())
    {
        tmpString += "."+ff.link.at(i);
        ff.link.removeAt(0);
    }
    return tmpString;
}

void PublicClass::AddErrMsg(ermsgtype msgtype, quint64 ernum, quint64 ersubnum, QString msg)
{
    QStringList filessl = QStringList() << "Супик" << "Компоненты" << "Добавление_справочника" << "Работа со складом" << "Редактор_складов" << \
                                           "Система" << "Справочники_гл" << "Комплексная_строка" << "Компоненты_гл" << "Редактор_системы" << \
                                           "Ред_системы_справочники" << "Диалог_дерево" << "Диалог_2_дерева" << "Диалог_2_столбца" <<
                                           "Модель_таблица" << "Модель_дерево" << "Таблицы" << "БД" << "Вход_в_систему";
    if (ermsgpool.size()>=ER_BUFMAX)
        ermsgpool.removeFirst();
    ermsg tmpm;
    tmpm.DateTime = DateTime;
    tmpm.type = msgtype;
    tmpm.ernum = ernum;
    tmpm.ersubnum = ersubnum;
    // Разбор кода ошибки
    QString prefix;
    if ((msg.isEmpty()) || (msg == " ")) // пробел выдаётся при пустом запросе в БД
    {
        switch (msgtype)
        {
        case ER_MSG: prefix = "Ошибка "; break;
        case WARN_MSG: prefix = "Проблема "; break;
        case INFO_MSG: prefix = "Инфо "; break;
        case DBG_MSG: prefix = "Отладка "; break;
        }

        msg = prefix+"в модуле " + filessl.at(ernum) + " строка " + QString::number(ersubnum);
    }
    tmpm.msg = msg;
    ermsgpool.append(tmpm);
}
