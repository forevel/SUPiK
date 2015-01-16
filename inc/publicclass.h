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

#include "../inc/publiclang.h"

#define MAX_DBS 50

#define SYS_RO      0x0001
#define SYS_FULL    0x0007
#define WH_RO       0x0008
#define WH_FULL     0x0038
#define ALT_RO      0x0040
#define ALT_FULL    0x01C0
#define TB_RO       0x0200
#define TB_FULL     0x0E00
#define SADM_RO     0x1000
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

class PublicClass
{
public:
    PublicClass();
    ~PublicClass();

    int timerperiod;
    QSqlDatabase ent;
    QSqlDatabase alt;
    QSqlDatabase sup;
    QSqlDatabase con;
    QSqlDatabase sol;
    QSqlDatabase sch;
    QSqlDatabase dev;
    QMap<QString, QSqlDatabase> db;
    QString Date; // Сегодняшняя дата
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

    static QMap<QChar, QString> ruslat()
    {
        QMap<QChar, QString>map;
        map.insert('А', "A");
        map.insert('Б', "B");
        map.insert('В', "V");
        map.insert('Г', "G");
        map.insert('Д', "D");
        map.insert('Е', "E");
        map.insert('Ё', "Yo");
        map.insert('Ж', "Zh");
        map.insert('З', "Z");
        map.insert('И', "I");
        map.insert('Й', "Y");
        map.insert('К', "K");
        map.insert('Л', "L");
        map.insert('М', "M");
        map.insert('Н', "N");
        map.insert('О', "O");
        map.insert('П', "P");
        map.insert('Р', "R");
        map.insert('С', "S");
        map.insert('Т', "T");
        map.insert('У', "U");
        map.insert('Ф', "F");
        map.insert('Х', "H");
        map.insert('Ц', "C");
        map.insert('Ч', "Ch");
        map.insert('Ш', "Sh");
        map.insert('Щ', "Shch");
        map.insert('Ъ', "Y");
        map.insert('Ы', "Y");
        map.insert('Ь', "Y");
        map.insert('Э', "E");
        map.insert('Ю', "Yu");
        map.insert('Я', "Ya");
        map.insert('а', "a");
        map.insert('б', "b");
        map.insert('в', "v");
        map.insert('г', "g");
        map.insert('д', "d");
        map.insert('е', "e");
        map.insert('ё', "yo");
        map.insert('ж', "zh");
        map.insert('з', "z");
        map.insert('и', "i");
        map.insert('й', "y");
        map.insert('к', "k");
        map.insert('л', "l");
        map.insert('и', "m");
        map.insert('н', "n");
        map.insert('о', "o");
        map.insert('п', "p");
        map.insert('р', "r");
        map.insert('с', "s");
        map.insert('т', "t");
        map.insert('у', "u");
        map.insert('ф', "f");
        map.insert('х', "h");
        map.insert('ц', "c");
        map.insert('ч', "ch");
        map.insert('ш', "sh");
        map.insert('щ', "shch");
        map.insert('ъ', "y");
        map.insert('ы', "y");
        map.insert('ь', "y");
        map.insert('э', "e");
        map.insert('ю', "yu");
        map.insert('я', "ya");
        return map;
    }

    int S_TabWidgetWidth;
    QMap <QString, QColor> TabColors;
    QColor colors[6]; // определение набора цветов шрифта
    QFont fonts[6]; // определение набора шрифтов
    QIcon icons[6]; // определение набора иконок

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
