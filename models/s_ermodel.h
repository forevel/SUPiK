#ifndef S_ERMODEL_H
#define S_ERMODEL_H

#define MAX_MSG     1000

#include <QAbstractTableModel>

class s_ermodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit s_ermodel(QObject *parent = 0);
    ~s_ermodel();
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

    void AddRow(msgtype type, QString text);

private:
    enum msgtype
    {
        ER_MSG,
        WARN_MSG,
        INFO_MSG,
        DBG_MSG
    };
    struct msg
    {
        QString DateTime;
        msgtype type;
        QString msg;
    };

    QList<msg> msgpool;
};

#endif // S_ERMODEL_H
