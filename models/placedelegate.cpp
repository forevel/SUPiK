#include "placedelegate.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqwidget.h"

PlaceDelegate::PlaceDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget* s_duniversal::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    s_tqWidget *wdgt = new s_tqWidget(parent);
    s_tqPushButton *pb = new s_tqPushButton("Состав размещения");
    s_tqPushButton *pb = new s_tqPushButton("Компоненты, находящиеся в размещении");
    return wdgt;
}

void s_duniversal::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if ((ff.delegate == FD_DISABLED) || (ff.delegate == FD_SIMPLE) || (ff.delegate == FD_SIMGRID))
        return;
    s_tqChooseWidget *wdgt = static_cast<s_tqChooseWidget *>(editor);
    connect(wdgt,SIGNAL(textchanged(QVariant)),this,SLOT(CommitChanges(QVariant)));
    wdgt->SetData(index.data(Qt::EditRole));
}

void s_duniversal::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if ((ff.delegate == FD_DISABLED) || (ff.delegate == FD_SIMPLE) || (ff.delegate == FD_SIMGRID))
        return;
    s_tqChooseWidget *wdgt = static_cast<s_tqChooseWidget *>(editor);
    model->setData(index,wdgt->Data(),Qt::EditRole);
}

void s_duniversal::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->save();
    if (index.isValid())
    {
        QRect rct = option.rect;
        PublicClass::fieldformat tmpff = pc.getFFfromLinks(index.data(Qt::UserRole).toString());
        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(Qt::SolidLine);
            painter->setPen(QColor(Qt::darkGreen));
            painter->drawRoundedRect(rct, 3, 3);
            painter->restore();
            return;
        }
        if (tmpff.delegate != FD_SIMPLE)
        {
            painter->setPen(Qt::SolidLine);
            painter->setPen(QColor(Qt::darkGray));
            painter->drawRoundedRect(rct, 3, 3);
        }
    }
    painter->restore();
}

void s_duniversal::CommitChanges(QVariant v)
{
    Q_UNUSED(v);
    emit commitData(wdgt);
    emit closeEditor(wdgt);
}
