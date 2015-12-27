#ifndef PUBLICCLASS_H
#define PUBLICCLASS_H

#define SUPIK_VERS  "SUPiK v1.0"

#include <QObject>

QT_BEGIN_NAMESPACE
class QSettings;
class QSqlDatabase;
class QColor;
QT_END_NAMESPACE

#include <QMap>
#include <QDateTime>
#include <QStringList>
#include <QColor>
#include <QFont>
#include <QIcon>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "publiclang.h"

#define PROGVER    "1.0.292"

// Макросы для выдачи сообщений
#define ERMSG(...)     pc.AddErrMsg(PublicClass::ER_MSG,__VA_ARGS__)
#define DBGMSG(...)    pc.AddErrMsg(PublicClass::DBG_MSG,__VA_ARGS__)
#define INFOMSG(...)   pc.AddErrMsg(PublicClass::INFO_MSG,__VA_ARGS__)
#define WARNMSG(...)   pc.AddErrMsg(PublicClass::WARN_MSG,__VA_ARGS__)

// тип редактируемого компонента (для cmp_...)
#define CTYPE_ALT   1
#define CTYPE_SCH   2
#define CTYPE_SOL   3
#define CTYPE_CON   4
#define CTYPE_DEV   5

// Максимальный размер буфера ошибок
#define ER_BUFMAX   0x40 // 64 элемента

#define DB_ALT      0x00
#define DB_ENT      0x01
#define DB_SUP      0x02
#define DB_SOL      0x03
#define DB_DEV      0x04
#define DB_SCH      0x05
#define DB_CON      0x06

#define ACC_SYS_RO      0x0001
#define ACC_SYS_WR      0x0002
#define ACC_WH_RO       0x0004
#define ACC_WH_WR       0x0008
#define ACC_ALT_RO      0x0010
#define ACC_ALT_WR      0x0020
#define ACC_TB_RO       0x0040
#define ACC_TB_WR       0x0080
#define ACC_SADM_RO     0x0100
#define ACC_SADM_WR     0x0200
#define ACC_DOC_RO      0x0400
#define ACC_DOC_WR      0x0800
#define ACC_NUM         12

// определение проблем
#define PR_Q    0x01 // есть новые элементы на карантине
#define PR_NM   0x02 // есть новые сообщения
#define PR_CHK  0x04 // есть задания на проверку

#define INSERT  1
#define UPDATE  2

#define QTMS_MW_D 0 // номер первой строчки диалога ENTRANCE из языковых файлов
#define SSD_D 4 // номер первой строчки диалога SETTINGS из языковых файлов
#define SUPIK_D 11 // номер первой строчки главного окна из языковых файлов

// тип поля
#define FW_COUNT    20 // количество типов полей
#define FW_AUTONUM  0 // поле с автонумерацией в соответствии с номером строки
#define FW_NUMBER   1 // простое число
#define FW_LINK     2 // простая ссылка на столбец в таблице
#define FW_DLINK    3 // ссылка на две таблицы
#define FW_ALLINK   4 // ссылка на alias в таблице
#define FW_MAXLINK  5 // ссылка на значение в столбце таблицы с максимальным ИД
#define FW_MASKED   6 // форматированная строка
#define FW_EQUAT    7 // арифметическое выражение
#define FW_PLAIN    8 // простое поле, без изысков
#define FW_RIGHTS   9 // поле с бинарным отображением и организации делегата FD_CHOOSE с диалогом rights
#define FW_FNUMBER  10 // форматированное число для делегата-счётчика
#define FW_SPECIAL  11 // сложная ссылка. Подробнее см. "СУПиК РП"
#define FW_SMETHOD  12 // ссылка на метод СУПиКа
#define FW_2CD      13 // отображение диалога редактирования строки таблицы (2cdialog)
#define FW_LLINK    14 // отображение диалога конструктора ссылок
#define FW_FLINK    15 // отображение диалога выбора имени файла
#define FW_ILINK    16 // отображение диалога выбора имени каталога
#define FW_FLLINK   17 // ссылка на элемент внутри файла
#define FW_DATE     18 // отображение диалога выбора даты
#define FW_ID       19 // автодополнение незначащими нулями

// тип делегата
#define FD_SIMPLE   0 // простой делегат без рамки для надписей
#define FD_LINEEDIT 1 // простое поле ввода
#define FD_CHOOSE   2 // поле ввода с кнопкой выбора по ссылке
#define FD_COMBO    3 // выпадающий список
#define FD_DISABLED 4 // неактивное поле
#define FD_SPIN     5 // счётчик
#define FD_CHOOSE_X 6 // поле ввода с кнопкой выбора по ссылке с возможностью редактирования
#define FD_SIMGRID  7 // простой делегат с рамкой
#define FD_CHECK    8 // поле бинарного выбора
#define DELEGNUM    9 // количество делегатов

// тип арифм операции
#define OP_PLUS     1
#define OP_MINUS    2
#define OP_MULT     3
#define OP_DIV      4

class PublicClass
{
public:
    PublicClass();
    ~PublicClass();

// Коды ошибок
    enum errors
    {
        ER_SUPIK,    // supik
        ER_COMP,     // cmp_compdialog
        ER_CMPMAIN,  // cmp_maindialog
        ER_CMPNS,    // cmp_newsubsection
        ER_DIRADD,   // dir_adddialog
        ER_WH,       // wh_dialog
        ER_WHED,     // wh_editor
        ER_SYS,      // sys_systemdialog
        ER_DIRMAIN,  // dir_maindialog
        ER_CHWDG,    // s_tqchoosewidget
        ER_SYSMENU,  // sysmenueditor
        ER_SYSDIR,   // sysdireditor
        ER_2TDLG,    // 2tdialog
        ER_2CTDLG,   // 2ctdialog
        ER_2CDLG,    // 2cdialog
        ER_NCMODEL,  // s_ncmodel
        ER_NTMODEL,  // s_ntmodel
        ER_TFIELD,   // tablefields
        ER_SQL,      // s_sql
        ER_START,    // startwindow
        ER_DEVDOC,   // dev_docdialog
        ER_ACC,      // s_accessdialog
        ER_SFTP,     // sftp
        ER_SYSSET,   // sys_settingsdialog
        ER_SYSICT,   // sys_importclass_t
        ER_TMODEL    // treemodel
    };

    // диалоговые окна в СУПиКе
    enum supikdialogwindows
    {
        TW_SET,     // настройки (Settings)
        TW_SYSST,   // SysStructEdit - редактор структуры системы
        TW_SYSIC,   // SysImportClasses - импорт классификатора
        TW_SYSBU,   // резервное копирование
        TW_SYSRS,   // восстановление
        TW_SYSDIR,  // системные справочники
        TW_COMP,    // редактор компонентов
        TW_DIR,     // справочники
        TW_PROB,    // диалог событий
        TW_WH,      // склады
        TW_QUAR,    // работа с карантином
        TW_DEV,     // редактор документов
        TW_ADM      // сисадминистрирование
    };

    double timerperiod;
    QString SQLUser, SQLPsw;
    QSqlDatabase ent, alt, sup, con, sol, sch, dev;
    QMap<QString, QSqlDatabase> db;
//    QString Date; // Сегодняшняя дата
    QString DateTime; // Сегодняшнее время
    int idRecord; // номер (id) текущей редактируемой записи в справочнике
    int idPers;  // текущий идентификатор пользователя, вошедшего в систему
    QString Pers; // фамилия пользователя, вошедшего в систему
    int idGroup; // ИД группы пользователя, определяет права пользователя в системе (ent.groups.idgroup)
    QString PathToLibs; // текущий путь до библиотек Altium
    QString PathToSup; // текущий путь до рабочей директории СУПиКа
    QString FtpServer; // адрес ftp-сервера, используемого для архивов
    QString CurLang; // обозначение текущего языка системы
    QString SQLPath; // путь к SQL-серверу
    QSettings *LandP; // переменная для работы с настройками системы в реестре
    QByteArray data, symfind, footfind; // data - массив хранения считанных из файлов библиотек бинарных данных,
                                        // symfind и footfind - строки поиска в библиотеках "LIBREFERENCE=" и "PATTERN="
    QStringList sysMessages;
    QStringList whMessages;
    QStringList altMessages;
    QStringList giMessages;
    QStringList sadmMessages;
    QStringList allprobs;
    QStringList supikprocs;
//    bool probsdetected;
//    int probpos;
    int Mode; // Режим работы (Справочники/Компоненты/Склады/Движение)
    int Altium; // граница между списком справочников и списком компонентов (для maintree)
    quint32 access; // текущие права доступа. Зашифрованы тройками бит (с младшего до старшего: право на чтение/изменение/удаление) (начиная с младших):
                // Системные, Складские, Альтиумовские, ГлавноИнженерские, Сисадминские, Архивариусовские
    int notify; // активные уведомления (см. PR_-defines выше)
    bool NewNotifyHasArrived; // признак поступления новых с момента последнего опроса событий
    bool Acknowledged; // признак того, что сообщение о поступлении новых событий мы уже выдавали
    bool UpdateInProgress; // признак того, что список сообщений обновляется

    struct
    {
        QString MDescLE;
        QString MFileLE;
        QString MNameCB;
        QString MParLE;
    } CompModelData;

    mutable QChar chr;

    static QMap<QChar, QString> ruslat()
    {
        QMap<QChar, QString>map;
        map.insert(QChar(1040), "A");
        map.insert(QChar(1041), "B");
        map.insert(QChar(1042), "V");
        map.insert(QChar(1043), "G");
        map.insert(QChar(1044), "D");
        map.insert(QChar(1045), "E");
        map.insert(QChar(1025), "Yo");
        map.insert(QChar(1046), "Zh");
        map.insert(QChar(1047), "Z");
        map.insert(QChar(1048), "I");
        map.insert(QChar(1049), "Y");
        map.insert(QChar(1050), "K");
        map.insert(QChar(1051), "L");
        map.insert(QChar(1052), "M");
        map.insert(QChar(1053), "N");
        map.insert(QChar(1054), "O");
        map.insert(QChar(1055), "P");
        map.insert(QChar(1056), "R");
        map.insert(QChar(1057), "S");
        map.insert(QChar(1058), "T");
        map.insert(QChar(1059), "U");
        map.insert(QChar(1060), "F");
        map.insert(QChar(1061), "H");
        map.insert(QChar(1062), "C");
        map.insert(QChar(1063), "Ch");
        map.insert(QChar(1064), "Sh");
        map.insert(QChar(1065), "Shch");
        map.insert(QChar(1066), "Y");
        map.insert(QChar(1067), "Y");
        map.insert(QChar(1068), "Y");
        map.insert(QChar(1069), "E");
        map.insert(QChar(1070), "Yu");
        map.insert(QChar(1071), "Ya");
        map.insert(QChar(1072), "a");
        map.insert(QChar(1073), "b");
        map.insert(QChar(1074), "v");
        map.insert(QChar(1075), "g");
        map.insert(QChar(1076), "d");
        map.insert(QChar(1077), "e");
        map.insert(QChar(1105), "yo");
        map.insert(QChar(1078), "zh");
        map.insert(QChar(1079), "z");
        map.insert(QChar(1080), "i");
        map.insert(QChar(1081), "y");
        map.insert(QChar(1082), "k");
        map.insert(QChar(1083), "l");
        map.insert(QChar(1084), "m");
        map.insert(QChar(1085), "n");
        map.insert(QChar(1086), "o");
        map.insert(QChar(1087), "p");
        map.insert(QChar(1088), "r");
        map.insert(QChar(1089), "s");
        map.insert(QChar(1090), "t");
        map.insert(QChar(1091), "u");
        map.insert(QChar(1092), "f");
        map.insert(QChar(1093), "h");
        map.insert(QChar(1094), "c");
        map.insert(QChar(1095), "ch");
        map.insert(QChar(1096), "sh");
        map.insert(QChar(1097), "shch");
        map.insert(QChar(1098), "y");
        map.insert(QChar(1099), "y");
        map.insert(QChar(1100), "y");
        map.insert(QChar(1101), "e");
        map.insert(QChar(1102), "yu");
        map.insert(QChar(1103), "ya");
        return map;
    }

    int S_TabWidgetWidth;

    QMap <int, QColor> TabColors;
    QColor colors[6]; // определение набора цветов шрифта
    QFont fonts[6]; // определение набора шрифтов
    QIcon icons[6]; // определение набора иконок
    typedef struct
    {
        int ftype;
        int delegate;
        int dependson;
        QStringList link;
    } fieldformat;

    PublicClass::fieldformat getFFfromLinks (QString links) const;
    QString getlinksfromFF (PublicClass::fieldformat ff);
    void InitiatePublicClass();
    void DBCheck();
    void fillallprob();
    void minutetest();
    QString getTranslit(QString);

    enum ermsgtype
    {
        ER_MSG,
        WARN_MSG,
        INFO_MSG,
        DBG_MSG
    };

    struct ermsg
    {
        QString DateTime;
        ermsgtype type;
        QString module;
        quint64 line;
        QString msg;
    };
    QList<ermsg> ermsgpool;

    void AddErrMsg(ermsgtype msgtype, quint64 ernum, quint64 ersubnum, QString msg="");

private:

    void addmessage(QStringList &sl, QString mes);
    void openBD(QSqlDatabase &db, QString dbid, QString dbname, QString login, QString psw);
};

extern PublicClass pc;

#endif // PUBLICCLASS_H
