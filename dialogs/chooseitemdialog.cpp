#include "chooseitemdialog.h"
#include "../widgets/treeview.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"
#include "../gen/s_tablefields.h"
#include "s_2cdialog.h"

#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>

ChooseItemDialog::ChooseItemDialog(QString hdr, QWidget *parent) :
    QDialog(parent)
{
    this->hdr=hdr;
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
//    QSizePolicy fixed(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    setSizePolicy(fixed);
}

void ChooseItemDialog::SetupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *pbLayout = new QHBoxLayout;
    TreeView *mainTV = new TreeView(TreeView::TV_PROXY);
    mainTV->setObjectName("mainTV");
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
    MainModel = new TreeModel;
    pMainModel = new ProxyModel;
    pMainModel->setSourceModel(MainModel);
    pMainModel->setFilterKeyColumn(1);
    pMainModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mainTV->setModel(MainModel);
    mainTV->setSortingEnabled(true);
    mainTV->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mainTV->horizontalHeader()->setVisible(false);
//    connect(pMainModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(resizemainTV(QModelIndex,QModelIndex)));
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
    connect(mainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(accepted(QModelIndex)));
    setLayout(mainLayout);
}

// процедура подготавливает дерево выбора tble по таблице tablefields

void ChooseItemDialog::Setup(QString tble, bool RootNeeded)
{
    this->tble = tble;
    this->RootNeeded = RootNeeded;
    SetupUI();
    MainModel->Setup(tble);
    ResizemainTV();
}

void ChooseItemDialog::ResizemainTV()
{
    TreeView *tv = this->findChild<TreeView *>("mainTV");
    if (tv == 0)
    {
        CHIDLGDBG;
        return;
    }
    tv->resizeColumnsToContents();
}

void ChooseItemDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/2cWallPaper.png"));
    e->accept();
}

void ChooseItemDialog::accepted(QModelIndex idx)
{
    Q_UNUSED(idx);
    accepted();
}

void ChooseItemDialog::accepted()
{
    TreeView *tv = this->findChild<TreeView *>("mainTV");
    if (tv == 0)
    {
        CHIDLGDBG;
        return;
    }
    QModelIndex curIndex;
    QModelIndex parIndex;
    curIndex = tv->currentIndex();
    QModelIndex index = MainModel->index(curIndex.row(), 0); // 0-я колонка - это всегда должен быть ИД, по нему потом выбираются значения из таблиц
    QString tmpString = index.data(Qt::DisplayRole).toString();
    emit changeshasbeenMade(tmpString);
    this->close();
}

void ChooseItemDialog::Root() // нажали кнопку "Корневой элемент"
{
    emit changeshasbeenMade("0");
    this->close();
}

void ChooseItemDialog::cancelled()
{
    this->close();
}

void ChooseItemDialog::setTvCurrentText(QString text)
{
    if (text.isEmpty())
        return;
    TreeView *tv = this->findChild<TreeView *>("mainTV");
    if (tv == 0)
    {
        CHIDLGDBG;
        return;
    }
//    ProxyModel *mdl = static_cast<ProxyModel *>(tv->model());
//    TreeModel *tmdl = static_cast<TreeModel *>(mdl->sourceModel());
    QList<QModelIndex> item = tv->model()->match(tv->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(text), 1, Qt::MatchRecursive);
    if (!item.isEmpty())
    {
//        QModelIndex idx = mdl->mapFromSource(item.at(0));
        tv->setCurrentIndex(item.at(0));
    }
}

void ChooseItemDialog::Filter()
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("filterle");
    if (le == 0)
    {
        CHIDLGDBG;
        return;
    }
    pMainModel->setFilterWildcard("*"+le->text()+"*");
}

void ChooseItemDialog::ShowFilterLineEdit()
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

void ChooseItemDialog::Unfilter()
{
    pMainModel->setFilterWildcard("*");
}

void ChooseItemDialog::AddItem()
{
    QString tmptble = tble;
    tmptble.remove("_полн");
    tmptble.remove("_сокращ");
    QString Caption = tmptble;
    tmptble.append("_полн");
    QString newID = tfl.insert(tmptble);
    if (tfl.result)
    {
        CHIDLGWARN;
        return;
    }
    tfl.idtois(tmptble,QStringList("ИД"),QStringList(newID)); // добавление полей idpers, deleted, date
    if (!tfl.result)
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
            CHIDLGWARN;
            return;
        }
    }
    else
        CHIDLGWARN;
}

void ChooseItemDialog::Update()
{
    if (MainModel->Setup(tble))
    {
        CHIDLGWARN;
        return;
    }
    ResizemainTV();
}
