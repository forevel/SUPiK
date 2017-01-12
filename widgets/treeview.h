#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTableView>
#include <QProxyStyle>
#include <QPaintEvent>

class TreeView : public QTableView
{
    Q_OBJECT
public:
    enum tv_proxies
    {
        TV_PROXY,
        TV_PLAIN
    };

    enum expandables
    {
        TV_EXPANDABLE,
        TV_EPLAIN
    };

    explicit TreeView(int Mode=TV_EPLAIN, int Proxy=TV_PLAIN, bool HeaderVisible=false, QWidget *parent = 0);

signals:
    void datachanged();
    void CellClicked(QModelIndex idx);

public slots:
    void setTVexpanded(QModelIndex index);

private:
    bool IsProxyModel;

private slots:


protected:

};

#endif // TREEVIEW_H
