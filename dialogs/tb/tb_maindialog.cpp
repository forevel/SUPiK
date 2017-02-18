#include "tb_maindialog.h"
#include "tb_func.h"
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
#include <QStringListModel>
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/treeview.h"
#include "../../widgets/wd_func.h"
#include "../../widgets/waitwidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../pers/persdialog.h"
#include "../gen/messagebox.h"

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
    PModel = new ProxyModel;
    PModel->setSourceModel(MainModel);
    TreeView *tv = new TreeView(TreeView::TV_EPLAIN, TreeView::TV_PROXY, true);
    tv->setObjectName("maintv");
    tv->setModel(PModel);
    tv->setSortingEnabled(true);
    GridDelegate *dlgt = new GridDelegate;
    tv->setItemDelegate(dlgt);
    tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tv->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tv,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(TvContext(QPoint)));
    connect(tv,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(ShowPers()));
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();
    lyout->addWidget(tv);

//    lyout->addStretch(300);
    setLayout(lyout);
    QApplication::restoreOverrideCursor();
}

void tb_maindialog::ShowPers()
{
    Pers = WDFunc::TVField(this, "maintv", 0);
    Mode = MODE_EDIT;
    ShowPersDlg();
}

void tb_maindialog::Refresh()
{
    SetupModel();
    WDFunc::TVAutoResize(this, "maintv");
}

void tb_maindialog::ShowPersDlg()
{
    QStringList IdPers, IdTBPers;
    tfl.valuesbyfield("Персонал_полн", QStringList("ИД"), "ФИО", Pers, IdPers);
    if ((tfl.result != TFRESULT_NOERROR) || (IdPers.isEmpty()))
    {
        WARNMSG("Ошибка обработки или пустой результат");
        return;
    }
    tfl.valuesbyfield("Персонал_ТБ_полн", QStringList("ИД"), "ИД сотрудника", IdPers.at(0), IdTBPers);
    if ((tfl.result != TFRESULT_ERROR) && (!IdTBPers.isEmpty()) && (Mode == MODE_EDITNEW)) // если такой сотрудник уже есть в списке
        Mode = MODE_EDIT;
    else if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("Ошибка обработки");
        return;
    }
    PersDialog *dlg = new PersDialog(IdPers.at(0), PersDialog::PDT_TB, Mode);
    connect(dlg,SIGNAL(DialogClosed()),this,SLOT(Refresh()));
    dlg->show();
}

void tb_maindialog::AddPers()
{
    QStringList IdPers;
    Mode = MODE_EDITNEW;
    tfl.GetValuesByColumn("Персонал_полн", "ФИО", IdPers);
    if ((tfl.result != TFRESULT_NOERROR) || (IdPers.isEmpty()))
    {
        WARNMSG("Ошибка обработки или пустой результат");
        return;
    }
    QStringListModel *IdPersModel = new QStringListModel;
    IdPersModel->setStringList(IdPers);
    s_tqComboBox *cb = new s_tqComboBox;
    cb->setModel(IdPersModel);
    cb->setObjectName("perscb");
    cb->setCurrentIndex(0);
    this->Pers = IdPers.at(0);
    connect(cb,SIGNAL(currentTextChanged(QString)),this,SLOT(SetPers(QString)));
    QDialog *dlg = new QDialog;
    s_tqLabel *lbl = new s_tqLabel("Выберите сотрудника:");
    QHBoxLayout *lyout = new QHBoxLayout;
    lyout->addWidget(lbl);
    lyout->addWidget(cb);
    s_tqPushButton *pb = new s_tqPushButton("Продолжить");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(ShowPersDlg()));
    connect(pb,SIGNAL(clicked(bool)),dlg,SLOT(close()));
    lyout->addWidget(pb);
    dlg->setLayout(lyout);
    dlg->exec();
//    Refresh();
}

void tb_maindialog::SetPers(const QString &pers)
{
    this->Pers = pers;
}

void tb_maindialog::SetupModel()
{
    int i;
    WaitWidget *w = new WaitWidget;
    w->Start();
    w->SetMessage("Подготовка таблицы: база...");
    QStringList PersIds, FIOs, TBGroups, POs, PBs, OTs;
    PublicClass::ValueStruct tmpv;
    QList<QColor> CList = {Qt::darkGreen, Qt::blue, Qt::red};
    MainModel->ClearModel();
    MainModel->AddColumn("ФИО");
    MainModel->AddColumn("Группа по ЭБ");
    MainModel->AddColumn("Дата экз. ЭБ");
    MainModel->AddColumn("Дата инстр. ОТ");
    MainModel->AddColumn("Дата инстр. ПБ");
    MainModel->AddColumn("Дата профосм.");
    QString table = "Персонал_ТБ_полн";
    QString header = "ИД сотрудника";
    tfl.GetValuesByColumn(table, header, PersIds);
    if (PersIds.empty())
    {
        WARNMSG("Список пуст");
        w->Stop();
        delete w;
        return;
    }
    table = "Персонал_полн";
    header = "ФИО";
    int PIDSize = PersIds.size();
    for (i=0; i<PIDSize; ++i)
    {
        tfl.idtov("2.2..Персонал_полн.ФИО", PersIds.at(i), tmpv);
        FIOs.append(tmpv.Value);
    }
    table = "Персонал_ТБ_полн";
    header = "Группа";
    tfl.GetValuesByColumn(table, header, TBGroups);
    header = "Дата профосмотра";
    tfl.GetValuesByColumn(table, header, POs);
    header = "Дата ПБ";
    tfl.GetValuesByColumn(table, header, PBs);
    header = "Дата ОТ";
    tfl.GetValuesByColumn(table, header, OTs);
    if ((TBGroups.size() < PIDSize) || (POs.size() < PIDSize) || (OTs.size() < PIDSize) || (PBs.size() < PIDSize))
    {
        WARNMSG("Размеры не совпадают");
        w->Stop();
        delete w;
        return;
    }
    // заполняем таблицу
    PublicClass::ValueStruct vls;
    for (i=0; i<FIOs.size(); ++i)
    {
        w->SetMessage("Подготовка таблицы: обработка "+QString::number(i)+" записей из "+QString::number(FIOs.size()));
        int row = MainModel->AddRow();
        SetMainModelData(row, 0, FIOs.at(i), Qt::black);
        QString TBGroup = (!TBGroups.at(i).isEmpty()) ? TBGroups.at(i) : TB_NODATA;
        SetMainModelData(row, 1, TBGroup, Qt::black);
        tfl.idtov("2.18..", POs.at(i), vls);
        SetMainModelData(row, 5, vls.Value, CList.at(TBFunc_CheckDate(DT_MED, vls.Value)));
        tfl.idtov("2.18..", PBs.at(i), vls);
        SetMainModelData(row, 4, vls.Value, CList.at(TBFunc_CheckDate(DT_PB, vls.Value)));
        tfl.idtov("2.18..", OTs.at(i), vls);
        SetMainModelData(row, 3, vls.Value, CList.at(TBFunc_CheckDate(DT_OT, vls.Value)));
        QStringList Dates, fl, vl;
        fl << "idpers" << "section";
        if (TBGroup == TB_NODATA)
            vl << PersIds.at(i) << "2"; // 2 - 2-я группа по ЭБ (без экзаменов)
        else
            vl << PersIds.at(i) << TBGroup;
        Dates = sqlc.GetValuesFromTableByColumnAndFields("tb", "examresults", "date", fl, vl, "date", false); // Dates - дата последнего экзамена
        if (Dates.size())
            SetMainModelData(row, 2, Dates.at(0), CList.at(TBFunc_CheckDateTime(DT_TB, Dates.at(0))));
        else
            SetMainModelData(row, 2, TB_NODATA, CList.at(TBDATE_BAD));
    }
    w->Stop();
    delete w;
}

void tb_maindialog::SetMainModelData(int row, int column, const QString &data, const QColor &color)
{
    if (data.isEmpty())
        MainModel->SetModelData(row, column, TB_NODATA, Qt::EditRole);
    else
        MainModel->SetModelData(row, column, data, Qt::EditRole);
    MainModel->SetModelData(row, column, color, Qt::ForegroundRole);
}

// контекстное меню основного дерева-

void tb_maindialog::TvContext(QPoint)
{
    QAction *NewAction = new QAction("Добавить сотрудника", this);
    connect (NewAction, SIGNAL(triggered()), this, SLOT(AddPers()));
    QAction *EditAction = new QAction("Редактировать", this);
    connect (EditAction, SIGNAL(triggered()), this, SLOT(ShowPers()));

    QMenu *SystemContextMenu = new QMenu;
    SystemContextMenu->addAction(NewAction);
    SystemContextMenu->addAction(EditAction);
    SystemContextMenu->exec(QCursor::pos());
}
