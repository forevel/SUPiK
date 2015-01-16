#ifndef S_GENERAL2COLUMNSDIALOG_H
#define S_GENERAL2COLUMNSDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "../inc/s_sql.h"
#include "../inc/s_ncmodel.h"

class s_2cdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_2cdialog(QString hdr="", QWidget *parent = 0);
    void setupUI(QString hdr);
    // sl1 - надписи, sl2 - поля для заполнения, links - формат полей sl2
    void setup(QStringList sl1, QStringList links1, QStringList sl2, QStringList links2);
    // sl - нередактируемые поля для списка, links - формат (можно не использовать), str - текущее значение
    void setup(QStringList sl, QStringList links=QStringList(), QString str="");
    // id - номер редактируемой строки таблицы db.tble
    int setup(QString tble, QString id);
    void sortModel();
    void setTvCurrentText(QString text);

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

private slots:
    void accepted();
    void cancelled();
    void resizemainTV();
    void updatedialogsize();

protected:
    void paintEvent(QPaintEvent *e);
    QSize minimumSizeHint();
};

#endif // S_GENERAL2COLUMNSDIALOG_H
