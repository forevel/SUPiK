#include "wd_func.h"
#include "s_tqlineedit.h"
#include "s_tqlabel.h"
#include "s_tqchoosewidget.h"
#include "s_tqtableview.h"
#include "s_tqcheckbox.h"
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

QString WDFunc::TVField(QWidget *w, const QString &tvname, int column, bool isid)
{
    s_tqTableView *tv = w->findChild<s_tqTableView *>(tvname);
    if (tv == 0)
        return QString();
    QString tmps = tv->model()->data(tv->model()->index(tv->currentIndex().row(),column,QModelIndex()),Qt::DisplayRole).toString();
    if (isid) // если поле с ИД, надо убрать первую цифру - номер таблицы и разделяющую точку, если они присутствуют
    {
        QStringList sl = tmps.split(".");
        if (sl.size() > 1) // есть номер таблицы
            tmps = sl.at(1);
        bool ok;
        int tmpi = tmps.toInt(&ok);
        if (!ok)
            return QString();
        tmps = QString::number(tmpi); // убираем старшие незначащие нули
    }
    return tmps;
}

void WDFunc::TVAutoResize(QWidget *w, const QString &tvname)
{
    s_tqTableView *tv = w->findChild<s_tqTableView *>(tvname);
    if (tv == 0)
        return;
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();
}

bool WDFunc::ChBData(QWidget *w, const QString &chbname)
{
    s_tqCheckBox *chb = w->findChild<s_tqCheckBox *>(chbname);
    if (chb == 0)
        return false;
    return chb->isChecked();
}

bool WDFunc::SetChBData(QWidget *w, const QString &chbname, bool data)
{
    s_tqCheckBox *chb = w->findChild<s_tqCheckBox *>(chbname);
    if (chb == 0)
        return false;
    chb->setChecked(data);
    return true;
}
