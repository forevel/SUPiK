#include "wgenfunc.h"
#include "s_tqlineedit.h"

bool SetLEData(QWidget *w, const QString &lename, const QString &levalue)
{
    s_tqLineEdit *le = w->findChild<s_tqLineEdit *>(lename);
    if (le == 0)
        return false;
    le->setText(levalue);
}

