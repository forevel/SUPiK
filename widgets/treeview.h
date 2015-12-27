#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTableView>
#include <QProxyStyle>
#include <QPaintEvent>

class TreeView : public QTableView
{
    Q_OBJECT
public:
    explicit TreeView(QWidget *parent = 0);

    void ResizeColumnsToContents();

signals:
    void datachanged();

public slots:
    void setTVexpanded(QModelIndex index);

private:

private slots:


protected:

};

class TreeViewstyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit TreeViewstyle();
    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const;
};

#endif // TREEVIEW_H
