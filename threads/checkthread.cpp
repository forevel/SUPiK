#include "checkthread.h"

//#include <QtDebug>
#include <QCoreApplication>
#include <QColor>
#include <QDate>
#include <QDateTime>
#include <QTimer>
#include <QThread>
#include "../gen/s_sql.h"

CheckThread::CheckThread(QObject *parent) : QObject(parent)
{
    NewNotifyHasArrived = false;
    Acknowledged = false;
    UpdateInProgress = false;
    IsAboutToFinish = false;
}

CheckThread::~CheckThread()
{
}

void CheckThread::Start()
{
    QTimer *tmr = new QTimer(this);
    tmr->setInterval(60000); // 1 минута
    connect(tmr,SIGNAL(timeout()),this,SLOT(MinuteTest()));
    CTHREADINFO("Проверка целостности БД начата");
    MainCheck();
    CTHREADINFO("Проверка целостности БД закончена");
    tmr->start();
    while (!IsAboutToFinish)
        Wait(100);
    emit finished();
}

// проверка целостности основных баз данных

void CheckThread::MainCheck()
{
    QStringList fields, values, tmpprob;
    QString tmpString;
    int i, j, res;
    // 1. проверим таблицы на пустые поля (alias, idalias, <idtble>, <tble>, idpers, date, deleted) и вообще на их наличие
    QStringList databases, tables;
    databases = pc.db.keys();
    PublicClass::ProblemStruct vl;
    if (pc.access & ACC_SYS_WR)
    {
        vl.ProblemType = PublicClass::PT_SYS;
        for (i = 0; i < databases.size(); i++)
        {
            if (databases.at(i) != "alt") // БД Altium построена по другому принципу
            {
                tables = sqlc.GetTablesFromDB(pc.db[databases.at(i)]);
                for (j = 0; j < tables.size(); j++)
                {
                    Wait(50);
                    vl.ProblemTable = databases.at(i) + "." + tables.at(j);
                    vl.ProblemSubType = PublicClass::PST_FIELDMISSED;
                    values = sqlc.GetColumnsFromTable(pc.db[databases.at(i)], tables.at(j));
                    if (values.indexOf("idpers") == -1)
                    {
                        vl.ProblemField = "idpers";
                        AddProblemToList(vl);
                    }
                    if (values.indexOf("deleted") == -1)
                    {
                        vl.ProblemField = "deleted";
                        AddProblemToList(vl);
                    }
                    if (values.indexOf("date") == -1)
                    {
                        vl.ProblemField = "date";
                        AddProblemToList(vl);
                    }
                    if (values.indexOf(tables.at(j)) != -1) // есть поле <tble>
                        fields << tables.at(j);
                    else
                    {
                        vl.ProblemField = tables.at(j);
                        AddProblemToList(vl);
                    }
                    if (values.indexOf("idalias") != -1)
                        fields << "idalias";
                    while (!fields.isEmpty())
                    {
                        Wait(50);
                        QString tmps = fields.takeFirst();
                        res = sqlc.CheckDBForEmptyFields(pc.db[databases.at(i)], tables.at(j), tmps, tmpprob);
                        if (res)
                        {
                            vl.ProblemSubType = PublicClass::PST_FIELDEMPTY;
                            while (!tmpprob.isEmpty())
                            {
                                vl.ProblemField = tmps;
                                vl.ProblemId = tmpprob.takeFirst();
                                AddProblemToList(vl);
                            }
                        }
                    }
                }
            }
        }
        // 2. проверим для каждой таблицы в tablefields поля со ссылками - есть ли такие элементы, на которые они ссылаются

        // 4. проверим справочники на правильность ссылок (наличие таких элементов в таблице, на которую дана ссылка)
    // 5. проверим справочники на наличие дублирующихся элементов (по основному полю - alias либо <tble>)
    // 6. проверим правильность полей date (дата не позднее текущей)
        // 7. проверим каждый элемент в каждой таблице (кроме symbols) в Altium на соответствие полей: NominalValue должно быть равно Nominal+Unit
        // 8. проверим все таблицы на предмет пустых строк (все поля пустые, исключая из проверки id<tble>,idpers,date,deleted)
    } // конец проверки системных проблем
    if (pc.access & ACC_WH_WR)
    {
        // проверим наличие элементов номенклатуры, у которых не задана группа
        // проверим наличие у каждого элемента в БД Altium, Schemagee и т.д. однозначного соответствия элементу в справочнике номенклатуры (кроме symbols)
    }
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

void CheckThread::AddProblemToList(PublicClass::ProblemStruct prob)
{
    pc.EPLMutex.lock();
    if (prob.ProblemType == PublicClass::PT_ALL)
        pc.ExchangeProblemsList.append(prob);
    else if ((pc.access & ACC_SYS_RO) && (prob.ProblemType == PublicClass::PT_SYS))
        pc.ExchangeProblemsList.append(prob);
    else if ((pc.access & ACC_WH_RO) && (prob.ProblemType == PublicClass::PT_WH))
        pc.ExchangeProblemsList.append(prob);
    else if ((pc.access & ACC_TB_RO) && (prob.ProblemType == PublicClass::PT_TB))
        pc.ExchangeProblemsList.append(prob);
    else if ((pc.access & ACC_SADM_RO) && (prob.ProblemType == PublicClass::PT_SADM))
        pc.ExchangeProblemsList.append(prob);
    else if ((pc.access & ACC_DOC_RO) && (prob.ProblemType == PublicClass::PT_DOC))
        pc.ExchangeProblemsList.append(prob);
    else if ((pc.access & ACC_ALT_RO) && (prob.ProblemType == PublicClass::PT_ALT))
        pc.ExchangeProblemsList.append(prob);
//    qDebug() << QString::number(pc.ExchangeProblemsList.size());
    pc.EPLMutex.unlock();
}

void CheckThread::Finish()
{
    IsAboutToFinish = true;
}

void CheckThread::Wait(int msec)
{
    this->thread()->msleep(msec);
/*    QTime tme;
    tme.start();
    while (tme.elapsed() < msec) */
        qApp->processEvents(QEventLoop::AllEvents);
}
