#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QHeaderView>
#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include "s_2tdialog.h"
#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqsplitter.h"
#include "../widgets/s_tqframe.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"
#include "../models/s_ncmodel.h"
#include "../models/s_duniversal.h"

s_2tdialog::s_2tdialog(QWidget *parent) :
    QDialog(parent)
{
}


void s_2tdialog::SetupUI(QString hdr)
{
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqLabel *lbl = new s_tqLabel(hdr);
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    lyout->addWidget(lbl, 0);
    lyout->setAlignment(lbl, Qt::AlignRight);
    s_tqTableView *SlaveTV = new s_tqTableView;
    s_tqTableView *MainTV = new s_tqTableView;
    MainTV->setObjectName("MainTV");
    SlaveTV->setObjectName("SlaveTV");
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
    rlyout->addWidget(SlaveTV);
    right->setLayout(rlyout);
    right->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    right->setLineWidth(1);
    spl->addWidget(right);
    spl->setOrientation(Qt::Horizontal);
    lyout->addWidget(spl, 90);
    QHBoxLayout *hbl = new QHBoxLayout;
    s_tqPushButton *pbOk = new s_tqPushButton("Готово");
    s_tqPushButton *pbCancel = new s_tqPushButton("Отмена");
    connect(pbOk,SIGNAL(clicked()),this,SLOT(accepted()));
    connect(pbCancel,SIGNAL(clicked()),this,SLOT(cancelled()));
    hbl->addWidget(pbOk);
    hbl->addWidget(pbCancel);
    lyout->addLayout(hbl);
    setLayout(lyout);
}

void s_2tdialog::Setup(QString links, QString hdr)
{
    SetupUI(hdr);
    s_ncmodel *mainmodel = new s_ncmodel;
    s_tqTableView *MainTV = this->findChild<s_tqTableView *>("MainTV");
    if (MainTV == 0)
    {
        ShowErMsg(ER_2TDLG+0x01);
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    connect (MainTV, SIGNAL(clicked(QModelIndex)), this, SLOT(MainItemChoosed(QModelIndex)));
    int res = mainmodel->setupcolumn(links.at(0), links.at(1));
    if (res)
    {
        ShowErMsg(ER_2TDLG+res+0x04);
        return;
    }
    QStringList tmpsl = tfl.tablefields(links.at(0),links.at(1));
    tblename = links.at(0);
    tablefield = tmpsl.at(1); // сохраняем имя колонки для последующего использования
    MainTV->setModel(mainmodel);
    MainTV->horizontalHeader()->setVisible(false);
    MainTV->verticalHeader()->setVisible(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->resizeColumnsToContents();
    MainTV->resizeRowsToContents();
    QApplication::restoreOverrideCursor();
}

void s_2tdialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/2cWallPaper.png"));
    e->accept();
}

void s_2tdialog::MainItemChoosed(QModelIndex idx)
{
    Q_UNUSED(idx);
    s_tqTableView *MainTV = this->findChild<s_tqTableView *>("MainTV");
    if (MainTV == 0)
    {
        ShowErMsg(ER_2TDLG+0x11);
        return;
    }
    QString tmpString = MainTV->model()->index(MainTV->currentIndex().row(), 0, QModelIndex()).data(Qt::DisplayRole).toString();
    QStringList tmpsl = tfl.tablefields(tblename, "Наименование"); // берём из главной таблицы данные столбца "Наименование", т.к. в нём содержатся названия таблиц, из которых брать элементы подчинённой таблицы
    if (tfl.result)
    {
        ShowErMsg(ER_2TDLG+0x14+tfl.result);
        return;
    }
    QString db = tmpsl.at(0).split(".").at(0); // table = <db>.<tble>
    QString mntble = tmpsl.at(0).split(".").at(1);
    QString sltble = sqlc.getvaluefromtablebyfield(sqlc.getdb(db),mntble,tmpsl.at(1),tablefield,tmpString); // берём из главной таблицы значение по полю "Наименование", для которого сохранённое tablefield равен текущему элементу таблицы
    if (sqlc.result)
    {
        ShowErMsg(ER_2TDLG+0x17+sqlc.result);
        return;
    }
    s_ncmodel *slavemodel = new s_ncmodel;
    s_tqTableView *SlaveTV = this->findChild<s_tqTableView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        ShowErMsg(ER_2TDLG+0x1A);
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    connect (SlaveTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(SlaveItemChoosed(QModelIndex)));
    int res = slavemodel->setupraw(db, sltble);
    if (res)
    {
        ShowErMsg(ER_2TDLG+res+0x1D);
        return;
    }
    SlaveTV->setModel(slavemodel);
    MainTV->horizontalHeader()->setVisible(true);
    MainTV->verticalHeader()->setVisible(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->resizeColumnsToContents();
    MainTV->resizeRowsToContents();
    QApplication::restoreOverrideCursor();
}

void s_2tdialog::SlaveItemChoosed(QModelIndex idx)
{
    Q_UNUSED(idx);
    s_tqTableView *MainTV = this->findChild<s_tqTableView *>("MainTV");
    if (MainTV == 0)
    {
        ShowErMsg(ER_2TDLG+0x21);
        return;
    }
    QString tmpString = MainTV->model()->index(MainTV->currentIndex().row(), 0, QModelIndex()).data(Qt::DisplayRole).toString();
    s_tqTableView *SlaveTV = this->findChild<s_tqTableView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        ShowErMsg(ER_2TDLG+0x24);
        return;
    }
    QString tmpString2 = SlaveTV->model()->index(SlaveTV->currentIndex().row(), 0, QModelIndex()).data(Qt::DisplayRole).toString();
    emit finished(tmpString,tmpString2);
    this->close();
}

/*void s_2tdialog::SetMainTvCurrentText(QString text)
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

void s_2tdialog::SetSlaveTvCurrentText(QString text)
{
    s_tqTreeView *tv = this->findChild<s_tqTreeView *>("mainTV");
    if (tv == 0)
        return;
    if (text.isEmpty())
        return;
    QList<QModelIndex> item = tv->model()->match(tv->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(text), 1, Qt::MatchRecursive);
    if (!item.isEmpty())
        tv->setCurrentIndex(item.at(0));
} */

void s_2tdialog::accepted()
{
    SlaveItemChoosed(QModelIndex());
}

void s_2tdialog::cancelled()
{
    this->close();
}

void s_2tdialog::ShowErMsg(int ermsg)
{
    QMessageBox::warning(this, "warning!", "Ошибка 0x" + QString::number(ermsg,16));
}
