#include "dev_devdialog.h"
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
#include "../../models/s_ntmodel.h"
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
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/newdocy.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(AddNewItem()));
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
//    lyout->addLayout(hlyout);
    s_tqLabel *lbl = new s_tqLabel("Классификатор");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);

    lyout->addLayout(hlyout);
    s_tqTreeView *MainTV = new s_tqTreeView;
    s_duniversal *gridItemDelegate = new s_duniversal;
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->setObjectName("mtv");
    s_ncmodel *mainmodel = new s_ncmodel;
    ProxyModel *mainproxymodel = new ProxyModel;
//    mainmodel->setup();
    mainproxymodel->setSourceModel(mainmodel);
    MainTV->setModel(mainproxymodel);
    MainTV->setSelectionMode(QAbstractItemView::NoSelection);
    MainTV->setSortingEnabled(true);
    MainTV->sortByColumn(0, Qt::AscendingOrder);
    MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (MainTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(MainContextMenu(QPoint)));
    connect (MainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditItem()));
    lyout->addWidget(MainTV);
    setLayout(lyout);
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
}

void dev_devdialog::DeleteDev()
{

}

void dev_devdialog::EditDev()
{

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
    if (pc.access & ACC_DOC) // права архивариуса
        ContextMenu->addAction(act);
    act = new QAction("Удалить", this);
    act->setSeparator(false);
    connect(act,SIGNAL(triggered()),this,SLOT(DeleteDev()));
    if (pc.access & ACC_DOC) // права архивариуса
        ContextMenu->addAction(act);
    ContextMenu->exec(QCursor::pos());
}

void dev_devdialog::ResizeMainTV()
{
    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("mtv");
    if (tv == 0)
    {
        DEVDOCDBG;
        return;
    }
    for (int i = 0; i < tv->header()->count(); i++)
        tv->resizeColumnToContents(i);
}
