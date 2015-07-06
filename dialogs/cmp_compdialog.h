#ifndef CMP_COMPDIALOG_H
#define CMP_COMPDIALOG_H

#include <QDialog>

#include "../gen/publicclass.h"
#include "../models/s_ncmodel.h"
#include "../models/s_ntmodel.h"

class ChooseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseWidget(QString, QWidget *parent=0);
    ~ChooseWidget();

    SetLinks(QString links);

signals:

public slots:

private slots:
    void pbclicked();

private:
    QString Links;
};

class cmp_compdialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit cmp_compdialog(QWidget *parent = 0);
    ~cmp_compdialog();

    int Action;
    bool SomethingChanged;

    void SetupUI();
    void FillNullDialog ();
    void FillDialog (QString);
    void ClearDialog ();
    void AddNewItem();

public slots:

    void DeletePBClicked();
    void ManufCBIndexChanged(const QString &arg1);
    void SetSomethingChanged ();

private slots:

    void UpdatePartNumber ();
    void DeclinePBClicked();
    void AcceptAndClosePBClicked();
    void DSheetPBClicked();
    void AddManufPBClicked();
    void VoltageOrAccuracyAccIsChecked ();
    void ModelParPBClicked();

protected:
    void paintEvent(QPaintEvent *);

private:
    int RevNotes;
    int curUnit;
    QString PathString;
    QMetaObject::Connection handle1, handle2, handle3, handle4, handle5, handle6, handle7; // 6 = 0.34-ah, 7 = 0.4

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
