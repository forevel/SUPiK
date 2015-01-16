#include "../inc/wh_func.h"
#include "../inc/s_sql.h"

wh_func::wh_func()
{
}

void wh_func::HowManyComponentsDoesWeHave(int idnk)
{
    db = sqlc.getdb("ent");
    tble = "flow";

}

void wh_func::HowManyComponentsDoesWeHave(QString nk)
{

}
