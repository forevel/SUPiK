#include "wh_editor.h"
#include "../widgets/s_tqlabel.h"
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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QStringListModel>
#include <QStandardItemModel>

Wh_Editor::Wh_Editor(QWidget *parent) : QDialog(parent)
{
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
    QVBoxLayout *vlyout = new QVBoxLayout;
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
    hlyout = new QHBoxLayout;
    IDs.push(0); // первоначальный ID - корневой
    Columns.push(MAXCOLS); // первоначальное значение количества столбцов
    Rows.push(MAXROWS); // первоначальное значение количества строк
    SetCells(&vlyout);
    wdgt->setLayout(vlyout);
    stw->addWidget(wdgt);
    stw->repaint();
    lyout->addWidget(stw);
    lyout->addStretch(1);
    setLayout(lyout);
    cb->setCurrentIndex(0); // принудительный вызов загрузки данных в окно
}

void Wh_Editor::SetCells(QLayout &lyout)
{
    for (int i = 0; i < MAXROWS; i++)
    {
        hlyout = new QHBoxLayout;
        for (int j = 0; j < MAXCOLS; j++)
        {
            QVBoxLayout *v2lyout = new QVBoxLayout;
            lbl = new s_tqLabel;
//            lbl->setPixmap(QPixmap(":/res/EmptyCell.png"));
            lbl->setObjectName("cell"+QString::number(IDs.top())+"."+QString::number(i)+"."+QString::number(j));
            connect(lbl,SIGNAL(clicked()),this,SLOT(UpdatePlace()));
            v2lyout->addWidget(lbl);
            v2lyout->setAlignment(lbl, Qt::AlignCenter);
            lbl = new s_tqLabel("");
            lbl->setObjectName("celllbl."+QString::number(IDs.top())+"."+QString::number(i)+"."+QString::number(j));
            v2lyout->addWidget(lbl);
            v2lyout->setAlignment(lbl,Qt::AlignCenter);
            hlyout->addLayout(v2lyout);
            UpdatePlacePicture(i, j);
            IndexIsPresent.push(false); // сначала все размещения на складе пустые, заполним их по загрузке данных
        }
        lyout->addLayout(hlyout);
    }
}

void Wh_Editor::UpdatePlace()
{
    int row, column, id;

    // вытащим номера столбца и строки из наименования источника сигнала
    QStringList CellNames = sender()->objectName().split(".");
    if (CellNames.size() < 4) // 0 - "cell", 1 - ID, 2 - row, 3 - column
    {
        WHEDDBG;
        return;
    }
    QString CellName = CellNames.at(0);
    if (CellName == "cell")
    {
        id = CellNames.at(1).toInt();
        row = CellNames.at(2).toInt();
        column = CellNames.at(3).toInt();
    }
    else
    {
        WHEDWARN;
        return;
    }
    int PlaceIndex = row * Columns.top() + column; // текущий индекс элемента
    if (IndexIsPresent.at(PlaceIndex)) // если размещение имеется
    {
        // возьмём ID элемента размещения по "Склады размещение" и положим его в стек
        QStringList fl = QStringList() << "ИД" << "Тип размещения";
        QStringList cmpfl = QStringList() << "Номер" << "ИД_а";
        QStringList cmpvl = QStringList() << QString::number(PlaceIndex) << QString::number(IDs.top());
        QStringList PlaceID = tfl.valuesbyfields("Склады размещение_полн",fl,cmpfl, cmpvl);
        if (tfl.result) // нет такого размещения в нашем "шкафу", значит, при записи когда-то был косяк, выход
        {
            WHEDWARN;
            return;
        }
        IDs.push(PlaceID.at(0).toInt());
        // по типу размещения найдём количество столбцов в размещении
        fl = QStringList() << "Кол-во рядов" << "Кол-во столбцов";
        QStringList PlaceColumnsCount = tfl.valuesbyfield("Склады типы размещения_полн",fl,"Наименование",PlaceID.at(1));
        if (tfl.result)
        {
            WHEDWARN;
            return;
        }
        Columns.push(PlaceColumnsCount.at(0).toInt()); // положили количество столбцов в стек
        Rows.push(PlaceColumnsCount.at(1).toInt()); // и количество строк в свой стек
        s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("stw");
        if (stw == 0)
        {
            WHEDDBG;
            return;
        }
    }
    s_tqWidget *wdgt = new s_tqWidget;
    s_tqLabel *lbl = new s_tqLabel("Тип размещения:");
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    hlyout->addWidget(lbl);
    hlyout->setAlignment(lbl,Qt::AlignRight);
    s_tqChooseWidget *cw = new s_tqChooseWidget(true);
    cw->Setup(("2.2..Склады типы размещения_сокращ.Наименование"));
    cw->setObjectName(QString::number(IDs.top())+"."+QString::number(row)+"."+QString::number(column)); // оставляем только номер строки и столбца
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(UpdatePicture(QVariant)));
    hlyout->addWidget(cw);
    lyout->addLayout(hlyout);
    lbl = static_cast<s_tqLabel *>(sender());
    s_tqLabel *PlacePic = new s_tqLabel;
    PlacePic->setObjectName("pp");
    QPixmap pm = *(lbl->pixmap());
    PlacePic->setPixmap(pm);
    lyout->addWidget(PlacePic);
    if (IndexIsPresent.at(PlaceIndex)) // если размещение имеется
    {
        // рисуем размещения по строкам и столбцам
        for (int i=0; i<Rows.top(); i++)
        {
            hlyout = new QHBoxLayout;
            for (int j=0; j<Columns.top(); j++)
            {
                QVBoxLayout *v2lyout = new QVBoxLayout;
                lbl = new s_tqLabel;
                lbl->setPixmap(QPixmap(":/res/EmptyCell.png"));
                lbl->setObjectName("cell."+QString::number(i)+"."+QString::number(j));
                connect(lbl,SIGNAL(clicked()),this,SLOT(UpdatePlace()));
                v2lyout->addWidget(lbl);
                v2lyout->setAlignment(lbl, Qt::AlignCenter);
                lbl = new s_tqLabel("");
                lbl->setObjectName("celllbl."+QString::number(i)+"."+QString::number(j));
                v2lyout->addWidget(lbl);
                v2lyout->setAlignment(lbl,Qt::AlignCenter);
                hlyout->addLayout(v2lyout);
                UpdatePlacePicture(i, j);
            }
            lyout->addLayout(hlyout);
        }
    }
    hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton("Записать и закрыть");
    pb->setIcon(QIcon(":/res/icon_zap.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(WriteAndClose()));
    hlyout->addWidget(pb);
    pb = new s_tqPushButton("Отмена");
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), wdgt, SLOT(close()));
    connect(pb,SIGNAL(clicked()),this,SLOT(PopIDs()));
    hlyout->addWidget(pb);
    lyout->addLayout(hlyout);
    wdgt->setAttribute(Qt::WA_DeleteOnClose);
    wdgt->setLayout(lyout);
    int idx = stw->addWidget(wdgt);
    stw->setCurrentIndex(idx);
    stw->repaint();
}

void Wh_Editor::PopIDs()
{
    if (!IDs.isEmpty())
        IDs.pop();
}

void Wh_Editor::UpdatePicture(QVariant value)
{
    int row, column;

    // вытащим номера столбца и строки из наименования источника сигнала
    QStringList CellNames = sender()->objectName().split(".");
    if (CellNames.size() < 3)
    {
        WHEDDBG;
        return;
    }
    int id = CellNames.at(0).toInt();
    row = CellNames.at(1).toInt();
    column = CellNames.at(2).toInt();
    if ((row != -1) && (column != -1))
    {
        s_tqLabel *lbl = this->findChild<s_tqLabel *>("pp");
        if (lbl == 0)
        {
            WHEDDBG;
            return;
        }
        QStringList PlaceTank = tfl.valuesbyfield("Склады типы размещения_полн", QStringList("Тип размещения"), "Наименование", value.toString());
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
        lbl->setPixmap(QPixmap(":/res/"+PlacePicture.at(0)+".png"));
    }
    else
    {
        WHEDWARN;
        return;
    }
}

void Wh_Editor::UpdatePlacePicture(int row, int column)
{
    if ((row > (MAXROWS-1)) || (column > (MAXCOLS-1)))
    {
        WHEDDBG;
        return;
    }
    int index = row*Columns.top()+column; // абсолютный индекс
    QStringList fl = QStringList() << "Тип размещения";
    QStringList cmpfl = QStringList() << "Номер" << "ИД_а";
    QStringList cmpvl = QStringList() << QString::number(index) << QString::number(IDs.top());
    QStringList PlaceType = tfl.valuesbyfields("Склады размещение_полн",fl,cmpfl, cmpvl,false);
    if (PlaceType.isEmpty())
    {
        lbl->setPixmap(QPixmap(":/res/EmptyCell.png"));
        return; // пусто
    }
    QStringList PlaceTank = tfl.valuesbyfield("Склады типы размещения_полн", QStringList("Тип размещения"), "ИД", PlaceType.at(0));
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
    s_tqLabel *lbl = this->findChild<s_tqLabel *>(QString::number(IDs.top())+"."+QString::number(row)+"."+QString::number(column));
    if (lbl == 0)
    {
        WHEDDBG;
        return;
    }
    lbl->setPixmap(QPixmap(PlacePicture.at(0)));
}

void Wh_Editor::WriteAndClose()
{
    UpdatePlacePicture(0, 0); // подставить сюда row и column
}

void Wh_Editor::AddNewWh()
{

}

void Wh_Editor::DeleteWh()
{

}

void Wh_Editor::CheckIndexes()
{
    for (int i=0; i<Rows.top(); i++)
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
    }
}

void Wh_Editor::ModifyWh(QString str)
{
    // достанем индекс склада по имени str из wh
    // затем достанем индекс размещения данного склада
    // и положим его на верхушку стека
    // обновим индексы
}
