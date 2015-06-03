#ifndef CMP_MAINDIALOG_H
#define CMP_MAINDIALOG_H

#include <QDialog>

class cmp_maindialog : public QDialog
{
    Q_OBJECT

public:
    explicit cmp_maindialog(QWidget *parent = 0);
    ~cmp_maindialog();

private:
    void SetupUI();
};

#endif // CMP_MAINDIALOG_H
