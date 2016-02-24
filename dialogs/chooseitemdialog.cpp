#include "chooseitemdialog.h"
#include "../widgets/treeview.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"
#include "../gen/s_tablefields.h"
#include "../models/griddelegate.h"
#include "s_2cdialog.h"

#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>

ChooseItemDialog::ChooseItemDialog(QWidget *parent) :
    QDialog(parent)
{
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
}

bool ChooseItemDialog::SetupTable(QString tble, QString hdr, bool RootNeeded)
{
    this->hdr=hdr;
    this->RootNeeded = RootNeeded;
    this->tble = tble;
    SetupUI();
    if (MainModel->Setup(tble) != 0)
        return false;
    ResizeMainTV();
    return true;
}

bool ChooseItemDialog::SetupFile(QString Filename, QString StringToFind, QString Str)
{
    SetupUI();
    if (MainModel->SetupFile(Filename, StringToFind) != 0)
        return false;
    SetTvCurrentText(Str);
    return true;
}

void ChooseItemDialog::SetupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    TreeView *MainTV = new TreeView(TreeView::TV_PROXY);
    MainTV->setObjectName("MainTV");
    s_tqLabel *lbl = new s_tqLabel;
    lbl->setText(hdr);
    QFont font;
    font.setPointSize(10);
    lbl->setFont(font);
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

    MainModel = new TreeModel;
    pMainModel = new ProxyModel;
    pMainModel->setSourceModel(MainModel);
    pMainModel->setFilterKeyColumn(1);
    pMainModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    MainTV->setModel(pMainModel);
    GridDelegate *MainDelegate = new GridDelegate;
    MainTV->setItemDelegate(MainDelegate);
    mainLayout->addWidget(MainTV, 100);

    s_tqPushButton *rootpb = new s_tqPushButton("Корневой");
    connect(rootpb,SIGNAL(clicked()),this,SLOT(Root()));
    if (RootNeeded)
        mainLayout->addWidget(rootpb);

    QHBoxLayout *pbLayout = new QHBoxLayout;
    s_tqPushButton *pbOk = new s_tqPushButton("Ага");
    pbOk->setIcon(QIcon(":/res/ok.png"));
    s_tqPushButton *pbCancel = new s_tqPushButton("Неа");
    pbCancel->setIcon(QIcon(":/res/cross.png"));
    pbLayout->addWidget(pbOk, 0);
    pbLayout->addWidget(pbCancel, 0);
    connect (pbOk, SIGNAL(clicked()), this, SLOT(accepted()));
    connect (pbCancel, SIGNAL(clicked()), this, SLOT(cancelled()));
    mainLayout->addLayout(pbLayout);
    connect(MainTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(accepted(QModelIndex)));
    connect(MainModel,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(ResizeMainTV()));
    setLayout(mainLayout);
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
    TreeView *tv = this->findChild<TreeView *>("MainTV");
    if (tv == 0)
    {
        CHIDLGDBG;
        return;
    }
    // 0-я колонка - это всегда должен быть ИД, по нему потом выбираются значения из таблиц
    QVariant tmpv = tv->model()->data(tv->model()->index(tv->currentIndex().row(),0,QModelIndex()),Qt::DisplayRole);
    QString tmpString = tmpv.toString();
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

void ChooseItemDialog::SetTvCurrentText(QString text)
{
    if (text.isEmpty())
        return;
    TreeView *tv = this->findChild<TreeView *>("MainTV");
    if (tv == 0)
    {
        CHIDLGDBG;
        return;
    }
    ProxyModel *mdl = static_cast<ProxyModel *>(tv->model());
    QModelIndex item = mdl->Find(1, QVariant::fromValue(text));
    item = item.sibling(item.row(), 0); // выбираем индекс ИД (из 0-й колонки)
    if (item.isValid())
        tv->setCurrentIndex(item);
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
    TreeView *tv = this->findChild<TreeView *>("MainTV");
    if (tv != 0)
        tv->resizeColumnsToContents();
}

void ChooseItemDialog::ResizeMainTV()
{
    TreeView *tv = this->findChild<TreeView *>("MainTV");
    if (tv == 0)
    {
        CHIDLGDBG;
        return;
    }
    tv->resizeColumnsToContents();
    int wdth = 0;
    for (int i = 0; i < tv->model()->columnCount(); i++)
        wdth += tv->columnWidth(i);
    tv->setMinimumWidth(wdth+30);
    int hgth = 0;
    for (int i = 0; i < tv->model()->rowCount(); i++)
        hgth += tv->rowHeight(i);
    int tmpi = QApplication::desktop()->availableGeometry().height()-350; // -350 - чтобы высота диалога не выходила за пределы видимой части экрана
    hgth = (hgth > tmpi) ? tmpi : hgth;
    tv->setMinimumHeight(hgth+10);
}
