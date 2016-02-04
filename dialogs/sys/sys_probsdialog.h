#ifndef SYS_PROBSDIALOG_H
#define SYS_PROBSDIALOG_H

#include <QDialog>
#include <QPoint>
#include <QStringListModel>

#include "../../models/treemodel.h"
#include "../../gen/publicclass.h"

class SysProblemsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SysProblemsDialog(QWidget *parent = 0);

    QList<PublicClass::ProblemStruct> ProblemsList;
    void AddProblem(PublicClass::ProblemStruct &prob);

signals:
    void editdirneeded();
    void ProblemsNumberUpdated();

public slots:
    void UpdateMainTv();

private:
    void SetupUI();
    void diredit(QString tble);

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
};

#endif // SYS_PROBSDIALOG_H
