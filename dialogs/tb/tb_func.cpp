#include "tb_func.h"
#include <QDateTime>
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"

TBFunc::TBFunc()
{

}

int TBFunc::CheckDate(int type, const QString &date)
{
    QDateTime dtme = QDateTime::fromString(date, "dd-MM-yyyy");
    return Check(type, dtme);
}

int TBFunc::CheckDateTime(int type, const QString &datetime)
{
    QDateTime dtme = QDateTime::fromString(datetime, "dd-MM-yyyy hh:mm:ss");
    return Check(type, dtme);
}

int TBFunc::Check(int type, const QDateTime &dtm)
{
    if (!dtm.isValid())
        return TBDATE_BAD;
    QDateTime CurDateTime = QDateTime::currentDateTime();
    QString periodstr, table, field;
    QStringList fl, vl;
    switch (type)
    {
    case DT_MED:
        periodstr = "int-po";
        break;
    case DT_OT:
        periodstr = "int-ot";
        break;
    case DT_PB:
        periodstr = "int-pb";
        break;
    case DT_TB:
        periodstr = "int-eb";
        break;
    default:
        return TBDATE_BAD;
    }
    table = "Настройки_ТБ_полн";
    field = "Обозначение";
    fl << "Значение";
    tfl.valuesbyfield(table, fl, field, periodstr, vl); // в vl - значение в месяцах периода проверок
    if ((tfl.result != TFRESULT_NOERROR) || (vl.isEmpty()))
    {
        WARNMSG("");
        return TBDATE_BAD;
    }
    bool ok;
    int months = vl.at(0).toInt(&ok);
    if (!ok)
    {
        WARNMSG("");
        return TBDATE_BAD;
    }
    QDateTime newdtm = dtm.addMonths(months);
    qint64 SubDays = newdtm.daysTo(CurDateTime);
    if (SubDays < -DAYS_TO_BAD) // две недели до конца
        return TBDATE_OK;
    if (SubDays <= 0)
        return TBDATE_WARN;
    else
        return TBDATE_BAD;
}
