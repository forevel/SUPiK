#ifndef SYS_ACCEPTEXIST_H
#define SYS_ACCEPTEXIST_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QTableView;
class QLabel;
class QDialogButtonBox;
class QVBoxLayout;
QT_END_NAMESPACE

class sys_acceptexist : public QDialog
{
    Q_OBJECT
public:
    explicit sys_acceptexist(QWidget *parent = 0);
    QTableView *ADTV;

signals:

public slots:

private:
    QDialogButtonBox *buttonBox;
    QLabel *SimilarL;
    QLabel *AreyousureL;
    QVBoxLayout *MainLayout;
    void SetupUI();

private slots:

};

#endif // SYS_ACCEPTEXIST_H
