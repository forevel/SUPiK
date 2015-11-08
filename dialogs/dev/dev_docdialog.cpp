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
#include "../s_2cdialog.h"
#include "../../models/proxymodel.h"
#include "../../models/s_duniversal.h"
#include "../../widgets/s_tqgroupbox.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../../widgets/s_tqtreeview.h"
#include "../../widgets/s_tqtableview.h"
#include "../../widgets/s_tqframe.h"
#include "../../widgets/s_tqsplitter.h"
#include "../../widgets/s_tqwidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../messagebox.h"

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
    pb->setIcon(QIcon(":/res/newcaty.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(AddNewSubsection()));
    pb->setToolTip("Создать новый раздел");
    hlyout->addWidget(pb);
    hlyout->addSpacing(5);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/newdocy.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(AddNewItem()));
    pb->setToolTip("Создать новый компонент");
    hlyout->addWidget(pb);
    hlyout->addSpacing(5);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);

    hlyout->addStretch(300);
//    lyout->addLayout(hlyout);
    s_tqLabel *lbl = new s_tqLabel("Документы");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);

    lyout->addLayout(hlyout);
    s_tqTableView *MainTV = new s_tqTableView;
    s_duniversal *gridItemDelegate = new s_duniversal;
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->setObjectName("mtv");
    MainTV->horizontalHeader()->setVisible(true);
    MainTV->verticalHeader()->setVisible(false);
    s_ncmodel *mainmodel = new s_ncmodel;
    ProxyModel *mainproxymodel = new ProxyModel;
    mainmodel->setup("Классы документации_сокращ");
    if (mainmodel->result)
    {
        DEVDOCWARN;
        return;
    }
    mainproxymodel->setSourceModel(mainmodel);
    MainTV->setModel(mainproxymodel);
    MainTV->resizeColumnsToContents();
    MainTV->resizeRowsToContents();
    MainTV->setSelectionMode(QAbstractItemView::NoSelection);
    MainTV->setSortingEnabled(true);
    MainTV->sortByColumn(0, Qt::AscendingOrder);
    connect(MainTV,SIGNAL(clicked(QModelIndex)),this,SLOT(MainItemChoosed(QModelIndex)));
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
    s_tqTableView *SlaveTV = new s_tqTableView;
    SlaveTV->setItemDelegate(gridItemDelegate);
    slavemodel = new s_ncmodel;
    ProxyModel *slaveproxymodel = new ProxyModel;
    slaveproxymodel->setSourceModel(slavemodel);
    SlaveTV->setModel(slaveproxymodel);
    SlaveTV->setObjectName("stv");
    SlaveTV->horizontalHeader()->setVisible(true);
    SlaveTV->verticalHeader()->setVisible(false);
    SlaveTV->setSelectionMode(QAbstractItemView::NoSelection);
    SlaveTV->setSortingEnabled(true);
    SlaveTV->sortByColumn(0, Qt::AscendingOrder);
    connect(SlaveTV,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(SlaveItemChoosed(QModelIndex)));
    SlaveTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(SlaveTV,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(SlaveContextMenu(QPoint)));
    rlyout->addWidget(SlaveTV);
    right->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    right->setLineWidth(1);
    right->setLayout(rlyout);
    spl->addWidget(right);
    spl->setOrientation(Qt::Horizontal);
    lyout->addWidget(spl, 90);
    setLayout(lyout);
}

void dev_docdialog::MainItemChoosed(QModelIndex idx)
{
/*    Q_UNUSED(idx);
    QApplication::setOverrideCursor(Qt::WaitCursor);
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
    if (tfl.result)
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
/*    Q_UNUSED(idx);
    s_tqTableView *tv = this->findChild<s_tqTableView *>("stv");
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

void dev_docdialog::AddNewItem()
{
    if (DevDocTble.isEmpty())
    {
        DEVDOCINFO("Не выбран раздел в левой части");
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
// добавление новой категории документов

void dev_docdialog::AddNewSubsection()
{
    QString newID = tfl.insert("Классы документации_полн");
    if (tfl.result)
    {
        DEVDOCWARN;
        return;
    }
    s_2cdialog *dlg = new s_2cdialog("Категории документов::Создание");
    dlg->setup("Классы документации_полн",MODE_EDITNEW,newID);
    if (dlg->result)
        DEVDOCWARN;
    else
        dlg->exec();
}

void dev_docdialog::SlaveContextMenu(QPoint)
{
/*    QAction *ChangeDataChild;
    ChangeDataChild = new QAction ("Изменить элемент", this);
    ChangeDataChild->setSeparator(false);
    connect(ChangeDataChild, SIGNAL(triggered()), this, SLOT(EditItem()));
    QAction *AddNewByExisting = new QAction ("Создать на основе...", this);
    AddNewByExisting->setSeparator(false);
    connect(AddNewByExisting, SIGNAL(triggered()), this, SLOT(AddNewOnExistingItem()));
    QAction *DeleteAction = new QAction("Удалить элемент", this);
    DeleteAction->setSeparator(false);
    connect (DeleteAction, SIGNAL(triggered()), this, SLOT(DeleteItem()));
    QMenu *ContextMenu = new QMenu;
    ContextMenu->setTitle("Context menu");
    if (pc.access & 0x2492) // права на изменение
    {
        ContextMenu->addAction(AddNewByExisting);
        ContextMenu->addAction(ChangeDataChild);
    }
    if (pc.access & 0x4924) // права на удаление
        ContextMenu->addAction(DeleteAction);
    ContextMenu->exec(QCursor::pos()); // если есть права на удаление, на изменение тоже должны быть */
}
