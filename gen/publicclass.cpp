#include "publicclass.h"

#include <QVariant>
#include <QColor>
#include <QString>
#include <QSettings>
#include <QDate>
#include <QDateTime>
#include "s_sql.h"

PublicClass pc;

static const QMap<QChar, QString> RUSLAT = PublicClass::ruslat();

PublicClass::PublicClass()
{
    LandP = new QSettings ("EvelSoft","Supik");
    TabColors[TW_PROB] = QColor(153, 153, 153); // GENERAL
    TabColors[TW_SYSBU] = TabColors[TW_SYSRS] = TabColors[TW_SYSST] = TabColors[TW_SYSDIR] = TabColors[TW_SYSIC] = QColor(194, 194, 194); // SYSTEM
    TabColors[TW_SET] = QColor(255, 204, 204); // SETTINGS
    TabColors[TW_COMP] = QColor(153, 204, 153); // COMPONENTS
    TabColors[TW_DIR] = QColor(255, 255, 153); // DIRECTORY
    TabColors[TW_ADM] = QColor(102, 102, 153); // ADMIN
    TabColors[TW_WH] = QColor(204, 204, 51); // WAREHOUSE
    TabColors[TW_DEV] = QColor(204, 204, 255); // DEVICES
}

PublicClass::~PublicClass()
{
    delete LandP;
}

void PublicClass::InitiatePublicClass()
{
    colors[0] = Qt::black;
    colors[1] = Qt::red;
    colors[2] = Qt::blue;
    colors[3] = Qt::darkRed;
    colors[4] = Qt::darkGreen;
    colors[5] = Qt::gray;
    QFont fontB = QFont("MS Sans Serif", -1, QFont::Bold);
    QFont fontN = QFont("MS Sans Serif", -1, QFont::Normal);
    fonts[0] = fonts[3] = fonts[5] = fontN;
    fonts[1] = fonts[2] = fonts[4] = fontB;
    icons[0] = QIcon(":/res/hr.png");
    icons[1] = QIcon(":/res/ok.png");
    icons[2] = QIcon(":/res/cross.png");
    icons[3] = QIcon(":/res/calend.png");
    icons[4] = QIcon(":/res/refresh.png");
    icons[5] = QIcon(":/res/TN.png");
//    Date = QDate::currentDate().toString("dd/MM/yyyy");
    DateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    SQLPath = LandP->value("settings/SQLPath","localhost").toString();
    PathToLibs = LandP->value("settings/pathtolibs","").toString();
    PathToSup = LandP->value("settings/pathtosup","").toString();
    timerperiod = LandP->value("settings/timerperiod","1").toInt();
    FtpServer = LandP->value("settings/FtpServer","ftp.asu-vei.ru").toString();
    openBD(alt, "ALT", "altium", "supik", "sysupik");
    openBD(con, "CON", "constructives", "supik", "sysupik");
    openBD(dev, "DEV", "devices", "supik", "sysupik");
    openBD(ent, "ENT", "enterprise", "supik", "sysupik");
    openBD(sch, "SCH", "schemagee", "supik", "sysupik");
    openBD(sol, "SOL", "solidworks", "supik", "sysupik");
    openBD(sup, "SUP", "supik", "supik", "sysupik");
    db["alt"]=alt;
    db["sup"]=sup;
    db["ent"]=ent;
    db["dev"]=dev;
    db["sol"]=sol;
    db["sch"]=sch;
    db["con"]=con;
    symfind = "LIBREFERENCE=";
    footfind = "PATTERN=";
    idRecord = -1;
}

// открытие БД

void PublicClass::openBD(QSqlDatabase &db, QString dbid, QString dbname, QString login, QString psw)
{
    if (!db.isOpen())
    {
        db = QSqlDatabase::addDatabase("QMYSQL", dbid);
        db.setHostName(SQLPath);
        db.setDatabaseName(dbname);
        db.setUserName(login);
        db.setPassword(psw);
        db.setPort(3306);
    }
}

QString PublicClass::getTranslit(QString str)
{
    int i;
    QString newstr = "";
    for (i = 0; i < str.size(); i++)
    {
        chr = str.at(i);
        if (RUSLAT.contains(chr))
            newstr += RUSLAT.value(chr);
        else
            newstr += chr;
//        QString tmpString = RUSLAT.value('е');
//        chr = RUSLAT.key("E");
    }
    return newstr;
}

PublicClass::FieldFormat PublicClass::getFFfromLinks(QString links) const
{
    QStringList tmpsl = links.split(".");
    FieldFormat ff;
    ff.ftype = 8;
    ff.delegate = FD_SIMGRID;
    ff.dependson = -1;
    ff.link.clear();
    if (!tmpsl.size())
        return ff;
    ff.delegate = tmpsl.at(0).toInt();
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    ff.ftype = tmpsl.at(0).toInt();
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    bool ok;
    ff.dependson = tmpsl.at(0).toInt(&ok, 10);
    if (!ok)
        ff.dependson = -1;
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return ff;
    for (int i = 0; i < tmpsl.size(); i++)
        ff.link << tmpsl.at(i);
    return ff;
}

QString PublicClass::getlinksfromFF(PublicClass::FieldFormat ff)
{
    QString tmpString = QString::number(ff.delegate,10)+"."+QString::number(ff.ftype,10)+".";
    if (ff.dependson != -1)
        tmpString += QString::number(ff.dependson,10);
    int i = 0;
    while (i < ff.link.size())
    {
        tmpString += "."+ff.link.at(i);
        ff.link.removeAt(0);
    }
    return tmpString;
}

void PublicClass::AddErrMsg(ermsgtype msgtype, quint64 ernum, quint64 ersubnum, QString msg)
{
    if (ermsgpool.size()>=ER_BUFMAX)
        ermsgpool.removeFirst();
    ermsg tmpm;
    tmpm.DateTime = DateTime;
    tmpm.type = msgtype;
    if (ernum < ermsgs().size())
        tmpm.module = ermsgs().value(ernum).toUtf8();
    else
        tmpm.module = "Неизвестно";
    tmpm.line = ersubnum;
    tmpm.msg = msg;
    ermsgpool.append(tmpm);
}

QString PublicClass::ConvertId(bool ColumnZero, QString Id)
{
    while (Id.at(0) == 0xFFFF)
        Id.remove(0, 1);
    if (ColumnZero) // в нулевом столбце всегда ИД элемента с нулями в начале, надо незначащие нули убрать
    {
        QStringList tmpsl = Id.split(".");
        if (tmpsl.size() > 1)
            Id = tmpsl.at(1);
        Id = QString::number(Id.toInt(0));
    }
    return Id;
}
