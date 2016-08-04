#ifndef SYS_IMPORTCLASS_H
#define SYS_IMPORTCLASS_H

#include <QDialog>
#include <QMap>
#include "../../widgets/waitwidget.h"

#define SIMPDBG     DBGMSG(PublicClass::ER_SIMP,__LINE__)
#define SIMPWARN    WARNMSG(PublicClass::ER_SIMP,__LINE__)
#define SIMPINFO(a) INFOMSG(PublicClass::ER_SIMP,__LINE__,a)

#define ROOTSIZE    2 // размер корневого элемента

class sys_ImportClass : public QDialog
{
    Q_OBJECT
public:
    explicit sys_ImportClass(QWidget *parent = 0);

signals:

public slots:

private:
    int MaxXLSColumn;
    bool isImport;
    QString filename, tblename;
    WaitWidget *WWidget;
    QMap<QString, QString> *XLSMap;

    void SetupUI();
    void updateView();

private slots:
    void ImpExpPBPressed();
    void CancelPBPressed();
    void LoadAndCheckFile();
    void TableChoosed(QString tble);
    void MakeConnection();
    void Done();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // SYS_IMPORTCLASS_H
