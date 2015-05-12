#include "s_tablefilterdialog.h"
#include "../models/s_duniversal.h"

#include <QLayoutItem>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVariant>
#include <QHeaderView>

s_tablefilterdialog::s_tablefilterdialog(QWidget *parent) :
    QDialog(parent)
{
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    MainLayout = new QVBoxLayout;
    SlaveLayout = new QHBoxLayout;
    setAttribute(Qt::WA_DeleteOnClose);
}

void s_tablefilterdialog::SetupUI(QList<QStringList> sl, QString id)
{
    slm = new s_aitemmodel(sl);
    tv = new s_tqtreeview;
    slm->isEditable = false;
    proxyModel = new QSortFilterProxyModel;
    proxyModel->setSourceModel(slm);
    tv->setModel(proxyModel);
    SetupUI(id);
}

void s_tablefilterdialog::SetupUI(QStringList sl, QString id)
{
    QList<QStringList> tmpsl;
    tmpsl.clear();
    tmpsl.append(sl);
    slm = new s_aitemmodel(tmpsl);
    tv = new s_tqtreeview;
    slm->isEditable = false;
    proxyModel = new QSortFilterProxyModel;
    proxyModel->setSourceModel(slm);
    tv->setModel(proxyModel);
    SetupUI(id);
}

void s_tablefilterdialog::SetupUI(QStringList sl1, QStringList sl2, QString id)
{
    slm = new s_aitemmodel(sl1, sl2);
    tv = new s_tqtreeview;
    slm->isEditable = false;
    proxyModel = new QSortFilterProxyModel;
    proxyModel->setSourceModel(slm);
    tv->setModel(proxyModel);
    connect(tv, SIGNAL(expanded(QModelIndex)), slm, SLOT(addExpandedIndex(QModelIndex)));
    connect(tv, SIGNAL(collapsed(QModelIndex)), slm, SLOT(removeExpandedIndex(QModelIndex)));
    connect(tv, SIGNAL(clicked(QModelIndex)), this, SLOT(settvexpanded(QModelIndex)));
    SetupUI(id);
}

void s_tablefilterdialog::SetupUI(QString id)
{
    int i;
    setAttribute(Qt::WA_DeleteOnClose);
    lbl = new s_tqLabel("");
    pb = new s_tqPushButton;
    pb->setText("Выбрать");
    connect (pb, SIGNAL(clicked()), this, SLOT(choosed()));
    filtpb = new s_tqPushButton;
    filtpb->setText("Фильтр");
    filtle = new s_tqLineEdit("");
    filtle->setObjectName("filtle");
    connect (filtpb, SIGNAL(clicked()), this, SLOT(filter()));
    connect (filtle, SIGNAL(textChanged(QString)), this, SLOT(filter(QString)));
    tv->setStyleSheet("QTreeView {background-color: rgba(204,204,153);}");
    s_duniversal *gridItemDelegate = new s_duniversal;
    tv->setItemDelegate(gridItemDelegate);
    tv->setIndentation(2);
    tv->header()->setVisible(false);
    connect (tv, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(choosed(QModelIndex)));
    QList<QModelIndex> item = tv->model()->match(tv->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(id), 1, Qt::MatchRecursive);
    if (!item.isEmpty())
        tv->setCurrentIndex(item.at(0));
    for (i = 0; i < tv->header()->count(); i++)
        tv->resizeColumnToContents(i);
    SlaveLayout->addWidget(pb, 10);
    SlaveLayout->addSpacing(10);
    SlaveLayout->addWidget(filtle, 70);
    SlaveLayout->addWidget(filtpb, 10);
    MainLayout->addLayout(SlaveLayout);
    MainLayout->addWidget(tv);
    setLayout(MainLayout);
}

void s_tablefilterdialog::choosed(QModelIndex index)
{
    Q_UNUSED(index);
    QString tmpString;
    if (tv->model()->rowCount(tv->currentIndex()) != 0) // для родителей запрещено иметь дополнительные поля
        settvexpanded(tv->currentIndex());
    else
    {
        tmpString = tv->model()->data(tv->model()->index(tv->currentIndex().row(), 0, QModelIndex())).toString();
        emit accepted(tmpString);
        this->close();
    }
}

void s_tablefilterdialog::choosed()
{
    QModelIndex index;
    QString tmpString;
    if (tv->model()->rowCount(tv->currentIndex()) != 0) // для родителей запрещено иметь дополнительные поля
        settvexpanded(tv->currentIndex());
    else
    {
        index = tv->model()->index(tv->currentIndex().row(), 0, tv->model()->parent(tv->currentIndex()));
        tmpString = index.data(Qt::DisplayRole).toString();
        emit accepted(tmpString);
        this->close();
    }
}

void s_tablefilterdialog::filter()
{
    filter(filtle->text());
}

void s_tablefilterdialog::filter(QString str)
{
    proxyModel->setFilterRegExp(str);
}

void s_tablefilterdialog::closeEvent(QCloseEvent *e)
{
    e->accept();
}

// обработка раскрывания корней дочернего дерева

void s_tablefilterdialog::settvexpanded(QModelIndex index)
{
    if (!index.column())
    {
        if (tv->isExpanded(index))
            tv->setExpanded(index, false);
        else
            tv->setExpanded(index, true);
    }
    for (int i = tv->header()->count(); i >= 0; --i)
        tv->resizeColumnToContents(i);
}
