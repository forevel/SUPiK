#include "s_tqtoolbar.h"

s_tqToolBar::s_tqToolBar(QWidget *parent) : QToolBar(parent)
{
    setStyleSheet("QToolBar {background-color: transparent; border: 1px solid gray; border-radius: 5px;}"\
                    "QToolButton {background-color: transparent; border: 1px solid gray; border-radius: 5px; padding: 2px 5px;}"\
                    "QToolButton:hover {border-style: outset; padding: 2px 5px;}"\
                    "QToolButton:pressed {border-style: inset; padding: 2px 5px;}");
}

