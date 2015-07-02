#include "wh_func.h"
#include "s_sql.h"

wh_func::wh_func()
{
}

void wh_func::HowManyComponentsDoesWeHave(int idnk)
{
    Q_UNUSED(idnk);
    db = sqlc.getdb("ent");
    tble = "flow";

}

void wh_func::HowManyComponentsDoesWeHave(QString nk)
{

}
