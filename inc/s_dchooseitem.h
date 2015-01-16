#ifndef S_DCHOOSEITEM_H
#define S_DCHOOSEITEM_H

#include <QStyledItemDelegate>
#include <QSqlDatabase>
#include "s_sqltreedialog.h"
#include "../inc/s_tqwidget.h"

class s_dchooseitem : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit s_dchooseitem(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData (QWidget *editor, const QModelIndex &index) const;
    void setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
//    void updateEditorGeometry (QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paint (QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

signals:
    void pbclicked (QModelIndex &);

public slots:

protected:

private slots:
    void pbclicked();
    void accepted(QString);

private:
    s_sqltreedialog *chooseDialog;
    mutable s_tqwidget *combWidget;
    mutable QSqlDatabase db1, db2;
    mutable QString tble1, tble2, col1, col2;
};

#endif // S_DCHOOSEITEM_H
