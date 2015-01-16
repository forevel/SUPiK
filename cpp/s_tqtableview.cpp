#include "../inc/s_tqtableview.h"
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>

s_tqTableView::s_tqTableView(bool autoResize=false, QWidget *parent) :
    QTableView(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setStyleSheet("QTableView {background-color: rgba(0,0,0,0);}");
    setFrameStyle(QFrame::NoFrame);
    setShowGrid(false);
    QSizePolicy fix(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setSizePolicy(fix);
    datachangedintable = false;
    this->autoResize=autoResize;
    if (autoResize)
        connect(this,SIGNAL(datachanged()), this, SLOT(resizeColumnsToContents()));
}

void s_tqTableView::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqTableView::getAData()
{
    return this->adata;
}

void s_tqTableView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);
//    if (autoResize)
 //       resizeColumnsToContents();
    datachangedintable=true;
    emit datachanged();
}

QSize s_tqTableView::minimumSizeHint() const
{
    if (datachangedintable)
    {
        int f1 = 0;
        int curheight = static_cast<QDialog *>(parent())->geometry().height();
        for (int i = 0; i < this->verticalHeader()->count(); i++)
            f1 += this->rowHeight(i)+20;
        if (f1 > curheight)
            f1 = curheight;
        int f2 = 0;
        int curwidth = static_cast<QDialog *>(parent())->geometry().width();
        for (int i = 0; i < this->horizontalHeader()->count(); i++)
            f2 += this->columnWidth(i)+20;
        if (f2 > curwidth)
            f2 = curwidth;
        datachangedintable = false;
        return QSize(f2, f1);
    }
    else
        return this->size();
}

void s_tqTableView::paintEvent(QPaintEvent *e)
{
    setFixedSize(minimumSizeHint());
    e->accept();
    QTableView::paintEvent(e);
}
