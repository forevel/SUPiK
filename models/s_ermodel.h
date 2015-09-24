#ifndef S_ERMODEL_H
#define S_ERMODEL_H

#define MAX_MSG     1000

#include <QAbstractTableModel>
#include "../gen/publicclass.h"

class s_ermodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit s_ermodel(QObject *parent = 0);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant data (const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &index = QModelIndex()) const;
    int columnCount(const QModelIndex &index = QModelIndex()) const;

    void AddRow(PublicClass::ermsg msg);

private:
    QList<QStringList> erdata;
    QList<PublicClass::ermsgtype> ertypedata;
    QStringList hdr;
    int MsgCount;
};

#endif // S_ERMODEL_H
