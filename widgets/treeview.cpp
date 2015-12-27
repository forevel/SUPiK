#include "treeview.h"
#include "../models/treemodel.h"
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>
#include <QScrollBar>

TreeView::TreeView(QWidget *parent) :
    QTableView(parent)
{
//    TreeViewstyle *style = new TreeViewstyle;
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setStyleSheet("QTableView {background-color: rgba(0,0,0,0);}");
    setFrameStyle(QFrame::NoFrame);
    setShowGrid(false);
//    setStyle(style);
    horizontalScrollBar()->setStyleSheet("QScrollBar {background-color: rgba(0,0,0,0);}");
    verticalScrollBar()->setStyleSheet("QScrollBar {background-color: rgba(0,0,0,0);}");
    horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: rgba(0,0,0,0);}");
    horizontalHeader()->setAttribute(Qt::WA_TranslucentBackground, true);
    verticalHeader()->setStyleSheet("QHeaderView::section {background-color: rgba(0,0,0,0);}");
    verticalHeader()->setAttribute(Qt::WA_TranslucentBackground, true);
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(setTVexpanded(QModelIndex)));
}

void TreeView::setTVexpanded(QModelIndex index)
{
    if (index.column() != 0)
        return;
    TreeModel *model = static_cast<TreeModel *>(this->model());
    model->GoIntoIndex(index);
    ResizeColumnsToContents();
}

void TreeView::ResizeColumnsToContents()
{
    resizeColumnsToContents();
}

TreeViewstyle::TreeViewstyle() : QProxyStyle()
{

}

void TreeViewstyle::drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    if (pe != QStyle::PE_IndicatorBranch)
        QProxyStyle::drawPrimitive(pe, opt, p, w);
}
