#include "publicclass.h"

#include <QVariant>
#include <QString>
#include <QSettings>
#include <QDate>
#include "s_sql.h"
#include "log.h"
#include "../dialogs/tb/tb_func.h"

PublicClass pc;
Log *PCLog;

static const QMap<QChar, QString> RUSLAT = PublicClass::ruslat();

PublicClass::PublicClass()
{
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
    ErNum = 1;
    TBFunc_Initialize();
}

PublicClass::~PublicClass()
{
    delete PCLog;
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
    DateTime = QDateTime::currentDateTime().toString(DATETIMEFORMAT);
    SQLPath = RegValue("settings/SQLPath","localhost");
    PathToLibs = RegValue("settings/pathtolibs","");
    PathToSup = RegValue("settings/pathtosup","");
    timerperiod = RegValue("settings/timerperiod","1").toInt();
    FtpServer = RegValue("settings/FtpServer","ftp.asu-vei.ru");
    SupikServer = RegValue("settings/Server","supik.mycompany.ru");
    SupikPort = RegValue("settings/Port","9687");
    DBMap.insert(DB_ALT, {QSqlDatabase(), "ALT", "altium", DBLOGIN, DBPSWD});
    DBMap.insert(DB_CON, {QSqlDatabase(), "CON", "constructives", DBLOGIN, DBPSWD});
    DBMap.insert(DB_DEV, {QSqlDatabase(), "DEV", "devices", DBLOGIN, DBPSWD});
    DBMap.insert(DB_ENT, {QSqlDatabase(), "ENT", "enterprise", DBLOGIN, DBPSWD});
    DBMap.insert(DB_SCH, {QSqlDatabase(), "SCH", "schemagee", DBLOGIN, DBPSWD});
    DBMap.insert(DB_SOL, {QSqlDatabase(), "SOL", "solidworks", DBLOGIN, DBPSWD});
    DBMap.insert(DB_SUP, {QSqlDatabase(), "SUP", "supik", DBLOGIN, DBPSWD});
    DBMap.insert(DB_TB, {QSqlDatabase(), "TB", "tb", DBLOGIN, DBPSWD});
    DBMap.insert(DB_SADM, {QSqlDatabase(), "SADM", "sysadm", DBLOGIN, DBPSWD});
    DBMap.insert(DB_OK, {QSqlDatabase(), "OK", "ok", DBLOGIN, DBPSWD});
    for (int i=0; i<DBMap.keys().size(); ++i)
    {
        DbConnections dbc = DBMap.value(DBMap.keys().at(i));
        dbc.db = dbs_array[i];
        DBMap[DBMap.keys().at(i)] = dbc;
    }
    symfind = "LIBREFERENCE=";
    footfind = "PATTERN=";
    idRecord = -1;
}

void PublicClass::StartLog()
{
    PCLog = new Log;
    PCLog->Init("sup.log");
}

// открытие БД

bool PublicClass::OpenAndCheckDBs()
{
    // подключать только те базы, к которым есть доступ у пользователя
    DbNotOpened = 0x0000;
    for (int i=0; i<DBMap.keys().size(); ++i)
        openBD(DBMap.keys().at(i));
    if (DbNotOpened != 0x0000)
        return false;
    return true;
}

bool PublicClass::openBD(int dbnum)
{
    if (DBMap.keys().contains(dbnum))
    {
        DbConnections db = DBMap.value(dbnum);
        if (!db.db.isOpen())
        {
            db.db = QSqlDatabase::addDatabase("QMYSQL", db.connname);
            db.db.setHostName(SQLPath);
            db.db.setDatabaseName(db.dbname);
            db.db.setUserName(db.user);
            db.db.setPassword(db.pswd);
            db.db.setPort(SQLPORT);
            db.db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=1"); // timeout 1s
            if (db.db.open())
                return true;
            else
                LastError = db.db.lastError().text();
        }
        else
            return true;
    }
    DbNotOpened |= dbnum;
    return false;
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

QString PublicClass::RegValue(const QString &key, const QString &defaultValue)
{
    QSettings LandP("EvelSoft","Supik");
    return LandP.value(key, defaultValue).toString();
}

void PublicClass::SetRegValue(const QString &key, const QString &value)
{
    QSettings LandP("EvelSoft", "Supik");
    LandP.setValue(key, value);
}
