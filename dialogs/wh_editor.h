#ifndef WH_EDITOR_H
#define WH_EDITOR_H

#include <QDialog>

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

protected:
    void paintEvent(QPaintEvent *);
};

#endif // WH_EDITOR_H
