#ifndef WH_DIALOG_H
#define WH_DIALOG_H

// относительная ширина столбцов по смысловому назначению
#define W_SPIN      40
#define W_DIS       70
#define W_COMBO     60
#define W_LINEEDIT  100
#define W_LINKS     250
#define W_OVERALL   1000
#define W_SIZE      5 // количество "ширин"

#include <QDialog>
#include <QSqlDatabase>
#include <QDate>
#include "../inc/s_ncmodel.h"
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
    s_ncmodel *mainmodel;
    s_tqTableView *mainTV;
    float widths[W_SIZE];
    QList<QString> CTypes;
    QList<int> FTypes;
    QStringList FlowFields;

    int SetupUI(QString id);
    int writeFlow();
    int readFlow();
    int fillFlow(QString id);
    int fillNullFlow();
    void updateDialog();
    void ShowMessage(int ernum);

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
