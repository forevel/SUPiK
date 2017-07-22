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

    enum RateFuncs
    {
        SBRF,
        EURCB,
        GOOGLE
    };

    void GetRates(int funcnum);
    void SetBaseCurrency(int Curr);
    QString GetBaseCurrency();

private:
    QMap<int, void (Currency::*)()> RateFunctions()
    {
        QMap<int, void (Currency::*)()> sl;
        sl[SBRF] = &Currency::GetSBRF;
        sl[EURCB] = &Currency::GetEURCB;
        sl[GOOGLE] = &Currency::GetGoogle;
        return sl;
    }

    int ThrCounter;
    void GetSBRF();
    void GetEURCB();
    void GetGoogle();
    void SaveRate(int rateidx);

private slots:
    void SetRate(int thrnum, QString fname);
    void SetError(int thrnum, int errnum);
};

#endif // CURRENCY_H
