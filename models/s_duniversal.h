#ifndef S_DUNIVERSAL_H
#define S_DUNIVERSAL_H

#include <QStyledItemDelegate>
#include <QSqlDatabase>
#include <QDate>

#include "../gen/publicclass.h"
#include "../widgets/s_tqwidget.h"

class s_duniversal : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit s_duniversal(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData (QWidget *editor, const QModelIndex &index) const;
    void setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
//    void updateEditorGeometry (QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void paint (QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
//    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setTableHeader(QString hdr);

signals:
    void pbclicked (QModelIndex &);

public slots:

protected:

private slots:
    void pbclicked();
    void accepted(QString);
    void dateChoosed(QDate);
    void commitChanges(QString);

private:
    mutable s_tqWidget *combWidget;
    mutable s_tqWidget *cbWidget;
    mutable QString query;
    mutable QString hdr;
    mutable PublicClass::fieldformat ff;
};

#endif // S_DUNIVERSAL_H
