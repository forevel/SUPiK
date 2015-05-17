#ifndef S_TABLEFILTERDIALOG_H
#define S_TABLEFILTERDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSortFilterProxyModel>
#include <QCloseEvent>

#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../models/s_aitemmodel.h"

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;
class QStandardItemModel;
QT_END_NAMESPACE

class s_tablefilterdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_tablefilterdialog(QWidget *parent = 0);

    void SetupUI(QSqlDatabase db, QString tble, QString id);
    void SetupUI(QList<QStringList> sl, QString id);
    void SetupUI(QStringList sl, QString id);
    void SetupUI(QStringList sl1, QStringList sl2, QString id);
    void SetupUI(QString id);

signals:
    void accepted(QString);

public slots:

private:
    QVBoxLayout *MainLayout;
    QHBoxLayout *SlaveLayout;
    s_aitemmodel *slm;
    QSortFilterProxyModel *proxyModel;
    s_tqTreeView *tv;
    s_tqLabel *lbl;
    s_tqPushButton *pb, *filtpb;
    s_tqLineEdit *filtle;

private slots:
    void choosed(QModelIndex index);
    void choosed();
    void filter();
    void filter(QString);
    void settvexpanded(QModelIndex);

protected:
    void closeEvent(QCloseEvent *e);
};

#endif // S_TABLEFILTERDIALOG_H
