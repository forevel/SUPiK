#include <QRegExp>
#include <QEventLoop>
#include "currency.h"
#include "s_TableFields.h"
#include "publicclass.h"
#include "files.h"
#include "http.h"

Currency::Currency(QObject *parent) : QObject(parent)
{
    BaseCurrency = "RUB";
    for (int i=0; i<CURNUM; ++i)
        Rates.append(0.0);
}

void Currency::GetRates(int funcnum)
{
    // проверить наличие курсов валют на текущую дату в БД
    QList<QStringList> lsl;
/*    QStringList fl = QStringList() << "Валюта" << "Значение";
    QStringList cmpfl = QStringList() << "Дата";
    QStringList cmpvl = QStringList() << pc.DateTime; */
    QStringList fl = QStringList() << "ИД" << "Наименование" << "Обозначение" << "Ёмкость размещения" << "Ряд" << "Столбец" << "Рядов" << "Столбцов";
    QStringList cmpfields = QStringList() << "ИД_а";
    QStringList cmpvalues = QStringList() << QString::number(1);
    tfl.GetValuesByFieldsMatrix("Склады размещение_полн", fl, cmpfields, cmpvalues, lsl);
//    tfl.GetValuesByFieldsMatrix("Персонал_ТБ_полн", fl, cmpfl, cmpvl, lsl);
//    if ((tfl.result != TFRESULT_NOERROR) || (lsl.isEmpty()))
        // вызвать соответствующую функцию получения данных по валютам
//        (this->*RateFunctions()[funcnum])();
/*    else
    {
        while (!lsl.isEmpty())
        {
            QStringList sl = lsl.takeAt(0);
            if (sl.size() < 2)
                continue;
            QString tmps;
            tfl.GetValueByField("Валюты_полн", "Валюта", "ИД", sl.at(0), tmps);
            if ((tfl.result != TFRESULT_NOERROR) || (tmps.isEmpty()))
            {
                WARNMSG("");
                return;
            }
            QList<int> tmpil = Curs().keys(tmps);
            if (!tmpil.isEmpty())
                Rates.replace(tmpil.at(0), sl.at(1).toDouble());
        }
    }*/
    // сделать запись по курсам валют в таблицу "Валюты движения"
}

void Currency::SetBaseCurrency(int Curr)
{
    BaseCurrency = Curs()[Curr];
}

QString Currency::GetBaseCurrency()
{
    return BaseCurrency;
}

void Currency::GetSBRF()
{

}

void Currency::GetEURCB()
{

}

void Currency::GetGoogle()
{
    QEventLoop DownloadLoop;
    QString tmpfile = pc.HomeDir+"/tmpcur";
    QString QueryPrefix = "https://www.google.com/finance/converter?a=1&from=";
    QString QuerySuffix = "&to="+BaseCurrency;
    Http *http = new Http;
    connect(http,SIGNAL(Finished()),&DownloadLoop,SLOT(quit()));
    for (int i=0; i<Curs().keys().size(); ++i)
    {
        QString curstr = Curs()[Curs().keys().at(i)];
        if (curstr == BaseCurrency)
        {
            Rates.replace(Curs().keys().at(i), 1.0); // курс валюты сам к себе равен единице
            continue;
        }
        QString Query = QueryPrefix + curstr + QuerySuffix;
        http->downloadFile(Query, tmpfile);
        DownloadLoop.exec();
        QRegExp pattern("<div id=currency_converter_result>1 "+curstr+" = <span class=bld>(.*) "+BaseCurrency+"</span>");
        Files file;
        QFile *fp = file.openFileForRead(tmpfile);
        if (fp == Q_NULLPTR)
            return;
        QString ba = fp->readAll();
        int pos = pattern.indexIn(ba);
        if (pos > -1)
        {
            QString valuestr = pattern.cap(1);
            bool ok;
            double value = valuestr.toDouble(&ok);
            if (ok)
                Rates.replace(Curs().keys().at(i), value);
        }
    }
    SaveRates();
}

void Currency::SaveRates()
{
    QString tmps, newid, curr;
    for (int i=0; i<Rates.size(); ++i)
    {
        curr = Curs().value(Curs().keys().at(i));
        tfl.GetValueByField("Валюты_полн", "ИД", "Валюта", curr, tmps);
        if ((tfl.result != RESULTOK) || (tmps.isEmpty()))
        {
            WARNMSG("Ошибка получения данных по справочнику [Валюты]");
            return;
        }
        tfl.Insert("Валюты движения_полн", newid);
        if (tfl.result != RESULTOK)
        {
            WARNMSG("Не удалось создать новую запись в [Валюты движения]");
            continue;
        }
        QStringList fl = QStringList() << "ИД" << "Валюта" << "Значение";
        QStringList vl = QStringList() << newid << curr << QString::number(Rates.at(i));
        tfl.Update("Валюты движения_полн", fl, vl);
        if (tfl.result != RESULTOK)
            WARNMSG("Не удалось записать движение валют по ИД="+newid);
    }
}

