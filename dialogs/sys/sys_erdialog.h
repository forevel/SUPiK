#ifndef SYS_ERDIALOG_H
#define SYS_ERDIALOG_H

#include <QDialog>
#include <QPaintEvent>

#include "../../gen/publicclass.h"

class SysErDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SysErDialog(QWidget *parent = 0);
    void NewProblemsDetected();
    void InitiateDialog();

signals:
    void ProblemsNumberUpdated();

public slots:

private:
    int ErMsgLastNum;

    void InitialModelFill();
    void AddRowToProt(PublicClass::ermsg ermsg);

private slots:
    void UpdateProblems();

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // SYS_ERDIALOG_H
