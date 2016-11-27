#ifndef DIR_ADDDIALOG_H
#define DIR_ADDDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqwidget.h"

#define FSIZE   32 // максимальное количество редактируемых полей справочника
#define FSZPR   8 // максимальное количество полей на одну вкладку

class dir_adddialog : public QDialog
{
    Q_OBJECT
public:
    explicit dir_adddialog(bool update, QString dirtype, QString dir="", QWidget *parent = 0);

signals:
    void closelinkdialog();

public slots:

private:
    void setupUI();
    bool isSomethingChanged, upd;
    int idx;
    const int module_number = 0x100;
    QString dir, dirtype;
    bool IsDir;
    QString ltype;
    QStringList sl1, sl2;
    QStringList FW_Links;
    QMap<QString, QString> dirBelongAliases;

    void addLineToDlg(QVBoxLayout *lyt, QList<QWidget *> wl, int idx);
    void adjustFieldSize(QWidget *wdgt, int widthInChar);
    s_tqWidget *SetWidget(int FType);

private slots:
    void updateTWFields(double dfn);
    void WriteAndClose();
    void CancelAndClose();
    void setAccessRights();
    void acceptAccess(QString);
    void FPBPressed();
    void fillFields();
    void transliteDirName();
//    void transliteFieldName(QString str, s_tqLineEdit *ptr);
    void DTypeCBIndexChanged(int);
    void LTypeCBIndexChanged(QString str);
    void ConstructLink();
    void TbleChoosed();
    void TbleNameChanged(QString tblename);
    void WriteToTfl (QStringList fl, QStringList vl, QStringList cmpfl, QStringList cmpvl);
};

#endif // DIR_ADDDIALOG_H
