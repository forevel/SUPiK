#ifndef ERRORPROTOCOLMODEL_H
#define ERRORPROTOCOLMODEL_H

#include <QWidget>

#include <QAbstractTableModel>
#include "../gen/publicclass.h"

#define ERPROTCLR   "rgba(248,214,255,255)"
#define MAX_MSG     1000

class ErrorProtocolModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ErrorProtocolModel(QObject *parent = 0);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant data (const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &index = QModelIndex()) const;
    int columnCount(const QModelIndex &index = QModelIndex()) const;
    void AddRow(PublicClass::ermsg ermsg);

private:
    QList<QStringList> erdata;
    QList<PublicClass::ermsgtype> ertypedata;
    QStringList hdr;
    int MsgCount;

private slots:
};

#endif // ERRORPROTOCOLMODEL_H
