#ifndef CHOOSEITEMDIALOG_H
#define CHOOSEITEMDIALOG_H

#include <QDialog>
#include "../models/proxymodel.h"
#include "../models/treemodel.h"

#define CHIDLGWARN WARNMSG(PublicClass::ER_CHIDLG, __LINE__)
#define CHIDLGDBG  DBGMSG(PublicClass::ER_CHIDLG, __LINE__)

class ChooseItemDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseItemDialog(QString tble, QString hdr="", bool RootNeeded=false, QWidget *parent = 0);
    void SetupUI();
    void sortModel();
    void setTvCurrentText(QString text);

signals:
    void changeshasbeenMade(QString);

public slots:

private:
    TreeModel *MainModel;
    ProxyModel *pMainModel;
    bool DialogIsNeedToBeResized;
    QString hdr, tble;
    bool RootNeeded;

    void Update();

private slots:
    void accepted();
    void accepted(QModelIndex idx);
    void Root();
    void cancelled();
    void Filter();
    void ShowFilterLineEdit();
    void Unfilter();
    void AddItem();
    void ResizeMainTV();

protected:
    void paintEvent(QPaintEvent *e);

};

#endif // CHOOSEITEMDIALOG_H
