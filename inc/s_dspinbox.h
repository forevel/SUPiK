#ifndef S_dspinbox_H
#define S_dspinbox_H

#include <QStyledItemDelegate>

class s_dspinbox : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit s_dspinbox(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData (QWidget *editor, const QModelIndex &index) const;
    void setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void paint (QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

signals:

public slots:

private:
};

#endif // S_dspinbox_H
