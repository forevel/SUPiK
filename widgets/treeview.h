#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTableView>
#include <QProxyStyle>
#include <QPaintEvent>

#define TV_MIN_COL_SIZE 20  // minimum size of stretchable column

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
    void SetTreeType(int Type);

signals:
    void datachanged();
    void CellClicked(QModelIndex idx);

public slots:
    void setTVexpanded(QModelIndex index);

private:
    bool IsProxyModel;
    int StretchableColumn, StretchableColumnSize;
    bool StretchableColumnIsSet;

    void SetStretchableColumn(int column);

private slots:
    void UpdateStretchableColumn();

protected:
    void resizeEvent(QResizeEvent * event)
    {
        UpdateStretchableColumn();
        setColumnWidth(StretchableColumn, StretchableColumnSize);
        resizeRowsToContents();
        event->accept();
    }

};

#endif // TREEVIEW_H
