#include "treeview.h"
#include "../models/treemodel.h"
#include "../models/proxymodel.h"
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>
#include <QScrollBar>

TreeView::TreeView(int Mode, int Proxy, bool HeaderVisible, QWidget *parent) :
    QTableView(parent)
{
    IsProxyModel = (Proxy == TV_PLAIN) ? false : true;
    horizontalScrollBar()->setStyleSheet("QScrollBar {background-color: rgba(0,0,0,0);}");
    verticalScrollBar()->setStyleSheet("QScrollBar {background-color: rgba(0,0,0,0);}");
//    horizontalScrollBar()->setAttribute(Qt::WA_TranslucentBackground, true);
//    verticalScrollBar()->setAttribute(Qt::WA_TranslucentBackground, true);
    horizontalHeader()->setVisible(HeaderVisible);
    horizontalHeader()->setStyleSheet("QHeaderView {background-color: transparent};"\
                                      "QHeaderView::section {background-color: transparent};");
    verticalHeader()->setVisible(false);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFrameStyle(QFrame::NoFrame);
    setShowGrid(false);
    if (Mode == TV_EXPANDABLE)
        connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(setTVexpanded(QModelIndex)));
}

void TreeView::setTVexpanded(QModelIndex index)
{
    if (index.column() != 0)
        return;
    if (IsProxyModel)
    {
        ProxyModel *pmdl = static_cast<ProxyModel *>(this->model());
        TreeModel *mdl = static_cast<TreeModel *>(pmdl->sourceModel());
        QModelIndex idx = pmdl->mapToSource(index);
        if (idx.isValid())
            mdl->GoIntoIndex(idx);
    }
    else
    {
        TreeModel *model = static_cast<TreeModel *>(this->model());
        model->GoIntoIndex(index);
    }
//    resizeColumnsToContents();
    resizeRowsToContents();
}

void TreeView::SetTreeType(int Type)
{
    if (Type == TV_EXPANDABLE)
        connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(setTVexpanded(QModelIndex)));
    else
        disconnect(this, SIGNAL(clicked(QModelIndex)));
}

void TreeView::SetColumnWidthInPercent(int column, int percent)
{
    if (column < model()->columnCount())
    {
        while (Percents.size() <= column)
            Percents.append(0);
        Percents.replace(column, percent);
    }
}
