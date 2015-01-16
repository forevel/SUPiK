#include "../inc/s_tqtreeview.h"
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>

s_tqtreeview::s_tqtreeview(QWidget *parent) :
    QTreeView(parent)
{
    s_tqtreeviewstyle *style = new s_tqtreeviewstyle;
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QTreeView {background-color: rgba(0,0,0,0);}");
    setFrameStyle(QFrame::NoFrame);
    setStyle(style);
    QSizePolicy fix(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setSizePolicy(fix);
    datachangedintable = false;
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(setTVexpanded(QModelIndex)));
}

s_tqtreeviewstyle::s_tqtreeviewstyle() : QProxyStyle()
{

}

void s_tqtreeviewstyle::drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    if (pe != QStyle::PE_IndicatorBranch)
        QProxyStyle::drawPrimitive(pe, opt, p, w);
}

void s_tqtreeview::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqtreeview::getAData()
{
    return this->adata;
}

void s_tqtreeview::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);
    datachangedintable = true;
    emit datachanged();
}

void s_tqtreeview::updateTVGeometry()
{
    datachangedintable = true;
    update();
    updateGeometry();
}

QSize s_tqtreeview::minimumSizeHint() const
{
    if (datachangedintable)
    {
        int f1 = 0;
        int curwidth = QApplication::desktop()->screenGeometry(this).width();
        int curheight = QApplication::desktop()->screenGeometry(this).height();
        for (int i = 0; i < 15; i++)
            f1 += 20;
        if (f1 > curheight)
            f1 = curheight;
        int f2 = 0;
        for (int i = 0; i < this->header()->count(); i++)
            f2 += this->columnWidth(i)+20;
        if (f2 > curwidth)
            f2 = curwidth;
        datachangedintable = false;
//        return QSize(f2, f1);
        return QSize(f2, f1);
    }
    else
        return this->size();
}

void s_tqtreeview::paintEvent(QPaintEvent *e)
{
    setFixedSize(minimumSizeHint());
    e->accept();
    QTreeView::paintEvent(e);
}

void s_tqtreeview::setTVexpanded(QModelIndex index)
{
    if (!index.column())
    {
        if (isExpanded(index))
            setExpanded(index, false);
        else
            setExpanded(index, true);
    }
    for (int i = 0; i < header()->count(); i++)
        resizeColumnToContents(i);
}
