#include "tb_func.h"
#include <QDateTime>
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"

TBFunc::TBFunc()
{

}

int TBFunc::CheckDate(int type, const QString &date)
{
    QDateTime dtme = QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss");
    if (!dtme.isValid())
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
    dtme = dtme.addMonths(months);
    qint64 SubDays = dtme.daysTo(CurDateTime);
    if (SubDays < -DAYS_TO_BAD) // две недели до конца
        return TBDATE_OK;
    if (SubDays <= 0)
        return TBDATE_WARN;
    else
        return TBDATE_BAD;
}
