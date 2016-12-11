#ifndef WD_FUNC
#define WD_FUNC

#include <QWidget>

class WDFunc
{
public:
    WDFunc();
    static bool SetCWData(QWidget *w, const QString &cwname, const QString &cwvalue);
    static QString CWData(QWidget *w, const QString &cwname);
    static bool SetCWColor(QWidget *w, const QString &cwname, const QColor &color);
    static bool SetLEData(QWidget *w, const QString &lename, const QString &levalue);
    static QString LEData(QWidget *w, const QString &lename);
    static bool SetLEColor(QWidget *w, const QString &lename, const QColor &color);
    static bool SetLBLImage(QWidget *w, const QString &lblname, QPixmap *pm);
};

#endif // WD_FUNC

