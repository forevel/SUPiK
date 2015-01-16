#ifndef S_dgriditem_H
#define S_dgriditem_H

#include <QStyledItemDelegate>

class s_dgriditem : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit s_dgriditem(QObject *parent = 0);

signals:

public slots:

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const;

};

#endif // S_dgriditem_H
