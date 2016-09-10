#include "s_tqcombobox.h"

s_tqComboBox::s_tqComboBox(QWidget *parent) :
    QComboBox(parent)
{
    setStyleSheet("QComboBox {background-color: rgba(0,0,0,0); border: 1px solid gray; border-radius: 5px;}"
                  "QComboBox::drop-down {background-color: rgba(100,100,100,255); width: 5px; border-style: none;}"
                  "QComboBox QAbstractItemView {background-color: rgb(255,255,255); border: 1px solid darkgray; selection-background-color: lightgray;}");
}
