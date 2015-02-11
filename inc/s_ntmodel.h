#ifndef S_NTMODEL_H
#define S_NTMODEL_H

#include <QAbstractItemModel>
#include <QSqlDatabase>
#include <QStringList>
#include <QColor>
#include <QFont>
#include <QIcon>
#include "s_ntitem.h"

// тип делегата
#define FD_SIMPLE   0 // простой делегат без рамки для надписей
#define FD_LINEEDIT 1 // простое поле ввода
#define FD_CHOOSE   2 // поле ввода с кнопкой выбора по ссылке
#define FD_COMBO    3 // выпадающий список
#define FD_DISABLED 4 // неактивное поле
#define FD_SPIN     5 // счётчик
#define FD_SIMGRID  7 // простой делегат с рамкой

class s_ntmodel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit s_ntmodel(QObject *parent = 0);
    ~s_ntmodel();

    QModelIndex parent(const QModelIndex &index) const;
    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    int getHeaderPosition(QVariant hdrtext, Qt::Orientation orientation, int role); // получение индекса элемента в заголовке, который содержит текст hdrtext
    void addRow(int position, int cfset, QModelIndex &parent = QModelIndex());
    QModelIndex getIndex(s_ntitem *item) const;
    s_ntitem *getItem(const QModelIndex &index) const;
    int Setup (bool twodb, QString table);
    int Setup (QString cattble, QString slvtble);
    void ClearModel();
    QList<QPersistentModelIndex> expandedIndexes;

signals:

public slots:

    void addExpandedIndex(const QModelIndex &index);
    void removeExpandedIndex(const QModelIndex &index);

private:
    s_ntitem *rootItem;
    QList<s_ntitem*> parents;
    QList<int> indentations;
    QList<QStringList> vl;
    int position;
    QStringList catlist;
    QString slvtble;
    bool twodb;
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
    s_ntmodel::fieldformat getFFfromLinks (QString links) const;

    int BuildTree (QString id);
    s_ntitem *addTreeCat (QString catlistid);
    int addTreeSlvItem(QList<QStringList> sl, int position, QString id);
    void additemtotree(int position, QStringList sl, int set);
};

#endif // S_NTMODEL_H
