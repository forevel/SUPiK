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

#include <QHBoxLayout>
#include <QIcon>
#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QStringListModel>
#include <QStandardItemModel>

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
    connect(cb,SIGNAL(currentIndexChanged(QString)),this,SLOT(ModifyWh(QString)));
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
    s_tqWidget *wdgt = new s_tqWidget;
    wdgt->setAttribute(Qt::WA_DeleteOnClose);
    connect(this,SIGNAL(CloseAllWidgets()),wdgt,SLOT(close())); // по нажатию кнопки "Закрыть вкладку" закрыть и удалить "корневой" виджет
/*    IDs.push(vl.at(0).toInt()); // первоначальный ID - корневой склада первого по списку
    Columns.push(MAXCOLS); // первоначальное значение количества столбцов
    Rows.push(MAXROWS); // первоначальное значение количества строк */
    lyout->addWidget(stw);
    lyout->addStretch(1);
    setLayout(lyout);
    cb->setCurrentIndex(0); // принудительный вызов загрузки данных в окно
}

void Wh_Editor::SetRootWidget()
{
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
    SetCells(vlyout);
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton("Записать и закрыть");
    pb->setIcon(QIcon(":/res/icon_zap.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(WriteAndClose()));
    connect(pb, SIGNAL(clicked()), this,SIGNAL(CloseAllWidgets()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton("Отмена");
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this,SIGNAL(CloseAllWidgets()));
//    connect(pb,SIGNAL(clicked()),this,SLOT(PopIDs()));
    hlyout->addWidget(pb);
    vlyout->addLayout(hlyout);
    wdgt->setLayout(vlyout);
    stw->addWidget(wdgt);
    stw->repaint();
}

void Wh_Editor::SetCells(QVBoxLayout *lyout)
{
    for (int i = 0; i < Rows.top(); i++)
    {
        QHBoxLayout *hlyout = new QHBoxLayout;
        for (int j = 0; j < Columns.top(); j++)
        {
            int index = i*Columns.top()+j;
            QVBoxLayout *v2lyout = new QVBoxLayout;
            s_tqLabel *celllbl = new s_tqLabel;
            celllbl->setObjectName(QString::number(IDs.top())+"."+QString::number(index));
            connect(celllbl,SIGNAL(clicked()),this,SLOT(UpdatePlace()));
            v2lyout->addWidget(celllbl);
            v2lyout->setAlignment(celllbl, Qt::AlignCenter);
            s_tqLabel *lbl = new s_tqLabel("");
            lbl->setObjectName("celllbl."+QString::number(IDs.top())+"."+QString::number(index));
            v2lyout->addWidget(lbl);
            v2lyout->setAlignment(lbl,Qt::AlignCenter);
            hlyout->addLayout(v2lyout);
            if (UpdatePlacePicture(celllbl))
                celllbl->setData(index); // есть размещение в данном месте
            else
                celllbl->setData(-1); // нет такого размещения
        }
        lyout->addLayout(hlyout);
    }
}

bool Wh_Editor::UpdatePlacePicture(s_tqLabel *lbl)
{
    WhPlacesTreeModel::WhPlacesTreeItem *item;
    int index = lbl->objectName().split(".").at(1).toInt();
    QStringList cmpvl = QStringList() << QString::number(IDs.top()) << QString::number(index);
    if (!WhModel->Find(0x22, cmpvl)) // 0x21 = WhNum | IdAlias
         item = WhModel->Next();
    else
    {
        lbl->setPixmap(QPixmap(":/res/EmptyCell.png"));
        return false; // пусто
    }
    if (item == 0)
    {
        lbl->setPixmap(QPixmap(":/res/EmptyCell.png"));
        return false; // пусто
    }
    QStringList PlaceTank = tfl.valuesbyfield("Склады типы размещения_полн", QStringList("Тип размещения"), "ИД", QString::number(item->WhPlaceTypeID));
    if (tfl.result)
    {
        WHEDWARN;
        return false;
    }
    QStringList PlacePicture = tfl.valuesbyfield("Склады ёмкости размещения_полн", QStringList("Картинка"), "ИД", PlaceTank.at(0));
    if (tfl.result)
    {
        WHEDWARN;
        return false;
    }
    lbl->setPixmap(QPixmap(PlacePicture.at(0)));
    return true;
}

void Wh_Editor::UpdatePlace()
{
    // вытащим номера столбца и строки из наименования источника сигнала
    QStringList CellNames = sender()->objectName().split(".");
    if (CellNames.size() < 2) // 0 - ID, 2 - index
    {
        WHEDDBG;
        return;
    }
    int id = CellNames.at(0).toInt();
    int PlaceIndex = CellNames.at(1).toInt(); // текущий индекс элемента
    s_tqLabel *lbl = static_cast<s_tqLabel *>(sender());
    if (lbl == 0)
    {
        WHEDDBG;
        return;
    }
    int DataIndex = lbl->getData().toInt(); // если DataIndex == -1, размещения в данном месте нет, иначе DataIndex - то же самое, что и PlaceIndex
    if (DataIndex != -1) // если размещение имеется
        PushNewPlaceOnStacks(id, PlaceIndex);
    else
    {
        IDs.push(-1);
        Columns.push(-1);
        Rows.push(-1);
    }
    SetChildWidget(lbl);
}

void Wh_Editor::PushNewPlaceOnStacks(int ID, int Index)
{
    // возьмём ID элемента размещения по "Склады размещение" и положим его в стек
    //    QStringList cmpfl = QStringList() << "Номер" << "ИД_а";
    int cmpfl = 0x22; // idalias = 0x02, WhNum = 0x20
    QStringList cmpvl = QStringList() << QString::number(ID) << QString::number(Index);
    WhPlacesTreeModel::WhPlacesTreeItem *item;
    if (WhModel->Find(cmpfl, cmpvl));
    else // нет такого размещения в нашем "шкафу", значит, при записи когда-то был косяк, выход
    {
        WHEDWARN;
        return;
    }
    item = WhModel->Next();
    //    QStringList fl = QStringList() << "ИД" << "Тип размещения";
    IDs.push(item->Id);
    // по типу размещения найдём количество столбцов в размещении
    QStringList fl = QStringList() << "Кол-во рядов" << "Кол-во столбцов";
    QStringList PlaceColumnsCount = tfl.valuesbyfield("Склады типы размещения_полн",fl,"Наименование",QString::number(item->WhPlaceTypeID));
    if (tfl.result)
    {
        PopIDs();
        WHEDWARN;
        return;
    }
    Columns.push(PlaceColumnsCount.at(0).toInt()); // положили количество столбцов в стек
    Rows.push(PlaceColumnsCount.at(1).toInt()); // и количество строк в свой стек
}

void Wh_Editor::PopIDs()
{
    if (!IDs.isEmpty())
        IDs.pop();
}

void Wh_Editor::SetChildWidget(s_tqLabel *celllbl)
{
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("stw");
    if (stw == 0)
    {
        WHEDDBG;
        return;
    }
    s_tqWidget *wdgt = new s_tqWidget;
    wdgt->setAttribute(Qt::WA_DeleteOnClose);
    connect(this,SIGNAL(CloseAllWidgets()),wdgt,SLOT(close()));

    s_tqLabel *lbl = new s_tqLabel("Тип размещения:");
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    s_tqChooseWidget *cw = new s_tqChooseWidget(true);
    cw->Setup(("2.2..Склады типы размещения_сокращ.Наименование"));
    QString tmps = QString::number(IDs.top())+"."+QString::number(Rows.top())+"."+QString::number(Columns.top());
    cw->setObjectName(tmps); // оставляем только номер строки и столбца
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(UpdatePicture(QVariant)));
    hlyout->addWidget(cw);
    lyout->addLayout(hlyout);
    int DataIndex = celllbl->getData().toInt();
    s_tqLabel *PlacePic = new s_tqLabel;
    PlacePic->setObjectName(tmps);
    PlacePic->setPixmap(*(celllbl->pixmap()));
    lyout->addWidget(PlacePic);
    QVBoxLayout *vlyout = new QVBoxLayout;
    s_tqWidget *w = new s_tqWidget;
    w->setObjectName("chw"+QString::number(IDs.top()));
//    if (DataIndex != -1) // если размещение имеется
        // рисуем размещения по строкам и столбцам
        SetCells(vlyout);
    w->setLayout(vlyout);
    lyout->addWidget(w);
    hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton("Записать и закрыть");
    pb->setIcon(QIcon(":/res/icon_zap.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(WriteAndClose()));
    connect(pb, SIGNAL(clicked()), this,SIGNAL(CloseAllWidgets()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton("Отмена");
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this,SIGNAL(CloseAllWidgets()));
    connect(pb,SIGNAL(clicked()),this,SLOT(PopIDs()));
    hlyout->addWidget(pb);
    lyout->addLayout(hlyout);
    wdgt->setAttribute(Qt::WA_DeleteOnClose);
    wdgt->setLayout(lyout);
    int idx = stw->addWidget(wdgt);
    stw->setCurrentIndex(idx);
    stw->repaint();
}

// обновление картинки типа размещения

void Wh_Editor::UpdatePicture(QVariant value)
{
/*    int row, column;

    // вытащим номера столбца и строки из наименования источника сигнала
    QStringList CellNames = sender()->objectName().split(".");
    if (CellNames.size() < 3)
    {
        WHEDDBG;
        return;
    }
    row = CellNames.at(1).toInt();
    column = CellNames.at(2).toInt();
    if ((row != -1) && (column != -1))
    {*/
    QStringList fl = QStringList() << "Тип размещения" << "Кол-во этажей" << "Кол-во рядов";
    QStringList PlaceTank = tfl.valuesbyfield("Склады типы размещения_полн", fl, "Наименование", value.toString());
    if (tfl.result)
    {
        WHEDWARN;
        return;
    }
    QStringList PlacePicture = tfl.valuesbyfield("Склады ёмкости размещения_полн", QStringList("Картинка"), "ИД", PlaceTank.at(0));
    if (tfl.result)
    {
        WHEDWARN;
        return;
    }
    s_tqLabel *lbl = this->findChild<s_tqLabel *>(sender()->objectName()); // имя ChooseWidget-а и имя строки совпадают
    if (lbl == 0)
    {
        WHEDDBG;
        return;
    }
    // добавляем строку с новым ИД в модель
    WhPlacesTreeModel::WhPlacesTreeItem *item = new WhPlacesTreeModel::WhPlacesTreeItem;
    item->WhPlaceTypeID = PlaceTank.at(0).toInt();
    item->
    lbl->setPixmap(QPixmap(":/res/"+PlacePicture.at(0)+".png"));
    UpdateChildWidget();
/*    }
    else
    {
        WHEDWARN;
        return;
    } */
}

void Wh_Editor::UpdateChildWidget()
{
    s_tqWidget *w = this->findChild<s_tqWidget *>("chw"+QString::number(IDs.top()));
    if (w == 0)
    {
        WHEDDBG;
        return;
    }
    QVBoxLayout *lyout = new QVBoxLayout;
    QLayout *l = w->layout();
    if (l != 0)
        delete l;
    SetCells(lyout);
    w->setLayout(lyout);
}

void Wh_Editor::WriteAndClose()
{
//    UpdatePlacePicture(0); // подставить сюда row и column
}

void Wh_Editor::AddNewWh()
{

}

void Wh_Editor::DeleteWh()
{

}

void Wh_Editor::CheckIndexes()
{
/*    for (int i=0; i<Rows.top(); i++)
    {
        for (int j=0; j<Columns.top(); j++)
        {
            int index = i * Columns.top() + j;
            // если элемент имеется в данном размещении, выставим соответствующий флаг
            QStringList fl = QStringList() << "ИД";
            QStringList cmpfl = QStringList() << "Номер" << "ИД_а";
            QStringList cmpvl = QStringList() << QString::number(index) << QString::number(IDs.top());
            QStringList PlaceID = tfl.valuesbyfields("Склады размещение_полн",fl,cmpfl, cmpvl,false);
            if (PlaceID.isEmpty());
            else
                IndexIsPresent.replace(index, true);
            // теперь надо установить текущую картинку
            UpdatePlacePicture(row, column);
        }
    } */
}

void Wh_Editor::ModifyWh(QString str)
{
    // достанем индекс склада по имени str из wh
    QStringList WHID = tfl.valuesbyfield("Склады_полн", QStringList("ИД"), "Наименование", str);
    if (tfl.result)
    {
        WHEDWARN;
        return;
    }
    // затем достанем индекс ID размещения данного склада
    QStringList cmpfl = QStringList() << "ИД_а" << "Склад";
    QStringList cmpvl = QStringList() << "0" << WHID.at(0); // у "корневого" склада idalias всегда равен 0, т.е. это корень для остальных
    QStringList PlaceID = tfl.valuesbyfields("Склады размещение_полн", QStringList("ИД"), cmpfl, cmpvl, false);
/*    if (tfl.result)
    {
        WHEDWARN;
        return;
    } */
    // обнулим стеки IDs, Rows, Columns
    IDs.clear();
    Rows.clear();
    Columns.clear();
    // положим ID на верхушку стека IDs, MAXROWS и MAXCOLS в соотв. стеки
    IDs.push(PlaceID.at(0).toInt());
    Rows.push(MAXROWS);
    Columns.push(MAXCOLS);
    // закроем все открытые виджеты, включая "корневой"
    emit CloseAllWidgets();
    // построим модель от данного корневого ИД склада
    if (WhModel != 0)
        delete WhModel;
    WhModel = new WhPlacesTreeModel;
    if (WhModel->Load(PlaceID.at(0).toInt()))
    {
        WHEDWARN;
        return;
    }
    // создадим новый корневой виджет и положим его в stw, вызовем SetCells для нового ID
    SetRootWidget();
}
