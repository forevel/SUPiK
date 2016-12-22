#include "publicclass.h"

#include <QVariant>
#include <QString>
#include <QSettings>
#include <QDate>
#include <QDir>
#include "s_sql.h"
#include "log.h"

PublicClass pc;
Log *PCLog;

static const QMap<QChar, QString> RUSLAT = PublicClass::ruslat();

PublicClass::PublicClass()
{
    HomeDir = QDir::homePath()+"/.supik/";
    LandP = new QSettings ("EvelSoft","Supik");
    TabColors[TW_PROB] = QColor(153, 153, 153); // GENERAL
    TabColors[TW_ERPROT] = TabColors[TW_SYSBU] = TabColors[TW_SYSRS] = TabColors[TW_SYSST] = TabColors[TW_SYSDIR] = TabColors[TW_SYSIC] = QColor(194, 194, 194); // SYSTEM
    TabColors[TW_SET] = QColor(255, 204, 204); // SETTINGS
    TabColors[TW_COMP] = QColor(153, 204, 153); // COMPONENTS
    TabColors[TW_DIR] = QColor(255, 255, 153); // DIRECTORY
    TabColors[TW_ADM] = QColor(102, 102, 153); // ADMIN
    TabColors[TW_WH] = QColor(204, 204, 51); // WAREHOUSE
    TabColors[TW_DEV] = QColor(204, 255, 255); // DEVICES
    TabColors[TW_TB] = QColor(204, 204, 255); // TB
    TabColors[TW_PERS] = QColor(102, 204, 255); // PERS
    AutonomousMode = true; // изначально неизвестно, доступен ли сервер, поэтому на всякий случай ставим признак автономности
    PCLog = new Log;
    PCLog->Init(HomeDir+"sup.log");
    ErNum = 1;
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
//    Date = QDate::currentDate().toString(DATEFORMAT);
    DateTime = QDateTime::currentDateTime().toString(DATETIMEFORMAT);
    SQLPath = LandP->value("settings/SQLPath","localhost").toString();
    PathToLibs = LandP->value("settings/pathtolibs","").toString();
    PathToSup = LandP->value("settings/pathtosup","").toString();
    timerperiod = LandP->value("settings/timerperiod","1").toInt();
    FtpServer = LandP->value("settings/FtpServer","ftp.asu-vei.ru").toString();
    SupikServer = LandP->value("settings/Server","supik.mycompany.ru").toString();
    SupikPort = LandP->value("settings/Port","9687").toString();


    symfind = "LIBREFERENCE=";
    footfind = "PATTERN=";
    idRecord = -1;
}

// открытие БД

bool PublicClass::OpenAndCheckDBs()
{
    // подключать только те базы, к которым есть доступ у пользователя
    DbNotOpened = 0x0000;
    openBD(alt, "ALT", "altium", "supik", "sysupik");
    alt.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=4");
    if (!alt.open())
    {
        DbNotOpened |= DB_ALT;
        LastError = alt.lastError().text();
    }
    openBD(con, "CON", "constructives", "supik", "sysupik");
    con.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=4");
    if (!con.open())
    {
        DbNotOpened |= DB_CON;
        LastError = alt.lastError().text();
    }
    openBD(dev, "DEV", "devices", "supik", "sysupik");
    dev.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=4");
    if (!dev.open())
    {
        DbNotOpened |= DB_DEV;
        LastError = alt.lastError().text();
    }
    openBD(ent, "ENT", "enterprise", "supik", "sysupik");
    ent.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=4");
    if (!ent.open())
    {
        DbNotOpened |= DB_ENT;
        LastError = alt.lastError().text();
    }
    openBD(sch, "SCH", "schemagee", "supik", "sysupik");
    sch.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=4");
    if (!sch.open())
    {
        DbNotOpened |= DB_SCH;
        LastError = alt.lastError().text();
    }
    openBD(sol, "SOL", "solidworks", "supik", "sysupik");
    sol.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=4");
    if (!sol.open())
    {
        DbNotOpened |= DB_SOL;
        LastError = alt.lastError().text();
    }
    openBD(sup, "SUP", "supik", "supik", "sysupik");
    sup.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=4");
    if (!sup.open())
    {
        DbNotOpened |= DB_SUP;
        LastError = alt.lastError().text();
    }
    openBD(tb, "TB", "tb", "supik", "sysupik");
    tb.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=4");
    if (!tb.open())
    {
        DbNotOpened |= DB_TB;
        LastError = alt.lastError().text();
    }
    openBD(sadm, "SADM", "sysadm", "supik", "sysupik");
    sadm.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=4");
    if (!sadm.open())
    {
        DbNotOpened |= DB_SADM;
        LastError = alt.lastError().text();
    }
    db["alt"]=alt;
    db["sup"]=sup;
    db["ent"]=ent;
    db["dev"]=dev;
    db["sol"]=sol;
    db["sch"]=sch;
    db["con"]=con;
    db["tb"]=tb;
    db["sadm"]=sadm;
    if (DbNotOpened != 0x0000)
        return false;
    return true;
}

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
//       db.setPort(3333); // временно для отладки с виртуальной машиной
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

void PublicClass::getFFfromLinks(const QString &links, FieldFormat &ff) const
{
    QStringList tmpsl = links.split(".");
    ff.ftype = 8;
    ff.delegate = FD_SIMGRID;
    ff.dependson = -1;
    ff.link.clear();
    if (!tmpsl.size())
        return;
    ff.delegate = tmpsl.at(0).toInt();
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return;
    ff.ftype = tmpsl.at(0).toInt();
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return;
    bool ok;
    ff.dependson = tmpsl.at(0).toInt(&ok, 10);
    if (!ok)
        ff.dependson = -1;
    tmpsl.removeAt(0);
    if (!tmpsl.size())
        return;
    for (int i = 0; i < tmpsl.size(); i++)
        ff.link << tmpsl.at(i);
}

void PublicClass::getlinksfromFF(PublicClass::FieldFormat &ff, QString &out)
{
    out = QString::number(ff.delegate,10)+"."+QString::number(ff.ftype,10)+".";
    if (ff.dependson != -1)
        out += QString::number(ff.dependson,10);
    int i = 0;
    while (i < ff.link.size())
    {
        out += "."+ff.link.at(i);
        ff.link.removeAt(0);
    }
}

void PublicClass::AddErrMsg(ermsgtype msgtype, QString file, int line, QString msg)
{
    if (ermsgpool.size()>=ER_BUFMAX)
        ermsgpool.removeFirst();
    ermsg tmpm;
    tmpm.DateTime = DateTime;
    tmpm.ErNum = ErNum;
    ErNum++;
    tmpm.type = msgtype;
/*    if (ernum < ermsgs().size())
        tmpm.module = ermsgs().value(ernum).toUtf8();
    else
        tmpm.module = "Неизвестно"; */
    tmpm.module = file;
    tmpm.line = QString::number(line);
    tmpm.msg = msg;

    // запись в log-файл
    switch (tmpm.type)
    {
    case ER_MSG:
    case DBG_MSG:
        PCLog->error(tmpm.module+"."+tmpm.line+": "+msg);
        break;
    case WARN_MSG:
        PCLog->warning(tmpm.module+"."+tmpm.line+": "+msg);
        break;
    case INFO_MSG:
        PCLog->info(tmpm.module+"."+tmpm.line+": "+msg);
        break;
    default:
        break;
    }
    ermsgpool.append(tmpm);
}

// remove leading zeros from id (if ColumnZero=0) and leading 0xFFFF's (tree indent)

void PublicClass::ConvertId(bool ColumnZero, QString &Id)
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
}
