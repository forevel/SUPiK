#ifndef CURRENCY_H
#define CURRENCY_H

#include <QObject>
#include "http.h"

#define CURNUM  7

class Currency : public QObject
{
    Q_OBJECT
public:
    Currency(QObject *parent = Q_NULLPTR);

/*    enum Currencies
    {
        EUR,
        USD,
        CHF,
        GBP,
        UAH,
        BYN,
        RUB
    }; */

    enum RateFuncs
    {
        SBRF,
        EURCB,
        GOOGLE
    };

    int CurrNum;

    int Init();
    void GetRates(int funcnum);
    void SetBaseCurrency(QString basecurr);
    QString GetBaseCurrency();
    QString RateText(int curridx);

private:
    QMap<int, void (Currency::*)()> RateFunctions()
    {
        QMap<int, void (Currency::*)()> sl;
        sl[SBRF] = &Currency::GetSBRF;
        sl[EURCB] = &Currency::GetEURCB;
        sl[GOOGLE] = &Currency::GetGoogle;
        return sl;
    }

    struct Curry
    {
        QString Id; // ИД деньги по таблице
        QString Name; // трёхбуквенное обозначение (RUB)
        double RateToRUB; // курс валюты по отношению к рублю
    };

    QList<Curry> Curs;

    int ThrCounter;
    int BaseCurrency; // индекс по списку Curs базовой валюты
    void GetSBRF();
    void GetEURCB();
    void GetGoogle();
    void SaveRate(int rateidx);

private slots:
    void SetRate(int thrnum, QString fname);
    void SetError(int thrnum, int errnum);
};

#endif // CURRENCY_H
