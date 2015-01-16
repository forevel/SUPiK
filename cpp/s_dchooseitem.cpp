#include "../inc/s_dchooseitem.h"
#include "../inc/s_tqlineedit.h"
#include "../inc/s_tqpushbutton.h"
#include "../inc/s_sql.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QRect>

s_dchooseitem::s_dchooseitem(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* s_dchooseitem::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QStringList tmpStringList;
    combWidget = new s_tqwidget(parent);
    combWidget->setStyleSheet("QWidget {background: khaki};");
    tmpStringList.clear();
    QHBoxLayout *ml = new QHBoxLayout;
    s_tqLineEdit *le = new s_tqLineEdit(combWidget);
    s_tqPushButton *pb = new s_tqPushButton(combWidget);
    le->setObjectName("le");
    pb->setObjectName("pb");
    QString tmpString;
    tmpStringList = index.data(Qt::UserRole).toString().split(".", QString::KeepEmptyParts);
    tmpString = tmpStringList.at(1);
    ml->addWidget(le, 80);
    if ((tmpString != "sup") && (tmpString != "ent") && (tmpString != "alt") && (tmpString != "con") && (tmpString != "sol")\
             && (tmpString != "sch") && (tmpString != "dev"));
    else
    {
        db1 = sqlc.getdb(tmpString);
        tble1 = tmpStringList.at(2);
        if (tmpStringList.at(3).isEmpty());
        else
        {
            db2 = sqlc.getdb(tmpStringList.at(3));
            if (db2.isValid())
                tble2 = tmpStringList.at(4);
        }
        ml->addWidget(pb,0);
        connect(pb, SIGNAL(clicked()), this, SLOT(pbclicked()));
    }
    ml->setContentsMargins(0, 0, 0, 0);
    combWidget->setLayout(ml);
    return combWidget;
}

void s_dchooseitem::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    s_tqLineEdit *le = editor->findChild<s_tqLineEdit*>("le");
    s_tqPushButton *pb = editor->findChild<s_tqPushButton*>("pb");
    pb->setText("...");
    le->setText(index.data(Qt::EditRole).toString());
}

void s_dchooseitem::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    s_tqLineEdit *le = editor->findChild<s_tqLineEdit*>("le");
    model->setData(index, le->text(), Qt::EditRole);
}

void s_dchooseitem::pbclicked()
{
    s_tqLineEdit *le = combWidget->findChild<s_tqLineEdit*>("le");
    if ((db1.isValid()) && (db2.isValid()))
    {
        chooseDialog = new s_sqltreedialog;
        chooseDialog->SetupUI(db1, tble1, db2, tble2, le->text());
        connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
        chooseDialog->setMinimumWidth(500);
        chooseDialog->exec();
    }
}

void s_dchooseitem::accepted(QString str)
{
    QString tmpString;
    s_tqLineEdit *le = combWidget->findChild<s_tqLineEdit*>("le");
    tmpString = sqlc.getvaluefromtablebyfield(db2, tble2, tble2, "id"+tble2, str);
    if (sqlc.result)
        tmpString = sqlc.getvaluefromtablebyfield(db1, tble1, tble1, "id"+tble1, str);
    le->setText(tmpString);
}

void s_dchooseitem::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid())
    {
        painter->setPen(Qt::SolidLine);
        painter->setPen(QColor(Qt::darkGray));

//        painter->drawRect(option.rect);
        QRect rct = option.rect;
        int lf = rct.left();
        int rg = lf + rct.width()-1;
        int tp = rct.top();
        int bt = tp + rct.height();
        painter->drawLine(QLine(QPoint(lf, bt), QPoint(rg, bt))); // bottomLeft - bottomRight
        painter->drawLine(QLine(QPoint(lf, tp), QPoint(rg, tp))); // topLeft - topRight
        painter->drawLine(QLine(QPoint(rg, tp), QPoint(rg, bt))); // topRight - bottomRight
        painter->drawLine(QLine(QPoint(lf, tp), QPoint(lf, bt))); // topLeft - bottomLeft
    }
    QStyledItemDelegate::paint(painter, option, index);
}
