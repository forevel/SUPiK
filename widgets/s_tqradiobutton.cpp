#include "s_tqradiobutton.h"

s_tqRadioButton::s_tqRadioButton(const QString &text, QWidget *parent) : QRadioButton(parent)
{
    setStyleSheet("QRadioButton {background-color: rgba(0,0,0,0);}");
//    setStyleSheet("QRadioButton {background-color: rgba(0,0,0,0); border: 1px solid gray; border-radius: 5px; border-style: outset; padding: 2px 5px;}");
    setText(text);
}
