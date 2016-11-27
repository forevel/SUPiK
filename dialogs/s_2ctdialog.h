#ifndef S_2CTDIALOG_H
#define S_2CTDIALOG_H

#include <QDialog>
#include "../models/proxymodel.h"
#include "../models/s_ntmodel.h"

class s_2ctdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_2ctdialog(QString hdr="", QWidget *parent = 0);
    void setupUI();
    void setup(QString tble, bool RootNeeded=false);
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
    ProxyModel *pmainmodel;
    bool DialogIsNeedToBeResized;
    QString hdr, tble;
    bool RootNeeded;

    void Update();

private slots:
    void accepted();
    void accepted(QModelIndex idx);
    void Root();
    void cancelled();
    void resizemainTV(QModelIndex, QModelIndex);
    void Filter();
    void SetExpandIndex(QModelIndex idx);
    void UnsetExpandIndex(QModelIndex idx);
    void ShowFilterLineEdit();
    void Unfilter();
    void AddItem();

protected:
    void paintEvent(QPaintEvent *e);

};

#endif // S_2CTDIALOG_H
