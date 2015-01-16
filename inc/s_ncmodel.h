#ifndef S_2CMODEL_H
#define S_2CMODEL_H

#include <QAbstractTableModel>
#include <QColor>
#include <QFont>
#include <QIcon>

// тип поля
#define FW_COUNT    19 // количество типов полей
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
#define FW_INDIRECT 11 // косвенная ссылка. Подробнее см. "СУПиК РП"
#define FW_SMETHOD  12 // ссылка на метод СУПиКа
#define FW_2CD      13 // отображение диалога редактирования строки таблицы (2cdialog)
#define FW_LLINK    14 // отображение диалога конструктора ссылок
#define FW_FLINK    15 // отображение диалога выбора имени файла
#define FW_ILINK    16 // отображение диалога выбора имени каталога
#define FW_FLLINK   17 // ссылка на элемент внутри файла
#define FW_DATE     18 // отображение диалога выбора даты

// тип делегата
#define FD_SIMPLE   0 // простой делегат без рамки для надписей
#define FD_LINEEDIT 1 // простое поле ввода
#define FD_CHOOSE   2 // поле ввода с кнопкой выбора по ссылке
#define FD_COMBO    3 // выпадающий список
#define FD_DISABLED 4 // неактивное поле
#define FD_SPIN     5 // счётчик
#define FD_SIMGRID  7 // простой делегат с рамкой

// тип арифм операции
#define OP_PLUS     1
#define OP_MINUS    2
#define OP_MULT     3
#define OP_DIV      4

#include <QAbstractItemModel>
#include <QStringList>
#include "s_ncitem.h"

class s_ncmodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit s_ncmodel(QObject *parent = 0);
    ~s_ncmodel();

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant data (const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &index = QModelIndex()) const;
    int rowCount(const QModelIndex &index = QModelIndex()) const;
    int columnCount(const QModelIndex &) const;
    bool insertColumns(int position, int columns, const QModelIndex &index = QModelIndex());
    bool removeColumns(int position, int columns, const QModelIndex &index = QModelIndex());
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    int getHeaderPosition(QVariant hdrtext, Qt::Orientation orientation, int role); // получение индекса элемента в заголовке, который содержит текст hdrtext
    void addColumn(const QString hdrtext); // добавление новой колонки с текстом в заголовке hdrtext для варианта двух столбцов
    void addRow(); // добавление строки
    void setcolumnlinks(int column, QString links); // установка типа поля на весь столбец модели
    void setcolumnlinks(int column, QStringList links); // установка типов полей на столбец модели
    void setrowlinks(int row, QStringList links); // установка типов полей на строку модели
    bool checkforEmptyRows(); // проверка наличия строк, в которых элементы по индексам fieldsToCheck содержат пустые поля
    void setCellAttr(QModelIndex index, int fcset=0, int icon=-1);
    void setRowAttr(int fcset=0, int icon=-1);
    void fillModel(QList<QStringList> sl);
    void prepareModel(QList<int> sl);
    int maxcolwidth(int column);
    int maxcolwidthsize();
    static QMap<QString, int> opers()
    {
        QMap<QString, int>map;
        map.insert("s", OP_PLUS);
        map.insert("r", OP_MINUS);
        map.insert("m", OP_MULT);
        map.insert("d", OP_DIV);
        return map;
    }

signals:

public slots:

private:
    QList<s_ncitem *> maindata;
    QList<int> fieldstoCheck;
    QStringList hdr;
    QColor colors[6]; // определение набора цветов шрифта
    QFont fonts[6]; // определение набора шрифтов
    QIcon icons[6]; // определение набора иконок
    QString getEq(QString arg1, QString arg2, int oper, const QModelIndex index, bool byRow) const; // подсчёт выражения "arg1 <oper> arg2"
    float getOperand(QString str, const QModelIndex index, bool byRow) const; // подсчёт арифм. выражения, содержащегося в строке str
    typedef struct
    {
        int ftype;
        int delegate;
        int dependson;
        QStringList link;
    } fieldformat;
    s_ncmodel::fieldformat getFFfromLinks (QString links) const;
    QList<int> maxcolswidth;
};

#endif // S_2CMODEL_H
