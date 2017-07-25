#include <QRegExp>
#include <QFileInfo>
#include <QCoreApplication>
#include <QEventLoop>
#include "currency.h"
#include "s_tablefields.h"
#include "publicclass.h"
#include "files.h"
#include "http.h"

Currency::Currency(QObject *parent) : QObject(parent)
{
}

int Currency::Init()
{
    Curry curr;
    QStringList IDs, Currs;
    tfl.GetValuesByColumn("Валюты_полн", "ИД", IDs);
    if (tfl.result)
    {
        WARNMSG("Ошибка получения значений по справочнику Валюты");
        return;
    }
    tfl.GetValuesByColumn("Валюты_полн", "Валюты", Currs);
    if (tfl.result)
    {
        WARNMSG("Ошибка получения значений по справочнику Валюты");
        return;
    }
    for (int i=0; i<IDs.size(); ++i)
    {
        if (i < Currs.size())
        {
            curr.Id = IDs.at(i);
            curr.Name = Currs.at(i);
            curr.RateToRUB = 0;
            Curs.append(curr);
        }
    }
    CurrNum = Curs.size();
}

void Currency::GetRates(int funcnum)
{
    try
    {
        // проверить наличие курсов валют на текущую дату в БД
        QList<QStringList> lsl;
        QStringList fl = QStringList() << "Валюта" << "Значение";
        QString Date = pc.DateTime.split(" ").at(0)+"*"; // поиск значения, которое начинается на дату
        tfl.SearchRLike("Валюты движения_полн", fl, "Дата", Date, lsl);
        if ((tfl.result != TFRESULT_NOERROR) || (lsl.isEmpty()))
            // вызвать соответствующую функцию получения данных по валютам
            (this->*RateFunctions()[funcnum])();
        else
        {
            while (!lsl.isEmpty())
            {
                QStringList sl = lsl.takeAt(0);
                for (int i=0; i<Curs.size(); ++i)
                {
                    if (Curs.at(i).Name == sl.at(0))
                    {
                        Curs[i].RateToRUB = sl.at(1).toDouble();
                        break;
                    }
                }
            }
        }
    }
    catch (...)
    {
        ERMSG("Exception caught: GetRates");
    }
}

void Currency::SetBaseCurrency(QString basecurr)
{
    for (int i=0; i<Curs.size(); ++i)
    {
        if (Curs.at(i).Name == basecurr)
        {
            BaseCurrency = i;
            return;
        }
    }
}

QString Currency::RateText(int curridx)
{
    if (curridx >= Curs.size())
        return QString();
    double rate = Curs.at(curridx).RateToRUB;
    double baserate = Curs.at(BaseCurrency).RateToRUB;
    QString tmps = "1 " + Curs.at(curridx).Name + " = " + \
            QString::number((rate/baserate),'f', 3) + " " + Curs.at(BaseCurrency).Name;
    return tmps;
}

void Currency::GetSBRF()
{

}

void Currency::GetEURCB()
{

}

void Currency::GetGoogle()
{
    QString QueryPrefix = "https://www.google.com/finance/converter?a=1&from=";
    QString QuerySuffix = "&to=RUB";
    ThrCounter = 0;
    for (int i=0; i<Curs.size(); ++i)
    {
        Http *http = new Http;
        connect(http,SIGNAL(Finished(int, QString)),this,SLOT(SetRate(int,QString)));
        connect(http,SIGNAL(Error(int,int)),this,SLOT(SetError(int,int)));
        QString curstr = Curs.at(i).Name;
        if (curstr == "RUB")
        {
            Curs[i].RateToRUB = 1.0; // курс валюты сам к себе равен единице
            continue;
        }
        QString Query = QueryPrefix + curstr + QuerySuffix;
        ++ThrCounter;
        http->DownloadFile(i, Query);
    }
    while (ThrCounter > 0)
    {
        QTime tme;
        tme.start();
        while (tme.elapsed() < TIME_GENERAL)
            QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
}

void Currency::SaveRate(int rateidx)
{
    QString newid, curr;
    curr = Curs.at(rateidx).Id;
    tfl.Insert("Валюты движения_полн", newid);
    if (tfl.result != RESULTOK)
    {
        WARNMSG("Не удалось создать новую запись в [Валюты движения]");
        return;
    }
    QStringList fl = QStringList() << "ИД" << "Валюта" << "Значение";
    QStringList vl = QStringList() << newid << curr << QString::number(Curs.at(rateidx).RateToRUB);
    tfl.Update("Валюты движения_полн", fl, vl);
    if (tfl.result != RESULTOK)
        WARNMSG("Не удалось записать движение валют по ИД="+newid);
}

void Currency::SetRate(int thrnum, QString fname)
{
    if (thrnum < Curs.size())
    {
        QRegExp pattern("<div id=currency_converter_result>1 "+Curs.at(thrnum).Name+ \
                        " = <span class=bld>(.*) RUB</span>");
        Files file;
        QFile *fp = file.openFileForRead(fname);
        if (fp == Q_NULLPTR)
        {
            --ThrCounter;
            return;
        }
        QString ba = fp->readAll();
        fp->remove();
        int pos = pattern.indexIn(ba);
        if (pos > -1)
        {
            QString valuestr = pattern.cap(1);
            bool ok;
            double value = valuestr.toDouble(&ok);
            if (ok)
                Curs[thrnum].RateToRUB = value;
            SaveRate(thrnum);
        }
        else
            SetError(thrnum, Http::NOTFOUND);
        --ThrCounter;
    }
}

void Currency::SetError(int thrnum, int errnum)
{
    ERMSG("Ошибка "+QString::number(errnum)+" в HTTP-потоке "+QString::number(thrnum));
}
