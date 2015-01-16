#ifndef S_dcombobox_H
#define S_dcombobox_H

#include <QStyledItemDelegate>

class s_dcombobox : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit s_dcombobox(QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const;
    void setEditorData (QWidget *editor, const QModelIndex &index) const;
    void setModelData (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

signals:
//    void changed(int row, int column, QString str);

public slots:

private slots:
//    void EmitPublicSignalChanged(QString str);

private:
    mutable int CBRow, CBColumn;
};

#endif // S_dcombobox_H
