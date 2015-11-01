#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QSortFilterProxyModel>

class ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ProxyModel(QObject *parent=0);
    ~ProxyModel();
    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public slots:

    void addExpandedIndex(const QModelIndex &index);
    void removeExpandedIndex(const QModelIndex &index);

};

#endif // PROXYMODEL_H
