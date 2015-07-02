#ifndef S_ACCESSDIALOG_H
#define S_ACCESSDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QGridLayout;
class QVBoxLayout;
QT_END_NAMESPACE

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
    QVBoxLayout *MainLayout;
    QGridLayout *CheckBoxLayout;
private slots:
    void OkPressed();
};

#endif // S_ACCESSDIALOG_H
