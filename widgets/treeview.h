#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTableView>
#include <QProxyStyle>
#include <QPaintEvent>

class TreeView : public QTableView
{
    Q_OBJECT
public:
    explicit TreeView(int Proxy, QWidget *parent = 0);

    enum tv_proxies
    {
        TV_PROXY,
        TV_PLAIN
    };

signals:
    void datachanged();

public slots:
    void setTVexpanded(QModelIndex index);

private:
    bool IsProxyModel;

private slots:


protected:

};

#endif // TREEVIEW_H
