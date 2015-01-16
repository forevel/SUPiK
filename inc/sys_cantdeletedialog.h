#ifndef SYS_CANTDELETEDIALOG_H
#define SYS_CANTDELETEDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QTableView;
QT_END_NAMESPACE

class sys_cantdeletedialog : public QDialog
{
    Q_OBJECT
public:
    explicit sys_cantdeletedialog(QWidget *parent = 0);
    QTableView *DTV;

signals:

public slots:

private:
    void SetupUI();
    QVBoxLayout *MainLayout;

private slots:
    void OkPressed();

};

#endif // SYS_CANTDELETEDIALOG_H
