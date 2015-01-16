#include "../inc/s_dspinbox.h"
#include <QRect>
#include <QVariant>
#include <QSpinBox>
#include <QPainter>

s_dspinbox::s_dspinbox(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* s_dspinbox::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QSpinBox *sb = new QSpinBox(parent);
    sb->setStyleSheet("QSpinBox {background: khaki};");
    sb->setMinimum(0);
    sb->setMaximum(99999);
    return sb;
}

void s_dspinbox::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSpinBox *sb = static_cast<QSpinBox *>(editor);
    sb->setValue(index.data(Qt::EditRole).toInt(0));
}

void s_dspinbox::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *sb = static_cast<QSpinBox *>(editor);
    model->setData(index, QVariant(sb->value()), Qt::EditRole);
}

void s_dspinbox::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid())
    {
        painter->setPen(Qt::SolidLine);
        painter->setPen(QColor(Qt::darkGray));

//        painter->drawRect(option.rect);
        QRect rct = option.rect;
        int lf = rct.left();
        int rg = lf + rct.width()-1;
        int tp = rct.top();
        int bt = tp + rct.height();
        painter->drawLine(QLine(QPoint(lf, bt), QPoint(rg, bt))); // bottomLeft - bottomRight
        painter->drawLine(QLine(QPoint(lf, tp), QPoint(rg, tp))); // topLeft - topRight
        painter->drawLine(QLine(QPoint(rg, tp), QPoint(rg, bt))); // topRight - bottomRight
        painter->drawLine(QLine(QPoint(lf, tp), QPoint(lf, bt))); // topLeft - bottomLeft
    }
    QStyledItemDelegate::paint(painter, option, index);
}
