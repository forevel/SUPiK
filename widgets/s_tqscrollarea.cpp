#include "s_tqscrollarea.h"

s_tqScrollArea::s_tqScrollArea(QWidget *parent) : QScrollArea(parent)
{
    setStyleSheet("QScrollArea {background-color: rgba(0,0,0,0);}");
}

