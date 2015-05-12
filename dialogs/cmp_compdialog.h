#ifndef CMP_COMPDIALOG_H
#define CMP_COMPDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QStringListModel;
class QPushButton;
class QGroupBox;
class QCheckBox;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
QT_END_NAMESPACE

#include "../gen/publicclass.h"
#include "../widgets/s_tqgroupbox.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqcombobox.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqcheckbox.h"
#include "../widgets/s_tqtreewidget.h"
#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqframe.h"


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
    QStringListModel *LibRefModel, *FootRefModel, *CompManufModel, *CompUnitsModel, *s_aemodel;
    QStringList GetListFromFile (QString, QByteArray &);
    int RevNotes;
    int curUnit;
    QString PathString;
    QMetaObject::Connection handle1, handle2, handle3, handle4, handle5, handle6, handle7; // 6 = 0.34-ah, 7 = 0.4

    bool CheckAndAdd();
    bool isModified();
    bool ConnectPartNumber();
    bool DisconnectPartNumber ();
    void UpdateManufCombobox ();
    void UpdateUnitsCombobox();
    void GetNextId ();
    void SetParNames();

// UI
    QGridLayout *CompGBLayout, *LibGBLayout, *PE3GBLayout, *PackageGBLAyout, *TechGBLayout, *DSheetGBLayout;

    QVBoxLayout *CompLayout, *LeftLayout, *RightLayout;
    QHBoxLayout *CenterLayout;

    s_tqFrame *TreeFrame, *CompFrame; // два фрейма - один для просмотра компонентов, второй - для редактора компонентов
    QHBoxLayout *TreeFrameLayout;
    QLayout *MainLayout;

    s_tqTreeWidget *MainTW;
    s_tqTableView *SlaveTV;

    s_tqGroupBox *CompGB;
    s_tqLineEdit *PartNumberLE;
    s_tqComboBox *ManufCB;
    s_tqLabel *ManufL;
    s_tqLabel *PartNumberL;
    s_tqPushButton *AddManufPB;

    s_tqGroupBox *LibGB;
    s_tqComboBox *FootPrintCB;
    s_tqLabel *LibRefL;
    s_tqLabel *FootPrintL;
    s_tqComboBox *LibRefCB;

    s_tqGroupBox *PE3GB;
    s_tqLineEdit *PEDescLE;
    s_tqLabel *PEDescL;
    s_tqLineEdit *PENotesLE;
    s_tqLabel *PENotesL;

    s_tqGroupBox *PackageGB;
    s_tqLabel *TypeL;
    s_tqLabel *MarkL;
    s_tqLineEdit *TypeLE;
    s_tqLineEdit *MarkLE;
    s_tqLabel *isSMDL;
    s_tqCheckBox *isSMDCB;

    s_tqGroupBox *TechGB;
    s_tqLabel *Par1L;
    s_tqLabel *Par2L;
    s_tqLabel *Par3L;
    s_tqLabel *Par4L;
    s_tqLabel *Par5L;
    s_tqLabel *AccuracyL;
    s_tqLabel *MinOpTL;
    s_tqLabel *MaxOpTL;
    s_tqLineEdit *Par1LE;
    s_tqLineEdit *Par2LE;
    s_tqLineEdit *Par3LE;
    s_tqLineEdit *Par4LE;
    s_tqLineEdit *Par5LE;
    s_tqLineEdit *AccuracyLE;
    s_tqLineEdit *MinOpTLE;
    s_tqLineEdit *MaxOpTLE;
    s_tqLineEdit *MaxPowerLE;
    s_tqLabel *MaxPowerL;
    s_tqLabel *TKCL;
    s_tqLineEdit *TKCLE;
    s_tqCheckBox *isNeedToAccVoltageInNameCB;
    s_tqLabel *isNeedToAccVoltageInNameL;
    s_tqCheckBox *isNeedToAccAccuracyInNameCB;
    s_tqLabel *isNeedToAccAccuracyInNameL;
    s_tqComboBox *UnitsCB;

    s_tqGroupBox *DSheetGB;
    s_tqPushButton *DSheetPB;
    s_tqLabel *DSheetL;
    s_tqLabel *DSheetLE;

    s_tqLabel *AddCompL;
    s_tqPushButton *AcceptAndClosePB;
    s_tqPushButton *DeclinePB;
    s_tqLabel *idCompLE;
    s_tqLabel *idCompL;
    s_tqLabel *SectionL;
    s_tqLabel *SectionLE;
    s_tqPushButton *DeletePB;
    s_tqPushButton *ModelParPB;
    s_tqLineEdit *PrefixLE;
    s_tqLabel *PrefixL;
    s_tqCheckBox *isActiveCB;
    s_tqLabel *isActiveL;
    s_tqLineEdit *CategoryLE;
    s_tqLabel *CategoryL;
    s_tqPushButton *CategoryPB;
};

#endif // cmp_COMPDIALOG_H
