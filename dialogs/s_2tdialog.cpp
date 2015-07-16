// Диалог с двумя таблицами - слева главная, справа - подчинённая
// Выбор осуществляется из обеих таблиц - сначала из главной, затем для выбранного пункта главной строится таблица подчинённой
// Главная таблица берётся из списка links (0 - таблица, 1 - поле), подчинённая - по ссылке из главной таблицы
// Пример: links = {"Компоненты описание_сокращ", "Описание"}
// Подчинённая таблица = Значение(alt.description.description)
// Элемент cursel = Значение(alt.description.description).<Значение(Подчинённая таблица).id<Подчинённая таблица>>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QHeaderView>
#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QFileSystemModel>
#include "s_2tdialog.h"
#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqsplitter.h"
#include "../widgets/s_tqframe.h"
#include "../gen/s_sql.h"
#include "../gen/publicclass.h"
#include "../gen/s_tablefields.h"
#include "../models/s_ncmodel.h"
#include "../models/s_ntmodel.h"
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
    connect (MainTV, SIGNAL(clicked(QModelIndex)), this, SLOT(MainItemChoosed(QModelIndex)));
    connect(MainTV, SIGNAL(datachanged()), this, SLOT(resizemainTV()));
    connect (SlaveTV, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(SlaveItemChoosed(QModelIndex)));
    connect(SlaveTV,SIGNAL(datachanged()),this,SLOT(resizeslaveTV()));
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

void s_2tdialog::Setup(QStringList links, QString cursel, QString hdr)
{
    SetupUI(hdr);
    s_ncmodel *mainmodel = new s_ncmodel;
    s_tqTableView *MainTV = this->findChild<s_tqTableView *>("MainTV");
    if (MainTV == 0)
    {
        emit error(ER_2TDLG,0x01);
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mainmodel->setupcolumn(links.at(0), links.at(1));
    if (mainmodel->result)
    {
        QApplication::restoreOverrideCursor();
        emit error(ER_2TDLG+mainmodel->result,0x02);
        return;
    }
    QStringList tmpsl = tfl.tablefields(links.at(0),links.at(1));
    tblename = links.at(0)+"_полн"; // имя таблицы на русском (Компоненты_описание_полн)
    tblefield = links.at(1); // имя поля на русском (Описание)
    tablefield = tmpsl.at(1); // сохраняем имя колонки для последующего использования (descriptionfull)
    MainTV->setModel(mainmodel);
    MainTV->horizontalHeader()->setVisible(false);
    MainTV->verticalHeader()->setVisible(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    connect(gridItemDelegate,SIGNAL(error(int,int)),this,SIGNAL(error(int,int)));
    MainTV->setItemDelegate(gridItemDelegate);
    MainTV->resizeColumnsToContents();
    MainTV->resizeRowsToContents();

    if ((!cursel.isEmpty()) && (cursel.contains('.')))
    {
        QStringList curselsl = cursel.split('.');
        QStringList tmpsl = tfl.htovlc(tblename, tblefield, "ИД", curselsl.at(0)); // берём из главной таблицы данные, для которого ИД=текущему элементу
        if (tfl.result)
        {
            emit error(ER_2TDLG+tfl.result,0x03);
            return;
        }
        SetMainTvCurrentText(tmpsl.at(0)); // устанавливаем текущий элемент главной таблицы в соответствии с переданной строкой
        MainItemChoosed(QModelIndex()); // принудительно вызываем слот вывода подчинённой таблицы по текущему элементу главной
        SetSlaveTvCurrentText(curselsl.at(1));
    }
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
        emit error(ER_2TDLG,0x11);
        return;
    }
    QString tmpString = MainTV->model()->index(MainTV->currentIndex().row(), 0, QModelIndex()).data(Qt::DisplayRole).toString();
    QStringList tmpsl = tfl.tablefields(tblename, "Наименование"); // берём из главной таблицы данные столбца "Наименование", т.к. в нём содержатся названия таблиц, из которых брать элементы подчинённой таблицы
    if (tfl.result)
    {
        emit error(ER_2TDLG+tfl.result,0x12);
        return;
    }
    QString db = tmpsl.at(0).split(".").at(0); // table = <db>.<tble>
    QString mntble = tmpsl.at(0).split(".").at(1);
    QString sltble = sqlc.getvaluefromtablebyfield(sqlc.getdb(db),mntble,tmpsl.at(1),tablefield,tmpString); // берём из главной таблицы значение по полю "Наименование", для которого сохранённое tablefield равен текущему элементу таблицы
    if (sqlc.result)
    {
        emit error(ER_2TDLG+sqlc.result,0x13);
        return;
    }
    s_ncmodel *slavemodel = new s_ncmodel;
    s_tqTableView *SlaveTV = this->findChild<s_tqTableView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        emit error(ER_2TDLG,0x14);
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    slavemodel->setupraw(db, sltble);
    if (slavemodel->result)
    {
        QApplication::restoreOverrideCursor();
        emit error(ER_2TDLG+slavemodel->result,0x15);
        return;
    }
    SlaveTV->setModel(slavemodel);
    SlaveTV->horizontalHeader()->setVisible(true);
    SlaveTV->verticalHeader()->setVisible(false);
    s_duniversal *gridItemDelegate = new s_duniversal;
    connect(gridItemDelegate,SIGNAL(error(int,int)),this,SIGNAL(error(int,int)));
    SlaveTV->setItemDelegate(gridItemDelegate);
    SlaveTV->resizeColumnsToContents();
    SlaveTV->resizeRowsToContents();
    QApplication::restoreOverrideCursor();
}

void s_2tdialog::SlaveItemChoosed(QModelIndex idx)
{
    Q_UNUSED(idx);
    s_tqTableView *MainTV = this->findChild<s_tqTableView *>("MainTV");
    if (MainTV == 0)
    {
        emit error(ER_2TDLG,0x21);
        return;
    }
    QString tmpString = MainTV->model()->index(MainTV->currentIndex().row(), 0, QModelIndex()).data(Qt::DisplayRole).toString();
    s_tqTableView *SlaveTV = this->findChild<s_tqTableView *>("SlaveTV");
    if (SlaveTV == 0)
    {
        emit error(ER_2TDLG,0x22);
        return;
    }
    QString tmpString2 = SlaveTV->model()->index(SlaveTV->currentIndex().row(), 0, QModelIndex()).data(Qt::DisplayRole).toString();

    QStringList tmpsl = tfl.htovlc(tblename, "ИД", tblefield, tmpString); // берём из главной таблицы данные столбца "ИД", для которого описание=текущему элементу главной таблицы
    if (tfl.result)
    {
        emit error(ER_2TDLG+tfl.result,0x23);
        return;
    }
    emit finished(tmpsl.at(0)+"."+tmpString2);
    this->close();
}

void s_2tdialog::resizemainTV()
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("MainTV");
    if (tv == 0)
        return;
    tv->resizeColumnsToContents();
    int wdth = 0;
    for (int i = 0; i < tv->model()->columnCount(); i++)
        wdth += tv->columnWidth(i);
    tv->setMinimumWidth(wdth+10);
    int hgth = 0;
    for (int i = 0; i < tv->model()->rowCount(); i++)
        hgth += tv->rowHeight(i);
    int tmpi = QApplication::desktop()->availableGeometry().height()-150; // -150 - чтобы высота диалога не выходила за пределы видимой части экрана
    hgth = (hgth > tmpi) ? tmpi : hgth;
    tv->setMinimumHeight(hgth+10);
}

void s_2tdialog::resizeslaveTV()
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("SlaveTV");
    if (tv == 0)
        return;
    tv->resizeColumnsToContents();
    int wdth = 0;
    for (int i = 0; i < tv->model()->columnCount(); i++)
        wdth += tv->columnWidth(i);
    tv->setMinimumWidth(wdth+10);
    int hgth = 0;
    for (int i = 0; i < tv->model()->rowCount(); i++)
        hgth += tv->rowHeight(i);
    int tmpi = QApplication::desktop()->availableGeometry().height()-150; // -150 - чтобы высота диалога не выходила за пределы видимой части экрана
    hgth = (hgth > tmpi) ? tmpi : hgth;
    tv->setMinimumHeight(hgth+10);
}

void s_2tdialog::SetMainTvCurrentText(QString text)
{
    s_tqTableView *tv = this->findChild<s_tqTableView *>("MainTV");
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
    s_tqTableView *tv = this->findChild<s_tqTableView *>("SlaveTV");
    if (tv == 0)
        return;
    if (text.isEmpty())
        return;
    QList<QModelIndex> item = tv->model()->match(tv->model()->index(0, 0), Qt::DisplayRole, QVariant::fromValue(text), 1, Qt::MatchRecursive);
    if (!item.isEmpty())
        tv->setCurrentIndex(item.at(0));
}

void s_2tdialog::accepted()
{
    SlaveItemChoosed(QModelIndex());
}

void s_2tdialog::cancelled()
{
    this->close();
}
