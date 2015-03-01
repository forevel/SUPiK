#ifndef S_NTMODEL_H
#define S_NTMODEL_H

#include <QAbstractItemModel>
#include <QSqlDatabase>
#include <QStringList>
#include <QColor>
#include <QFont>
#include <QIcon>
#include "../inc/s_ntitem.h"

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
    int Setup (QString table);
    int Setup (QString maintble, QString slvtble);
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
    QStringList slvtblefields;
    QColor colors[6]; // определение набора цветов шрифта
    QFont fonts[6]; // определение набора шрифтов
    QIcon icons[6]; // определение набора иконок
/*    typedef struct
    {
        int ftype;
        int delegate;
        int dependson;
        QStringList link;
    } fieldformat;
    s_ntmodel::fieldformat getFFfromLinks (QString links) const; */

    int BuildTree (QString id, bool twodb);
    s_ntitem *addTreeCat (QString catlistid);
    int addTreeSlvItem(int position, QString id);
    void additemtotree(int position, QStringList sl, int set);
};

#endif // S_NTMODEL_H
