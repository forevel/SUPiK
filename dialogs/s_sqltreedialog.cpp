#include "../gen/s_sql.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "s_sqltreedialog.h"
#include "../models/s_duniversal.h"
#include "s_sqlfieldsdialog.h"
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

s_sqltreedialog::s_sqltreedialog(QWidget *parent) :
    QDialog(parent)
{
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    MainLayout = new QVBoxLayout;
}

void s_sqltreedialog::SetupUI(QSqlDatabase db, QString tble, QString index)
{
    this->db1 = db;
    this->tble1 = tble;
    treeModel = new s_aitemmodel(db, tble, QSqlDatabase(), "", false);
    SetupUI(index);
}

void s_sqltreedialog::SetupUI(QSqlDatabase db1, QString tble1, QSqlDatabase db2, QString tble2, QString index)
{
    this->db1 = db1;
    this->tble1 = tble1;
    this->db2 = db2;
    this->tble2 = tble2;
    treeModel = new s_aitemmodel(db1, tble1, db2, tble2, false);
    SetupUI(index);
}

void s_sqltreedialog::SetupUI(QString index)
{
    int i;

    prModel = new QSortFilterProxyModel;
    mainTV = new s_tqTreeView;
    treeModel->isEditable = false;
    prModel->setSourceModel(treeModel);
    prModel->setFilterKeyColumn(-1);
    prModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mainTV->setModel(prModel);
    mainTV->setSortingEnabled(true);
    connect(mainTV, SIGNAL(expanded(QModelIndex)), treeModel, SLOT(addExpandedIndex(QModelIndex)));
    connect(mainTV, SIGNAL(collapsed(QModelIndex)), treeModel, SLOT(removeExpandedIndex(QModelIndex)));
    connect(mainTV, SIGNAL(clicked(QModelIndex)), this, SLOT(setMainTVExpanded(QModelIndex)));
    mainTV->setIndentation(2);
    mainTV->setAnimated(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    mainTV->setItemDelegate(gridItemDelegate);
    QList<QModelIndex> item = mainTV->model()->match(mainTV->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(index), 1, Qt::MatchRecursive);
    if (!item.isEmpty())
        mainTV->setCurrentIndex(item.at(0));
    for (i = 0; i < mainTV->header()->count(); i++)
        mainTV->resizeColumnToContents(i);
    mainTV->setStyleSheet("QTreeView {background-color: rgba(204,204,153);}");
    mainTV->sortByColumn(0, Qt::AscendingOrder);
    connect(mainTV, SIGNAL(expanded(QModelIndex)), treeModel, SLOT(addExpandedIndex(QModelIndex)));
    connect(mainTV, SIGNAL(collapsed(QModelIndex)), treeModel, SLOT(removeExpandedIndex(QModelIndex)));
    connect(mainTV, SIGNAL(clicked(QModelIndex)), this, SLOT(setMainTVExpanded(QModelIndex)));
    connect(mainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(choosed(QModelIndex)));

    s_tqPushButton *newPB = new s_tqPushButton;
    newPB->setIcon(QIcon(":/res/newdocy.png"));
    newPB->setToolTip("Создать новый элемент");
    connect(newPB, SIGNAL(clicked()), this, SLOT(execSqlFieldsDialog()));
    s_tqLabel *lb = new s_tqLabel ("Фильтр:");
    s_tqLineEdit *le = new s_tqLineEdit;
    s_tqLabel *ls = new s_tqLabel ("Поиск:");
    s_tqLineEdit *lls = new s_tqLineEdit;
    lls->setObjectName("lls");
    s_tqPushButton *spb = new s_tqPushButton;
    spb->setIcon(QIcon(":/res/lupa.gif"));
    connect (le, SIGNAL(textChanged(QString)), this, SLOT(filter(QString)));
    connect(spb, SIGNAL(clicked()), this, SLOT(search()));
    QHBoxLayout *FilterLayout = new QHBoxLayout;
    FilterLayout->addWidget(newPB);
    FilterLayout->addWidget(lb);
    FilterLayout->addWidget(le, 50);
    FilterLayout->addWidget(ls);
    FilterLayout->addWidget(lls, 50);
    FilterLayout->addWidget(spb);
    MainLayout->addLayout(FilterLayout);
    MainLayout->addWidget(mainTV);
    s_tqPushButton *pb = new s_tqPushButton(QString("Ага"));
    connect (pb, SIGNAL(clicked()), this, SLOT(commitChanges()));
    MainLayout->addWidget(pb);
    setLayout(MainLayout);
}

// обработка двойного клика

void s_sqltreedialog::choosed(QModelIndex index)
{
    QModelIndex tmpIndex;
    QString tmpString;
    if (mainTV->model()->rowCount(mainTV->currentIndex()) != 0) // для родителей запрещено иметь дополнительные поля
        setMainTVExpanded(mainTV->currentIndex());
    else
    {
        tmpIndex = mainTV->model()->index(index.row(), 0, mainTV->model()->parent(index));
        tmpString = tmpIndex.data(Qt::DisplayRole).toString();
        emit changeshasbeenMade(tmpString);
        this->close();
    }
}

// обработка раскрывания корней дерева

void s_sqltreedialog::setMainTVExpanded(QModelIndex index)
{
    if (!index.column())
    {
        if (mainTV->isExpanded(index))
            mainTV->setExpanded(index, false);
        else
            mainTV->setExpanded(index, true);
    }
    for (int i = mainTV->header()->count(); i >= 0; --i)
        mainTV->resizeColumnToContents(i);
}

void s_sqltreedialog::commitChanges(QModelIndex &index)
{
    Q_UNUSED(index);
    commitChanges();
}

void s_sqltreedialog::commitChanges()
{
    QModelIndex curIndex;
    QModelIndex parIndex;
    curIndex = mainTV->currentIndex();
    parIndex = mainTV->currentIndex().parent();
    QModelIndex index = mainTV->model()->index(curIndex.row(), 0, parIndex);
    emit changeshasbeenMade(index.data(Qt::DisplayRole).toString());
    this->close();
}

void s_sqltreedialog::filter(QString str)
{
    prModel->setFilterRegExp(str);
}

void s_sqltreedialog::search()
{
    QSqlDatabase db = (db2.isValid()) ? db2 : db1;
    QString tble = (db2.isValid()) ? tble2 : tble1;
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("lls");
    QString tmpString = le->text();
    if (le->text().isEmpty())
    {
        prModel->setSourceModel(treeModel);
        prModel->setFilterKeyColumn(-1);
        prModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        mainTV->setModel(prModel);
        mainTV->resizeColumnToContents(0);
    }
    else
    {
        QStringList tmpsl;
        tmpsl.clear();
        tmpsl << "id"+tble << tble;
        QList<QStringList> sl = sqlc.searchintablefieldlike(db, tble, tmpsl, tble, le->text());
        s_aitemmodel *tmpModel = new s_aitemmodel(sl);
        tmpModel->isEditable = false;
        prModel->setSourceModel(tmpModel);
        prModel->setFilterKeyColumn(-1);
        prModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        mainTV->setModel(prModel);
    }
}

void s_sqltreedialog::execSqlFieldsDialog()
{
    s_sqlfieldsdialog *tmpDialog = new s_sqlfieldsdialog;
    int res;
    QSqlDatabase db = (db2.isValid()) ? db2 : db1;
    QString tble = (db2.isValid()) ? tble2 : tble1;

    if (!(res = tmpDialog->SetupUI(db, tble, "0", "Справочник:добавление")))
    {
        connect(tmpDialog, SIGNAL(newidaccepted(int)), this, SLOT(SqlFieldsDialogFinished(int)));
        tmpDialog->exec();
    }
    else
    {
        delete tmpDialog;
        QMessageBox::information(this,"Ошибка при создании диалога!",\
                             QString::number(res),\
                             QMessageBox::Ok, QMessageBox::NoButton);
    }
}

void s_sqltreedialog::SqlFieldsDialogFinished(int id)
{
    QSqlDatabase db = (db2.isValid()) ? db2 : db1;
    QString tble = (db2.isValid()) ? tble2 : tble1;
    QString tmpValue = sqlc.getvaluefromtablebyid(db, tble, "id"+tble, QString::number(id));
    emit changeshasbeenMade(tmpValue);
    this->close();
/*    delete treeModel;
    treeModel = new s_aitemmodel(db1, tble1, db2, tble2, false);
    treeModel->isEditable = false;
    prModel->setSourceModel(treeModel);
    prModel->setFilterKeyColumn(-1);
    prModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mainTV->setModel(prModel);
    mainTV->resizeColumnToContents(0); */
}
