#include "s_tqtreeview.h"
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>

s_tqTreeView::s_tqTreeView(QWidget *parent) :
    QTreeView(parent)
{
    s_tqTreeViewstyle *style = new s_tqTreeViewstyle;
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QTreeView {background-color: rgba(0,0,0,0);}");
    setFrameStyle(QFrame::NoFrame);
    setStyle(style);
//    QSizePolicy fix(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    setSizePolicy(fix);
//    datachangedintable = false;
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(setTVexpanded(QModelIndex)));
}

s_tqTreeViewstyle::s_tqTreeViewstyle() : QProxyStyle()
{

}

void s_tqTreeViewstyle::drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    if (pe != QStyle::PE_IndicatorBranch)
        QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void s_tqTreeView::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqTreeView::getAData()
{
    return this->adata;
}

/*void s_tqTreeView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);
//    datachangedintable = true;
    emit datachanged();
}*/

/*void s_tqTreeView::updateTVGeometry()
{
//    datachangedintable = true;
//    update();
//    updateGeometry();
}*/

/*QSize s_tqTreeView::minimumSizeHint()
{
    if (datachangedintable)
    {
        int f1 = 0;
        int f2 = 0;
        int i;
        for (i = 0; i < header()->count(); i++)
        {
            f2 += this->columnWidth(i);
            resizeColumnToContents(i);
        }
        int curwidth = static_cast<QDialog *>(parent())->geometry().width()-10;
        int curheight = static_cast<QDialog *>(parent())->geometry().height()-60;
        if (isTree)
        {
            f1 = shown_rows * 20;
        }
        else
        {
            for (i = 0; i < model()->rowCount(); i++)
            f1 += 20;
        }
        if (f1 > curheight)
            f1 = curheight;
        if (f2 > curwidth)
            f2 = curwidth;
        datachangedintable = false;
        return QSize(f2, f1);
    }
    else
        return this->size();
} */

/*void s_tqTreeView::paintEvent(QPaintEvent *e)
{
//    setFixedSize(minimumSizeHint());
    QTreeView::paintEvent(e);
    e->accept();
}*/

void s_tqTreeView::setTVexpanded(QModelIndex index)
{
    if (!index.column())
    {
        if (isExpanded(index))
        {
//            shown_rows -= getVisibleChildsCount(static_cast<s_ntmodel *>(model())->getItem(index));
//            recalculateGeometry();
            setExpanded(index, false);
        }
        else
        {
//            shown_rows += getVisibleChildsCount(static_cast<s_ntmodel *>(model())->getItem(index));
//            recalculateGeometry();
            setExpanded(index, true);
        }
    }
//    updateTVGeometry();
    for (int i = 0; i < header()->count(); i++)
        resizeColumnToContents(i);
}

void s_tqTreeView::ResizeColumnsToContents()
{
    for (int i = 0; i < header()->count(); i++)
        resizeColumnToContents(i);
}

/*int s_tqTreeView::getVisibleChildsCount(s_ntitem *item)
{
// нужно вычислить, сколько всего открытых потомков у узла, до уровня листьев дерева
    int childs_count = item->childCount();
    int hidden_childs = childs_count;
    for (int i=0;i<childs_count;i++)
    {
        s_ntitem *ti = item->child(i);
        QModelIndex idx = static_cast<s_ntmodel *>(model())->getIndex(ti);
        if (isExpanded(idx))
            hidden_childs += getVisibleChildsCount(ti);
    }
    return hidden_childs;
}

//пересчёт размера
void s_tqTreeView::recalculateGeometry()
{
    int curheight = static_cast<QDialog *>(parent())->geometry().height();
    int new_height = qMin(curheight, shown_rows*15);
    setMinimumHeight(new_height);
    setMaximumHeight(new_height);
}

void s_tqTreeView::setShownRows(int rowcount)
{
    shown_rows = rowcount;
} */
