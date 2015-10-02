#ifndef WH_EDITOR_H
#define WH_EDITOR_H

#include <QDialog>

#include "../widgets/s_tqcombobox.h"

#define WHEDWARN WARNMSG(PublicClass::ER_WHED, __LINE__)
#define WHEDDBG  DBGMSG(PublicClass::ER_WHED, __LINE__)

class Wh_Editor : public QDialog
{
    Q_OBJECT
public:
    explicit Wh_Editor(QWidget *parent=0);

private:
    void SetupUI();

private slots:
    void AddNewWh();
    void DeleteWh();
    void ModifyWh(QString WhName);
    void UpdateSmallTWWithNewQuantities();
    void UpdatePlacePicture(QString txt, s_tqComboBox *ptr);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // WH_EDITOR_H
