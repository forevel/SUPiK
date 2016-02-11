#include "treeview.h"
#include "../models/treemodel.h"
#include "../models/proxymodel.h"
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>
#include <QScrollBar>

TreeView::TreeView(int Proxy, bool HeaderVisible, QWidget *parent) :
    QTableView(parent)
{
    IsProxyModel = (Proxy == TV_PLAIN) ? false : true;
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setFrameStyle(QFrame::NoFrame);
    setStyleSheet("QTableView {background-color: rgba(0,0,0,0);}");
    setShowGrid(false);
/*    horizontalScrollBar()->setStyleSheet("QScrollBar {background-color: rgba(0,0,0,0);}");
    verticalScrollBar()->setStyleSheet("QScrollBar {background-color: rgba(0,0,0,0);}");
    horizontalScrollBar()->setAttribute(Qt::WA_TranslucentBackground, true);
    verticalScrollBar()->setAttribute(Qt::WA_TranslucentBackground, true); */
    horizontalHeader()->setVisible(HeaderVisible);
    verticalHeader()->setVisible(false);
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
    resizeColumnsToContents();
    resizeRowsToContents();
}
