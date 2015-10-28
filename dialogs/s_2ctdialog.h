#ifndef S_2CTDIALOG_H
#define S_2CTDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "../models/s_ntmodel.h"

#define CT2WARN WARNMSG(PublicClass::ER_2CTDLG, __LINE__)
#define CT2DBG  DBGMSG(PublicClass::ER_2CTDLG, __LINE__)

class s_2ctdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_2ctdialog(QString hdr="", QWidget *parent = 0);
    void setupUI();
    int setup(QString tble, bool RootNeeded=false);
//    QSize minimumSizeHint();
    void sortModel();
    void setTvCurrentText(QString text);

signals:
    void changeshasbeenMade(QString);
    void error(int,int);

public slots:

private:
    QMetaObject::Connection ExpandHandle, CollapseHandle;
    s_ntmodel *mainmodel;
    QSortFilterProxyModel *pmainmodel;
    bool DialogIsNeedToBeResized;
    QString hdr;
    int constheight;
    bool RootNeeded;

private slots:
    void accepted();
    void accepted(QModelIndex idx);
    void Root();
    void cancelled();
    void resizemainTV(QModelIndex, QModelIndex);
    void updatedialogsize();
    void Filter();

protected:
    void paintEvent(QPaintEvent *e);

};

#endif // S_2CTDIALOG_H
