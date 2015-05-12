#include "s_colortabbar.h"

S_ColorTabBar::S_ColorTabBar(QWidget *parent) :
    QTabBar(parent)
{
    setTabsClosable(true);
    cachedHeight = -1;
}

void S_ColorTabBar::paintEvent(QPaintEvent *event)
{
    QStylePainter p(this);
    QPainter S_painter;
    QColor S_color;
    int S_YEnd;

    S_painter.begin(this);

    for (int i = 0; i < count(); i++)
    {
        QStyleOptionTab option;
        QTabBar::initStyleOption(&option, i);


            QRect S_tabRect = tabRect(i);
            S_color = pc.TabColors[tabData(i).toString()];
            S_YEnd = 0;
            if (i != currentIndex()) // если неактивная вкладка
            {
                S_color = S_color.lighter(105); // подсвечиваем
                S_YEnd = 2;
            }

            QPainterPath roundRectPath;
            int S_CurrentWidthOfTab = this->tabSizeHint(i).width();
            int S_CurrentHeightOfTab = this->tabSizeHint(i).height();
            int S_XYDiag = S_CurrentWidthOfTab/50;
            int S_XBeg = 2;
            int S_XEnd = S_CurrentWidthOfTab-2;
            S_XBeg += i*S_CurrentWidthOfTab;
            S_XEnd += i*S_CurrentWidthOfTab;
            roundRectPath.moveTo(S_XBeg, S_CurrentHeightOfTab);
            roundRectPath.lineTo(S_XBeg, S_YEnd+S_XYDiag);
            roundRectPath.lineTo(S_XBeg+S_XYDiag, S_YEnd);
            roundRectPath.lineTo(S_XEnd, S_YEnd);
            roundRectPath.lineTo(S_XEnd, S_CurrentHeightOfTab);
            roundRectPath.lineTo(S_XBeg, S_CurrentHeightOfTab);
//            S_painter.setPen(S_color.darker(120));
            QBrush S_brush(S_color, Qt::SolidPattern);
            S_painter.fillPath(roundRectPath, S_brush);
            S_painter.setPen(Qt::black);
            S_painter.drawPath(roundRectPath);
            S_painter.setPen(Qt::black);
            S_painter.drawText(S_tabRect, Qt::AlignVCenter | Qt::AlignHCenter, \
                               fontMetrics().elidedText(tabText(i), Qt::ElideMiddle, S_tabRect.width()-20));
            if (i == currentIndex())
                S_tabRect.adjust(0, -1, 0, 1); // и подвыпячиваем
            else
                S_tabRect.adjust(0, 1, 0, -1);

//        }

    }
    S_painter.end();
    event->accept();
}

bool S_ColorTabBar::event(QEvent *event)
{
    if (event->type() == QEvent::Polish)
        cachedHeight = -1;
    return QTabBar::event(event);
}

QSize S_ColorTabBar::sizeHint() const
{
    if (count() > 0)
        return QSize(tabSizeHint(0).width()*count(), QTabBar::sizeHint().height());
    else
        return QSize(pc.S_TabWidgetWidth, QTabBar::sizeHint().height());
}

QSize S_ColorTabBar::tabSizeHint(int index) const
{
    if (cachedHeight == -1)
        cachedHeight = QTabBar::tabSizeHint(index).height();
    return QSize(qBound(50, pc.S_TabWidgetWidth/count(), 500), cachedHeight);
}
