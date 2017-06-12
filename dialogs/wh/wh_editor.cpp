#include "wh_editor.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/s_tqspinbox.h"
#include "../../widgets/s_tqgroupbox.h"
#include "../../widgets/s_tqwidget.h"
#include "../../widgets/s_tqchoosewidget.h"
#include "../../widgets/s_tqscrollarea.h"
#include "../../widgets/wd_func.h"
#include "../../gen/s_tablefields.h"
#include "../../gen/publicclass.h"
#include "../gen/messagebox.h"
#include "../gen/twocoldialog.h"

#include <QMessageBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QStringListModel>
#include <QStandardItemModel>

WhPlacesModel::WhPlacesModel(QObject *parent) : QObject(parent)
{

    ClearModel();
}

WhPlacesModel::WhPlacesItem WhPlacesModel::Item(int index)
{
    if ((index < Items.count()) && (index >= 0))
        return Items.at(index);
    WhPlacesItem item;
    item.Id = "#"; // неправильный итем
    return item;
}

void WhPlacesModel::SetItem(int index, WhPlacesModel::WhPlacesItem &value)
{
    if (index < Items.count())
        Items.replace(index, value);
}

void WhPlacesModel::InsertItem(WhPlacesModel::WhPlacesItem &value)
{
    Items.append(value);
}

int WhPlacesModel::SetupModel(int rootid)
{
    RootID = rootid;
    ClearModel();
    QStringList fl = QStringList() << "ИД" << "Наименование" << "Обозначение" << "Ёмкость размещения" << "Ряд" << "Столбец" << "Рядов" << "Столбцов";
    QStringList cmpfields = QStringList() << "ИД_а";
    QStringList cmpvalues = QStringList() << QString::number(RootID);
    QList<QStringList> lsl;
    tfl.valuesbyfieldsmatrix("Склады размещение_полн", fl, cmpfields, cmpvalues, lsl);
    if (tfl.result == TFRESULT_ERROR)
        return RESULTBAD;
    while (lsl.size() > 0)
    {
        QStringList tmpsl = lsl.takeAt(0);
        if (tmpsl.size() < 8)
            continue;
        WhPlacesItem item;
        item.Id = tmpsl.at(0);
        item.Description = tmpsl.at(1);
        item.Name = tmpsl.at(2);
        item.WhPlacesTanksID = tmpsl.at(3).toInt(); // idwhplacestanks
        item.Row = tmpsl.at(4).toInt();
        item.Column = tmpsl.at(5).toInt();
        item.Rows = tmpsl.at(6).toInt();
        item.Columns = tmpsl.at(7).toInt();
        if (item.WhPlacesTanksID != "0") // склад
        {
            tfl.valuesbyfield("Склады ёмкости размещения_полн", QStringList("Приоритет вложенности"), "ИД", QString::number(item.WhPlacesTanksID), tmpsl);
            if ((tfl.result == TFRESULT_NOERROR) && !tmpsl.isEmpty())
                item.Priority = tmpsl.at(0).toInt();
            else item.Priority = WRONGNUM;
        }
        else
            item.Priority = -1; // склад
        InsertItem(item);
    }
    return RESULTOK;
}

int WhPlacesModel::Save()
{
    while (Items.size())
    {
        WhPlacesItem item = Items.takeAt(0);
        QStringList fl = QStringList() << "ИД" << "Наименование" << "Обозначение" << "Ёмкость размещения" << "Ряд" << "Столбец" << "Рядов" << "Столбцов";
        QStringList vl = QStringList() << item.Id << item.Description << item.Name << QString::number(item.WhPlacesTanksID) << \
                                          QString::number(item.Row) << QString::number(item.Column) << QString::number(item.Rows) << QString::number(item.Columns);
        tfl.Update("Склады размещение_полн", fl, vl);
        if (tfl.result != RESULTOK)
            WARNMSG("Ошибка при обновлении данных, ИД=" + item.Id);
    }
    return RESULTOK;
}

void WhPlacesModel::ClearModel()
{
    Items.clear();
}

// ################################# WH_EDITOR #######################################

Wh_Editor::Wh_Editor(QWidget *parent) : QDialog(parent)
{
    SomethingChanged = false;
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
}

void Wh_Editor::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/WhWallpaper.png"));
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
/*    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/whPB-red.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(DeleteWh()));
    pb->setToolTip("Расформировать и удалить склад");
    hlyout->addWidget(pb); */
    hlyout->addSpacing(5);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);
    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel("Редактор складов");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);
/*    hlyout = new QHBoxLayout;
    // Комбобокс с наименованиями складов из wh
    s_tqComboBox *cb = WDFunc::NewCB(this, "whcb");
    connect(cb,SIGNAL(currentIndexChanged(QString)),this,SLOT(ChangeWh(QString)));
    lbl = new s_tqLabel("Редактируемый склад:");
    hlyout->addWidget(lbl, 0);
    hlyout->addWidget(cb, 2);
    hlyout->addStretch(20);
    lyout->addLayout(hlyout); */
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    lyout->addWidget(line);
    s_tqStackedWidget *stw = new s_tqStackedWidget;
    stw->setObjectName("stw");
    lyout->addWidget(stw);
    stw->setAttribute(Qt::WA_DeleteOnClose);
    s_tqScrollArea *area = new s_tqScrollArea;
    area->setWidget(stw);
    area->setWidgetResizable(true);
    lyout->addWidget(area);
    setLayout(lyout);
    // создаём корневой элемент, в котором находятся склады
    WhPlacesModel::WhPlacesItem item;
    item.Id = 0;
    item.Priority = MAXPRIORITY;
    ItemsStack.push(item);
    OpenSpace();
}

void Wh_Editor::OpenSpace()
{
    bool ok;
    // построим модель от данного корневого ИД склада
    CheckChanges();
    if (WhModel != 0)
        delete WhModel;
    WhModel = new WhPlacesModel;
    int ID = ItemsStack.last().Id.toInt(&ok);
    if (!ok)
    {
        WARNMSG("Ошибка при построении места размещения"+ItemsStack.last().Id);
        return;
    }
    if (WhModel->SetupModel(ID))
    {
        WARNMSG("Ошибка при построении модели WhModel");
        return;
    }
    SomethingChanged = false;
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget>("stw");
    if (stw == 0)
    {
        DBGMSG;
        return;
    }
    s_tqWidget *w = new s_tqWidget;
    QVBoxLayout *vlyout = new QVBoxLayout;
    // построим текстовое поле места размещения
    QString PlaceNameString;
    if (!ItemsStack.isEmpty())
    {
        WhPlacesModel::WhPlacesItem item = ItemsStack.last();
        int idi = item.Id.toInt();
        QString Id = item.Id;
        if (idi == -1)
        {
            WARNMSG("");
            return;
        }
        while (Id != "0")
        {
            QStringList tmpsl;
            QStringList sl = QStringList() << "Наименование" << "Обозначение" << "ИД_а";
            tfl.valuesbyfield("Склады размещение_полн", sl, "ИД", Id, tmpsl);
            if ((tfl.result) || (tmpsl.size()<3))
            {
                WARNMSG("");
                return;
            }
            QString tmps = "/" + tmpsl.at(0) + " " + tmpsl.at(1);
            PlaceNameString.insert(0, tmps);
            Id = tmpsl.at(2);
        }
        PlaceNameString.insert(0, ":"); // ":" - обозначение "корня"
        PlaceNameString.append(WhModel->Item(idi).Description + " " + WhModel->Item(idi).Name);
    }
    else
        PlaceNameString = "";

    s_tqLabel *lbl = new s_tqLabel(PlaceNameString);
    vlyout->addWidget(lbl);
    QHBoxLayout *hlyout = new QHBoxLayout;

    s_tqWidget *CellW = SetupCells();
    vlyout->addWidget(CellW);
    hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton("На уровень выше");
    pb->setIcon(QIcon(":/res/back.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(GoBack()));
    hlyout->addWidget(pb);
    if (ItemsStack.size() < 2) // this place is warehouse
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
    w->setLayout(vlyout);
    stw->clear();
    stw->addWidget(w);
    // взять по ID наименование места размещения
    UpdatePlace(); // принудительное обновление данных в рабочем пространстве
    stw->repaint();

}

void Wh_Editor::CheckChanges()
{
    if (SomethingChanged)
    {
        if (MessageBox2::question(this, "Данные были изменены", "Сохранить изменения?"))
            WhModel->Save(); // если модель уже существует, надо сохранить данные в БД, чтобы не потерялись
    }
}

void Wh_Editor::AddNewWh()
{
    CheckChanges();
    QString newID;
    QString table = "Склады размещение_полн";
    tfl.Insert(table, newID);
    QStringList fl = QStringList() << "ИД" << "ИД_а" << "Тип размещения";
    QStringList vl = QStringList() << newID << "0" << "4"; // 4 - тип размещения "Склад"
    tfl.Update(table, fl, vl);
    TwoColDialog *dlg = new TwoColDialog("Склады::Добавить");
    if (dlg->setup(table, MODE_EDITNEW, newID) == RESULTBAD)
        WARNMSG("");
    else
        dlg->exec();
    UpdateWhComboBox();
}

void Wh_Editor::UpdateWhComboBox()
{
    QStringList vl;
    QString table = "Склады размещение_полн";
    QString field = "Наименование";
    QString cmpfield = "ИД_а";
    QString cmpvalue = "0";
    tfl.GetValuesByColumnAndField(table, field, cmpfield, cmpvalue, vl);
    WDFunc::SetCBList(this, "whcb", vl);
}

void Wh_Editor::WriteAndClose()
{
    if (WhModel->Save())
        return;
    else
        MessageBox2::information(this, "Внимание", "Записано успешно!");
    this->close();
}

void Wh_Editor::CancelAndClose()
{
    // удалим новосозданные элементы из таблицы
    CheckChanges();
    this->close();
}

void Wh_Editor::AddNewPlace()
{
    QString newID;
    QString table = "Склады типы размещения_полн";
    tfl.Insert(table, newID);
    TwoColDialog *dlg = new TwoColDialog("Склады::Размещения::Добавить");
    if (dlg->setup("Склады типы размещения_полн", MODE_EDITNEW, newID) != RESULTOK)
        WARNMSG("");
    else
        dlg->exec();
    // отобразить новое размещение на экране
    WhModel->SetupModel(ItemsStack.last().Id.toInt());
//    Update();
}

void Wh_Editor::ChangeWh(QString str)
{
    // достанем индекс склада по имени str из whplaces
    QStringList PlaceID;
    QString table = "Склады размещение_полн";
    QStringList fields = QStringList("ИД");
    QString field = "Наименование";
    tfl.valuesbyfield(table, fields, field, str, PlaceID, false);
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    BuildWorkspace();
}

void Wh_Editor::BuildWorkspace()
{
}

// input: ItemsStack.last() - информация о месте размещения, содержимое которого требуется отобразить
// WhModel - модель размещения

void Wh_Editor::UpdatePlace()
{
    if (ItemsStack.isEmpty())
        return;
    WhPlacesModel::WhPlacesItem item = ItemsStack.last();
    QStringList fl = QStringList() << "Картинка";
    QString table = "Склады ёмкости размещения_полн";
    QStringList vl;
    tfl.valuesbyfield(table, fl, "ИД", QString::number(item.WhPlacesTanksID), vl);
    if ((tfl.result == TFRESULT_ERROR) || (vl.isEmpty()))
    {
        WARNMSG("");
        return;
    }
    s_tqChooseWidget *cw = this->findChild<s_tqChooseWidget *>("chooseplace");
    if (cw == 0)
    {
        DBGMSG;
        return;
    }
    cw->SetValue(CurIDProperties.ChoosePlaceString);
    // затем для данного curID отображаем места размещения
    ClearLayout();
    SetCells();
}

void Wh_Editor::ClearLayout()
{
    s_tqWidget *w = this->findChild<s_tqWidget *>("cellwidget");
    if (w == 0)
    {
        DBGMSG;
        return;
    }
    QLayout *lyout = w->layout();
    if (lyout == 0)
        return;
    for (int j=0; j<lyout->count(); ++j)
    {
        QLayoutItem *item = lyout->itemAt(j);
        if (item)
        {
            lyout->removeItem(item);
            delete item;
        }
    }
}

// input: CurIDProperties - информация о месте размещения, содержимое которого требуется отобразить

s_tqWidget *Wh_Editor::SetupCells()
{
/*    s_tqWidget *w = this->findChild<s_tqWidget *>("cellwidget");
    if (w == 0)
    {
        DBGMSG;
        return;
    }

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
    w->setLayout(lyout); */
}

void Wh_Editor::GoToPlace()
{
    int ID = sender()->objectName().toInt();
    if (ID == -1)
    {
        WARNMSG("");
        return;
    }
    PushItemStackByID(ID);
    BuildWorkspace();
}

void Wh_Editor::GoBack()
{
    if (ItemsStack.size() == 1) // для верхнего уровня выше переходить уже некуда
        return;
    ItemsStack.pop();
    BuildWorkspace();
}

void Wh_Editor::ChangePlace(QVariant PlaceName)
{
/*    if (PlaceName == CurIDProperties.ChoosePlaceString) // если ничего не поменялось, выход
        return;
    if (!CheckPriorities(PlaceName.toString()))
    {
        MessageBox2::information(this, "Внимание", "Невозможно создать размещение, не соблюдены правила вложенности");
        return;
    }
    WhPlacesTreeModel::WhPlacesTreeItem *item = WhModel->Data(CurID);
    if (item == NULL)
    {
        DBGMSG;
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
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    item->WhPlaceTypeID = vl.at(0).toInt();
    SomethingChanged = true;
    UpdatePlace();
    return; */
}

// проверка возможности создания в размещении CurID->idalias элемента с именем PlaceName

bool Wh_Editor::CheckPriorities(QString PlaceName)
{
/*    // вытаскиваем по PlaceName приоритет размещения PrNew
    QStringList vl;
    QString table = "Склады типы размещения_полн";
    QStringList fields = QStringList("Тип размещения");
    QString field = "Наименование";
    tfl.valuesbyfield(table, fields, field, PlaceName, vl);
    if ((tfl.result == TFRESULT_ERROR) || (vl.size() < 1))
    {
        WARNMSG("");
        return false;
    }
    table = "Склады ёмкости размещения_полн";
    fields = QStringList("Приоритет вложенности");
    field = "ИД";
    QString value = vl.at(0);
    tfl.valuesbyfield(table, fields, field, value, vl);
    if ((tfl.result == TFRESULT_ERROR) || (vl.size() < 1))
    {
        WARNMSG("");
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
    if ((tfl.result == TFRESULT_ERROR) || (vl.size() < 1))
    {
        WARNMSG("");
        return false;
    }
    table = "Склады типы размещения_полн";
    value = vl.at(0);
    tfl.valuesbyfield(table, fields, field, value, vl);
    if ((tfl.result == TFRESULT_ERROR) || (vl.size() < 1))
    {
        WARNMSG("");
        return false;
    }
    table = "Склады ёмкости размещения_полн";
    fields = QStringList("Приоритет вложенности");
    value = vl.at(0);
    tfl.valuesbyfield(table, fields, field, value, vl);
    if ((tfl.result == TFRESULT_ERROR) || (vl.size() < 1))
    {
        WARNMSG("");
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
    return true; */
    return true;
}

void Wh_Editor::PushItemStackByID(int ID)
{
    QStringList fl = QStringList() << "Наименование" << "Описание" << "Обозначение" << "Тип размещения";
    QString cmpfield = "ИД";
    QString cmpvalue = QString::number(ID);
    QStringList sl;
    tfl.valuesbyfield("Склады размещение_полн", fl, cmpfield, cmpvalue, sl);
    if (tfl.result != TFRESULT_NOERROR)
    {
        WARNMSG("");
        return;
    }
    if (sl.size() < 4)
    {
        WARNMSG("");
        return;
    }
    WhPlacesModel::WhPlacesItem item;
    item.Id = cmpvalue; // ID
    item.Alias = sl.at(0);
    item.Description = sl.at(1);
    item.Name = sl.at(2);
    item.WhPlaceTypeID = sl.at(3);
    QString tmps;
    tfl.valuesbyfield("Склады типы размещения_полн", QStringList("Тип размещения"), "ИД", item.WhPlaceTypeID, sl);
    if ((tfl.result == TFRESULT_NOERROR) && !sl.isEmpty())
    {
        tmps = sl.at(0);
        tfl.valuesbyfield("Склады ёмкости размещения_полн", QStringList("Приоритет вложенности"), "ИД", tmps, sl);
        if ((tfl.result == TFRESULT_NOERROR) && !sl.isEmpty())
            item.Priority = sl.at(0).toInt();
        else item.Priority = WRONGNUM;
    }
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
    Q_UNUSED(ID);
/*    WhPlacesTreeModel::WhPlacesTreeItem *item = WhModel->Data(ID);
    if (item == NULL)
        return QStringList();
//    QStringList fl = QStringList() << "Наименование" << "Обозначение" << "Тип размещения";
//    QStringList PlaceProp = tfl.valuesbyfield("Склады размещение_полн", fl, "ИД", QString::number(ID), false);
//    if ((tfl.result == TFRESULT_ERROR) || (PlaceProp.size()<3))
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
    if ((tfl.result == TFRESULT_ERROR) || (PlaceTank.size() < 1))
    {
//        WARNMSG("");
        return QStringList(); // размещение ещё пустое (нет ссылки на элемент размещения)
    }
    table = "Склады ёмкости размещения_полн";
    fields = QStringList("Картинка");
    value = PlaceTank.at(0);
    QStringList PlacePicture;
    tfl.valuesbyfield(table, fields, field, value, PlacePicture);
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return QStringList();
    }
    tmps = ":/res/"+PlacePicture.at(0)+".png";
    sl << tmps;
    return sl; */
    return QStringList();
}
