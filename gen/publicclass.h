#ifndef PUBLICCLASS_H
#define PUBLICCLASS_H

#include <QObject>

#include <QMap>
#include <QDateTime>
#include <QStringList>
#include <QSettings>
#include <QColor>
#include <QFont>
#include <QIcon>
#include <QMutex>
#include <QPixmap>
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqlabel.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "publiclang.h"

// Макросы для выдачи сообщений

#define WARNMSG(a)  pc.AddErrMsg(PublicClass::ER_MSG, __FILE__, __LINE__, a)
#define DBGMSG      pc.AddErrMsg(PublicClass::DBG_MSG, __FILE__, __LINE__, "")
#define INFOMSG(a)  pc.AddErrMsg(PublicClass::INFO_MSG, __FILE__, __LINE__, a)
#define ERMSG(a)    pc.AddErrMsg(PublicClass::ER_MSG, __FILE__, __LINE__, a)

// тип редактируемого компонента (для cmp_...)
#define CTYPE_ALT   1
#define CTYPE_SCH   2
#define CTYPE_SOL   3
#define CTYPE_CON   4
#define CTYPE_DEV   5

// Максимальный размер буфера ошибок
#define ER_BUFMAX   0x40 // 64 элемента

#define DB_ALT      0x0001
#define DB_ENT      0x0002
#define DB_SUP      0x0004
#define DB_SOL      0x0008
#define DB_DEV      0x0010
#define DB_SCH      0x0020
#define DB_CON      0x0040
#define DB_TB       0x0080
#define DB_SADM     0x0100

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
#define ACC_ALL         0xFFFF

#define INSERT  1
#define UPDATE  2

#define QTMS_MW_D 0 // номер первой строчки диалога ENTRANCE из языковых файлов
#define SSD_D 4 // номер первой строчки диалога SETTINGS из языковых файлов
#define SUPIK_D 11 // номер первой строчки главного окна из языковых файлов

// тип поля
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
#define FW_BOOL     12 // булево значение
#define FW_2CD      13 // отображение диалога редактирования строки таблицы (2cdialog)
#define FW_LLINK    14 // отображение диалога конструктора ссылок
#define FW_FLINK    15 // отображение диалога выбора имени файла
#define FW_ILINK    16 // отображение диалога выбора имени каталога
#define FW_FLLINK   17 // ссылка на элемент внутри файла
#define FW_DATE     18 // отображение диалога выбора даты
#define FW_ID       19 // автодополнение незначащими нулями
#define FW_CRYPT    20 // значение с шифрованием
#define FW_COUNT    21 // количество типов полей

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

// типы возвращаемых значений
#define VS_STRING   0 // простая строка
#define VS_ICON     1 // значение в виде ссылки на иконку

// общие результаты
#define RESULTBAD   -1
#define RESULTOK    0

class PublicClass
{
public:
    PublicClass();
    ~PublicClass();

    // диалоговые окна в СУПиКе
    enum SupikDialogWindows
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
        TW_ERPROT,  // протокол ошибок
        TW_WH,      // склады
        TW_QUAR,    // работа с карантином
        TW_DEV,     // редактор документов
        TW_ADM,     // сисадминистрирование
        TW_TB,      // техника безопасности
        TW_PERS     // сотрудники
    };

    // определение проблем
    enum ProblemTypes
    {
        PT_SYS,
        PT_ALT,
        PT_WH,
        PT_SADM,
        PT_TB,
        PT_DOC,
        PT_ALL
    };

    static QMap<int, QColor> ProblemBackgroundColors()
    {
        QMap<int, QColor> map;
        map.insert(PT_SYS, QColor(194, 194, 194));
        map.insert(PT_ALT, QColor(153, 204, 153));
        map.insert(PT_WH, QColor(204, 204, 51));
        map.insert(PT_SADM, QColor(102, 102, 153));
        map.insert(PT_TB, QColor(255, 204, 204));
        map.insert(PT_DOC, QColor(255, 255, 255));
        map.insert(PT_ALL, QColor(153, 153, 153));
        return map;
    }

    static QMap<int, QColor> ProblemForegroundColors()
    {
        QMap<int, QColor> map;
        map.insert(PT_SYS, Qt::black);
        map.insert(PT_ALT, Qt::black);
        map.insert(PT_WH, Qt::black);
        map.insert(PT_SADM, Qt::white);
        map.insert(PT_TB, Qt::black);
        map.insert(PT_DOC, Qt::black);
        map.insert(PT_ALL, Qt::black);
        return map;
    }

    static QStringList DifferentColors()
    {
        QStringList list;
        list.append("255,255,51,40");
        list.append("0,51,255,40");
        list.append("0,204,51,40");
        list.append("153,153,51,40");
        list.append("255,204,51,40");
        list.append("255,0,102,40");
        list.append("0,153,204,40");
        return list;
    }

    enum ProblemSubTypes
    {
        PST_FIELDMISSED,    // отсутствует поле ProblemField в таблице ProblemTable
        PST_FIELDEMPTY,     // не заполнено поле ProblemField в таблице ProblemTable по индексу ProblemId
        PST_NOSUCHID,       // в таблице ProblemTable поле ProblemField в строке ProblemId ссылается на несуществующий ИД
        PST_INCORRECT,      // некорректная ссылка в поле ProblemField в строке ProblemId таблицы ProblemTable
        PST_DUPLICATE,      // найдены одинаковые элементы в таблице ProblemTable с ИД ProblemId и ProblemId2
        PST_INCORRECTDATE,  // некорректная дата в строке ProblemId в таблице ProblemField
        PST_ALTIUMINC,      // несоответствие полей NominalValue и Nominal+Unit в строке ProblemId таблицы ProblemTable
        PST_EMPTYFIELDS,    // найдена пустая запись в строке ProblemId таблицы ProblemTable
        PST_NKALTIUM,       // для элемента ProblemId в таблице ProblemTable не найдена соответствующая запись в таблице ProblemTable2
        PST_TBOVERDUE,      // просрочен срок сдачи экзамена у тов. ProblemPerson
        PST_TBOVERDUEINC,   // до конца действия срока сдачи экзамена у тов. ProblemPerson осталось менее 14 дней
        PST_NEWAQ,          // есть новые элементы в карантине Altium
        PST_NEWISSUE        // есть задания для выполнения
    };

    struct ProblemStruct
    {
        int ProblemType;
        int ProblemSubType;
        QString ProblemTable;
        QString ProblemTable2;
        QString ProblemField;
        QString ProblemId;
        QString ProblemId2;
        QString ProblemPerson;
    };

    QList<ProblemStruct> ExchangeProblemsList, ProblemsList;
    QMutex EPLMutex;

    // структура возвращаемых значений по полю links
    struct ValueStruct
    {
        int Type; // тип значения VL_
        QString Value; // само значение
        QString Links; // строка ссылок
    };

    double timerperiod; //, ErWidgetPeriod;
//    bool ErWidgetShowing;
    bool AutonomousMode; // если сервер СУПиК (становится) недоступен, признак автономного режима становится равен true
    QString LastError;
    QSqlDatabase ent, alt, sup, con, sol, sch, dev, tb, sadm;
    QMap<QString, QSqlDatabase> db;
    quint16 DbNotOpened;
//    QString Date; // Сегодняшняя дата
    QString DateTime; // Сегодняшнее время
    int idRecord; // номер (id) текущей редактируемой записи в справочнике
    int idPers;  // текущий идентификатор пользователя, вошедшего в систему
    QString Pers; // фамилия пользователя, вошедшего в систему
    QString PersLogin, PersPsw; // логин и пароль пользователя
    int idGroup; // ИД группы пользователя, определяет права пользователя в системе (ent.groups.idgroup)
    QString PathToLibs; // текущий путь до библиотек Altium
    QString PathToSup; // текущий путь до рабочей директории СУПиКа
    QString FtpServer; // адрес ftp-сервера, используемого для архивов
    QString CurLang; // обозначение текущего языка системы
    QString HomeDir; // путь к рабочей папке пользователя с СУПиКом (Win: c:\Users\<user>\.supik, *nix: ~/.supik)
    QString SupikServer, SupikPort; // IP-адрес и порт сервера СУПиК
    QString SQLPath; // путь к SQL-серверу
    QSettings *LandP; // переменная для работы с настройками системы в реестре
    QByteArray data, symfind, footfind; // data - массив хранения считанных из файлов библиотек бинарных данных,
                                        // symfind и footfind - строки поиска в библиотеках "LIBREFERENCE=" и "PATTERN="
    QStringList supikprocs;
    int Mode; // Режим работы (Справочники/Компоненты/Склады/Движение)
    int Altium; // граница между списком справочников и списком компонентов (для maintree)
    quint32 access; // текущие права доступа. Зашифрованы тройками бит (с младшего до старшего: право на чтение/изменение/удаление) (начиная с младших):
                // Системные, Складские, Альтиумовские, ГлавноИнженерские, Сисадминские, Архивариусовские

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
    } FieldFormat;

    void getFFfromLinks(QString &links, FieldFormat &ff) const;
    void getlinksfromFF(FieldFormat &ff, QString &out);
    void InitiatePublicClass();
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
        QString line;
        QString msg;
        int ErNum;
    };
    QList<ermsg> ermsgpool;
    int ErNum;

    void AddErrMsg(ermsgtype msgtype, QString file, int line, QString msg="");
    void ConvertId(bool ColumnZero, QString &Id); // преобразование <tble>.000<id> в нормальный id
    bool OpenAndCheckDBs();


private:
    void openBD(QSqlDatabase &db, QString dbid, QString dbname, QString login, QString psw);
};

extern PublicClass pc;

#endif // PUBLICCLASS_H
