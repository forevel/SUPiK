#include "wh_editor.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqstackedwidget.h"
#include "../widgets/s_tqspinbox.h"
#include "../widgets/s_tqgroupbox.h"
#include "../widgets/s_tqwidget.h"
#include "../widgets/s_tqchoosewidget.h"
#include "../widgets/s_tqtableview.h"
#include "../gen/s_tablefields.h"
#include "../gen/publicclass.h"
#include "../models/placedelegate.h"
#include "s_2cdialog.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QMessageBox>

Wh_Editor::Wh_Editor(QWidget *parent) : QDialog(parent)
{
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
    QStringList vl = tfl.htovl("Склады_сокращ", "Наименование");
    mdl->setStringList(vl);
    s_tqComboBox *cb = new s_tqComboBox;
    cb->setModel(mdl);
    cb->setCurrentIndex(-1);
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
    cb->setCurrentIndex(0); // принудительный вызов загрузки данных в окно
}

void Wh_Editor::WriteAndClose()
{
//    UpdatePlacePicture(0); // подставить сюда row и column
    emit CloseAllWidgets();
}

void Wh_Editor::CancelAndClose()
{
    // удалим новосозданные элементы из таблицы
    WhModel->Find(0x80, WHP_CREATENEW);

    emit CloseAllWidgets();
}

void Wh_Editor::AddNewPlace()
{
    QString newID = tfl.insert("Склады типы размещения_полн");
    s_2cdialog *dlg = new s_2cdialog("Склады::Размещения::Добавить");
    dlg->setup("Склады типы размещения_полн", MODE_EDITNEW, newID);
    if (dlg->result)
        WHEDWARN;
    else
        dlg->exec();
}

void Wh_Editor::ChangeWh(QString str)
{
    // достанем индекс склада по имени str из wh
    QStringList WhID = tfl.valuesbyfield("Склады_полн", QStringList("ИД"), "Наименование", str);
    if ((tfl.result) || (WhID.isEmpty()))
    {
        WHEDWARN;
        return;
    }
    Wh = WhID.at(0).toInt();
    // затем достанем индекс ID размещения данного склада
    QStringList cmpfl = QStringList() << "ИД_а" << "Склад";
    QStringList cmpvl = QStringList() << "0" << WhID.at(0); // у корневого места размещения idalias всегда равен 0, т.е. это корень для остальных
    QStringList PlaceID = tfl.valuesbyfields(WHPLACES, QStringList("ИД"), cmpfl, cmpvl, false);
    if (tfl.result)
    {
        WHEDWARN;
        return;
    }
    // закроем все открытые виджеты, включая "корневой"
    emit CloseAllWidgets();
    // построим модель от данного корневого ИД склада
    if (WhModel != 0)
        delete WhModel;
    WhModel = new WhPlacesTreeModel;
    int ID = PlaceID.at(0).toInt();
    if (WhModel->Load(ID))
    {
        WHEDWARN;
        return;
    }
    IDs.clear();
    IDs.push(ID);
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
        QString tmps = "\\" + item->Alias + " " + item->Name; // формируем имя места размещения как "alias name", например: "Мешок А5"
        PlaceNameString.insert(0, tmps);
        ID = item->IdAlias;
    }
    PlaceNameString.insert(0, ":"); // ":" - обозначение "корня"
    s_tqLabel *lbl = new s_tqLabel(PlaceNameString);
    vlyout->addWidget(lbl);
    QHBoxLayout *hlyout;
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
        s_tqLabel *PlacePic = new s_tqLabel;
        PlacePic->setObjectName("placepic");
        PlacePic->setPixmap(QPixmap(":/res/EmptyCell.png"));
        vlyout->addWidget(PlacePic);
    }
    s_tqWidget *CellW = new s_tqWidget;
    CellW->setObjectName("cellwidget");
    vlyout->addWidget(CellW); // подготовка виджета для отображения мест размещения
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
    QStringList vl = tfl.valuesbyfield("Склады типы размещения_полн", fl, "ИД", QString::number(item->WhPlaceTypeID));
    if ((tfl.result) || (vl.size() < 4)) // нет размещения в БД
    {
        WHEDWARN;
        return;
    }
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
        CurIDProperties.PlacePrefix = "Склад";
    else
    {
        fl = QStringList() << "Наименование" << "Картинка";
        vl = tfl.valuesbyfield("Склады ёмкости размещения_полн", fl, "ИД", QString::number(CurIDProperties.PlaceType));
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
        cw->SetData(CurIDProperties.ChoosePlaceString);
        // сначала в метке PlacePic отображаем картинку для места размещения
        s_tqLabel *lbl = this->findChild<s_tqLabel *>("placepic");
        if (lbl != 0)
        {
            QString tmps;
            if (CurIDProperties.PlaceType != 0) // непустое размещение
            {
                tmps = ":/res/" + CurIDProperties.Picture + ".png";
            }
            else
                tmps = ":/res/EmptyCell.png";
            lbl->setPixmap(QPixmap(tmps));
        }
    }
    if (item->IdAlias != 0)
        item->Alias = CurIDProperties.PlacePrefix;
    item->WhID = Wh;
//    WhModel->Update(item->Id, item);
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
            int index = (ChildrenIndex < ChildrenSize) ? Children.at(ChildrenIndex) : 0;
            ChildrenIndex++;
            celllbl->setObjectName(QString::number(index));
            connect(celllbl,SIGNAL(clicked()),this,SLOT(GoToPlace()));
            v2lyout->addWidget(celllbl);
            v2lyout->setAlignment(celllbl, Qt::AlignCenter);
            s_tqLabel *lbl = new s_tqLabel;
            v2lyout->addWidget(lbl);
            v2lyout->setAlignment(lbl,Qt::AlignCenter);
            hlyout->addLayout(v2lyout);
            if (index != 0)
            {
                sl = NameAndPicture(index);
                if (sl.size() > 1)
                {
                    celllbl->setPixmap(QPixmap(sl.at(1)));
                    lbl->setText(sl.at(0));
                }
                else
                {
                    celllbl->setPixmap(QPixmap(":/res/EmptyCell.png"));
                    lbl->setText("");
                }
            }
            else
            {
                celllbl->setPixmap(QPixmap(":/res/EmptyCell.png"));
                lbl->setText("");
            }
        }
        lyout->addLayout(hlyout);
    }
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
    if (ID == 0) // нет размещения - надо создать новое
    {
        WhPlacesTreeModel::WhPlacesTreeItem *item = new WhPlacesTreeModel::WhPlacesTreeItem;
        item->Alias = "";
        item->Name = "";
        item->IdAlias = IDs.top();
        item->UpdIns = WHP_CREATENEW;
        item->WhPlaceTypeID = 0;
        ID = WhModel->Insert(item);
        if (ID == -1)
        {
            WHEDWARN;
            return;
        }
        sender()->setObjectName(QString::number(ID));
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
    WhPlacesTreeModel::WhPlacesTreeItem *item = WhModel->Data(CurID);
    QStringList vl;
    // проверяем, нет ли размещения в БД
    QStringList PlaceType = tfl.valuesbyfield("Склады размещение_полн", QStringList("Наименование"), "ИД", QString::number(item->Id), false);
    if (!PlaceType.isEmpty()) // есть такое размещение
    {
        if (PlaceType.at(0).toInt() != 0) // непустое размещение
        {
            if (QMessageBox::question(this, "Размещение уже имеется", "Вы уверены, что хотите расформировать старое размещение?", QMessageBox::Yes|QMessageBox::No,\
                                  QMessageBox::No) == QMessageBox::No)
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
            if (QMessageBox::question(this, "Размещение уже имеется", "Вы уверены, что хотите расформировать старое размещение?", QMessageBox::Yes|QMessageBox::No,\
                                  QMessageBox::No) == QMessageBox::No)
                return;
            Disband(CurID);
        }
    }
    if (item->UpdIns == WHP_CREATENEW) // новосозданный элемент
        item->UpdIns = WHP_UPDATENEW;
    if (item->UpdIns == WHP_UNCHANGED)
        item->UpdIns = WHP_UPDATE;
    vl = tfl.valuesbyfield("Склады типы размещения_полн", QStringList("ИД"), "Наименование", PlaceName.toString());
    if (tfl.result)
    {
        WHEDWARN;
        return;
    }
    item->WhPlaceTypeID = vl.at(0).toInt();
//    WhModel->Update(item->Id, item);
    UpdatePlace();
    return;
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
    QStringList fl = QStringList() << "Наименование" << "Обозначение" << "Тип размещения";
    QStringList PlaceProp = tfl.valuesbyfield("Склады размещение_полн", fl, "ИД", QString::number(ID), false);
    if ((tfl.result) || (PlaceProp.size()<3))
        return QStringList(); // пустое размещение
    QString tmps = PlaceProp.at(0) + " " + PlaceProp.at(1);
    QStringList sl;
    sl << tmps;
    fl = QStringList() << "Тип размещения";
    QStringList PlaceTank = tfl.valuesbyfield("Склады типы размещения_полн", fl, "ИД", PlaceProp.at(2));
    if (tfl.result)
    {
        WHEDWARN;
        return QStringList();
    }
    QStringList PlacePicture = tfl.valuesbyfield("Склады ёмкости размещения_полн", QStringList("Картинка"), "ИД", PlaceTank.at(0));
    if (tfl.result)
    {
        WHEDWARN;
        return QStringList();
    }
    tmps = ":/res/"+PlacePicture.at(0)+".png";
    sl << tmps;
    return sl;
}
