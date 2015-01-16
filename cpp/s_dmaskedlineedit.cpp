#include "../inc/s_dmaskedlineedit.h"
#include "../inc/s_moneylineedit.h"
#include <QPainter>

s_dmaskedlineedit::s_dmaskedlineedit(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* s_dmaskedlineedit::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    s_moneylineedit *le;
    if (index.data(Qt::UserRole).toString().left(3) == "6.m")
        le = new s_moneylineedit(MONEY, parent);
    else if (index.data(Qt::UserRole).toString().left(3) == "6.n")
        le = new s_moneylineedit(INTEG2, parent);
    else
        le = new s_moneylineedit(UNKNOWN, parent);
    le->setStyleSheet("QLineEdit {background: khaki};");
    return le;
}

void s_dmaskedlineedit::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    s_moneylineedit *le = static_cast<s_moneylineedit *>(editor);
    le->setText(index.data(Qt::EditRole).toString());
}

void s_dmaskedlineedit::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    s_moneylineedit *le = static_cast<s_moneylineedit *>(editor);
    model->setData(index, le->text(), Qt::EditRole);
}

void s_dmaskedlineedit::paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const
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
