#include "wd_func.h"
#include "s_tqlabel.h"
#include "s_tqchoosewidget.h"
#include "treeview.h"
#include "s_tqtextedit.h"
#include <QPalette>
#include <QStringListModel>

void WDFunc::SetCWData(QWidget *w, const QString &cwname, const QString &cwvalue)
{
    s_tqChooseWidget *cw = w->findChild<s_tqChooseWidget *>(cwname);
    if (cw == 0)
    {
        DBGMSGT("Problem in CWData, name=" + cwname);
        return;
    }
    cw->SetValue(cwvalue);
}

void WDFunc::SetCWColor(QWidget *w, const QString &cwname, const QColor &color)
{
    s_tqChooseWidget *cw = w->findChild<s_tqChooseWidget *>(cwname);
    if (cw == 0)
    {
        DBGMSGT("Problem in SetCWColor, name=" + cwname);
        return;
    }
    // http://forum.sources.ru/index.php?showtopic=313950
    QPalette pal = cw->palette();
    pal.setColor(QPalette::Window, color);
    cw->setPalette(pal);
}

QString WDFunc::CWData(QWidget *w, const QString &cwname)
{
    s_tqChooseWidget *cw = w->findChild<s_tqChooseWidget *>(cwname);
    if (cw == 0)
    {
        DBGMSGT("Problem in CWData, name=" + cwname);
        return QString();
    }
    return cw->Value();
}

s_tqLineEdit *WDFunc::NewLE(QWidget *w, const QString &lename, const QString &letext, const QString &lecolor)
{
    s_tqLineEdit *le = new s_tqLineEdit(w);
    le->setObjectName(lename);
    le->setText(letext);
    if (!lecolor.isEmpty())
    {
        QString tmps = "QLineEdit {background-color: " + lecolor + ";}";
        le->setStyleSheet(tmps);
    }
    return le;
}

void WDFunc::SetLEData(QWidget *w, const QString &lename, const QString &levalue, bool enabled)
{
    s_tqLineEdit *le = w->findChild<s_tqLineEdit *>(lename);
    if (le == 0)
    {
        DBGMSGT("Problem in SetLEData, name=" + lename);
        return;
    }
    le->setText(levalue);
    le->setEnabled(enabled);
}

void WDFunc::SetLEColor(QWidget *w, const QString &lename, const QColor &color)
{
    s_tqLineEdit *le = w->findChild<s_tqLineEdit *>(lename);
    if (le == 0)
    {
        DBGMSGT("Problem in SetLEColor, name=" + lename);
        return;
    }
    // http://forum.sources.ru/index.php?showtopic=313950
    QPalette pal = le->palette();
    pal.setColor(QPalette::Text, color);
    le->setPalette(pal);
}

void WDFunc::LEData(QWidget *w, const QString &lename, QString &levalue)
{
    s_tqLineEdit *le = w->findChild<s_tqLineEdit *>(lename);
    if (le == 0)
    {
        DBGMSGT("Problem in LEData, name=" + lename);
        return;
    }
    levalue = le->text();
}

void WDFunc::SetTEData(QWidget *w, const QString &tename, const QString &tetext)
{
    s_tqTextEdit *te = w->findChild<s_tqTextEdit *>(tename);
    if (te == 0)
    {
        DBGMSGT("Problem in SetTEData, name=" + tename);
        return;
    }
    te->setText(tetext);
}

void WDFunc::TEData(QWidget *w, const QString &tename, QString &tevalue)
{
    s_tqTextEdit *te = w->findChild<s_tqTextEdit *>(tename);
    if (te == 0)
    {
        DBGMSGT("Problem in TEData, name=" + tename);
        return;
    }
    tevalue = te->toPlainText();
}

s_tqComboBox *WDFunc::NewCB(QWidget *parent, const QString &cbname, const QStringList &cbsl, const QString &cbcolor)
{
    s_tqComboBox *cb = new s_tqComboBox(parent);
    cb->setObjectName(cbname);
    if (!cbsl.isEmpty())
    {
        QStringListModel *cblm = new QStringListModel;
        cblm->setStringList(cbsl);
        cb->setModel(cblm);
    }
    if (!cbcolor.isEmpty())
    {
        QString tmps = "QComboBox {background-color: " + cbcolor + ";}";
        cb->setStyleSheet(tmps);
    }
    return cb;
}

void WDFunc::SetCBList(QWidget *w, const QString &cbname, const QStringList &cbsl)
{
    if (cbsl.isEmpty())
        return;
    s_tqComboBox *cb = w->findChild<s_tqComboBox *>(cbname);
    if (cb == 0)
    {
        DBGMSGT("Problem in CBData, name=" + cbname);
        return;
    }
    cb->clear();
    cb->insertItems(0, cbsl);
    cb->setCurrentIndex(0);
}

void WDFunc::CBData(QWidget *w, const QString &cbname, QString &cbvalue)
{
    s_tqComboBox *cb = w->findChild<s_tqComboBox *>(cbname);
    if (cb == 0)
    {
        DBGMSGT("Problem in CBData, name=" + cbname);
        return;
    }
    cbvalue = cb->currentText();
}

void WDFunc::CBIndex(QWidget *w, const QString &cbname, int &index)
{
    s_tqComboBox *cb = w->findChild<s_tqComboBox *>(cbname);
    if (cb == 0)
    {
        DBGMSGT("Problem in CBIndex, name=" + cbname);
        return;
    }
    index = cb->currentIndex();
}

void WDFunc::SetCBData(QWidget *w, const QString &cbname, const QString &cbvalue)
{
    s_tqComboBox *cb = w->findChild<s_tqComboBox *>(cbname);
    if (cb == 0)
    {
        DBGMSGT("Problem in CBData, name=" + cbname);
        return;
    }
    cb->setCurrentText(cbvalue);
}

void WDFunc::SetCBIndex(QWidget *w, const QString &cbname, int index)
{
    s_tqComboBox *cb = w->findChild<s_tqComboBox *>(cbname);
    if (cb == 0)
    {
        DBGMSGT("Problem in CBIndex, name=" + cbname);
        return;
    }
    cb->setCurrentIndex(index);
}

s_tqSpinBox *WDFunc::NewSPB(QWidget *parent, const QString &spbname, double min, double max, double step, int decimals, const QString &spbcolor)
{
    s_tqSpinBox *dspbls = new s_tqSpinBox(parent);
    dspbls->setObjectName(spbname);
    dspbls->setSingleStep(step);
    dspbls->setDecimals(decimals);
    dspbls->setMinimum(min);
    dspbls->setMaximum(max);
    if (!spbcolor.isEmpty())
    {
        QString tmps = "QDoubleSpinBox {background-color: " + spbcolor + ";}";
        dspbls->setStyleSheet(tmps);
    }
    return dspbls;
}

void WDFunc::SPBData(QWidget *w, const QString &spbname, double &spbvalue)
{
    s_tqSpinBox *spb = w->findChild<s_tqSpinBox *>(spbname);
    if (spb == 0)
    {
        DBGMSGT("Problem in SPBData, name=" + spbname);
        return;
    }
    spbvalue = spb->value();
}

void WDFunc::SetSPBData(QWidget *w, const QString &spbname, const double &spbvalue)
{
    s_tqSpinBox *spb = w->findChild<s_tqSpinBox *>(spbname);
    if (spb == 0)
    {
        DBGMSGT("Problem in SetSPBData, name=" + spbname);
        return;
    }
    spb->setValue(spbvalue);
}

void WDFunc::SetLBLImage(QWidget *w, const QString &lblname, QPixmap *pm)
{
    s_tqLabel *lbl = w->findChild<s_tqLabel *>(lblname);
    if (lbl == 0)
    {
        DBGMSGT("Problem in SetLBLImage, name=" + lblname);
        return;
    }
    lbl->setPixmap(*pm);
}

void WDFunc::SetLBLText(QWidget *w, const QString &lblname, const QString &lbltext, bool enabled)
{
    s_tqLabel *lbl = w->findChild<s_tqLabel *>(lblname);
    if (lbl == 0)
    {
        DBGMSGT("Problem in SetLBLText, name=" + lblname);
        return;
    }
    lbl->setText(lbltext);
    lbl->setEnabled(enabled);
}

QString WDFunc::TVField(QWidget *w, const QString &tvname, int column, bool isid)
{
    TreeView *tv = w->findChild<TreeView *>(tvname);
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
    TreeView *tv = w->findChild<TreeView *>(tvname);
    if (tv == 0)
        return;
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();
}

s_tqCheckBox *WDFunc::NewChB(QWidget *parent, const QString &chbname, const QString &chbtext, const QString &chbcolor)
{
    s_tqCheckBox *chb = new s_tqCheckBox(parent);
    chb->setObjectName(chbname);
    chb->setText(chbtext);
    if (!chbcolor.isEmpty())
    {
        QString tmps = "QCheckBox {background-color: "+chbcolor+";}";
        chb->setStyleSheet(tmps);
    }
    return chb;
}

void WDFunc::ChBData(QWidget *w, const QString &chbname, bool &data)
{
    s_tqCheckBox *chb = w->findChild<s_tqCheckBox *>(chbname);
    if (chb == 0)
    {
        DBGMSGT("Problem in ChBData, name=" + chbname);
        return;
    }
    data = chb->isChecked();
}

void WDFunc::SetChBData(QWidget *w, const QString &chbname, bool data)
{
    s_tqCheckBox *chb = w->findChild<s_tqCheckBox *>(chbname);
    if (chb == 0)
    {
        DBGMSGT("Problem in SetChBData, name=" + chbname);
        return;
    }
    chb->setChecked(data);
}

void WDFunc::SetEnabled(QWidget *w, const QString &wname, bool enabled)
{
    QWidget *wdgt = w->findChild<QWidget *>(wname);
    wdgt->setEnabled(enabled);
}
