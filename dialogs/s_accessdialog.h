#ifndef S_ACCESSDIALOG_H
#define S_ACCESSDIALOG_H

#include <QDialog>

#define ACCDBG  DBGMSG(PublicClass::ER_ACC,__LINE__)

class s_accessdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_accessdialog(QWidget *parent = 0);
    void SetupUI(QString rights);

signals:
    void acceptChanges(QString);

public slots:

private:

private slots:
    void OkPressed();
};

#endif // S_ACCESSDIALOG_H
