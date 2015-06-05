#ifndef S_GENERAL2COLUMNSDIALOG_H
#define S_GENERAL2COLUMNSDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "../gen/s_sql.h"
#include "../models/s_ncmodel.h"

#define MODE_CHOOSE 1
#define MODE_EDIT   2

#define CD_ERROR    0x0200

class s_2cdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_2cdialog(QString tble, QString id="", QString caption="", int Mode=MODE_CHOOSE, bool isQuarantine=false, QWidget *parent = 0);
    // sl - нередактируемые поля для списка, caption - заголовок окна, links - формат (можно не использовать), str - текущее значение
    explicit s_2cdialog(QStringList sl, QString caption="", QStringList links=QStringList(), QString str="", QWidget *parent=0);
    void setupUI();
    // sl1 - надписи, sl2 - поля для заполнения, links - формат полей sl2
    //    int setup(QStringList sl1, QStringList links1, QStringList sl2, QStringList links2);
    void sortModel();
    bool IsQuarantine;
    int Mode, result;
    QString tble,caption;

signals:
    void changeshasbeenMade(QString);

public slots:

private:
    s_ncmodel *mainmodel;
    QSortFilterProxyModel *pmainmodel;
    bool DialogIsNeedToBeResized;
    int constheight;
    void fillModelAdata();
    void ShowErMsg(int ernum);

private slots:
    void accepted();
    void cancelled();
    void resizemainTV();

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // S_GENERAL2COLUMNSDIALOG_H
