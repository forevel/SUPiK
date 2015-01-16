#ifndef WH_DIALOG_H
#define WH_DIALOG_H

// относительная ширина столбцов по смысловому назначению
#define SPIN    40
#define DIS     70
#define CB      60
#define MASK    100
#define OVERALL 1000
#define EDIT    400

// тип поля
#define FW_AUTONUM  0
#define FW_NUMBER   1
#define FW_LINK     2
#define FW_DLINK    3
#define FW_ALLINK   4
#define FW_MAXLINK  5
#define FW_MASKED   6
#define FW_EQUAT    7

#include <QDialog>
#include <QSqlDatabase>
#include <QDate>
#include "../inc/s_whitemmodel.h"
#include "../inc/s_tqtableview.h"

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
QT_END_NAMESPACE

class wh_dialog : public QDialog
{
    Q_OBJECT
public:
    explicit wh_dialog(bool isIncoming, QString id, QWidget *parent = 0);

signals:

public slots:

private:
    QVBoxLayout *mainbl;
    QHBoxLayout *ml1, *ml2;
    QGridLayout *bl1, *bl2;
    bool firstShow, isIncoming, needtorefresh, SomethingChanged;
    QString ScanPath;
    QString Supplier, Consumer, DocNum, Reason;
    s_whitemmodel *mainmodel;
    s_tqTableView *mainTV;
    float sw, dw, cw, mw, ew;
    QList<QString> CTypes;
    QList<int> FTypes;
    QStringList FlowFields;

    int SetupUI(QString id);
    int writeFlow();
    int readFlow();
    int fillFlow(QString id);
    int fillNullFlow();
    void updateDialog();

private slots:
    void chooseSupplier();
    void chooseConsumer();
    void chooseReason();
    void chooseTN();
    void viewTN();
    void supplierChoosed(QString str);
    void consumerChoosed(QString str);
    void reasonChoosed(QString str);
    void resizeMainTV(QModelIndex index1, QModelIndex index2);
    void cancelled();
    void acceptandclose();
    void chooseDate();
    void dateChoosed(QDate);
    void CBChanged(QWidget *);

protected:
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);
};

#endif // WH_DIALOG_H
