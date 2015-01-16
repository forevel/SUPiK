#ifndef S_dmaskedlineedit_H
#define S_dmaskedlineedit_H

#include <QStyledItemDelegate>

class s_dmaskedlineedit : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit s_dmaskedlineedit(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const;
    void setEditorData (QWidget *editor, const QModelIndex &index) const;
    void setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

signals:

public slots:

};

#endif // S_dmaskedlineedit_H
