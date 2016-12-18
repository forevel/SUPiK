#ifndef TB_FUNC_H
#define TB_FUNC_H

#include <QString>
#include <QDateTime>

#define TBDATE_OK   0
#define TBDATE_WARN 1
#define TBDATE_BAD  2

#define DAYS_TO_BAD 14

#define TB_NODATA   "N/A"

class TBFunc
{

public:

    enum DateTypes
    {
        DT_MED,
        DT_OT,
        DT_PB,
        DT_TB
    };

    TBFunc();

    static int CheckDate(int type, const QString &date);
    static int CheckDateTime(int type, const QString &datetime);

private:
    static int Check(int type, const QDateTime &dtm);
};

#endif // TB_FUNC_H
