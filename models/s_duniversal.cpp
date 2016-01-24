#include "s_duniversal.h"
#include <QPainter>
#include <QRect>

s_duniversal::s_duniversal(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* s_duniversal::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QString links = index.data(TreeModel::LinksRole).toString();
    ff = pc.getFFfromLinks(links);
    if ((ff.delegate == FD_DISABLED) || (ff.delegate == FD_SIMPLE) || (ff.delegate == FD_SIMGRID))
    {
        return 0;
    }
    QString hdr=index.data(TreeModel::HeaderTextRole).toString(); // в UserRole+1 должна содержаться aData, в которой находится подзаголовок диалога редактирования, вызываемого по кнопке в делегате
    wdgt = new s_tqChooseWidget(false,parent);
    wdgt->Setup(links,hdr);
    return wdgt;
}

void s_duniversal::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if ((ff.delegate == FD_DISABLED) || (ff.delegate == FD_SIMPLE) || (ff.delegate == FD_SIMGRID))
        return;
    s_tqChooseWidget *wdgt = static_cast<s_tqChooseWidget *>(editor);
    connect(wdgt,SIGNAL(textchanged(QVariant)),this,SLOT(CommitChanges(QVariant)));
    PublicClass::ValueStruct vs;
    vs.Value = index.data(Qt::EditRole).toString();
    QStringList tmpsl = index.data(TreeModel::CellInfoRole).toString().split(".");
    if (tmpsl.size()<2)
        vs.Type = VS_STRING;
    else if (tmpsl.at(0) == "i")
        vs.Type = VS_ICON;
    else
        vs.Type = VS_STRING;
    wdgt->SetData(vs);
}

void s_duniversal::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if ((ff.delegate == FD_DISABLED) || (ff.delegate == FD_SIMPLE) || (ff.delegate == FD_SIMGRID))
        return;
    s_tqChooseWidget *wdgt = static_cast<s_tqChooseWidget *>(editor);
    PublicClass::ValueStruct vs = wdgt->Data();
    if (vs.Type == VS_STRING)
        model->setData(index,vs.Value,Qt::EditRole);
    else if (vs.Type == VS_ICON)
    {
        model->setData(index,QIcon(vs.Value),Qt::DecorationRole);
        model->setData(index, vs.Value, TreeModel::CellInfoRole);
    }
}

void s_duniversal::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    painter->save();
    if (index.isValid())
    {
        QRect rct = option.rect;
        PublicClass::FieldFormat tmpff = pc.getFFfromLinks(index.data(Qt::UserRole).toString());
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
