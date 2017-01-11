#include "s_2ctdialog.h"
#include "s_2cdialog.h"
#include "../models/s_duniversal.h"
#include "../widgets/treeview.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/wd_func.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"
#include "../gen/s_tablefields.h"

#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>

s_2ctdialog::s_2ctdialog(QString hdr, QWidget *parent) :
    QDialog(parent)
{
    this->hdr=hdr;
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
    QSizePolicy fixed(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setSizePolicy(fixed);
}

void s_2ctdialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *pbLayout = new QHBoxLayout;
    TreeView *mainTV = new TreeView(TreeView::TV_PROXY);
    mainTV->setObjectName("mainTV");
    s_duniversal *uniDelegate = new s_duniversal;
    s_tqPushButton *pbOk = new s_tqPushButton("Ага");
    pbOk->setIcon(QIcon(":/res/ok.png"));
    s_tqPushButton *pbCancel = new s_tqPushButton("Неа");
    pbCancel->setIcon(QIcon(":/res/cross.png"));
    s_tqLabel *lbl = new s_tqLabel;
    lbl->setText(hdr);
    QFont font;
    font.setPointSize(10);
    lbl->setFont(font);
    pbLayout->addWidget(pbOk, 0);
    pbLayout->addWidget(pbCancel, 0);
    connect (pbOk, SIGNAL(clicked()), this, SLOT(accepted()));
    connect (pbCancel, SIGNAL(clicked()), this, SLOT(cancelled()));
    pmainmodel = new ProxyModel;
    pmainmodel->setSourceModel(mainmodel);
    pmainmodel->setFilterKeyColumn(1);
    pmainmodel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mainTV->setModel(pmainmodel);
    mainTV->setSortingEnabled(true);
    mainTV->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mainTV->setItemDelegate(uniDelegate);
    connect(pmainmodel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(resizemainTV()));
    mainLayout->addWidget(lbl, 0, Qt::AlignRight);
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/lupa.gif"));
    pb->setToolTip("Поиск");
    connect(pb,SIGNAL(clicked()),this,SLOT(ShowFilterLineEdit()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton;
    pb->setToolTip("Сбросить фильтр");
    pb->setIcon(QIcon(":/res/crossgray.png"));
    connect(pb,SIGNAL(clicked()),this,SLOT(Unfilter()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton;
    pb->setToolTip("Создать новый элемент");
    pb->setIcon(QIcon(":/res/newdocy.png"));
    connect(pb,SIGNAL(clicked()),this,SLOT(AddItem()));
    hlyout->addWidget(pb);
    hlyout->addStretch(1);
    mainLayout->addLayout(hlyout);
    mainLayout->addWidget(mainTV, 100, Qt::AlignLeft);
    s_tqPushButton *rootpb = new s_tqPushButton("Корневой");
    connect(rootpb,SIGNAL(clicked()),this,SLOT(Root()));
    if (RootNeeded)
        mainLayout->addWidget(rootpb);
    mainLayout->addLayout(pbLayout);
/*    ExpandHandle = connect(mainTV, SIGNAL(expanded(QModelIndex)), this, SLOT(SetExpandIndex(QModelIndex)));
    CollapseHandle = connect(mainTV, SIGNAL(collapsed(QModelIndex)), this, SLOT(UnsetExpandIndex(QModelIndex))); */
    connect(mainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(accepted()));
    setLayout(mainLayout);
}

// процедура подготавливает дерево выбора tble по таблице tablefields

void s_2ctdialog::setup(QString tble, bool RootNeeded)
{
    this->tble = tble;
    mainmodel = new s_ntmodel;
    mainmodel->Setup(tble);
    this->RootNeeded = RootNeeded;
    setupUI();
    resizemainTV();
}

void s_2ctdialog::resizemainTV()
{
//    int i;
    WDFunc::TVAutoResize(this, "mainTV");
/*    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("mainTV");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    for (i = 0; i < tv->header()->count(); i++)
        tv->resizeColumnToContents(i); */
}

void s_2ctdialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/2cWallPaper.png"));
    e->accept();
}

void s_2ctdialog::accepted()
{
/*    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("mainTV");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QModelIndex curIndex;
    QModelIndex parIndex;
    curIndex = tv->currentIndex();
    parIndex = tv->currentIndex().parent();
    QModelIndex index = pmainmodel->index(curIndex.row(), 0, parIndex); // 0-я колонка - это всегда должен быть ИД, по нему потом выбираются значения из таблиц
    QString tmpString = index.data(Qt::DisplayRole).toString(); */
    QString tmpString = WDFunc::TVField(this, "mainTV", 0);
    emit changeshasbeenMade(tmpString);
    this->close();
}

void s_2ctdialog::Root() // нажали кнопку "Корневой элемент"
{
    emit changeshasbeenMade("0");
    this->close();
}

void s_2ctdialog::cancelled()
{
    this->close();
}

void s_2ctdialog::setTvCurrentText(QString text)
{
    if (text.isEmpty())
        return;
    TreeView *tv = this->findChild<TreeView *>("mainTV");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QList<QModelIndex> item = tv->model()->match(tv->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(text), 1, Qt::MatchRecursive);
    if (!item.isEmpty())
        tv->setCurrentIndex(item.at(0));
}

void s_2ctdialog::Filter()
{
/*    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("filterle");
    if (le == 0)
    {
        DBGMSG;
        return;
    }
    if (!le->text().isEmpty())
    {
        if (static_cast<bool>(ExpandHandle))
        {
            disconnect(ExpandHandle);
            disconnect(CollapseHandle);
        }
    }
    else
    {
        s_tqTreeView *tv = this->findChild<s_tqTreeView *>("mainTV");
        if (tv == 0)
        {
            DBGMSG;
            return;
        }
        ExpandHandle = connect(tv, SIGNAL(expanded(QModelIndex)), this, SLOT(SetExpandIndex(QModelIndex)));
        CollapseHandle = connect(tv, SIGNAL(collapsed(QModelIndex)), this, SLOT(UnsetExpandIndex(QModelIndex)));
    } */
    pmainmodel->setFilterWildcard("*"+WDFunc::LEData(this, "filterle")+"*");
}

/*void s_2ctdialog::SetExpandIndex(QModelIndex idx)
{
    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("mainTV");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QModelIndex ModelIndex = tv->model()->index(idx.row(),idx.column(), QModelIndex());
//    pmainmodel->addExpandedIndex(ModelIndex);
}

void s_2ctdialog::UnsetExpandIndex(QModelIndex idx)
{
    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("mainTV");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QModelIndex ModelIndex = tv->model()->index(idx.row(),idx.column(), QModelIndex());
//    pmainmodel->removeExpandedIndex(ModelIndex);
} */

void s_2ctdialog::ShowFilterLineEdit()
{
    QDialog *dlg = new QDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    s_tqPushButton *pb = qobject_cast<s_tqPushButton *>(sender());
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

void s_2ctdialog::Unfilter()
{
    pmainmodel->setFilterWildcard("*");
}

void s_2ctdialog::AddItem()
{
    QString tmptble = tble;
    tmptble.remove("_полн");
    tmptble.remove("_сокращ");
    QString Caption = tmptble;
    tmptble.append("_полн");
    QString newID;
    tfl.Insert(tmptble, newID);
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    tfl.Update(tmptble,QStringList("ИД"),QStringList(newID)); // добавление полей idpers, deleted, date
    if (tfl.result != TFRESULT_ERROR)
    {
        s_2cdialog *newdialog = new s_2cdialog(Caption);
        newdialog->setup(tmptble, MODE_EDITNEW, newID);
        if (!newdialog->result)
        {
            newdialog->setModal(true);
            newdialog->exec();
            Update();
        }
        else
        {
            WARNMSG("");
            return;
        }
    }
    else
        WARNMSG("");
}

void s_2ctdialog::Update()
{
    if (mainmodel->Setup(tble))
    {
        WARNMSG("");
        return;
    }
    resizemainTV();
}
