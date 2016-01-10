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
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFrameStyle(QFrame::NoFrame);
    setShowGrid(false);
    horizontalScrollBar()->setStyleSheet("QScrollBar {background-color: rgba(0,0,0,0);}");
    verticalScrollBar()->setStyleSheet("QScrollBar {background-color: rgba(0,0,0,0);}");
    horizontalScrollBar()->setAttribute(Qt::WA_TranslucentBackground, true);
    verticalScrollBar()->setAttribute(Qt::WA_TranslucentBackground, true);
/*    horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: rgba(0,0,0,0);}");
    horizontalHeader()->setAttribute(Qt::WA_TranslucentBackground, true);
    verticalHeader()->setStyleSheet("QHeaderView::section {background-color: rgba(0,0,0,0);}");
    verticalHeader()->setAttribute(Qt::WA_TranslucentBackground, true); */
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);
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
