#ifndef WH_FUNC_H
#define WH_FUNC_H

#include <QSqlDatabase>

class wh_func
{
public:
    wh_func();
    void HowManyComponentsDoesWeHave(QString nk);
    void HowManyComponentsDoesWeHave(int idnk);

private:
    QSqlDatabase db;
    QString tble;
};

#endif // WH_FUNC_H
