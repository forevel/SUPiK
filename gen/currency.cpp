#include <QRegExp>
#include <QEventLoop>
#include "currency.h"
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
    (this->*RateFunctions()[funcnum])();
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
        QRegExp pattern("<div id=currency_converter_result>1 "+curstr+" = <span class=bld>(.*) RUB</span>");
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
}

