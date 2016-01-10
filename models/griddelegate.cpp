#include "griddelegate.h"
#include <QPainter>
#include <QRect>

GridDelegate::GridDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void GridDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->save();
    if (index.isValid())
    {
        QRect rct = option.rect;
        painter->setPen(Qt::SolidLine);
        if (option.state & QStyle::State_Selected)
            painter->setPen(QColor(Qt::darkGreen));
        else
            painter->setPen(QColor(Qt::lightGray));
        painter->drawRoundedRect(rct, 3, 3);
    }
    painter->restore();
}
