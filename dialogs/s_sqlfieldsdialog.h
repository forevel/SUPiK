#ifndef S_SQLFIELDSDIALOG_H
#define S_SQLFIELDSDIALOG_H

// возможные типы полей
#define FT_PLAIN        1
#define FT_LINK         2
#define FT_INDIRECT     3
#define FT_RIGHTS       4
#define FT_METHOD       5
#define FT_EMLINK       6
#define FT_TBLINK       7

#include <QDialog>
#include <QSqlDatabase>
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqcombobox.h"
#include "../widgets/s_tqcheckbox.h"
#include "s_sqltreedialog.h"
#include "s_tablefilterdialog.h"
#include "s_accessdialog.h"

QT_BEGIN_NAMESPACE
class QGridLayout;
//class QVBoxLayout;
QT_END_NAMESPACE

class s_sqlfieldsdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_sqlfieldsdialog(QWidget *parent = 0);

    QGridLayout *FLLayout1, *FLLayout2, *GridLayout; // подчинённые layout с полями и значениями и главный Layout
    int SetupUI(QSqlDatabase db, QString tble, QString id, QString hdr);
    bool isSomethingChanged;

signals:
    void newidaccepted(int);

public slots:

private:
    QList<int> ftypes; // типы полей
    QStringList fdb, ftble, fid;
    s_tqPushButton *OkPB, *CancelPB;
    QSqlDatabase db;
    QString tble;
    QString id;
    QStringList sl, vl, fl, ll, sl1, sl2;
    s_sqltreedialog *sqlTreeDialog;
    s_tablefilterdialog *tabledialog;
    s_accessdialog *accessdialog;

    QSqlDatabase sdb;
    QString stble;
    int idx;

    int getLinkedTable (QString tble, QString &hdr, QStringList &sl1, QStringList &sl2);
    void addLineEdit (QString lbl, QString ledt, int idx, QGridLayout &lyout, int pos, bool isEnabled = true);
//  int addComboBox(QString lbl, QString ledt, int idx, QGridLayout &lyout, int pos, bool isEnabled = true);
    int addPushButtonWithDialog(QString ledt, int idx, QGridLayout &lyout);

private slots:
    void OkPBPressed();
    void CancelPBPressed();
//    void ComboBoxChanged(QString str, s_tqComboBox *ptr);
    void LineEditChanged(QString str, s_tqLineEdit *ptr);
    void PushButtonPressed(s_tqPushButton *ptr);
    void acceptChanges(QString str);
    void acceptChanges(long rights);
//    void prepareslsfortree(QStringList &sl1, QStringList &sl2);

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
};

#endif // S_SQLFIELDSDIALOG_H
