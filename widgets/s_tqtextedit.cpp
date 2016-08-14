#include "s_tqtextedit.h"

s_tqTextEdit::s_tqTextEdit(QWidget *parent) : QTextEdit(parent)
{
    setStyleSheet("QTextEdit {background-color: transparent; border:0px solid black;");
}

s_tqTextEdit::s_tqTextEdit(const QString &text, QWidget *parent) : QTextEdit(parent)
{
    setStyleSheet("QTextEdit {background-color: transparent; border:0px solid black;");
    setText(text);
}
