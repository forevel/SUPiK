#ifndef s_ddisabled_H
#define s_ddisabled_H

#include <QStyledItemDelegate>

class s_ddisabled : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit s_ddisabled(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const;

signals:

public slots:

};

#endif // s_ddisabled_H
