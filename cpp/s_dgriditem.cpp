#include "../inc/s_dgriditem.h"

#include <QPainter>

s_dgriditem::s_dgriditem(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void s_dgriditem::paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    if (index.isValid())
    {
        painter->setPen(Qt::SolidLine);
        painter->setPen(QColor(Qt::darkGray));

//        painter->drawRect(option.rect);
        QRect rct = opt.rect;
        int lf = rct.left();
        int rg = lf + rct.width()-1;
        int tp = rct.top();
        int bt = tp + rct.height();
        painter->drawLine(QLine(QPoint(lf, bt), QPoint(rg, bt))); // bottomLeft - bottomRight
        painter->drawLine(QLine(QPoint(lf, tp), QPoint(rg, tp))); // topLeft - topRight
        painter->drawLine(QLine(QPoint(rg, tp), QPoint(rg, bt))); // topRight - bottomRight
        painter->drawLine(QLine(QPoint(lf, tp), QPoint(lf, bt))); // topLeft - bottomLeft
    }
    QStyledItemDelegate::paint(painter, opt, index);
}
