#ifndef DIR_ADDDIALOG_H
#define DIR_ADDDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include "../inc/s_tqpushbutton.h"
#include "../inc/s_tqcombobox.h"
#include "../inc/s_tqlineedit.h"

#define FSIZE 16
#define FSIZE2 7

class dir_adddialog : public QDialog
{
    Q_OBJECT
public:
    explicit dir_adddialog(bool update, QString dir="", QWidget *parent = 0);

signals:

public slots:

private:
    void setupUI();
    bool isSomethingChanged, upd;
    int idx;
    QString dir;
    QStringList sl1, sl2;
    void addLineToDlg(QList<QWidget *> wl, QGridLayout &lyt, int row);
    void adjustFieldSize(QWidget *wdgt, int widthInChar);

private slots:
    void updateTWFields(int fn);
    void WriteAndClose();
    void CancelAndClose();
    void WarningAndClose(int res);
    void WarningStr(QString str);
    void setAccessRights();
    void acceptAccess(long);
    void FPBPressed(s_tqPushButton *ptr);
    void CBPressed(QString str, s_tqComboBox *ptr);
    void updateFLE(QString str);
    void updateALE(long rights);
    void fillFields();
};

#endif // DIR_ADDDIALOG_H
