#ifndef SYS_IMPORTCLASS_H
#define SYS_IMPORTCLASS_H

#include <QDialog>
#include <QStringListModel>
#include "../../models/s_ncmodel.h"
#include "../../widgets/s_tqtableview.h"
#include "../../widgets/waitwidget.h"

#define ROOTSIZE    2 // размер корневого элемента

class sys_ImportClass : public QDialog
{
    Q_OBJECT
public:
    explicit sys_ImportClass(QWidget *parent = 0);

signals:

public slots:

private:
    s_ncmodel *mainmodel;
    QStringListModel *dirM, *fM;
    void SetupUI();
    void updateView();
    bool isImport;
    QString filename;
    WaitWidget *WWidget;

private slots:
    void ImpExpPBPressed();
    void CancelPBPressed();
    void SysICTFinished();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // SYS_IMPORTCLASS_H
