#include "s_ermodel.h"
#include "../gen/publicclass.h"

s_ermodel::s_ermodel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

s_ermodel::~s_ermodel()
{

}

void s_ermodel::AddRow(msgtype type, QString text)
{
    if (msgpool.size() < MAX_MSG)
    {
        int pos = rowCount();
        insertRow(pos-1);
        msg tmpmsg;
        tmpmsg.DateTime = pc.DateTime;
        tmpmsg.type = type;
        tmpmsg.msg = text;
        msgpool.replace(pos,tmpmsg);
    }
}
