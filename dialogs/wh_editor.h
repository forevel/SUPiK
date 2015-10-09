#ifndef WH_EDITOR_H
#define WH_EDITOR_H

#include <QDialog>
#include <QStack>

#include "../widgets/s_tqcombobox.h"

#define WHEDWARN WARNMSG(PublicClass::ER_WHED, __LINE__)
#define WHEDDBG  DBGMSG(PublicClass::ER_WHED, __LINE__)

#define MAXROWS 2
#define MAXCOLS 6

class Wh_Editor : public QDialog
{
    Q_OBJECT
public:
    explicit Wh_Editor(QWidget *parent=0);

private:
    QStack<int> IDs;
    QStack<bool> IndexIsPresent;
    QStack<int> Columns;
    QStack<int> Rows;

    void SetupUI();
    void UpdatePlacePicture(int row, int column);
    void SetCells(QLayout &lyout);
    void CheckIndexes();

private slots:
    void AddNewWh();
    void DeleteWh();
    void UpdatePlace();
    void UpdatePicture(QVariant value);
    void WriteAndClose();
    void ModifyWh(QString);
    void PopIDs();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // WH_EDITOR_H
