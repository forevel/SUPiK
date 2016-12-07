#include "wgenfunc.h"
#include "s_tqlineedit.h"
#include "s_tqlabel.h"

bool SetLEData(QWidget *w, const QString &lename, const QString &levalue)
{
    s_tqLineEdit *le = w->findChild<s_tqLineEdit *>(lename);
    if (le == 0)
        return false;
    le->setText(levalue);
    return true;
}

bool SetLBLImage(QWidget *w, const QString &lblname, QPixmap *pm)
{
    s_tqLabel *lbl = w->findChild<s_tqLabel *>(lblname);
    if (lbl == 0)
        return false;
    lbl->setPixmap(*pm);
    return true;
}
