#ifndef GOODBADWIDGET_H
#define GOODBADWIDGET_H

#include <QWidget>
#include <QAbstractTableModel>
#include <QIcon>
#include "s_tqtableview.h"

class GoodBadTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit GoodBadTableModel(QObject *parent = 0);

    enum ItemTypes // состояние ответа - правильное, неправильное или ещё неотвеченное
    {
        GBIT_GOOD,
        GBIT_BAD,
        GBIT_NEUTRAL
    };

    QVariant data (const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &index = QModelIndex()) const;
    int columnCount(const QModelIndex &index = QModelIndex()) const;
    bool insertColumns(int position, int columns, const QModelIndex &index = QModelIndex());
    bool removeColumns(int position, int columns, const QModelIndex &index = QModelIndex());
    bool Setup(int NumItems);
    bool SetItem(int ItemNum, int ItemType);

signals:

public slots:

private:
    QList<int> Items;
    QIcon Icons[3];
};

class GoodBadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GoodBadWidget(QWidget *parent = 0);
    ~GoodBadWidget();

    bool SetItemsCount(int NumItems);
    bool SetItem(int ItemNum, int ItemType);

signals:

public slots:

private slots:

private:
    s_tqTableView *tv;
    GoodBadTableModel *tm;
};

#endif // GOODBADWIDGET_H
