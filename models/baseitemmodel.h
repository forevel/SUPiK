#ifndef BASEITEMMODEL_H
#define BASEITEMMODEL_H

#include <QAbstractTableModel>
#include "baseitem.h"

#define ICON_NO ":/res/cross.png"
#define ICON_YES ":/res/ok.png"

class BaseItemModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    BaseItemModel();
    ~BaseItemModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data (const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int Column, const QModelIndex &index = QModelIndex()) const;
    int rowCount(const QModelIndex &index = QModelIndex()) const;
    int columnCount(const QModelIndex &index = QModelIndex()) const;
    bool insertColumns(int position, int Columns, const QModelIndex &index = QModelIndex());
    bool removeColumns(int position, int Columns, const QModelIndex &index = QModelIndex());
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    void AddColumn(const QString Hdr); // добавление новой колонки с текстом в заголовке hdrtext
    int AddRow();
    bool SetModelData(int row, int column, const QVariant &value, int role);
    void ClearModel();

signals:

public slots:

private:
    QList<BaseItem *> MainData;
    QStringList Hdr, IconList;
};

#endif // BASEITEMMODEL_H
