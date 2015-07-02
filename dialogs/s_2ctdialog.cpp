#include "s_2ctdialog.h"
#include "../models/s_duniversal.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"

#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QHeaderView>
#include <QMessageBox>
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
    s_tqPushButton *pbCancel = new s_tqPushButton("Неа");
    s_tqLabel *lbl = new s_tqLabel;
    lbl->setText(hdr);
    this->hdr=hdr;
    QFont font;
    font.setPointSize(10);
    lbl->setFont(font);
    pbLayout->addWidget(pbOk, 0);
    pbLayout->addWidget(pbCancel, 0);
    connect (pbOk, SIGNAL(clicked()), this, SLOT(accepted()));
    connect (pbCancel, SIGNAL(clicked()), this, SLOT(cancelled()));
    connect(mainTV, SIGNAL(datachanged()), this, SLOT(updatedialogsize()));
//    pmainmodel = new QSortFilterProxyModel;
//    pmainmodel->setSourceModel(mainmodel);
//    mainTV->setModel(pmainmodel);
    mainTV->setModel(mainmodel);
    mainTV->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mainTV->header()->setVisible(false);
    mainTV->setItemDelegate(uniDelegate);
    mainTV->setIndentation(2);
    connect(mainmodel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(resizemainTV(QModelIndex,QModelIndex)));
    mainLayout->addWidget(lbl, 0, Qt::AlignRight);
    mainLayout->addWidget(mainTV, 100, Qt::AlignLeft);
    s_tqPushButton *rootpb = new s_tqPushButton("Корневой");
    connect(rootpb,SIGNAL(clicked()),this,SLOT(Root()));
    if (RootNeeded)
        mainLayout->addWidget(rootpb);
    mainLayout->addLayout(pbLayout);
    constheight=lbl->minimumSizeHint().height()+pbOk->minimumSizeHint().height();
//    mainTV->updateTVGeometry();
    connect(mainTV, SIGNAL(expanded(QModelIndex)), mainmodel, SLOT(addExpandedIndex(QModelIndex)));
    connect(mainTV, SIGNAL(collapsed(QModelIndex)), mainmodel, SLOT(removeExpandedIndex(QModelIndex)));
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
        return;
    QModelIndex curIndex;
    QModelIndex parIndex;
    curIndex = tv->currentIndex();
    parIndex = tv->currentIndex().parent();
    QModelIndex index = mainmodel->index(curIndex.row(), 0, parIndex); // 0-я колонка - это всегда должен быть ИД, по нему потом выбираются значения из таблиц
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
        return;
    if (text.isEmpty())
        return;
    QList<QModelIndex> item = tv->model()->match(tv->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(text), 1, Qt::MatchRecursive);
    if (!item.isEmpty())
        tv->setCurrentIndex(item.at(0));
}
