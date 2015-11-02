#ifndef SYS_SETTINGSDIALOG_H
#define SYS_SETTINGSDIALOG_H

#include <QDialog>
#include <QIcon>
#include "../../gen/publicclass.h"
#include "../../gen/publiclang.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqspinbox.h"

QT_BEGIN_NAMESPACE
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QFont;
class QFrame;
QT_END_NAMESPACE

#define SYS_BGCOLOR "#FF6666"

class sys_settingsdialog : public QDialog
{
    Q_OBJECT
    
public:
    QString PathToLibs, SQLPath, PathToSupik, Lang;
    double TimerPeriod;
    explicit sys_settingsdialog(QWidget *parent=0);
    void SetupUI ();

private slots:
    void LangChoosed(QString);
    void TimerPeriodChoosed(double);
    void CancelPBClicked();
    void OKPBClicked();
    void ChooseDirPBClicked();
    void ChooseSDirPBClicked();

private:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // SYS_SETTINGSDIALOG_H
