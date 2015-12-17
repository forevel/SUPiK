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
//    lyout->addLayout(hlyout);
    s_tqLabel *lbl = new s_tqLabel("Классификатор");
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
    MainTV->setSelectionMode(QAbstractItemView::NoSelection);
    MainTV->setSortingEnabled(true);
    MainTV->sortByColumn(0, Qt::AscendingOrder);
    MainTV->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (MainTV, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(MainContextMenu(QPoint)));
    connect (MainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditDev()));
    lyout->addWidget(MainTV);
    setLayout(lyout);
    Refresh();
}

void dev_devdialog::Refresh()
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mtv");
    if (tv == 0)
    {
        DEVDOCDBG;
        return;
    }
    ProxyModel *tvmodel = new ProxyModel;
    s_ncmodel *mdl = new s_ncmodel;
    tvmodel->setSourceModel(mdl);
    QItemSelectionModel *m = tv->selectionModel();
    tv->setModel(tvmodel);
    delete m;
    mdl->setup("Изделия_сокращ");
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
    s_2cdialog *dlg = new s_2cdialog("Изделия:Добавить");
    QString newID = tfl.insert("Изделия_полн");
    if (tfl.result)
    {
        DEVDOCWARN;
        return;
    }
    dlg->setup("Изделия_полн",MODE_EDITNEW,newID);
    if (dlg->result)
        DEVDOCWARN;
    else
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
    tfl.remove("Изделия_полн", GetIndex(0));
    if (tfl.result)
    {
        DEVDOCWARN;
        return;
    }
    else
        DEVDOCINFO("Удалено успешно!");
    Refresh();
}

QString dev_devdialog::GetIndex(int column)
{
    s_tqTableView *MainTV = this->findChild<s_tqTableView *>("mtv");
    if (MainTV == 0)
    {
        DEVDOCDBG;
        return QString();
    }
    QString tmpString = MainTV->model()->index(MainTV->currentIndex().row(), column, QModelIndex()).data(Qt::DisplayRole).toString();
    if (!column) // в нулевом столбце всегда ИД элемента с нулями в начале, надо незначащие нули убрать
        tmpString = QString::number(tmpString.toInt(0));
    return tmpString;
}

void dev_devdialog::EditDev()
{
    QString ID = GetIndex(0);
    if (ID.isEmpty())
    {
        DEVDOCWARN;
        return;
    }
    s_2cdialog *dlg = new s_2cdialog("Изделия:Изменить");
    dlg->setup("Изделия_полн",MODE_EDIT,ID);
    if (dlg->result)
        DEVDOCWARN;
    else
        dlg->exec();
    Refresh();
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
    s_tqTableView *tv = this->findChild<s_tqTableView *>("mtv");
    if (tv == 0)
    {
        DEVDOCDBG;
        return;
    }
    for (int i = 0; i < tv->horizontalHeader()->count(); i++)
        tv->resizeColumnToContents(i);
}
