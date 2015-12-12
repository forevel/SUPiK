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
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(setTVexpanded(QModelIndex)));
    setExpandsOnDoubleClick(false);
    RootIndexes.clear();
}

void s_tqTreeView::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqTreeView::getAData()
{
    return this->adata;
}

void s_tqTreeView::setTVexpanded(QModelIndex index)
{
    if (index.column() != 0)
        return;
    if (isExpanded(index))
    {
        QModelIndex parent;
        if (!RootIndexes.isEmpty())
            parent = RootIndexes.pop();
        for (int i = 0; model()->index(i, 0, parent).isValid(); i++)
            setRowHidden(i, parent, false);
        if (!RootIndexes.isEmpty())
            parent = RootIndexes.top();
        else
            parent = QModelIndex();
        setRootIndex(parent);
        setExpanded(index, false);
    }
    else
    {
        if (index.child(0,0).isValid())
        {
            setExpanded(index, true);
            QModelIndex parent = index.parent();
            if (!parent.isValid())
                parent = rootIndex();
            RootIndexes.push(parent);
            setRootIndex(parent);
            int indexrow = index.row();
            for (int i = 0; model()->index(i,0,parent).isValid(); i++)
            {
                if (i != indexrow)
                    setRowHidden(i, parent, true);
            }
        }
    }
    ResizeColumnsToContents();
}

void s_tqTreeView::ResizeColumnsToContents()
{
    for (int i = 0; i < header()->count(); i++)
        resizeColumnToContents(i);
}

s_tqTreeViewstyle::s_tqTreeViewstyle() : QProxyStyle()
{

}

void s_tqTreeViewstyle::drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    if (pe != QStyle::PE_IndicatorBranch)
        QProxyStyle::drawPrimitive(pe, opt, p, w);
}
