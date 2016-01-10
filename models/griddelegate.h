#ifndef GRIDDELEGATE_H
#define GRIDDELEGATE_H

#include <QStyledItemDelegate>

class GridDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit GridDelegate(QObject *parent = 0);
    virtual void paint (QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

public slots:

protected:

private slots:

private:
};

#endif // GRIDDELEGATE_H
