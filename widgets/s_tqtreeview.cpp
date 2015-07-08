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
    if (!index.column())
    {
        if (isExpanded(index))
            setExpanded(index, false);
        else
            setExpanded(index, true);
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
