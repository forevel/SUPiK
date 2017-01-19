#include "s_tqradiobutton.h"
#include <QStylePainter>
#include <QStyleOption>

s_tqRadioButton::s_tqRadioButton(const QString &text, QWidget *parent) : QRadioButton(parent)
{
    setStyleSheet("QRadioButton {background-color: rgba(0,0,0,0);}");
//    setStyleSheet("QRadioButton {background-color: rgba(0,0,0,0); border: 1px solid gray; border-radius: 5px; border-style: outset; padding: 2px 5px;}");
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, QSizePolicy::RadioButton));

/*    QStyleOptionButton opt;
    initStyleOption(&opt);

    QRect label_rect = style()->subElementRect(QStyle::SE_RadioButtonContents, &opt, this);

    _label = new QLabel(this);
    _label->setWordWrap(true);
    _label->setMouseTracking(true);
    //_label->setMinimumHeight(label_rect.height());

    _layout = new QHBoxLayout(this);
    _layout->setContentsMargins(label_rect.left()+S_TQRADIOBUTTON_MARGIN, S_TQRADIOBUTTON_MARGIN/2, \
                                S_TQRADIOBUTTON_MARGIN/2, S_TQRADIOBUTTON_MARGIN/2);
    _layout->setSpacing(0);
    _layout->addWidget(_label);

    setLayout(_layout);*/
    QRadioButton::setText(text);
}

/*void s_tqRadioButton::setText(const QString & text)
{
    _label->setText(text);
    QRadioButton::setText(text);
}
*/
/*void s_tqRadioButton::paintEvent(QPaintEvent *e)
{
/*    QStylePainter p(this);
    QStyleOptionButton opt;
    initStyleOption(&opt);

    QStyleOptionButton subopt = opt;
    subopt.rect = style()->subElementRect(QStyle::SE_RadioButtonIndicator, &opt, this);
    subopt.rect.moveTop(opt.rect.top()+S_TQRADIOBUTTON_MARGIN/2); // align indicator to top

    style()->proxy()->drawPrimitive(QStyle::PE_IndicatorRadioButton, &subopt, &p, this);

    if (opt.state & QStyle::State_HasFocus)
    {
        QStyleOptionFocusRect fropt;
        fropt.QStyleOption::operator=(opt);
        fropt.rect = style()->subElementRect(QStyle::SE_RadioButtonFocusRect, &opt, this);
        style()->proxy()->drawPrimitive(QStyle::PE_FrameFocusRect, &fropt, &p, this);
    }*/
/*
    QStylePainter p(this);
    QStyleOptionButton opt;
    initStyleOption(&opt);
    p.setBrush(QBrush(Qt::lightGray));
    p.drawControl(QStyle::CE_RadioButton, opt);
    QRadioButton::paintEvent(e);
}

QSize s_tqRadioButton::sizeHint() const
{
    return QSize(); // will be calculated by layout
}

bool s_tqRadioButton::hitButton(const QPoint &pos) const
{
    QStyleOptionButton opt;
    initStyleOption(&opt);
    return opt.rect.contains(pos); // hit all button
}
*/
