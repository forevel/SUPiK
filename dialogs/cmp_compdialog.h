#ifndef CMP_COMPDIALOG_H
#define CMP_COMPDIALOG_H

#define CMP_ALTIUM      0x01
#define CMP_SOLIDWORKS  0x02
#define CMP_SHEMAGEE    0x03
#define CMP_CONSTR      0x04
#define CMP_DEVICES     0x05

#include <QDialog>

#include "../gen/publicclass.h"
#include "../models/s_ncmodel.h"
#include "../models/s_ntmodel.h"

class cmp_compdialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit cmp_compdialog(int type, QWidget *parent = 0);
    ~cmp_compdialog();

    int Action;
    bool SomethingChanged;

    void SetupUI();
    void FillNullDialog ();
    void FillDialog (QString);
    void ClearDialog ();

signals:
    void error(int,int);

public slots:

    void DeletePBClicked();
    void ManufCBIndexChanged(const QString &arg1);
    void SetSomethingChanged ();

private slots:
    void emiterror(int,int);
    void MainItemChoosed(QModelIndex);
    void SlaveItemChoosed(QModelIndex);
    void AddNewItem();

    void UpdatePartNumber ();
    void DeclinePBClicked();
    void AcceptAndClosePBClicked();
    void DSheetPBClicked();
    void AddManufPBClicked();
    void VoltageOrAccuracyAccIsChecked ();
    void ModelParPBClicked();
    void test();

protected:
    void paintEvent(QPaintEvent *);

private:
//    int RevNotes;
//    int curUnit;
    QString PathString;
    QMetaObject::Connection handle1, handle2, handle3, handle4, handle5, handle6, handle7; // 6 = 0.34-ah, 7 = 0.4
    QString CompLetter, CompDb;
    int CompType, CompTble;
    s_ncmodel *slavemodel;

    QStringList GetListFromFile (QString, QByteArray &);
    bool CheckAndAdd();
    bool isModified();
    bool ConnectPartNumber();
    bool DisconnectPartNumber ();
    void UpdateManufCombobox ();
    void UpdateUnitsCombobox();
    void GetNextId ();
    void SetParNames();
};

#endif // cmp_COMPDIALOG_H
