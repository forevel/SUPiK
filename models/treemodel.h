#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QColor>
#include <QFont>
#include <QIcon>
#include <QStack>
#include "treeitem.h"

#define TMODELDBG         DBGMSG(PublicClass::ER_TMODEL,__LINE__)
#define TMODELWARN        WARNMSG(PublicClass::ER_TMODEL,__LINE__)
#define TMODELINFO(a)     INFOMSG(PublicClass::ER_TMODEL,__LINE__,a)

class TreeModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent = 0);
    ~TreeModel();

    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QModelIndex index(int row, int column, const QModelIndex &index = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertColumns(int column, int count, const QModelIndex &parent);
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    void SetLastItem(QColor Color, QFont Font, QIcon Icon);
    int Setup (QString Table);
//    int Setup (QString Maintble, QString Slvtble);
    void ClearModel();
    void ClearOnlyData();

    QColor Colors[7]; // определение набора цветов шрифта
    QFont Fonts[7]; // определение набора шрифтов
    QIcon Icons[7]; // определение набора иконок

signals:

public slots:
    void GoIntoIndex(QModelIndex idx);
    void GoOut(QModelIndex idx);

private:
    enum Roles
    {
        MyItemRole = Qt::UserRole+1
    };

    QList<TreeItem *> maindata;
    QStringList hdr;
    QStringList TableHeaders;
    QString MainDb, MainTable;
    QStack<QString> RootIDs;
/*    QString Slvtble;
    QStringList Slvtblefields, Slvtblelinks; */

    int BuildTree (QString Id, bool Twodb);
//    int AddTreeSlvItem(int Position, QString Id);
    void AddItemToTree(QStringList Fields);
};

#endif // TREEMODEL_H
