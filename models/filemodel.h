#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractItemModel>
#include <QColor>
#include <QFont>
#include <QIcon>
#include <QStack>
#include "fileitem.h"
#include "../gen/publicclass.h"

#define FM_SIMPLE_ELEMENT   "0"
#define FM_DIR_WITH_FILES   "1"

#define FM_HDR_COUNT    5 // icon, filename, size, attr, datetime

// возвращаемые результаты
#define FM_OK           0
#define FM_BADSOURCE    1 // некорректные входные данные
#define FM_BADRESULT    2 // ошибка при выполнении алгоритма

class FileModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    explicit FileModel(QObject *parent = 0);
    ~FileModel();

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
    int Setup(QString DirName);
    void Refresh();
    void SetLastItem(QColor FColor, QColor BColor, QFont Font, QIcon Icon, QString AData="0");
    void ClearModel();
    void AddItemToTree(QStringList &vl);

    // роли для моделей
    enum Roles
    {
        SizeRole = Qt::UserRole,
        AttrRole = Qt::UserRole+1,
    };

signals:

public slots:
    void GotoFile(QModelIndex idx);

private:
    QColor Colors[7]; // определение набора цветов шрифта
    QFont Fonts[7]; // определение набора шрифтов
    QIcon Icons[7]; // определение набора иконок
    QList<FileItem *> maindata;
    QStringList hdr;
    QStack<QString> Folders; // элементы записываются в виде: <номер_таблицы>.<ИД>

    int BuildTree ();
    int SetFirstTreeElements();
    int SetTable(int Table, QString Id);
    int PrepareTable(QString Table); // подготовить очередные TableHeaders, DBs и Tables по таблице Table
    void ClearOnlyData();
};

#endif // FILEMODEL_H
