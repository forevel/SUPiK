#ifndef S_TQTEXTEDIT_H
#define S_TQTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>

class s_tqTextEdit : public QTextEdit
{
public:
    s_tqTextEdit(QWidget *parent=0);
    s_tqTextEdit(const QString &text, QWidget *parent=0);
};

#endif // S_TQTEXTEDIT_H
