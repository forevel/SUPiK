#include "tb_func.h"
#include <QMap>
//#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"

static QMap<int, TypesStruct> TypesMap;

void TBFunc_Initialize()
{
    TypesMap[DT_MED].months = TypesMap[DT_OT].months = TypesMap[DT_PB].months = TypesMap[DT_TB].months = 0;
    TypesMap[DT_MED].TypeString = "int-po";
    TypesMap[DT_OT].TypeString = "int-ot";
    TypesMap[DT_PB].TypeString = "int-pb";
    TypesMap[DT_TB].TypeString = "int-eb";
}

int TBFunc_CheckDate(int type, const QString &date)
{
    QDateTime dtme = QDateTime::fromString(date, "dd-MM-yyyy");
    return TBFunc_Check(type, dtme);
}

int TBFunc_CheckDateTime(int type, const QString &datetime)
{
    QDateTime dtme = QDateTime::fromString(datetime, "dd-MM-yyyy hh:mm:ss");
    return TBFunc_Check(type, dtme);
}

int TBFunc_Check(int type, const QDateTime &dtm)
{
    if (!dtm.isValid())
        return TBDATE_BAD;
    QDateTime CurDateTime = QDateTime::currentDateTime();
    QStringList fl, vl;
    if (TypesMap.find(type) == TypesMap.end())
        return TBDATE_BAD;
    int months = TypesMap[type].months;
    if (months == 0)
    {
        fl << "Значение";
        tfl.valuesbyfield("Настройки_ТБ_полн", fl, "Обозначение", TypesMap[type].TypeString, vl); // в vl - значение в месяцах периода проверок
        if ((tfl.result != TFRESULT_NOERROR) || (vl.isEmpty()))
        {
            WARNMSG("");
            return TBDATE_BAD;
        }
        bool ok;
        months = vl.at(0).toInt(&ok);
        if (!ok)
        {
            WARNMSG("");
            return TBDATE_BAD;
        }
        TypesMap[type].months = months;
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
