#include "../inc/s_tqtreewidget.h"

s_tqTreeWidget::s_tqTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setFrameStyle(QFrame::NoFrame);
    header()->setVisible(false);
}
