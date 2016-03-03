#ifndef DEVMAINDIALOG_H
#define DEVMAINDIALOG_H

#include <QDialog>
#include <QVariant>

#define DEVMAINWARN    WARNMSG(PublicClass::ER_DEVMAIN, __LINE__)
#define DEVMAINDBG     DBGMSG(PublicClass::ER_DEVMAIN, __LINE__)
#define DEVMAININFO(a) INFOMSG(PublicClass::ER_DEVMAIN, __LINE__,a)
#define DEVMAINER(a)   ERMSG(PublicClass::ER_DEVMAIN, __LINE__,a)

class DevMainDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DevMainDialog(QString DevID="", QWidget *parent = 0);
    ~DevMainDialog();

signals:

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
