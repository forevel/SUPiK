#ifndef S_2CMODEL_H
#define S_2CMODEL_H

#include <QAbstractTableModel>
#include <QByteArray>
#include <QColor>
#include <QFont>
#include <QIcon>
#include <QModelIndexList>
#include "../gen/publicclass.h"

#include "s_ncitem.h"

#define NCMWARN     WARNMSG(PublicClass::ER_NCMODEL, __LINE__)
#define NCMDBG      DBGMSG(PublicClass::ER_NCMODEL, __LINE__)
#define NCMINFO(a)  INFOMSG(PublicClass::ER_NCMODEL, __LINE__, a)

class FlowModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit FlowModel(QObject *parent = 0);
    ~FlowModel();

    int result;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant data (const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &index = QModelIndex()) const;
    int rowCount(const QModelIndex &index = QModelIndex()) const;
    int columnCount(const QModelIndex &index = QModelIndex()) const;
    bool insertColumns(int position, int columns, const QModelIndex &index = QModelIndex());
    bool removeColumns(int position, int columns, const QModelIndex &index = QModelIndex());
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    QModelIndexList match(QString text);
    int getHeaderPosition(QVariant hdrtext, Qt::Orientation orientation, int role); // получение индекса элемента в заголовке, который содержит текст hdrtext
    void addColumn(const QString hdrtext); // добавление новой колонки с текстом в заголовке hdrtext
    void AddColumns(QStringList hdrs); // добавление новых колонок с текстом из hdrs
    void addRow(); // добавление строки
    void setcolumnlinks(int column, QString links); // установка типа поля на весь столбец модели
    void setcolumnlinks(int column, QStringList links); // установка типов полей на столбец модели
    void setrowlinks(int row, QStringList links); // установка типов полей на строку модели
    bool checkforEmptyRows(); // проверка наличия строк, в которых элементы по индексам fieldsToCheck содержат пустые поля
    void setCellAttr(QModelIndex index, int fcset=0, int icon=-1);
    QString getCellType(int row, int column);
    void setRowAttr(int fcset=0, int icon=-1);
    void ClearModel();
    void fillModel();
    QStringList cvalues(int column); // выдать значения по столбцу column в выходной QStringList
    QStringList rvalues(int row); // выдать значения по строке row в выходной QStringList
    QString value(int row, int column); // взять значение по строке row и столбцу column
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
    void setup(QString tble);
    void Add(QString tble);
    void setup(QString tble, QString id);
    void setupcolumn(QString tble, QString header);
    void setDataToWrite(QList<QStringList> sl);
    int setupraw(QString db, QString tble, QStringList fl=QStringList(),QString orderfield=""); // загрузка модели из таблицы db.tble базы данных по полям fl без всяких tablefields и links

    bool isEditable;

signals:
    void test();

public slots:

private:
    QList<s_ncitem *> maindata;
    QList<int> fieldstoCheck;
    QList<QStringList> DataToWrite;
    QStringList hdr;
    QColor colors[6]; // определение набора цветов шрифта
    QFont fonts[6]; // определение набора шрифтов
    QIcon icons[6]; // определение набора иконок
    QString getEq(QString arg1, QString arg2, int oper, const QModelIndex index, bool byRow) const; // подсчёт выражения "arg1 <oper> arg2"
    float getOperand(QString str, const QModelIndex index, bool byRow) const; // подсчёт арифм. выражения, содержащегося в строке str
    QList<int> maxcolswidth;
    int rcount; // количество реально записанных рядов (индекс следующего пустого ряда)
};

#endif // S_2CMODEL_H
