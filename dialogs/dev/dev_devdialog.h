#ifndef DEV_DEVDIALOG_H
#define DEV_DEVDIALOG_H

#include <QDialog>
#include "../../models/s_ncmodel.h"

#define DEVDOCWARN    WARNMSG(PublicClass::ER_DEVDOC, __LINE__)
#define DEVDOCDBG     DBGMSG(PublicClass::ER_DEVDOC, __LINE__)
#define DEVDOCINFO(a) INFOMSG(PublicClass::ER_DEVDOC, __LINE__,a)

class dev_devdialog : public QDialog
{
    Q_OBJECT
public:
    explicit dev_devdialog(QWidget *parent = 0);
    ~dev_devdialog();

signals:

public slots:

private:
    void SetupUI();
    void ResizeMainTV();
    void Refresh();
    QString GetIndex(int column);

private slots:
    void AddNewDev();
    void EditDev();
    void DeleteDev();
    void MainContextMenu(QPoint pt);
    void Filter();
    void Unfilter();

protected:
    void paintEvent(QPaintEvent *);


};

#endif // DEV_DEVDIALOG_H
