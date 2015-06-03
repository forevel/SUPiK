#ifndef PUBLICCLASS_H
#define PUBLICCLASS_H

#define SUPIK_VERS  "SUPiK v0.4b"

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

// диалоговые окна в СУПиКе
#define TW_SET      0x100 // настройки (Settings)
#define TW_SYSST    0x101 // SysStructEdit - редактор структуры системы
#define TW_COMP     0x102 // редактор компонентов
#define TW_DIR      0x103 // справочники
#define TW_PROB     0x104 // диалог событий
#define TW_WH       0x105 // склады
#define TW_SYSBU    0x106 // резервное копирование
#define TW_SYSRS    0x107 // восстановление
#define TW_QUAR     0x108 // работа с карантином
#define TW_DOC      0x109 // редактор документов
#define TW_ADM      0x10A // сисадминистрирование

// Коды ошибок
#define ER_TFIELD   0x0200 // tablefields
#define ER_NTMODEL  0x3000 // s_ntmodel
#define ER_NCMODEL  0x4000 // s_ncmodel
#define ER_DIRMAIN  0x100000 // dir_maindialog
#define ER_2CDLG    0x10000 // 2cdialog
#define ER_2CTDLG   0x20000 // 2ctdialog

#define DB_ALT      0x00
#define DB_ENT      0x01
#define DB_SUP      0x02
#define DB_SOL      0x03
#define DB_DEV      0x04
#define DB_SCH      0x05
#define DB_CON      0x06

#define MAX_DBS     50

#define SYS_RO      0x0001
#define SYS_WR      0x0002
#define SYS_FULL    0x0007
#define WH_RO       0x0008
#define WH_WR       0x0010
#define WH_FULL     0x0038
#define ALT_RO      0x0040
#define ALT_WR      0x0080
#define ALT_FULL    0x01C0
#define TB_RO       0x0200
#define TB_WR       0x0400
#define TB_FULL     0x0E00
#define SADM_RO     0x1000
#define SADM_WR     0x2000
#define SADM_FULL   0x7000

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
#define FW_MASKED   6 // форматированное число
#define FW_EQUAT    7 // арифметическое выражение
#define FW_PLAIN    8 // простое поле, без изысков
#define FW_RIGHTS   9 // поле с бинарным отображением и организации делегата FD_CHOOSE с диалогом rights
#define FW_TLINK    10 // ссылка на таблицу в списке таблиц
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
#define DELEGNUM    8 // количество делегатов

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

    double timerperiod;
    QString SQLUser, SQLPsw;
    QSqlDatabase ent;
    QSqlDatabase alt;
    QSqlDatabase sup;
    QSqlDatabase con;
    QSqlDatabase sol;
    QSqlDatabase sch;
    QSqlDatabase dev;
    QMap<QString, QSqlDatabase> db;
//    QString Date; // Сегодняшняя дата
    QString DateTime; // Сегодняшнее время
    int idRecord; // номер (id) текущей редактируемой записи в справочнике
    int idPers;  // текущий идентификатор пользователя, вошедшего в систему
    QString Pers; // фамилия пользователя, вошедшего в систему
    int idGroup; // ИД группы пользователя, определяет права пользователя в системе (ent.groups.idgroup)
    QString PathToLibs; // текущий путь до библиотек Altium
    QString PathToSup; // текущий путь до рабочей директории СУПиКа
    QString CurLang; // обозначение текущего языка системы
    QString SQLPath; // путь к SQL-серверу
    QSettings *LandP; // переменная для работы с настройками системы в реестре
    QDateTime CurDateTime;
    QByteArray data, symfind, footfind; // data - массив хранения считанных из файлов библиотек бинарных данных,
                                        // symfind и footfind - строки поиска в библиотеках "LIBREFERENCE=" и "PATTERN="
//    QStringList CapList, ResList, IndList, NumList, VAList, WList, LedList, ProtList, VList, MemList, KList, OptoList, VelList; // 0.34-aj
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
    int dbs_index; // для справочников содержит индекс справочника (NK=0,MANUF=1 и т. д.).
                    // для компонентов содержит индекс в таблице синонимов названий таблиц компонентов
                    // в БД altium. Т.е. -1 в этом поле для справочников (pc.ent) означает
                    // ошибку, отсутствие такого справочника или что ни один справочник не выделен
                    // в дереве, то для компонентов это означает только, что не найден alias для
                    // выбранного элемента дерева (например, таблицу компонентов ic_fairchild уже
                    // создали с компонентами, а названия "Микросхемы Fairchild" для дерева
                    // ещё не присвоили
    QString InterchangeString;
    struct
    {
        QString dbs; // наименование таблицы (nk, wh, ...)
        QString alias; // наименования элемента дерева-меню ("номенклатура", "склады", ...)
    } dbs[MAX_DBS]; // таблица соответствия названий таблиц наименованиям в дереве-меню
    int access; // текущие права доступа. Зашифрованы тройками бит (с младшего до старшего: право на чтение/изменение/удаление) (начиная с младших):
                // Системные, Складские, Альтиумовские, ГлавноИнженерские, Сисадминские
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

private:
    void addmessage(QStringList &sl, QString mes);
    void openBD(QSqlDatabase &db, QString dbid, QString dbname, QString login, QString psw);
};

extern PublicClass pc;

#endif // PUBLICCLASS_H