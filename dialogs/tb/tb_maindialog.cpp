#include "tb_maindialog.h"
#include "../../models/treemodel.h"
#include "../../models/griddelegate.h"
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QHeaderView>
#include <QGridLayout>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqtableview.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../pers/persdialog.h"
#include "../messagebox.h"

// --------------------------------------
// Конструктор
// --------------------------------------

tb_maindialog::tb_maindialog(QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    MainModel = new BaseItemModel;
    SetupUI();
}

// --------------------------------------
// Деструктор
// --------------------------------------

tb_maindialog::~tb_maindialog()
{
}

void tb_maindialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/TBWallpaper.png"));

    event->accept();
}

// Настройка интерфейса

void tb_maindialog::SetupUI()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);
    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel("ОТ и ТБ: сводные данные");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);

    SetupModel();
    s_tqTableView *tv = new s_tqTableView;
    tv->setObjectName("maintv");
    tv->setModel(MainModel);
    GridDelegate *dlgt = new GridDelegate;
    tv->setItemDelegate(dlgt);
    tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tv->verticalHeader()->setVisible(false);
    connect(tv,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(ShowPers(QModelIndex)));
    tv->resizeColumnsToContents();
    lyout->addWidget(tv);

    lyout->addStretch(300);
    setLayout(lyout);
    QApplication::restoreOverrideCursor();
}

void tb_maindialog::ShowPers(QModelIndex idx)
{
    Q_UNUSED(idx);
    s_tqTableView *tv = this->findChild<s_tqTableView *>("maintv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    QString FIO = tv->model()->data(tv->model()->index(tv->currentIndex().row(), 0, QModelIndex()), Qt::DisplayRole).toString();
    QStringList IdPers;
    tfl.valuesbyfield("Персонал_полн", QStringList("ИД"), "ФИО", FIO, IdPers);
    if ((tfl.result != TFRESULT_NOERROR) || (IdPers.isEmpty()))
    {
        WARNMSG("");
        return;
    }
    PersDialog *dlg = new PersDialog(IdPers.at(0), PersDialog::PDT_TB);
    dlg->show();
}

void tb_maindialog::SetupModel()
{
    int i;
    QStringList PersIds, FIOs, TBGroups, POs, PBs, OTs;
    PublicClass::ValueStruct tmpv;
    QList<QColor> CList = {Qt::darkGreen, Qt::darkYellow, Qt::red};
    MainModel->ClearModel();
    MainModel->AddColumn("ФИО");
    MainModel->AddColumn("Группа по ЭБ");
    MainModel->AddColumn("Дата экз. ЭБ");
    MainModel->AddColumn("Дата инстр. ОТ");
    MainModel->AddColumn("Дата инстр. ПБ");
    MainModel->AddColumn("Дата профосм.");
    QString table = "Персонал_ТБ_полн";
    QString header = "ИД сотрудника";
    tfl.htovl(table, header, PersIds);
    if (PersIds.empty())
        return;
    table = "Персонал_полн";
    header = "ФИО";
    for (i=0; i<PersIds.size(); ++i)
    {
        tfl.idtov("2.2..Персонал_полн.ФИО", PersIds.at(i), tmpv);
        FIOs.append(tmpv.Value);
    }
    table = "Персонал_ТБ_полн";
    header = "Группа";
    tfl.htovl(table, header, TBGroups);
    header = "Дата профосмотра";
    tfl.htovl(table, header, POs);
    header = "Дата ПБ";
    tfl.htovl(table, header, PBs);
    header = "Дата ОТ";
    tfl.htovl(table, header, OTs);
    // заполняем таблицу
    QString tmps;
    for (i=0; i<FIOs.size(); ++i)
    {
        int row = MainModel->AddRow();
        SetMainModelData(row, 0, FIOs.at(i), Qt::black);
        QString TBGroup = (i < TBGroups.size()) ? TBGroups.at(i) : TB_NODATA;
        SetMainModelData(row, 1, TBGroup, Qt::black);
        tmps = (i < POs.size()) ? POs.at(i) : TB_NODATA;
        SetMainModelData(row, 5, tmps, CList.at(CheckDate(DT_MED, tmps)));
        tmps = (i < PBs.size()) ? PBs.at(i) : TB_NODATA;
        SetMainModelData(row, 4, tmps, CList.at(CheckDate(DT_PB, tmps)));
        tmps = (i < OTs.size()) ? OTs.at(i) : TB_NODATA;
        SetMainModelData(row, 3, tmps, CList.at(CheckDate(DT_OT, tmps)));
        QStringList Dates, fl, vl;
        fl << "idpers" << "section";
        if (TBGroup == TB_NODATA)
            vl << PersIds.at(i) << "3"; // 3 - 3-я группа по ЭБ
        else
            vl << PersIds.at(i) << TBGroup;
        Dates = sqlc.GetValuesFromTableByColumnAndFields("tb", "examresults", "date", fl, vl, "date", false); // Dates - дата последнего экзамена
        if (Dates.size())
            SetMainModelData(row, 2, Dates.at(0), CList.at(CheckDate(DT_TB, Dates.at(0))));
        else
            SetMainModelData(row, 2, TB_NODATA, CList.at(TBDATE_BAD));
    }
}

void tb_maindialog::SetMainModelData(int row, int column, const QString &data, const QColor &color)
{
    if (data.isEmpty())
        MainModel->SetModelData(row, column, TB_NODATA, Qt::EditRole);
    else
        MainModel->SetModelData(row, column, data, Qt::EditRole);
    MainModel->SetModelData(row, column, color, Qt::ForegroundRole);
}

int tb_maindialog::CheckDate(int type, const QString &date)
{
    QDateTime dtme = QDateTime::fromString(date, "yyyy-MM-dd hh:mm:ss");
    if (!dtme.isValid())
        return TBDATE_BAD;
    QDateTime CurDateTime = QDateTime::currentDateTime();
    QString periodstr, table, field;
    QStringList fl, vl;
    switch (type)
    {
    case DT_MED:
        periodstr = "int-po";
        break;
    case DT_OT:
        periodstr = "int-ot";
        break;
    case DT_PB:
        periodstr = "int-pb";
        break;
    case DT_TB:
        periodstr = "int-eb";
        break;
    default:
        return TBDATE_BAD;
    }
    table = "Настройки_ТБ_полн";
    field = "Обозначение";
    fl << "Значение";
    tfl.valuesbyfield(table, fl, field, periodstr, vl); // в vl - значение в месяцах периода проверок
    if ((tfl.result != TFRESULT_NOERROR) || (vl.isEmpty()))
    {
        WARNMSG("");
        return TBDATE_BAD;
    }
    bool ok;
    int months = vl.at(0).toInt(&ok);
    if (!ok)
    {
        WARNMSG("");
        return TBDATE_BAD;
    }
    dtme = dtme.addMonths(months);
    qint64 SubDays = dtme.daysTo(CurDateTime);
    if (SubDays < -DAYS_TO_BAD) // две недели до конца
        return TBDATE_OK;
    if (SubDays <= 0)
        return TBDATE_WARN;
    else
        return TBDATE_BAD;
}
