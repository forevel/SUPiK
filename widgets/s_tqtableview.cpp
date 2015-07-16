#include "s_tqtableview.h"
#include <QDialog>
#include <QScrollBar>

s_tqTableView::s_tqTableView(QWidget *parent) :
    QTableView(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setStyleSheet("QTableView {background-color: rgba(0,0,0,0);}");
    setFrameStyle(QFrame::NoFrame);
    setShowGrid(false);
    horizontalScrollBar()->setStyleSheet("QScrollBar {background-color: rgba(0,0,0,0);}");
    verticalScrollBar()->setStyleSheet("QScrollBar {background-color: rgba(0,0,0,0);}");
}

void s_tqTableView::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqTableView::getAData()
{
    return this->adata;
}

void s_tqTableView::dataChanged()
{
    resizeColumnsToContents();
}
