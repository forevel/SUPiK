#ifndef S_WHITEMMODEL_H
#define S_WHITEMMODEL_H

#include <QAbstractItemModel>
#include <QStringList>
#include "../gen/publicclass.h"
#include "s_whitem.h"

class s_whitemmodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit s_whitemmodel(QObject *parent = 0);
    ~s_whitemmodel();

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    int getHeaderPosition(QVariant hdrtext, Qt::Orientation orientation, int role);
    QVariant data (const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &index = QModelIndex()) const;
    int rowCount(const QModelIndex &index = QModelIndex()) const;
    int columnCount(const QModelIndex &) const;
//    s_whitem *getItem(const QModelIndex &index) const;
    bool insertColumns(int position, int columns, const QModelIndex &index = QModelIndex());
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeColumns(int position, int columns, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    bool addColumn(const QString hdrtext, QString sqllink);
    bool addRow(QStringList sl);
    bool checkforEmptyRows();
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
    QList<s_whitem *> md;
    QList<int> columnsToCheck;
    QStringList hdr;
    QStringList links;

    QString getEq(QString arg1, QString arg2, int oper, const QModelIndex index) const;
    float getOperand(QString str, const QModelIndex index) const;
};

static const QMap<QString, int> OPER_MAP = s_whitemmodel::opers();

#endif // S_WHITEMMODEL_H
