#ifndef S_DUNIVERSAL_H
#define S_DUNIVERSAL_H

#include <QStyledItemDelegate>
#include "../gen/publicclass.h"
#include "../widgets/s_tqchoosewidget.h"
#include "treemodel.h"

class s_duniversal : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit s_duniversal(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData (QWidget *editor, const QModelIndex &index) const;
    void setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void paint (QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

public slots:

protected:

private slots:
    void CommitChanges(QVariant);

private:
    mutable PublicClass::FieldFormat ff;
    mutable s_tqChooseWidget *wdgt;
};

#endif // S_DUNIVERSAL_H
