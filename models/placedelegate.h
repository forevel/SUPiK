#ifndef PLACEDELEGATE_H
#define PLACEDELEGATE_H

#include <QStyledItemDelegate>

class PlaceDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PlaceDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData (QWidget *editor, const QModelIndex &index) const;
    void setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void paint (QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

private slots:
    void CommitChanges(QVariant);

};

#endif // PLACEDELEGATE_H
