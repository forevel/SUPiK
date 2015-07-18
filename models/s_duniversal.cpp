#include "s_duniversal.h"
#include "../widgets/s_tqchoosewidget.h"
#include <QPainter>
#include <QRect>

s_duniversal::s_duniversal(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* s_duniversal::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QString links = index.data(Qt::UserRole).toString();
    ff = pc.getFFfromLinks(links);
    if ((ff.delegate == FD_DISABLED) || (ff.delegate == FD_SIMPLE) || (ff.delegate == FD_SIMGRID))
        return 0;
    QString hdr=index.data(Qt::UserRole+1).toString(); // в UserRole+1 должна содержаться aData, в которой находится подзаголовок диалога редактирования, вызываемого по кнопке в делегате
    wdgt = new s_tqChooseWidget(false,parent);
    wdgt->Setup(links,hdr);
    connect(wdgt,SIGNAL(error(int,int)),this,SIGNAL(error(int,int)));
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
