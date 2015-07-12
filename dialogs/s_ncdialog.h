#ifndef S_NCDIALOG_H
#define S_NCDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QStringListModel>
#include "../models/s_ncmodel.h"
#include "dir_adddialog.h"
#include "../widgets/s_tqpushbutton.h"

// вид поля
#define FT_SIMPLE   0 // простое поле
#define FT_LINEEDIT 1 // простое поле ввода
#define FT_CHOOSE   2 // поле ввода с кнопкой выбора по ссылке
#define FT_COMBO    3 // выпадающий список
#define FT_VLINE    4 // вертикальная линия
#define FT_SPIN     5 // счётчик
#define FT_MASKED   6 // поле ввода с шаблоном
#define FT_HLINE    7 // горизонтальная линия
#define FT_TABLE    8 // таблица
#define FT_PB       9 // кнопка
#define FT_CHECK    10 // поле с отметкой (checkbox)
#define FT_LABEL    11 // надпись (содержимое в dialogsfields)
#define FT_AUTO     12 // поле с автоматической нумерацией
#define FT_STRETCH  13 // поле пустого пространства

// определение поля (link)
#define L_LINK      2 // обычная ссылка на столбец таблицы, формат: <db>.<tble>.<col>
#define L_DLINK     3 // ссылка на две таблицы, формат: <db>.<tble>.<col>.<db>.<tble>.<col>
#define L_ALINK     4 // alias родительского элемента, у которого необходимо выбрать одного из детёнышей. Формат: <db>.<tble>.<alias>
#define L_MLINK     5 // подстановка максимального по <idtble> значения. Формат: <db>.<tble>.<valuefield>.<col>.<value> или <db>.<tble>.<fieldvalue>.<col>.<№_поля>
#define L_EQUAT     7 // расчёт выражения. Формат: <op>.<arg1>.<arg2>, где op - операция (s,r,m,d), arg1,2 - аргументы, содержат выражения из чисел, знаков операций и ссылок на поля (c1) или элементы таблицы (tc2 или tr2)
#define L_RIGHTS    9 // диалог выбора прав доступа
#define L_INDIRECT  11 // косвенная ссылка
#define L_SMETHOD   12 // ссылка на метод диалога
#define L_FLINK     15 // ссылка на файл.
#define L_ILINK     16 // ссылка на каталог.
#define L_FLLINK    17 // ссылка на элемент внутри файла. Формат: <file>.<type>, где type = A(altium) или S(schemagee)
#define L_DATE      18 // диалог выбора даты

// местоположение поля
#define FL_HEADER   30 // заголовок диалога
#define FL_UPPER1   31 // верхняя часть диалога, обычно поле с текущим ИД и датой
#define FL_MIDDLE   32 // средняя часть диалога, обычно общие параметры, не требующие табличного ввода
#define FL_MAIN     33 // основная часть диалога, обычно таблица ввода
#define FL_LOWER    34 // нижняя часть диалога, обычно с полями "Итого", кнопками "Обновить" и т.п.
#define FL_UPPER2   35 // вторая верхняя часть диалога, обычно поле с персоналией, а также кнопками подтверждения и отмены

// тип диалога
#define DT_GENERAL  0 // диалог обычного типа (верх, средний фрейм с полями ввода, основная часть с таблицей и нижняя часть для итоговых полей и кнопок)
#define DT_2TREES   1 // диалог с двумя взаимоподчинёнными деревьями, отделёнными по вертикали друг от друга

class s_ncdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_ncdialog(QWidget *parent = 0);
    void setupUI(QString dialog, QString wallpaper, int dialogtype, QString id="0");

signals:
    void error(int,int);

public slots:

private:
    s_ncmodel *mainmodel;
    bool Somethingchanged;
    QList<s_ncmodel *> slavemodels;
    QList<QStringListModel *> slmodels;
    QString wallpaper;
    QString id;
    int dialogtype;
    QString keydb, keytble;
    QList<QStringList> vl;
    QHash <QString, void (s_ncdialog::*)()> pf;
    QWidget *getWidget(int id, QString ftext, QString link, QString defstr);

    void setTable(s_ncmodel &model, QString link);
    void updateDialog();

    // own methods
    void Close();
    void Accept();
    void Refresh();
    void New();
    void Filter();
    void Search();

    // dir methods
    void dir_Add();
    void dir_Edit();
    void dir_SubDel();
    void dir_SubEdit();
    void dir_SubAdd();

private slots:
    void methodpbclicked(s_tqPushButton*);
    void somethingchanged();
    void somethingchanged(QString);
    void somethingchanged(QModelIndex);
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // S_NCDIALOG_H
