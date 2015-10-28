#include "s_2ctdialog.h"
#include "../models/s_duniversal.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"

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
    DialogIsNeedToBeResized = false;
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
    QSizePolicy fixed(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setSizePolicy(fixed);
}

void s_2ctdialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *pbLayout = new QHBoxLayout;
    s_tqTreeView *mainTV = new s_tqTreeView;
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
    connect(mainTV, SIGNAL(datachanged()), this, SLOT(updatedialogsize()));
    pmainmodel = new QSortFilterProxyModel;
    pmainmodel->setSourceModel(mainmodel);
    pmainmodel->setFilterKeyColumn(1);
    pmainmodel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mainTV->setModel(pmainmodel);
    mainTV->setSortingEnabled(true);
//    mainTV->setModel(mainmodel);
    mainTV->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mainTV->header()->setVisible(false);
    mainTV->setItemDelegate(uniDelegate);
    mainTV->setIndentation(2);
    connect(mainmodel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(resizemainTV(QModelIndex,QModelIndex)));
    mainLayout->addWidget(lbl, 0, Qt::AlignRight);
    QHBoxLayout *hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Фильтр:");
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setObjectName("filterle");
    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/lupa.gif"));
    connect(le,SIGNAL(returnPressed()),this,SLOT(Filter()));
    connect(pb,SIGNAL(clicked()),this,SLOT(Filter()));
    hlyout->addWidget(lbl);
    hlyout->addWidget(le, 1);
    hlyout->addWidget(pb);
    mainLayout->addLayout(hlyout);
    mainLayout->addWidget(mainTV, 100, Qt::AlignLeft);
    s_tqPushButton *rootpb = new s_tqPushButton("Корневой");
    connect(rootpb,SIGNAL(clicked()),this,SLOT(Root()));
    if (RootNeeded)
        mainLayout->addWidget(rootpb);
    mainLayout->addLayout(pbLayout);
    constheight=lbl->minimumSizeHint().height()+pbOk->minimumSizeHint().height();
//    mainTV->updateTVGeometry();
    ExpandHandle = connect(mainTV, SIGNAL(expanded(QModelIndex)), mainmodel, SLOT(addExpandedIndex(QModelIndex)));
    CollapseHandle = connect(mainTV, SIGNAL(collapsed(QModelIndex)), mainmodel, SLOT(removeExpandedIndex(QModelIndex)));
    connect(mainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(accepted(QModelIndex)));
    setLayout(mainLayout);
}

// процедура подготавливает дерево выбора tble по таблице tablefields

int s_2ctdialog::setup(QString tble, bool RootNeeded)
{
    mainmodel = new s_ntmodel;
    mainmodel->Setup(tble);
    this->RootNeeded = RootNeeded;
    setupUI();
    resizemainTV(QModelIndex(),QModelIndex());
    DialogIsNeedToBeResized = true;
    return 0;
}

void s_2ctdialog::resizemainTV(QModelIndex index1, QModelIndex index2)
{
    int i;
    Q_UNUSED(index1);
    Q_UNUSED(index2);
    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("mainTV");
    if (tv == 0)
    {
        CT2DBG;
        return;
    }
    for (i = 0; i < tv->header()->count(); i++)
        tv->resizeColumnToContents(i);
}

void s_2ctdialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/2cWallPaper.png"));
//    setFixedSize(minimumSizeHint());
    e->accept();
}

void s_2ctdialog::accepted(QModelIndex idx)
{
    Q_UNUSED(idx);
    accepted();
}

void s_2ctdialog::accepted()
{
    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("mainTV");
    if (tv == 0)
    {
        CT2DBG;
        return;
    }
    QModelIndex curIndex;
    QModelIndex parIndex;
    curIndex = tv->currentIndex();
    parIndex = tv->currentIndex().parent();
    QModelIndex index = pmainmodel->index(curIndex.row(), 0, parIndex); // 0-я колонка - это всегда должен быть ИД, по нему потом выбираются значения из таблиц
    QString tmpString = index.data(Qt::DisplayRole).toString();
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

void s_2ctdialog::updatedialogsize()
{
    DialogIsNeedToBeResized = true;
}

/*QSize s_2ctdialog::minimumSizeHint()
{
    if (DialogIsNeedToBeResized)
    {
        int curwidth = QApplication::desktop()->screenGeometry(this).width();
        int curheight = QApplication::desktop()->screenGeometry(this).height();
        int f2 = 0;
        s_tqTreeView *tv = this->findChild<s_tqTreeView *>("mainTV");
        for (int i = 0; i < tv->header()->count(); i++)
            f2 += tv->columnWidth(i)+20;
        if (f2 > curwidth)
            f2 = curwidth;
        if (f2 < 300) // диалоги слишком узкие нам не нужны
            f2 = 300;
        int f1 = constheight+tv->minimumSizeHint().height();
        if (f1>curheight)
            f1 = curheight;
        DialogIsNeedToBeResized = false;
//        return QSize(f2, this->size().height());
        return QSize(f2,f1);
    }
    else
        return this->size();
} */

void s_2ctdialog::sortModel()
{
//    pmainmodel->sort(0, Qt::AscendingOrder);
}

void s_2ctdialog::setTvCurrentText(QString text)
{
    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("mainTV");
    if (tv == 0)
    {
        CT2DBG;
        return;
    }
    if (text.isEmpty())
    {
        CT2WARN;
        return;
    }
    QList<QModelIndex> item = tv->model()->match(tv->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(text), 1, Qt::MatchRecursive);
    if (!item.isEmpty())
        tv->setCurrentIndex(item.at(0));
}

void s_2ctdialog::Filter()
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("filterle");
    if (le == 0)
    {
        CT2DBG;
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
            CT2DBG;
            return;
        }
        ExpandHandle = connect(tv, SIGNAL(expanded(QModelIndex)), mainmodel, SLOT(addExpandedIndex(QModelIndex)));
        CollapseHandle = connect(tv, SIGNAL(collapsed(QModelIndex)), mainmodel, SLOT(removeExpandedIndex(QModelIndex)));
    }
    pmainmodel->setFilterWildcard("*"+le->text()+"*");
}
