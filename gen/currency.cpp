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
    for (int i=0; i<CURNUM; ++i)
        pc.Rates.append(0.0);
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
                if (sl.size() < 2)
                    continue;
                QString tmps;
                tfl.GetValueByField("Валюты_полн", "Валюта", "ИД", sl.at(0), tmps);
                if ((tfl.result != TFRESULT_NOERROR) || (tmps.isEmpty()))
                {
                    WARNMSG("");
                    return;
                }
                QList<int> tmpil = pc.Curs().keys(tmps);
                if (!tmpil.isEmpty())
                    pc.Rates.replace(tmpil.at(0), sl.at(1).toDouble());
            }
        }
    }
    catch (...)
    {
        ERMSG("Exception caught: GetRates");
    }
}

void Currency::SetBaseCurrency(int Curr)
{
    pc.BaseCurrency = pc.Curs()[Curr];
}

QString Currency::GetBaseCurrency()
{
    return pc.BaseCurrency;
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
    QString QuerySuffix = "&to="+pc.BaseCurrency;
    ThrCounter = 0;
    for (int i=0; i<pc.Curs().keys().size(); ++i)
    {
        Http *http = new Http;
        connect(http,SIGNAL(Finished(int, QString)),this,SLOT(SetRate(int,QString)));
        connect(http,SIGNAL(Error(int,int)),this,SLOT(SetError(int,int)));
        QString curstr = pc.Curs()[pc.Curs().keys().at(i)];
        if (curstr == pc.BaseCurrency)
        {
            pc.Rates.replace(pc.Curs().keys().at(i), 1.0); // курс валюты сам к себе равен единице
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
    QString tmps, newid, curr;
    curr = pc.Curs().value(pc.Curs().keys().at(rateidx));
    tfl.GetValueByField("Валюты_полн", "ИД", "Валюта", curr, tmps);
    if ((tfl.result != RESULTOK) || (tmps.isEmpty()))
    {
        WARNMSG("Ошибка получения данных по справочнику [Валюты], "+curr);
        return;
    }
    tfl.Insert("Валюты движения_полн", newid);
    if (tfl.result != RESULTOK)
    {
        WARNMSG("Не удалось создать новую запись в [Валюты движения]");
        return;
    }
    QStringList fl = QStringList() << "ИД" << "Валюта" << "Значение";
    QStringList vl = QStringList() << newid << curr << QString::number(pc.Rates.at(rateidx));
    tfl.Update("Валюты движения_полн", fl, vl);
    if (tfl.result != RESULTOK)
        WARNMSG("Не удалось записать движение валют по ИД="+newid);
}

void Currency::SetRate(int thrnum, QString fname)
{
    QRegExp pattern("<div id=currency_converter_result>1 "+pc.Curs()[thrnum]+ \
                    " = <span class=bld>(.*) "+pc.BaseCurrency+"</span>");
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
            pc.Rates.replace(pc.Curs().keys().at(thrnum), value);
        SaveRate(thrnum);
    }
    else
        SetError(thrnum, Http::NOTFOUND);
    --ThrCounter;
}

void Currency::SetError(int thrnum, int errnum)
{
    ERMSG("Ошибка "+QString::number(errnum)+" в HTTP-потоке "+QString::number(thrnum));
}
