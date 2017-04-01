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
    void SetColumnWidthInPercent(int column, int percent);

signals:
    void datachanged();
    void CellClicked(QModelIndex idx);

public slots:
    void setTVexpanded(QModelIndex index);

private:
    bool IsProxyModel;
    QList<int> Percents;

private slots:


protected:
    void resizeEvent(QResizeEvent * event)
    {
        if(model() && model()->columnCount())
            for(int column = 0; column < model()->columnCount(); column++)
                setColumnWidth(column, event->size().width() / model()->columnCount());
    }

};

#endif // TREEVIEW_H
