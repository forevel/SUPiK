#ifndef WD_FUNC
#define WD_FUNC

#include <QWidget>
#include "s_tqcombobox.h"
#include "s_tqspinbox.h"
#include "s_tqcheckbox.h"
#include "s_tqlineedit.h"
#include "s_tqlabel.h"

class WDFunc
{
public:
    WDFunc();
    static void SetCWData(QWidget *w, const QString &cwname, const QString &cwvalue);
    static QString CWData(QWidget *w, const QString &cwname);
    static void SetCWColor(QWidget *w, const QString &cwname, const QColor &color);
    static s_tqLineEdit *NewLE(QWidget *w, const QString &lename, const QString &letext="", const QString &lecolor="");
    static void SetLEData(QWidget *w, const QString &lename, const QString &levalue, bool enabled=true);
    static void LEData(QWidget *w, const QString &lename, QString &levalue);
    static void SetTEData(QWidget *w, const QString &tename, const QString &tetext);
    static void TEData(QWidget *w, const QString &tename, QString &tevalue);
    static s_tqComboBox *NewCB(QWidget *parent, const QString &cbname, const QStringList &cbsl=QStringList(), const QString &cbcolor="");
    static void SetCBList(QWidget *w, const QString &cbname, const QStringList &cbsl=QStringList());
    static void SetCBData(QWidget *w, const QString &cbname, const QString &cbvalue);
    static void SetCBIndex(QWidget *w, const QString &cbname, int index);
    static void CBData(QWidget *w, const QString &cbname, QString &cbvalue);
    static void CBIndex(QWidget *w, const QString &cbname, int &index);
    static s_tqSpinBox *NewSPB(QWidget *parent, const QString &spbname, double min, double max, double step, int decimals, const QString &spbcolor="");
    static void SetSPBData(QWidget *w, const QString &spbname, const double &spbvalue);
    static void SPBData(QWidget *w, const QString &spbname, double &spbvalue);
    static void SetLEColor(QWidget *w, const QString &lename, const QColor &color);
    static s_tqLabel *NewLBL(QWidget *parent, const QString &lblname, const QString &txt, QPixmap *pm=Q_NULLPTR);
    static void SetLBLImage(QWidget *w, const QString &lblname, QPixmap *pm);
    static void SetLBLText(QWidget *w, const QString &lblname, const QString &lbltext, bool enabled=true);
    static QString TVField(QWidget *w, const QString &tvname, int column, bool isid=false);
    static void TVAutoResize(QWidget *w, const QString &tvname);
    static s_tqCheckBox *NewChB(QWidget *parent, const QString &chbname, const QString &chbtext, const QString &chbcolor="");
    static void ChBData(QWidget *w, const QString &chbname, bool &data);
    static void SetChBData(QWidget *w, const QString &chbname, bool data);
    static void SetEnabled(QWidget *w, const QString &wname, bool enabled=true);
};

#endif // WD_FUNC

