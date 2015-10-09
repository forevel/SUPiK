#ifndef PLACEDELEGATE_H
#define PLACEDELEGATE_H

#include <QStyledItemDelegate>
#include "../gen/publicclass.h"
#include "../widgets/s_tqpushbutton.h"

class PlaceDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PlaceDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData (QWidget *editor, const QModelIndex &index) const;
    void setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void paint (QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

private:
    bool Changed;

private slots:
    void CommitChanges(QVariant);

public slots:
    void buttonPressed(s_tqPushButton *ptr);

protected:
    bool eventFilter(QObject *object, QEvent *event);

};

#endif // PLACEDELEGATE_H
