#ifndef SYS_PROBSDIALOG_H
#define SYS_PROBSDIALOG_H

#include <QDialog>
#include <QPoint>
#include <QPaintEvent>
#include <QStringListModel>

#include "../../models/treemodel.h"
#include "../../gen/publicclass.h"

#define PROBSER(a)   ERMSG(PublicClass::ER_PROBS,__LINE__,a)
#define PROBSWARN    WARNMSG(PublicClass::ER_PROBS,__LINE__)
#define PROBSINFO(a) INFOMSG(PublicClass::ER_PROBS,__LINE__,a)
#define PROBSDBG     DBGMSG(PublicClass::ER_PROBS,__LINE__)

class SysProblemsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SysProblemsDialog(QWidget *parent = 0);
    void NewProblemsDetected();

signals:
    void ProblemsNumberUpdated();

public slots:

private:
    void SetupUI();
    void diredit(QString tble);
    QString ProbStringCombiner(PublicClass::ProblemStruct vl);
    void InitialModelFill();

    QStringListModel *slmodel;
    TreeModel *mainmodel;
    QString field;
    QString tble;
//    QSqlDatabase db;
    QDialog *dlg;

private slots:
    void MainTvContextMenu(QPoint);
    void RemoveProb();
    void refresh();
    void addcol();
    void UpdateMainTv();
    void UpdateProblems();

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // SYS_PROBSDIALOG_H
