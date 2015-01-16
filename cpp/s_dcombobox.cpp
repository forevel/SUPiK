#include "../inc/s_dcombobox.h"
#include "../inc/s_sql.h"
#include <QComboBox>
#include <QSqlDatabase>
#include <QStringListModel>
#include <QPainter>

s_dcombobox::s_dcombobox(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* s_dcombobox::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox *cb;
    cb = new QComboBox(parent);
    cb->setStyleSheet("QComboBox {background: khaki};");
    QStringListModel *tmpModel = new QStringListModel;
    QStringList tmpStringList = index.data(Qt::UserRole).toString().split(".", QString::KeepEmptyParts);
    CBRow = index.row();
    CBColumn = index.column();
    QString tmpString = tmpStringList.at(1);
    if ((tmpString != "sup") && (tmpString != "ent") && (tmpString != "alt"));
    else
    {
        QSqlDatabase db = sqlc.getdb(tmpString);
        QString tble = tmpStringList.at(2);
        QString col = tmpStringList.at(3);
        if (tmpStringList.size() < 5) // ссылка на обычную колонку в обычной таблице
        {
            tmpStringList = sqlc.getvaluesfromtablebycolumn(db, tble, col);
        }
        else
        {
            QString id = tmpStringList.at(4);
            id = sqlc.getvaluefromtablebyfield(db, tble, "id"+tble, "alias", id);
            tmpStringList = sqlc.getvaluesfromtablebycolumnandfield(db, tble, col, "idalias", id);
        }
        tmpModel->setStringList(tmpStringList);
        cb->setModel(tmpModel);
    }
    return cb;
}

void s_dcombobox::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = static_cast<QComboBox *>(editor);
    cb->setCurrentText(index.data(Qt::EditRole).toString());
}

void s_dcombobox::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = static_cast<QComboBox  *>(editor);
    model->setData(index, cb->currentText(), Qt::EditRole);
//    emit changed(index.row(), index.column(), cb->currentText());
}

void s_dcombobox::paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    if (index.isValid())
    {
        painter->setPen(Qt::SolidLine);
        painter->setPen(QColor(Qt::darkGray));

//        painter->drawRect(option.rect);
        QRect rct = opt.rect;
        int lf = rct.left();
        int rg = lf + rct.width()-1;
        int tp = rct.top();
        int bt = tp + rct.height();
        painter->drawLine(QLine(QPoint(lf, bt), QPoint(rg, bt))); // bottomLeft - bottomRight
        painter->drawLine(QLine(QPoint(lf, tp), QPoint(rg, tp))); // topLeft - topRight
        painter->drawLine(QLine(QPoint(rg, tp), QPoint(rg, bt))); // topRight - bottomRight
        painter->drawLine(QLine(QPoint(lf, tp), QPoint(lf, bt))); // topLeft - bottomLeft
    }
    QStyledItemDelegate::paint(painter, opt, index);
}
/*
void s_dcombobox::EmitPublicSignalChanged(QString str)
{
    emit changed(CBRow, CBColumn, str);
}
*/
