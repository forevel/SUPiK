#include "../inc/s_maskedle.h"
#include <QLocale>
#include <QRegExp>
#include <QValidator>

s_maskedle::s_maskedle(QString regexp, QWidget *parent) :
    QLineEdit(parent)
{
    QRegExp re;
    re.setPattern(regexp);
    QValidator *val = new QRegExpValidator(re, this);
    setValidator(val);
}

void s_maskedle::keyPressEvent(QKeyEvent *e)
{
/*    QLocale german(QLocale::German);
    float tmpf = german.toFloat(text());
    if (e->key() >= Qt::Key_0 && e->key() <= Qt::Key_9)
    {
        tmpf = tmpf * 10 + static_cast<int>(e->key()-0x30)*0.01;
        setText(german.toString(tmpf, 'f', 2));
    }
    else if (e->key() == Qt::Key_Backspace)
    {
        tmpf = static_cast<int>(tmpf / 10);
        setText(german.toString(tmpf, 'f', 2));
    }
    else */
    {
        QLineEdit::keyPressEvent(e);
    }
}
/*
bool s_maskedle::event(QEvent *e)
{
    if(e->type() == QEvent::KeyPress)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);
        if (ke->key() == Qt::Key_Tab)
        {
            emit editingFinished();
            this->focusNextChild();
            return true;
        }
    }
    return QLineEdit::event(e);
}
*/
