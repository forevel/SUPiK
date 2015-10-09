#include "placedelegate.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqwidget.h"

#include <QPainter>
#include <QPushButton>
#include <QApplication>
#include <QEvent>
#include <QKeyEvent>

PlaceDelegate::PlaceDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    Changed = false;
}

QWidget* PlaceDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
/*    QString PlaceID = index.data(Qt::UserRole).toString(); // достаём ИД размещения по БД whplaces
    s_tqWidget *wdgt = new s_tqWidget(parent);
    wdgt->setAData(PlaceID); // присваиваем данному виджету ИД размещения для обработки в слоте */
    s_tqPushButton* pb = new s_tqPushButton( index.data().toString(), parent );
    pb->setAutoDefault( true );
    pb->setAData(index.data(Qt::UserRole));
    connect( pb, SIGNAL( clicked(s_tqPushButton *) ), this, SLOT( buttonPressed(s_tqPushButton*)) );
    emit pb->clicked(pb);
    return pb;
/*    s_tqPushButton *pb = new s_tqPushButton("Состав размещения");
    s_tqPushButton *pb = new s_tqPushButton("Компоненты, находящиеся в размещении");
    return wdgt; */
}

void PlaceDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(index);
}

void PlaceDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(index);
    Q_UNUSED(model);
}

void PlaceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    painter->save();
    QStyleOptionButton opt;
    opt.rect = option.rect;
    opt.text = index.data().toString();
    if (QWidget *w = dynamic_cast<QWidget *>(painter->device()))
    {
        if (Changed)
            opt.state |= QStyle::State_Sunken;
        if (w->isEnabled())
            opt.state |= QStyle::State_Enabled;
        if (w->window()->isActiveWindow())
            opt.state |= QStyle::State_Active;
    }
    else
    {
        opt.state |= QStyle::State_Enabled;
    }
    QApplication::style()->drawControl(QStyle::CE_PushButton, &opt, painter);
//    painter->restore();
}

bool PlaceDelegate::eventFilter( QObject* o, QEvent* e )
{
    if (e->type() == QEvent::HoverEnter)
    {
        Changed = true;
    }
    if (e->type() == QEvent::HoverLeave)
        Changed = false;
    if( e->type() == QEvent::FocusOut )
    {
        return false;
    }
    return QStyledItemDelegate::eventFilter( o, e );
}

void PlaceDelegate::CommitChanges(QVariant v)
{
    Q_UNUSED(v);
}

void PlaceDelegate::buttonPressed(s_tqPushButton *ptr)
{
    Q_UNUSED(ptr);
    QString AData;
    s_tqPushButton *pb = static_cast<s_tqPushButton *>(sender());
    if (pb != 0)
        AData = pb->getAData().toString();
    AData = AData;
    return;
}
