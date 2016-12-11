#include "wd_func.h"
#include "s_tqlineedit.h"
#include "s_tqlabel.h"
#include "s_tqchoosewidget.h"
#include <QPalette>

bool WDFunc::SetCWData(QWidget *w, const QString &cwname, const QString &cwvalue)
{
    s_tqChooseWidget *cw = w->findChild<s_tqChooseWidget *>(cwname);
    if (cw == 0)
        return false;
    cw->SetValue(cwvalue);
    return true;
}

QString WDFunc::CWData(QWidget *w, const QString &cwname)
{
    s_tqChooseWidget *cw = w->findChild<s_tqChooseWidget *>(cwname);
    if (cw == 0)
        return QString();
    return cw->Value();
}

bool WDFunc::SetCWColor(QWidget *w, const QString &cwname, const QColor &color)
{
    s_tqChooseWidget *cw = w->findChild<s_tqChooseWidget *>(cwname);
    if (cw == 0)
        return false;
    // http://forum.sources.ru/index.php?showtopic=313950
    QPalette pal = cw->palette();
    pal.setColor(QPalette::Window, color);
    cw->setPalette(pal);
    return true;
}

bool WDFunc::SetLEData(QWidget *w, const QString &lename, const QString &levalue)
{
    s_tqLineEdit *le = w->findChild<s_tqLineEdit *>(lename);
    if (le == 0)
        return false;
    le->setText(levalue);
    return true;
}

QString WDFunc::LEData(QWidget *w, const QString &lename)
{
    s_tqLineEdit *le = w->findChild<s_tqLineEdit *>(lename);
    if (le == 0)
        return QString();
    return le->text();
}

bool WDFunc::SetLEColor(QWidget *w, const QString &lename, const QColor &color)
{
    s_tqLineEdit *le = w->findChild<s_tqLineEdit *>(lename);
    if (le == 0)
        return false;
    // http://forum.sources.ru/index.php?showtopic=313950
    QPalette pal = le->palette();
    pal.setColor(QPalette::Text, color);
    le->setPalette(pal);
    return true;
}

bool WDFunc::SetLBLImage(QWidget *w, const QString &lblname, QPixmap *pm)
{
    s_tqLabel *lbl = w->findChild<s_tqLabel *>(lblname);
    if (lbl == 0)
        return false;
    lbl->setPixmap(*pm);
    return true;
}
