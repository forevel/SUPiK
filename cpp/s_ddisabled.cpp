#include "../inc/s_ddisabled.h"
#include <QPainter>

s_ddisabled::s_ddisabled(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* s_ddisabled::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(parent);
    Q_UNUSED(option);
    Q_UNUSED(index);
    return 0;
}

void s_ddisabled::paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const
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
