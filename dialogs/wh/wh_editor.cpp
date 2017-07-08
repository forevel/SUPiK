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
#include "../../gen/client.h"
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

WhPlacesModel::WhPlacesItem WhPlacesModel::Item(int row, int column)
{
    foreach (WhPlacesModel::WhPlacesItem item, Items)
    {
        if ((item.Row == row) && (item.Column == column))
            return item;
    }
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
        item.PictureIndex = tmpsl.at(3).toInt(); // idwhplacestanks
        item.Row = tmpsl.at(4).toInt();
        item.Column = tmpsl.at(5).toInt();
        item.Rows = tmpsl.at(6).toInt();
        item.Columns = tmpsl.at(7).toInt();
        if (item.PictureIndex != 0) // не склад и не пустое место размещения
        {
            tfl.valuesbyfield("Склады ёмкости размещения_полн", QStringList("Приоритет вложенности"), "ИД", QString::number(item.PictureIndex), tmpsl);
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
        QStringList vl = QStringList() << item.Id << item.Description << item.Name << QString::number(item.PictureIndex) << \
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
    // подготовка списка картинок
    QStringList sl;
    QStringList fl;
    tfl.TableHeaders("Склады ёмкости размещения_полн", fl);
    tfl.GetAllValuesFromTable("Склады ёмкости размещения_полн", sl);
    if (tfl.result)
    {
        WARNMSG("");
        return;
    }
    int ididx, picidx; // индексы полей ИД и Картинка
    ididx = fl.indexOf("ИД");
    picidx = fl.indexOf("Картинка");
    if ((ididx == WRONGNUM) || (picidx == WRONGNUM))
    {
        WARNMSG("Таблица \"Склады ёмкости размещения\" содержит ошибки");
        return;
    }
    int recnum = sl.size() / fl.size(); // количество записей
    picidx *= recnum;
    ididx *= recnum; // записи идут подряд - сначала все по одному полю, затем все по второму и т.д.
    for (int i=0; i<recnum; ++i)
    {
        if ((ididx >= sl.size()) || (picidx >= sl.size()))
        {
            WARNMSG("Таблица \"Склады ёмкости размещения\" содержит ошибки");
            return;
        }
        bool ok;
        int id = sl.at(ididx).toInt(&ok);
        if (ok)
        {
            int res = Cli->GetFile(FLT_WH, FLST_PHOTO, sl.at(picidx));
            if (res == Client::CLIER_NOERROR)
            {
                QString PixFilename = pc.HomeDir + "/wh/photo/" + sl.at(picidx);
                QPixmap *pm = new QPixmap(PixFilename);
                Pictures.insert(Pictures.end(), id, pm);
            }
        }
        ++ididx;
        ++picidx;
    }
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
    hlyout->addWidget(pb);
    hlyout->addSpacing(5); */
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
    hlyout = new QHBoxLayout;
    // Комбобокс с наименованиями складов из wh
    s_tqComboBox *cb = WDFunc::NewCB(this, "whcb");
    connect(cb,SIGNAL(currentIndexChanged(QString)),this,SLOT(ChangeWh(QString)));
    lbl = new s_tqLabel("Склад:");
    hlyout->addWidget(lbl, 0);
    hlyout->addWidget(cb, 2);
    pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/editdoc.png"));
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(EditWarehouse()));
    hlyout->addWidget(pb);
    hlyout->addStretch(20);
    lyout->addLayout(hlyout);
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    lyout->addWidget(line);
    s_tqStackedWidget *stw = new s_tqStackedWidget;
    stw->setObjectName("stw");
    lyout->addWidget(stw);
/*    s_tqScrollArea *area = new s_tqScrollArea;
    area->setWidget(stw);
    area->setWidgetResizable(true);
    lyout->addWidget(area); */
    setLayout(lyout);
    UpdateWhComboBox(); // обновляем комбобокс со складами
//    ChangeWh(cb->currentText());
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
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("stw");
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

    s_tqScrollArea *CellW = SetupCells();
    if (CellW == 0)
    {
        WARNMSG("");
        return;
    }
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
    stw->repaint();
}

// input: CurIDProperties - информация о месте размещения, содержимое которого требуется отобразить

s_tqScrollArea *Wh_Editor::SetupCells()
{
    QString tmps;
    s_tqScrollArea *SArea = new s_tqScrollArea;
    SArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    s_tqWidget *w = new s_tqWidget;
    if (ItemsStack.isEmpty())
    {
        WARNMSG("");
        return 0;
    }
    WhPlacesModel::WhPlacesItem item = ItemsStack.last();

    QVBoxLayout *lyout = new QVBoxLayout;
    for (int i = 0; i < item.Rows; ++i)
    {
        QHBoxLayout *hlyout = new QHBoxLayout;
        for (int j = 0; j < item.Columns; ++j)
        {
            QString CellName = QString::number(i)+"."+QString::number(j);
            QVBoxLayout *v2lyout = new QVBoxLayout;
            s_tqLabel *celllbl = new s_tqLabel;
            celllbl->setMaximumHeight(50);
            celllbl->setObjectName(CellName);
            connect(celllbl,SIGNAL(clicked()),this,SLOT(GoToPlace()));
            s_tqLineEdit *le = new s_tqLineEdit;
            le->setObjectName(CellName);
            WhPlacesModel::WhPlacesItem ModelItem = WhModel->Item(i, j);
            if (ModelItem.Id == "#") // нет итема в данном поле
            {
                WARNMSG("Отсутствует элемент ("+QString::number(i)+","+QString::number(j)+") в модели для ИД "+item.Id);
                // создать новое пустое размещение с родителем idalias = CurID
                ModelItem.Description = "";
                ModelItem.Name = QString::number(i+10, 36).toUpper()+QString::number(j+1);
                tfl.NewID("Склады размещение_полн", tmps);
                ModelItem.Id = tmps.toInt();
                ModelItem.PictureIndex = 0;
                ModelItem.Priority = 0;
                ModelItem.Row = i;
                ModelItem.Column = j;
                ModelItem.Rows = 0;
                ModelItem.Columns = 0;
                WhModel->InsertItem(ModelItem);
                SomethingChanged = true; // добавили элемент, которого нет в БД, надо записать в будущем
            }
            if (ModelItem.PictureIndex != 0) // не пустое место размещения
            {
                if (Pictures.keys().contains(ModelItem.PictureIndex))
                    celllbl->setPixmap(*(Pictures[ModelItem.PictureIndex]));
                else
                    celllbl->setPixmap(QPixmap());
                le->setText(ModelItem.Description + " " + ModelItem.Name);
            }
            else
            {
                Cli->GetFile(FLT_WH, FLST_PHOTO, "EmptyCell");
                celllbl->setPixmap(QPixmap(pc.HomeDir + "/wh/photo/EmptyCell"));
                le->setText(ModelItem.Description + " " + ModelItem.Name);
            }
            v2lyout->addWidget(celllbl);
            v2lyout->setAlignment(celllbl, Qt::AlignCenter);
            QHBoxLayout *h2lyout = new QHBoxLayout;
            h2lyout->addWidget(le);
            s_tqPushButton *pb = new s_tqPushButton;
            pb->setIcon(QIcon(":/res/editdoc.png"));
            pb->setObjectName(QString::number(i)+"."+QString::number(j));
            connect(pb,SIGNAL(clicked(bool)),this,SLOT(EditPlace()));
            h2lyout->addWidget(pb);
            v2lyout->addLayout(h2lyout);
//            v2lyout->setAlignment(le,Qt::AlignCenter);
            hlyout->addLayout(v2lyout);
        }
        hlyout->addStretch(1);
        lyout->addLayout(hlyout);
    }
    lyout->addSpacing(50);
    w->setLayout(lyout);
    SArea->setWidget(w);
    return SArea;
}

void Wh_Editor::GoToPlace()
{
    bool ok;
    int row, column;
    QStringList sl = sender()->objectName().split(".");
    if (sl.size() < 2)
        return;
    row = sl.at(0).toInt(&ok);
    if (!ok)
        return;
    column = sl.at(1).toInt(&ok);
    if (!ok)
        return;
    PushItemStack(row, column);
    OpenSpace();
}

void Wh_Editor::PushItemStack(int row, int column)
{
    WhPlacesModel::WhPlacesItem item = WhModel->Item(row, column);
    if (item.Id != "#")
        ItemsStack.push(item);
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
    QStringList fl = QStringList() << "ИД" << "ИД_а" << "Ёмкость размещения";
    QStringList vl = QStringList() << newID << "0" << "0"; // нет предков, нет ёмкости размещения
    tfl.Update(table, fl, vl);
    TwoColDialog *dlg = new TwoColDialog("Склады::Добавить");
    if (dlg->setup(table, MODE_EDITNEW, newID) == RESULTBAD)
        WARNMSG("");
    else
        dlg->exec();
    // сформировать структуру склада в БД

    UpdateWhComboBox();
}

void Wh_Editor::EditPlace()
{
    QStringList sl = sender()->objectName().split(".");
    if (sl.size() < 2)
        return;
    int row = sl.at(0).toInt();
    int column = sl.at(1).toInt();
    WhPlacesModel::WhPlacesItem item = WhModel->Item(row, column);
    TwoColDialog *dlg = new TwoColDialog("Склады::Редактировать");
    if (dlg->setup("Склады размещение_полн", MODE_EDIT, item.Id) == RESULTBAD)
        WARNMSG("");
    else
        dlg->exec();
    // проверить изменения склада сравнением записанного в БД по индексу item.Id с данными item (rows, columns)
    OpenSpace();
}

void Wh_Editor::EditWarehouse()
{
    WhPlacesModel::WhPlacesItem item = ItemsStack.first(); // первый элемент - это сам склад
    TwoColDialog *dlg = new TwoColDialog("Склады::Редактировать");
    if (dlg->setup("Склады размещение_полн", MODE_EDIT, item.Id) == RESULTBAD)
        WARNMSG("");
    else
        dlg->exec();
    OpenSpace();
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
}

void Wh_Editor::ChangeWh(QString str)
{
    // достанем индекс склада по имени str из whplaces
    QStringList PlaceID;
    QString table = "Склады размещение_полн";
    QStringList fields = QStringList() << "ИД" << "Рядов" << "Столбцов";
    QString field = "Наименование";
    tfl.valuesbyfield(table, fields, field, str, PlaceID, false);
    if ((tfl.result == TFRESULT_ERROR) || (PlaceID.size() < 3))
    {
        WARNMSG("");
        return;
    }
    bool ok;
    PlaceID.at(0).toInt(&ok);
    if (ok)
    {
        ItemsStack.clear();
        WhPlacesModel::WhPlacesItem item;
        item.Id = PlaceID.at(0);
        item.Description = str;
        item.Name.clear();
        item.PictureIndex = -1;
        item.Row = 0;
        item.Column = 0;
        item.Rows = PlaceID.at(1).toInt(&ok);
        if (ok)
            item.Columns = PlaceID.at(2).toInt();
        item.Priority = MAXPRIORITY;
        ItemsStack.push(item);
        OpenSpace();
    }
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

void Wh_Editor::GoBack()
{
    if (ItemsStack.size() == 1) // для верхнего уровня выше переходить уже некуда
        return;
    ItemsStack.pop();
    OpenSpace();
}

void Wh_Editor::ChangePlace(QVariant PlaceName)
{
    Q_UNUSED(PlaceName);
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
