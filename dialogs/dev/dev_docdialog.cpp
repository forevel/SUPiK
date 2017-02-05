#include "dev_docdialog.h"
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include "../../models/proxymodel.h"
#include "../../models/griddelegate.h"
#include "../../models/treemodel.h"
#include "../../widgets/s_tqgroupbox.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../../widgets/treeview.h"
#include "../../widgets/s_tqframe.h"
#include "../../widgets/s_tqsplitter.h"
#include "../../widgets/s_tqwidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../gen/messagebox.h"

dev_docdialog::dev_docdialog(QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    DevDocTble = "";
    SetupUI();
}

dev_docdialog::~dev_docdialog()
{

}

void dev_docdialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/DevWallpaper.png"));

    event->accept();
}

// Настройка интерфейса

void dev_docdialog::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/lupa.gif"));
    pb->setToolTip("Поиск изделия");
    connect(pb,SIGNAL(clicked()),this,SLOT(ShowFilterLineEdit()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton;
    pb->setToolTip("Сбросить фильтр");
    pb->setIcon(QIcon(":/res/crossgray.png"));
    connect(pb,SIGNAL(clicked()),this,SLOT(Unfilter()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);

    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel("Документы");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);

    lyout->addLayout(hlyout);
    TreeView *MainTV = new TreeView(TreeView::TV_EXPANDABLE, TreeView::TV_PROXY);
    GridDelegate *gridItemDelegate = new GridDelegate;
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->setObjectName("mtv");
    TreeModel *MainModel = new TreeModel;
    MainProxyModel = new ProxyModel;
    MainProxyModel->setSourceModel(MainModel);
    QStringList sl = QStringList() << "Изделия_сокращ" << "Классы документов_сокращ" << "Документы_сокращ";
    MainModel->Setup(sl, TreeModel::TT_TYPE2);
    MainTV->setModel(MainProxyModel);
    MainTV->setSelectionMode(QAbstractItemView::NoSelection);
    MainTV->setSortingEnabled(true);
    MainTV->sortByColumn(0, Qt::AscendingOrder);
    MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (MainTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(MainContextMenu(QPoint)));
    connect (MainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditDoc()));
    s_tqSplitter *spl = new s_tqSplitter;
    s_tqFrame *left = new s_tqFrame;
    QVBoxLayout *leftlyout = new QVBoxLayout;
    leftlyout->addWidget(MainTV);
    left->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    left->setLineWidth(1);
    left->setLayout(leftlyout);
    spl->addWidget(left);
    s_tqFrame *right = new s_tqFrame;
    QVBoxLayout *rlyout = new QVBoxLayout;
    s_tqWidget *wdgt = new s_tqWidget;

    // рисуем окно свойств документов/изделий

    rlyout->addWidget(wdgt);
    right->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    right->setLineWidth(1);
    right->setLayout(rlyout);
    spl->addWidget(right);
    spl->setOrientation(Qt::Horizontal);
    lyout->addWidget(spl, 90);
    setLayout(lyout);
    ResizeMainTV();
}

void dev_docdialog::ShowFilterLineEdit()
{
    QDialog *dlg = new QDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    QPushButton *pb = qobject_cast<QPushButton *>(sender());
    dlg->move(pb->mapToGlobal(pb->pos()));
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Фильтр:");
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setObjectName("filterle");
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(le,SIGNAL(returnPressed()),this,SLOT(Filter()));
    connect(pb,SIGNAL(clicked()),dlg,SLOT(close()));
    hlyout->addWidget(lbl);
    hlyout->addWidget(le, 1);
    hlyout->addWidget(pb);
    dlg->setLayout(hlyout);
    dlg->exec();
}

void dev_docdialog::Filter()
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("filterle");
    if (le == 0)
    {
        DBGMSG;
        return;
    }
    MainProxyModel->setFilterWildcard("*"+le->text()+"*");
}

void dev_docdialog::Unfilter()
{
    MainProxyModel->setFilterWildcard("*");
}

void dev_docdialog::MainItemChoosed(QModelIndex idx)
{
    Q_UNUSED(idx);
/*    QApplication::setOverrideCursor(Qt::WaitCursor);
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mtv");
    if (tv == 0)
    {
        COMPDBG;
        return;
    }
    QString tmps = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    QStringList fl = QStringList() << "Наименование";
    CompTble = tmps.toInt();
    tmps = QString::number(CompTble); // убираем старшие незначащие нули
    QStringList sl = tfl.valuesbyfield(CompLetter+"Компоненты_описание_полн",fl,"ИД",tmps); // взяли имя таблицы в БД, описание которой выбрали в главной таблице
    if (tfl.result == TFRESULT_ERROR)
    {
        COMPWARN;
        return;
    }
    // теперь надо вытащить в slavemodel все компоненты из выбранной таблицы
    fl = QStringList() << "id" << "PartNumber" << "Manufacturer";
    CompTbles = sl.at(0);
    slavemodel->setupraw(CompDb,CompTbles,fl,"id"); // строим таблицу с сортировкой по ИД
    if (slavemodel->result)
        COMPINFO("Проблемы при построении таблицы "+CompTbles);
    tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        COMPDBG;
        return;
    }
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();
    QApplication::restoreOverrideCursor(); */
}
/*
void dev_docdialog::EditItem()
{
    SlaveItemChoosed(QModelIndex());
}
*/
void dev_docdialog::SlaveItemChoosed(QModelIndex idx)
{
    Q_UNUSED(idx);
/*    s_tqTableView *tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        COMPDBG;
        return;
    }
    QString CompIDs = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    if (CompTble == 0) // не была задана таблица компонентов (раздел)
    {
        COMPINFO("Не выбран раздел в левой части");
        return;
    }
    StartCompDialog(CompIDs);*/
}
/*
void dev_docdialog::StartCompDialog(QString Id, bool ByExisting)
{
    cmp_maindialog *dlg = new cmp_maindialog;
    dlg->SetupUI(CompType,CompTble,Id.toInt());
    if (ByExisting)
        dlg->SetID();
    connect(dlg,SIGNAL(cancelled()),this,SLOT(EditCancelled()));
    dlg->exec();
}

void dev_docdialog::EditCancelled()
{
    Cancelled = true;
}
*/
// --------------------------------------
// Добавляем новый элемент
// --------------------------------------

void dev_docdialog::AddNewDoc()
{
    if (DevDocTble.isEmpty())
    {
        MessageBox2::information(this, "Внимание", "Не выбран раздел в левой части");
        return;
    }
}

/*
void dev_docdialog::AddNewOnExistingItem()
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        COMPDBG;
        return;
    }
    QString CompIDs = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    Cancelled = false;
    StartCompDialog(CompIDs,true); // создаём на базе компонента CompIDs компонент с новым индексом
    if (!Cancelled)
        CheckNkAndAdd(CompIDs.toInt());
}

void dev_docdialog::DeleteItem()
{
    if (!(MessageBox::question(this, "Удалить элемент", "Вы уверены, что хотите удалить элемент?")))
        return;
    s_tqTableView *tv = this->findChild<s_tqTableView *>("stv");
    if (tv == 0)
    {
        COMPDBG;
        return;
    }
    QString CompIDs = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole).toString();
    sqlc.DeleteFromDB(sqlc.GetDB(CompDb),CompTbles,"id",CompIDs);
    if (sqlc.result)
    {
        COMPWARN;
        return;
    }
    else
    {
        COMPINFO("Удалено успешно!");
        MainItemChoosed(QModelIndex());
    }
}
 */

void dev_docdialog::MainContextMenu(QPoint)
{
    QMenu *ContextMenu = new QMenu;
    QAction *act;
    act = new QAction ("Добавить документ", this);
    act->setSeparator(false);
    connect(act,SIGNAL(triggered()),this,SLOT(AddNewDoc()));
    ContextMenu->addAction(act);
    act = new QAction ("Изменить", this);
    act->setSeparator(false);
    connect(act,SIGNAL(triggered()),this,SLOT(EditDoc()));
    if (pc.access & ACC_DOC_WR) // права архивариуса
        ContextMenu->addAction(act);
    act = new QAction("Удалить", this);
    act->setSeparator(false);
    connect(act,SIGNAL(triggered()),this,SLOT(DeleteDoc()));
    if (pc.access & ACC_DOC_WR) // права архивариуса
        ContextMenu->addAction(act);
    ContextMenu->exec(QCursor::pos());
}

void dev_docdialog::ResizeMainTV()
{
    TreeView *tv = this->findChild<TreeView *>("mtv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    for (int i = 0; i < tv->horizontalHeader()->count(); i++)
        tv->resizeColumnToContents(i);
}

void dev_docdialog::EditDoc()
{

}

void dev_docdialog::DeleteDoc()
{

}
