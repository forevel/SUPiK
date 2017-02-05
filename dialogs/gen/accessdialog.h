#ifndef ACCESSDIALOG_H
#define ACCESSDIALOG_H

#include <QDialog>

class AccessDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AccessDialog(QWidget *parent = 0);
    void SetupUI(QString rights);

signals:
    void acceptChanges(QString);

public slots:

private:

private slots:
    void OkPressed();
};

#endif // ACCESSDIALOG_H
