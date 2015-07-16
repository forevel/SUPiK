#ifndef S_TQTABLEVIEW_H
#define S_TQTABLEVIEW_H

#include <QTableView>

class s_tqTableView : public QTableView
{
    Q_OBJECT
public:
    explicit s_tqTableView(QWidget *parent = 0);
    QVariant getAData();
    void setAData(QVariant dat);

signals:

protected slots:
    void dataChanged();

private:
    QVariant adata;

protected:
};

#endif // S_TQTABLEVIEW_H
