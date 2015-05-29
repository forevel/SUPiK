#include "s_tqsplitter.h"

s_tqSplitter::s_tqSplitter(QWidget *parent) : QSplitter(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QSplitter {background-color: rgba(0,0,0,0);}");
}
