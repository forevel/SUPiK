#ifndef DEVMAINDIALOG_H
#define DEVMAINDIALOG_H

#include <QDialog>
#include <QVariant>

class DevMainDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DevMainDialog(QString DevID="", QWidget *parent = 0);
    ~DevMainDialog();

signals:
    void DialogClosed();

public slots:

private:
    void SetupUI();
    void Refresh();
    QString GetIndex(int column);

private slots:
    void Filter();
    void Unfilter();
    int Fill(QString DevID);
    void SetDecimalByManuf(QVariant Manuf);
    void SetClassByManuf(QVariant Class);
    void History();
    void Objects();
    void Trips();

protected:
    void paintEvent(QPaintEvent *);


};

#endif // DEVMAINDIALOG_H
