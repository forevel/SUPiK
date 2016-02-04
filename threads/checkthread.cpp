#include "checkthread.h"

#include <QColor>
#include <QDate>
#include <QDateTime>
#include <QTimer>
#include "../gen/s_sql.h"

CheckThread::CheckThread(QObject *parent) : QObject(parent)
{
    NewNotifyHasArrived = false;
    Acknowledged = false;
    UpdateInProgress = false;
}

CheckThread::~CheckThread()
{
}

void CheckThread::Start()
{
    QTimer *tmr = new QTimer(this);
    tmr->setInterval(60000); // 1 минута
    connect(tmr,SIGNAL(timeout()),this,SLOT(MinuteTest()));
    MainCheck();
    tmr->start();
}

// проверка целостности основных баз данных

void CheckThread::MainCheck()
{
/*    sysMessages.clear();
    whMessages.clear();
    altMessages.clear();
    giMessages.clear();
    sadmMessages.clear();
    QStringList fields, values, tmpprob;
    QString tmpString;
    int i, j, res;
    // 1. проверим справочники на пустые поля (alias, idalias, <idtble>, <tble>, idpers, date, deleted)
    QStringList databases, tables;
    databases = db.keys();
    if (access & ACC_SYS_WR)
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
        // 3. проверим системные справочники (personel, syslist, dirlist на корректность данных и заполненность полей)
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
    // 4. проверим справочники на правильность ссылок (наличие таких элементов в таблице, на которую дана ссылка)
    // 5. проверим справочники на наличие дублирующихся элементов (по основному полю - alias либо <tble>)
    // 6. проверим правильность полей date (дата не позднее текущей)
        // 7. проверим каждый элемент в каждой таблице (кроме symbols) в Altium на соответствие полей: NominalValue должно быть равно Nominal+Unit
        // 8. проверим все таблицы на предмет пустых строк (все поля пустые, исключая из проверки id<tble>,idpers,date,deleted)
    }
    if (access & ACC_WH_WR)
    {
        // проверим наличие элементов номенклатуры, у которых не задана группа
        // проверим наличие у каждого элемента в БД Altium, Schemagee и т.д. однозначного соответствия элементу в справочнике номенклатуры (кроме symbols)
    } */
    // здесь следует проверка на просроченность экзаменов по ТБ для sys|gi
    // далее следует проверка на приближение сроков окончания регистрации доменов, антивирусных лицензий для sadm
    // далее следует проверка на наличие изделий, находящихся в ремонте (БД repair) для alt
    // далее следует проверка на наличие записей в базах altium, solidworks, schemagee, devices и constructives, которые не имеют записи в номенклатуре
    // далее следует проверка на наличие записей с одинаковым <tble>, но с разным id<tble> в одной таблице
}

// функция таймера
// каждые Х минут (задаётся в настройках Settings) проверять наличие новых записей в таблицах qnk и qaltium

void CheckThread::MinuteTest()
{
    // 1. проверим, не появилось ли новых элементов в карантине
/*    if (pc.access & ACC_ALT_WR)
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
    } */
    // 2. проверим, нет ли новых уведомлений об изменениях в каталогах проектов
    // 3. проверим, не назначено ли нашему idpers каких-то заданий
}
