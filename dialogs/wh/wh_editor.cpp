#include "wh_editor.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/s_tqspinbox.h"
#include "../../widgets/s_tqgroupbox.h"
#include "../../widgets/s_tqwidget.h"
#include "../../widgets/s_tqchoosewidget.h"
#include "../../widgets/s_tqtableview.h"
#include "../../gen/s_tablefields.h"
#include "../../gen/publicclass.h"
#include "../../models/placedelegate.h"
#include "../messagebox.h"
#include "../s_2cdialog.h"

#include <QMessageBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QScrollArea>

Wh_Editor::Wh_Editor(QWidget *parent) : QDialog(parent)
{
    SomethingChanged = false;
    WhModel = 0;
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
}

void Wh_Editor::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/WhWallpaper.jpg"));
    e->accept();
}

void Wh_Editor::SetupUI()
{
    // Кнопки "создать новый", "расформировать и удалить", "выход" и надпись "Редактор складов"
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/whPB-grn.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(AddNewWh()));
    pb->setToolTip("Создать новый склад");
    hlyout->addWidget(pb);
    hlyout->addSpacing(5);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/whPB-red.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(DeleteWh()));
    pb->setToolTip("Расформировать и удалить склад");
    hlyout->addWidget(pb);
    hlyout->addSpacing(5);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    connect(pb,SIGNAL(clicked()),this,SIGNAL(CloseAllWidgets())); // по нажатию кнопки "Закрыть вкладку" закрыть и удалить все виджеты
    hlyout->addWidget(pb);

    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel("Редактор складов");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    // Комбобокс с наименованиями складов из wh
    QStringListModel *mdl = new QStringListModel;
//    QStringList vl = tfl.htovl("Склады_сокращ", "Наименование");
//    mdl->setStringList(vl);
    s_tqComboBox *cb = new s_tqComboBox;
    cb->setObjectName("whcb");
    cb->setModel(mdl);
//    cb->setCurrentIndex(-1);
    connect(cb,SIGNAL(currentIndexChanged(QString)),this,SLOT(ChangeWh(QString)));
    lbl = new s_tqLabel("Редактируемый склад:");
    hlyout->addWidget(lbl, 0);
    hlyout->addWidget(cb, 2);
    hlyout->addStretch(20);
    lyout->addLayout(hlyout);
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    lyout->addWidget(line);
    // Табвиджет
    s_tqStackedWidget *stw = new s_tqStackedWidget;
    stw->setObjectName("stw");
    lyout->addWidget(stw);
    lyout->addStretch(1);
    setLayout(lyout);
    UpdateWhComboBox();
    cb->setCurrentIndex(0); // принудительный вызов загрузки данных в окно
}

void Wh_Editor::AddNewWh()
{
    QString newID;
    QString table = "Склады размещение_полн";
    tfl.insert(table, newID);
    QStringList fl = QStringList() << "ИД" << "ИД_а" << "Склад" << "Тип размещения";
    QStringList vl = QStringList() << newID << "0" << newID << "4";
    tfl.idtois(table, fl, vl);
    s_2cdialog *dlg = new s_2cdialog("Склады::Добавить");
    dlg->setup(table, MODE_EDITNEW, newID);
    if (dlg->result)
        WHEDWARN;
    else
        dlg->exec();
    UpdateWhComboBox();
}

void Wh_Editor::UpdateWhComboBox()
{
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>("whcb");
    if (cb == 0)
    {
        WHEDDBG;
        return;
    }
    QStringListModel *mdl = qobject_cast<QStringListModel *>(cb->model());
    QStringList vl;
    QString table = "Склады размещение_полн";
    QString field = "Наименование";
    QString cmpfield = "ИД_а";
    QString cmpvalue = "0";
    tfl.htovlc(table, field, cmpfield, cmpvalue, vl);
    mdl->setStringList(vl);
}

void Wh_Editor::WriteAndClose()
{
    if (WhModel->Save())
        return;
    else
        WHEDINFO("Записано успешно!");
    emit CloseAllWidgets();
}

void Wh_Editor::CancelAndClose()
{
    // удалим новосозданные элементы из таблицы
    if (WhModel->DeleteNew())
    {
        WHEDWARN;
        return;
    }
    emit CloseAllWidgets();
    this->close();
}

void Wh_Editor::AddNewPlace()
{
    QString newID;
    QString table = "Склады типы размещения_полн";
    tfl.insert(table, newID);
    s_2cdialog *dlg = new s_2cdialog("Склады::Размещения::Добавить");
    dlg->setup("Склады типы размещения_полн", MODE_EDITNEW, newID);
    if (dlg->result)
        WHEDWARN;
    else
        dlg->exec();
}

void Wh_Editor::ChangeWh(QString str)
{
    // достанем индекс склада по имени str из whplaces
    QStringList PlaceID;
    QString table = "Склады размещение_полн";
    QStringList fields = QStringList("ИД");
    QString field = "Наименование";
    tfl.valuesbyfield(table, fields, field, str, PlaceID, false);
    if (tfl.result)
    {
        WHEDWARN;
        return;
    }
    // закроем все открытые виджеты, включая "корневой"
    emit CloseAllWidgets();
    // построим модель от данного корневого ИД склада
    if (WhModel != 0)
    {
        if (SomethingChanged)
        {
            if (MessageBox2::question(this, "Данные были изменены", "Сохранить изменения?"))
                WhModel->Save(); // если модель уже существует, надо сохранить данные в БД, чтобы не потерялись
            else
                WhModel->DeleteNew();
        }
        delete WhModel;
    }
    WhModel = new WhPlacesTreeModel;
    int ID = PlaceID.at(0).toInt();
    Wh = ID;
    if (WhModel->Load(ID))
    {
        WHEDWARN;
        return;
    }
    IDs.clear();
    IDs.push(ID);
    SomethingChanged = false;
    // создадим новый корневой виджет и положим его в stw, вызовем SetCells для нового ID
    BuildWorkspace(ID, true);
}

void Wh_Editor::BuildWorkspace(int ID, bool IsWarehouse)
{
    CloseAllWidgets();
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("stw");
    if (stw == 0)
    {
        WHEDDBG;
        return;
    }
    s_tqWidget *wdgt = new s_tqWidget;
    wdgt->setAttribute(Qt::WA_DeleteOnClose);
    connect(this,SIGNAL(CloseAllWidgets()),wdgt,SLOT(close()));
    QVBoxLayout *vlyout = new QVBoxLayout;
    // построим текстовое поле места размещения
    QString PlaceNameString;
    CurID = ID;
    while (ID != 0)
    {
        WhPlacesTreeModel::WhPlacesTreeItem *item = WhModel->Data(ID);
        if (item == 0)
        {
            WHEDWARN;
            return;
        }
        QString tmps = "\\" + item->Alias;
        if (item->IdAlias != 0)
            tmps += " " + item->Name; // формируем имя места размещения как "alias name", например: "Мешок А5"
        PlaceNameString.insert(0, tmps);
        ID = item->IdAlias;
    }
    PlaceNameString.insert(0, ":"); // ":" - обозначение "корня"
    s_tqLabel *lbl = new s_tqLabel(PlaceNameString);
    vlyout->addWidget(lbl);
    QHBoxLayout *hlyout = new QHBoxLayout;
    // если текущий ID относится не к корневому месту размещения (складу), то рисуем дополнительные виджеты выбора типа места размещения
    if (!IsWarehouse)
    {
        s_tqLabel *lbl = new s_tqLabel("Тип размещения:");
        hlyout = new QHBoxLayout;
        hlyout->addWidget(lbl);
        hlyout->setAlignment(lbl,Qt::AlignRight);
        s_tqChooseWidget *cw = new s_tqChooseWidget(true);
        cw->Setup(("2.2..Склады типы размещения_сокращ.Наименование"));
        cw->setObjectName("chooseplace");
        connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(ChangePlace(QVariant)));
        hlyout->addWidget(cw);
        s_tqPushButton *pb = new s_tqPushButton("Создать");
        pb->setIcon(QIcon(":/res/oblachko.png"));
        connect(pb,SIGNAL(clicked()),this,SLOT(AddNewPlace()));
        pb->setToolTip("Создать новый тип размещения");
        hlyout->addWidget(pb);
        vlyout->addLayout(hlyout);
        hlyout = new QHBoxLayout;
    }
    QScrollArea *area = new QScrollArea;
    s_tqWidget *CellW = new s_tqWidget;
    area->setWidget(CellW);
    area->setWidgetResizable(true);
    CellW->setObjectName("cellwidget"); // подготовка виджета для отображения мест размещения
    vlyout->addWidget(area, 1);
    hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton("Назад");
    pb->setIcon(QIcon(":/res/back.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(GoBack()));
    hlyout->addWidget(pb);
    if (IsWarehouse)
        pb->setEnabled(false);
    else
        pb->setEnabled(true);
    pb = new s_tqPushButton("Записать и закрыть");
    pb->setIcon(QIcon(":/res/icon_zap.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(WriteAndClose()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton("Отмена");
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this,SLOT(CancelAndClose()));
    hlyout->addWidget(pb);
    vlyout->addLayout(hlyout);
    wdgt->setLayout(vlyout);
    stw->addWidget(wdgt);
    // взять по ID наименование места размещения
    UpdatePlace(); // принудительное обновление данных в рабочем пространстве
    stw->repaint();
}

void Wh_Editor::UpdatePlace()
{
    WhPlacesTreeModel::WhPlacesTreeItem *item = WhModel->Data(CurID);
    if (item == NULL)
        return;
    if (item->UpdIns == WHP_CREATENEW) // новый элемент, его ещё нет в базе данных и по нему не заполнена модель
        return;
    QStringList fl = QStringList() << "Тип размещения" << "Наименование" << "Кол-во рядов" << "Кол-во этажей";
    QStringList vl;
    QString table = "Склады типы размещения_полн";
    QString field = "ИД";
    QString value = QString::number(item->WhPlaceTypeID);
    tfl.valuesbyfield(table, fl, field, value, vl);
    if ((tfl.result) || (vl.size() < 4)) // нет размещения в БД
        return;
    CurIDProperties.PlaceType = vl.at(0).toInt();
    CurIDProperties.ChoosePlaceString = vl.at(1);
    CurIDProperties.Columns = vl.at(2).toInt();
    CurIDProperties.Rows = vl.at(3).toInt();
    if ((CurIDProperties.Columns == -1) || (CurIDProperties.Rows == -1))
    {
        WHEDWARN;
        return;
    }
    if (CurIDProperties.PlaceType == 0) // текущее размещение - склад (корневой элемент)
        CurIDProperties.PlacePrefix = item->Alias;
    else
    {
        fl = QStringList() << "Наименование" << "Картинка";
        table = "Склады ёмкости размещения_полн";
        value = QString::number(CurIDProperties.PlaceType);
        tfl.valuesbyfield(table, fl, field, value, vl);
        if ((tfl.result) || (vl.size() < 2))
        {
            WHEDWARN;
            return;
        }
        CurIDProperties.PlacePrefix = vl.at(0);
        CurIDProperties.Picture = vl.at(1);
        s_tqChooseWidget *cw = this->findChild<s_tqChooseWidget *>("chooseplace");
        if (cw == 0)
        {
            WHEDDBG;
            return;
        }
        cw->SetValue(CurIDProperties.ChoosePlaceString);
    }
    item->Alias = CurIDProperties.PlacePrefix;
    item->WhID = Wh;
    // затем для данного curID отображаем места размещения
    s_tqWidget *w = this->findChild<s_tqWidget *>("cellwidget");
    if (w == 0)
    {
        WHEDDBG;
        return;
    }
    QLayout *l = w->layout();
    ClearLayout(l);
    SetCells(w);
}

void Wh_Editor::ClearLayout(QLayout *lyout)
{
    if (lyout == 0)
        return;
    QLayoutItem *item;
    while((item = lyout->takeAt(0)))
    {
        if (item->layout())
        {
            ClearLayout(item->layout());
            delete item->layout();
        }
    }
}

void Wh_Editor::SetCells(QWidget *w)
{
    QStringList sl;
    QList<int> Children = WhModel->Children(CurID);
    int ChildrenSize = Children.size();
    int ChildrenIndex = 0;
    QVBoxLayout *lyout = new QVBoxLayout;
    for (int i = 0; i < CurIDProperties.Rows; i++)
    {
        QHBoxLayout *hlyout = new QHBoxLayout;
        for (int j = 0; j < CurIDProperties.Columns; j++)
        {
            QVBoxLayout *v2lyout = new QVBoxLayout;
            s_tqLabel *celllbl = new s_tqLabel;
            celllbl->setMaximumHeight(50);
            int index = (ChildrenIndex < ChildrenSize) ? Children.at(ChildrenIndex) : 0;
            ChildrenIndex++;
            celllbl->setObjectName(QString::number(index));
            connect(celllbl,SIGNAL(clicked()),this,SLOT(GoToPlace()));
            v2lyout->addWidget(celllbl);
            v2lyout->setAlignment(celllbl, Qt::AlignCenter);
            s_tqLineEdit *le = new s_tqLineEdit;
            le->setObjectName(QString::number(index));
            v2lyout->addWidget(le);
            v2lyout->setAlignment(le,Qt::AlignCenter);
            hlyout->addLayout(v2lyout);
            sl = NameAndPicture(index);
            if (sl.size() > 1)
            {
                celllbl->setPixmap(QPixmap(sl.at(1)));
                le->setText(sl.at(0));
            }
            else
            {
                celllbl->setPixmap(QPixmap(":/res/EmptyCell.png"));
                le->setText(QString::number(i+10, 36).toUpper()+QString::number(j+1));
                if (index == 0)
                {
                    // создать новое пустое размещение с родителем idalias = CurID
                    WhPlacesTreeModel::WhPlacesTreeItem *item = new WhPlacesTreeModel::WhPlacesTreeItem;
                    item->Alias = "";
                    item->Name = le->text();
                    item->IdAlias = CurID;
                    item->UpdIns = WHP_CREATENEW;
                    item->WhPlaceTypeID = 0;
                    item->Description = "";
                    item->WhID = Wh;
                    item->WhNum = i*CurIDProperties.Columns + j;
                    index = WhModel->Insert(item);
                    SomethingChanged = true;
                    celllbl->setObjectName(QString::number(index));
                    le->setObjectName(QString::number(index));
                }
            }
        }
        hlyout->addStretch(1);
        lyout->addLayout(hlyout);
    }
    lyout->addSpacing(50);
    w->setLayout(lyout);
}

void Wh_Editor::GoToPlace()
{
    int ID = sender()->objectName().toInt();
    if (ID == -1)
    {
        WHEDWARN;
        return;
    }
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>(QString::number(ID));
    if (le == 0)
    {
        WHEDDBG;
        return;
    }
    IDs.push(ID);
    BuildWorkspace(ID, false);
}

void Wh_Editor::GoBack()
{
    if (IDs.size() == 1) // для верхнего уровня выше переходить уже некуда
        return;
    IDs.pop();
    int ID = IDs.top();
    if (IDs.size() > 1)
        BuildWorkspace(ID, false);
    else
        BuildWorkspace(ID, true);
}

void Wh_Editor::ChangePlace(QVariant PlaceName)
{
    if (PlaceName == CurIDProperties.ChoosePlaceString) // если ничего не поменялось, выход
        return;
    if (!CheckPriorities(PlaceName.toString()))
    {
        WHEDINFO("Невозможно создать размещение, не соблюдены правила вложенности");
        return;
    }
    WhPlacesTreeModel::WhPlacesTreeItem *item = WhModel->Data(CurID);
    if (item == NULL)
    {
        WHEDDBG;
        return;
    }
    QStringList vl;
    // проверяем, нет ли размещения в БД
    QStringList PlaceType;
    QString table = "Склады размещение_полн";
    QStringList fields = QStringList("Наименование");
    QString field = "ИД";
    QString value = QString::number(item->Id);
    tfl.valuesbyfield(table, fields, field, value, PlaceType, false);
    if (!PlaceType.isEmpty()) // есть такое размещение
    {
        if (PlaceType.at(0).toInt() != 0) // непустое размещение
        {
            if (!(MessageBox2::question(this, "Размещение уже имеется", "Вы уверены, что хотите расформировать старое размещение?")))
                return;
            Disband(CurID);
        }
    }
    else
    {
        // теперь проверяем, нет ли размещения в модели
        QList<int> tmpl = WhModel->Children(item->Id);
        if (!tmpl.isEmpty()) // есть размещение
        {
            if (!(QMessageBox::question(this, "Размещение уже имеется", "Вы уверены, что хотите расформировать старое размещение?")))
                return;
            Disband(CurID);
        }
    }
    if (item->UpdIns == WHP_CREATENEW) // новосозданный элемент
        item->UpdIns = WHP_UPDATENEW;
    if (item->UpdIns == WHP_UNCHANGED)
        item->UpdIns = WHP_UPDATE;
    table = "Склады типы размещения_полн";
    fields = QStringList("ИД");
    field = "Наименование";
    value = PlaceName.toString();
    tfl.valuesbyfield(table, fields, field, value, vl);
    if (tfl.result)
    {
        WHEDWARN;
        return;
    }
    item->WhPlaceTypeID = vl.at(0).toInt();
    SomethingChanged = true;
    UpdatePlace();
    return;
}

// проверка возможности создания в размещении CurID->idalias элемента с именем PlaceName

bool Wh_Editor::CheckPriorities(QString PlaceName)
{
    // вытаскиваем по PlaceName приоритет размещения PrNew
    QStringList vl;
    QString table = "Склады типы размещения_полн";
    QStringList fields = QStringList("Тип размещения");
    QString field = "Наименование";
    tfl.valuesbyfield(table, fields, field, PlaceName, vl);
    if ((tfl.result) || (vl.size() < 1))
    {
        WHEDWARN;
        return false;
    }
    table = "Склады ёмкости размещения_полн";
    fields = QStringList("Приоритет вложенности");
    field = "ИД";
    QString value = vl.at(0);
    tfl.valuesbyfield(table, fields, field, value, vl);
    if ((tfl.result) || (vl.size() < 1))
    {
        WHEDWARN;
        return false;
    }
    int PrNew = vl.at(0).toInt();
    // вытаскиваем по CurID->idalias приоритет размещения PrParent
    WhPlacesTreeModel::WhPlacesTreeItem *item = WhModel->Data(CurID);
    int IdAlias = item->IdAlias;
    table = "Склады размещение_полн";
    fields = QStringList("Тип размещения");
    value = QString::number(IdAlias);
    tfl.valuesbyfield(table,fields,field,value, vl);
    if ((tfl.result) || (vl.size() < 1))
    {
        WHEDWARN;
        return false;
    }
    table = "Склады типы размещения_полн";
    value = vl.at(0);
    tfl.valuesbyfield(table, fields, field, value, vl);
    if ((tfl.result) || (vl.size() < 1))
    {
        WHEDWARN;
        return false;
    }
    table = "Склады ёмкости размещения_полн";
    fields = QStringList("Приоритет вложенности");
    value = vl.at(0);
    tfl.valuesbyfield(table, fields, field, value, vl);
    if ((tfl.result) || (vl.size() < 1))
    {
        WHEDWARN;
        return false;
    }
    int PrParent = vl.at(0).toInt();
    // если PrParent > PrNew, то нельзя создать размещение
    if (PrParent > PrNew)
        return false;
    // если PrParent = PrNew и это 0, то нельзя создать размещение
    if (PrNew = 0) // склад создаётся другими способами
        return false;
    // в противном случае можно
    return true;
}

void Wh_Editor::Disband(int ID)
{
    Q_UNUSED(ID);
    // сначала по ИД вытаскиваем элемент из модели
    // проверяем его признак Updins, если новый, то расформировывать нечего, ничего ещё не положено
    // если старый, то надо расформировать размещение:
    // найти все элементы номенклатуры, у которых данное размещение указано по умолчанию и удалить его
    // найти все элементы номенклатуры, которые лежали в этом размещении или в размещениях, которые находятся в этом размещении и переложить их в общую кучу
    // удалить размещение и всех его потомков
}

QStringList Wh_Editor::NameAndPicture(int ID)
{
    WhPlacesTreeModel::WhPlacesTreeItem *item = WhModel->Data(ID);
    if (item == NULL)
        return QStringList();
//    QStringList fl = QStringList() << "Наименование" << "Обозначение" << "Тип размещения";
//    QStringList PlaceProp = tfl.valuesbyfield("Склады размещение_полн", fl, "ИД", QString::number(ID), false);
//    if ((tfl.result) || (PlaceProp.size()<3))
//        return QStringList(); // пустое размещение
//    QString tmps = PlaceProp.at(0) + " " + PlaceProp.at(1);
    QString tmps = item->Alias + " " + item->Name;
    QStringList sl;
    sl << tmps;
//    fl = QStringList() << "Тип размещения";
//    QStringList PlaceTank = tfl.valuesbyfield("Склады типы размещения_полн", fl, "ИД", PlaceProp.at(2));
    QStringList PlaceTank;
    QString table = "Склады типы размещения_полн";
    QStringList fields = QStringList("Тип размещения");
    QString field = "ИД";
    QString value = QString::number(item->WhPlaceTypeID);
    tfl.valuesbyfield(table, fields, field, value, PlaceTank);
    if ((tfl.result) || (PlaceTank.size() < 1))
    {
//        WHEDWARN;
        return QStringList(); // размещение ещё пустое (нет ссылки на элемент размещения)
    }
    table = "Склады ёмкости размещения_полн";
    fields = QStringList("Картинка");
    value = PlaceTank.at(0);
    QStringList PlacePicture;
    tfl.valuesbyfield(table, fields, field, value, PlacePicture);
    if (tfl.result)
    {
        WHEDWARN;
        return QStringList();
    }
    tmps = ":/res/"+PlacePicture.at(0)+".png";
    sl << tmps;
    return sl;
}
