#ifndef EDITMODEL_H
#define EDITMODEL_H

#include <QAbstractTableModel>
#include <QByteArray>
#include <QColor>
#include <QFont>
#include <QIcon>
#include <QModelIndexList>
#include "../gen/publicclass.h"

#define EMODELWARN     WARNMSG(PublicClass::ER_EMODEL, __LINE__)
#define EMODELDBG       DBGMSG(PublicClass::ER_EMODEL, __LINE__)
#define EMODELINFO(a)   INFOMSG(PublicClass::ER_EMODEL, __LINE__, a)

class EditModelItem
{
public:
    explicit EditModelItem(EditModelItem *parent = 0);

    QString Data(int Column) const;
    QString LinksData(int Column) const;
    QString HeaderData(int Column) const;
    void SetData(int Column, const QString &Data);
    void SetLinksData(int Column, const QString &Data);
    void SetHeaderData(int Column, const QString &Data);
    bool IsEmpty(QList<int> ColumnsToCheck);
    void SetColor(int Column, QColor Color);
    void SetFont(int Column, QFont Font);
    void SetIcon(int Column, QIcon Icon);
    void SetCellInfoData(QString Str);
    void SetTableNumberData(int Column, QString Str);
    QColor Color(int Column);
    QFont Font(int Column);
    QIcon Icon(int Column);
    QString CellInfoData();
    QString TableNumberData(int Column);

private:
    QStringList ItemData; // содержимое строки
    QStringList LData; // содержимое поля со ссылками
    QStringList HData; // содержимое вспомогательного поля
    QString CellInfo; // вспомогательное содержимое для всего итема
    QStringList TableNumbers; // вспомогательное содержимое для каждой ячейки итема
    QList<QColor> ItemColor;
    QList<QFont> ItemFont;
    QList<QIcon> ItemIcon;
};

class EditModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit EditModel(QObject *parent = 0);
    ~EditModel();

    // роли для моделей
    enum Roles
    {
        LinksRole = Qt::UserRole,
        HeaderTextRole = Qt::UserRole+1,
        TableNumberRole = Qt::UserRole+2,
        CellInfoRole = Qt::UserRole+3
    };

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
    void AddColumns(QStringList Hdrs); // добавление новых колонок с текстом из hdrs
    QString getCellType(int row, int Column);
    void setRowAttr(int fcset=0, int icon=-1);
    void ClearModel();
    QString Value(int Row, int Column); // взять значение по строке row и столбцу Column
    int Setup(QString Table, QString Id);
    int SetupRaw(QString Db, QString Tble, QString Id); // заполнение модели из таблицы, которой нет в tablefields
    QStringList Headers();
    QStringList Values();
    QStringList Links();
    PublicClass::ValueStruct CellValue(int row, int column);

signals:

public slots:

private:
    QList<EditModelItem *> MainData;
    QStringList Hdr;
    QColor Colors[6]; // определение набора цветов шрифта
    QFont Fonts[6]; // определение набора шрифтов
    QIcon Icons[6]; // определение набора иконок

    void AddRow(QList<PublicClass::ValueStruct> &ValuesSl);
};

#endif // EDITMODEL_H
