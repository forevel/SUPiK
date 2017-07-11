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

    enum Currencies
    {
        EUR,
        USD,
        CHF,
        GBP,
        UAH,
        BYN,
        RUB
    };

    enum RateFuncs
    {
        SBRF,
        EURCB,
        GOOGLE
    };

    const QMap<int, QString> Curs()
    {
        QMap<int, QString> sl;
        sl.insert(USD, "USD");
        sl.insert(EUR, "EUR");
        sl.insert(CHF, "CHF");
        sl.insert(GBP, "GBP");
        sl.insert(UAH, "UAH");
        sl.insert(BYN, "BYN");
        sl.insert(RUB, "RUB");
        return sl;
    }

    void GetRates(int funcnum);
    void SetBaseCurrency(int Curr);
    QString GetBaseCurrency();
    QList<double> Rates;

private:
    QMap<int, void (Currency::*)()> RateFunctions()
    {
        QMap<int, void (Currency::*)()> sl;
        sl[SBRF] = &Currency::GetSBRF;
        sl[EURCB] = &Currency::GetEURCB;
        sl[GOOGLE] = &Currency::GetGoogle;
        return sl;
    }
    QString BaseCurrency;

    void GetSBRF();
    void GetEURCB();
    void GetGoogle();
};

#endif // CURRENCY_H
