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

    void EURUSD();
    void EURRUB();
    void USDRUB();
    void CHFRUB();
    void GBPRUB();
    void UAHRUB();
    void GetRates(int funcnum);

private:
    QList<double> Rates;
    QMap<int, void (Currency::*)()> RateFunctions()
    {
        QMap<int, void (Currency::*)()> sl;
        sl[SBRF] = &Currency::GetSBRF;
        sl[EURCB] = &Currency::GetEURCB;
        sl[GOOGLE] = &Currency::GetGoogle;
        return sl;
    }


    void GetSBRF();
    void GetEURCB();
    void GetGoogle();
};

#endif // CURRENCY_H
