#ifndef S_TQTREEVIEW_H
#define S_TQTREEVIEW_H

#include <QTreeView>
#include <QProxyStyle>
#include <QPaintEvent>

class s_tqtreeview : public QTreeView
{
    Q_OBJECT
public:
    explicit s_tqtreeview(QWidget *parent = 0);
    QVariant getAData();
    void setAData(QVariant dat);
    QSize minimumSizeHint() const;
    void updateTVGeometry();

signals:
    void datachanged();

public slots:
    void setTVexpanded(QModelIndex index);

private:
    void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index);
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    QVariant adata;
    mutable bool datachangedintable;

protected:
    void paintEvent(QPaintEvent *e);
};

class s_tqtreeviewstyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit s_tqtreeviewstyle();
    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const;
};

#endif // S_TQTREEVIEW_H
