#ifndef DEV_DOCDIALOG_H
#define DEV_DOCDIALOG_H

#include <QDialog>
#include "../../models/s_ncmodel.h"

#define DEVDOCWARN    WARNMSG(PublicClass::ER_DEVDOC, __LINE__)
#define DEVDOCDBG     DBGMSG(PublicClass::ER_DEVDOC, __LINE__)
#define DEVDOCINFO(a) INFOMSG(PublicClass::ER_DEVDOC, __LINE__,a)

class dev_docdialog : public QDialog
{
    Q_OBJECT
public:
    explicit dev_docdialog(QWidget *parent = 0);
    ~dev_docdialog();

signals:

public slots:

private:
    QString DevDocTble;
    s_ncmodel *slavemodel;

    void SetupUI();
    void ResizeMainTV();

private slots:
    void AddNewSubsection();
    void AddNewItem();
    void EditItem();
    void MainItemChoosed(QModelIndex idx);
    void SlaveItemChoosed(QModelIndex idx);
    void MainContextMenu(QPoint pt);

protected:
    void paintEvent(QPaintEvent *);


};

#endif // DEV_DOCDIALOG_H
