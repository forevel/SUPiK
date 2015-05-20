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
    explicit s_2cdialog(QString hdr="", QWidget *parent = 0);
    void setupUI(QString hdr);
    // sl1 - надписи, sl2 - поля для заполнения, links - формат полей sl2
//    int setup(QStringList sl1, QStringList links1, QStringList sl2, QStringList links2);
    // sl - нередактируемые поля для списка, links - формат (можно не использовать), str - текущее значение
    int setup(QStringList sl, QStringList links=QStringList(), QString str="");
//    // tble - таблица по tablefields, из которой построить список выбора, links - формат (можно не использовать), str - текущее значение
//    int setupchoosetable(QString tble, QString id="");
    // id - номер редактируемой строки таблицы db.tble
    int setup(QString links, QString id="");
    void sortModel();
    void setTvCurrentText(QString text);
    bool IsQuarantine;
    int Mode;
    QString tble;

signals:
    void changeshasbeenMade(QString);

public slots:

private:
    s_ncmodel *mainmodel;
    QSortFilterProxyModel *pmainmodel;
    bool DialogIsNeedToBeResized;
    QString hdr;
    int constheight;
    void fillModelAdata();
    void ShowErMsg(int ernum);

private slots:
    void accepted();
    void cancelled();
    void resizemainTV();
    void updatedialogsize();

protected:
    void paintEvent(QPaintEvent *e);
//    QSize minimumSizeHint();
};

#endif // S_GENERAL2COLUMNSDIALOG_H
