#ifndef S_2CTDIALOG_H
#define S_2CTDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "../gen/s_sql.h"
#include "../models/s_ntmodel.h"

class s_2ctdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_2ctdialog(QString hdr="", QWidget *parent = 0);
    void setupUI();
    int setup(QString tble);
    QSize minimumSizeHint();
    void sortModel();
    void setTvCurrentText(QString text);

signals:
    void changeshasbeenMade(QString);

public slots:

private:
    s_ntmodel *mainmodel;
    QSortFilterProxyModel *pmainmodel;
    bool DialogIsNeedToBeResized;
    QString hdr;
    int constheight;

private slots:
    void accepted();
    void accepted(QModelIndex idx);
    void cancelled();
    void resizemainTV(QModelIndex, QModelIndex);
    void updatedialogsize();

protected:
    void paintEvent(QPaintEvent *e);

};

#endif // S_2CTDIALOG_H
