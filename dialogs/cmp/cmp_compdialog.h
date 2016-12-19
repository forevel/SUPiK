#ifndef CMP_COMPDIALOG_H
#define CMP_COMPDIALOG_H

#define CMP_ALTIUM      0x01
#define CMP_SOLIDWORKS  0x02
#define CMP_SСHEMAGEE   0x03
#define CMP_CONSTR      0x04
#define CMP_DEVICES     0x05

#include <QDialog>

#include "../../models/treemodel.h"

class cmp_compdialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit cmp_compdialog(int type, QWidget *parent = 0);
    ~cmp_compdialog();

    int Action;
    bool SomethingChanged;

    void SetupUI();

signals:

public slots:

private slots:
    void MainItemChoosed(QModelIndex);
    void SlaveItemChoosed(QModelIndex);
    void EditItem();
    void AddNewSubsection();
    void AddNewItem();
    void AddNewOnExistingItem();
    void DeleteItem();
    void StartCompDialog(QString Id, int Mode, bool ByExisting = false);
    void SlaveContextMenu(QPoint);
    void EditCancelled();

protected:
    void paintEvent(QPaintEvent *);

private:
    QString CompLetter, CompDb, CompTbles;
    bool Cancelled;
    int CompType, CompTble;
    TreeModel *MainModel, *SlaveModel;

    bool CheckAndAdd();
    bool isModified();
    void CheckNkAndAdd(int id); // проверка на элемента на существование в БД номенклатуры и его добавление
};

#endif // cmp_COMPDIALOG_H
