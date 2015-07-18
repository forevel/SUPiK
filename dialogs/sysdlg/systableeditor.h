#ifndef SYSTABLEEDITOR_H
#define SYSTABLEEDITOR_H

#include <QDialog>
#include <QModelIndex>
#include <QPoint>

class systableeditor : public QDialog
{
    Q_OBJECT

public:
    systableeditor(QWidget *parent = 0);
    void SetupUI(QString tblename);

private:


};

#endif // SYSTABLEEDITOR_H
