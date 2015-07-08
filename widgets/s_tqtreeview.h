#ifndef S_TQTREEVIEW_H
#define S_TQTREEVIEW_H

#include <QTreeView>
#include <QProxyStyle>
#include <QPaintEvent>
#include "../models/s_ntmodel.h"

class s_tqTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit s_tqTreeView(QWidget *parent = 0);
    QVariant getAData();
    void setAData(QVariant dat);

    void ResizeColumnsToContents();

signals:
    void datachanged();

public slots:
    void setTVexpanded(QModelIndex index);

private:
    QVariant adata;

protected:
};

class s_tqTreeViewstyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit s_tqTreeViewstyle();
    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const;
};

#endif // S_TQTREEVIEW_H
