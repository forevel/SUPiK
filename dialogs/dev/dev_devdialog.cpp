#include "dev_devdialog.h"
#include "dev_maindialog.h"
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPainter>
#include <QMessageBox>
#include <QPaintEvent>
#include <QApplication>
#include "../s_2cdialog.h"
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
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/s_tqwidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../messagebox.h"

dev_devdialog::dev_devdialog(QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
}

dev_devdialog::~dev_devdialog()
{

}

void dev_devdialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/DevWallpaper.png"));

    event->accept();
}

// Настройка интерфейса

void dev_devdialog::SetupUI()
{
    QVBoxLayout *mlyout = new QVBoxLayout;
    s_tqStackedWidget *stw = new s_tqStackedWidget;
    stw->setObjectName("stw");
    s_tqWidget *w = new s_tqWidget;
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/newdocy.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(AddNewDev()));
    pb->setToolTip("Новое изделие");
    hlyout->addWidget(pb);
    hlyout->addSpacing(5);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/lupa.gif"));
    pb->setToolTip("Поиск изделия");
    connect(pb,SIGNAL(clicked()),this,SLOT(Filter()));
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
    s_tqLabel *lbl = new s_tqLabel("Классификатор");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);

    lyout->addLayout(hlyout);
    TreeView *MainTV = new TreeView(TreeView::TV_EXPANDABLE, TreeView::TV_PROXY, true);
    GridDelegate *gridItemDelegate = new GridDelegate;
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->setObjectName("mtv");
    MainTV->setSelectionMode(QAbstractItemView::NoSelection);
    MainTV->setSortingEnabled(true);
    MainTV->sortByColumn(1, Qt::AscendingOrder); // сортировка по децимальному номеру
    MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (MainTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(MainContextMenu(QPoint)));
    connect (MainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditDev()));
    lyout->addWidget(MainTV);
    w->setLayout(lyout);
    stw->addWidget(w);
    mlyout->addWidget(stw);
    setLayout(mlyout);
    Refresh();
}

void dev_devdialog::Refresh()
{
    TreeView *tv = this->findChild<TreeView *>("mtv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    ProxyModel *tvmodel = new ProxyModel;
    TreeModel *mdl = new TreeModel;
    tvmodel->setSourceModel(mdl);
    QItemSelectionModel *m = tv->selectionModel();
    tv->setModel(tvmodel);
    delete m;
    mdl->Setup("Изделия_сокращ");
    ResizeMainTV();
}

void dev_devdialog::Filter()
{

}

void dev_devdialog::Unfilter()
{

}

// --------------------------------------
// Добавляем новый элемент
// --------------------------------------

void dev_devdialog::AddNewDev()
{
    QString newID;
    QString table = "Изделия_полн";
    tfl.Insert(table, newID);
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    DevMainDialog *dlg = new DevMainDialog(newID);
    dlg->exec();
    Refresh();
}

void dev_devdialog::DeleteDev()
{
    QString Dev = GetIndex(1);
    QMessageBox msgBox;
    msgBox.setText("Вы уверены, что хотите удалить элемент "+Dev+"?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int res = msgBox.exec();
    if (res == QMessageBox::Cancel)
        return;
    QString table = "Изделия_полн";
    tfl.Remove(table, GetIndex(0));
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    else
        MessageBox2::information(this, "Успешно", "Удалено успешно!");
    Refresh();
}

QString dev_devdialog::GetIndex(int column)
{
    TreeView *MainTV = this->findChild<TreeView *>("mtv");
    if (MainTV == 0)
    {
        DBGMSG;
        return QString();
    }
    QString tmpString = MainTV->model()->index(MainTV->currentIndex().row(), column, QModelIndex()).data(Qt::DisplayRole).toString();
    pc.ConvertId(!column, tmpString);
    return tmpString;
}

void dev_devdialog::EditDev()
{
    QString ID = GetIndex(0);
    if (ID.isEmpty())
    {
        WARNMSG("");
        return;
    }
    DevMainDialog *dlg = new DevMainDialog(ID);
    connect(dlg,SIGNAL(DialogClosed()),this,SLOT(Refresh()));
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("stw");
    if (stw == 0)
    {
        DBGMSG;
        return;
    }
    stw->addWidget(dlg);
    stw->setCurrentWidget(dlg);
    stw->repaint();
//    dlg->exec();
//    Refresh();
}

void dev_devdialog::MainContextMenu(QPoint)
{
    QMenu *ContextMenu = new QMenu;
    QAction *act;
    act = new QAction ("Добавить изделие", this);
    act->setSeparator(false);
    connect(act,SIGNAL(triggered()),this,SLOT(AddNewDev()));
    ContextMenu->addAction(act);
    act = new QAction ("Изменить", this);
    act->setSeparator(false);
    connect(act,SIGNAL(triggered()),this,SLOT(EditDev()));
    if (pc.access & ACC_DOC_WR) // права архивариуса
        ContextMenu->addAction(act);
    act = new QAction("Удалить", this);
    act->setSeparator(false);
    connect(act,SIGNAL(triggered()),this,SLOT(DeleteDev()));
    if (pc.access & ACC_DOC_WR) // права архивариуса
        ContextMenu->addAction(act);
    ContextMenu->exec(QCursor::pos());
}

void dev_devdialog::ResizeMainTV()
{
    TreeView *tv = this->findChild<TreeView *>("mtv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    tv->resizeColumnsToContents();
}
