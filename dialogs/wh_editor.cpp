#include "wh_editor.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_colortabwidget.h"
#include "../widgets/s_tqspinbox.h"
#include "../widgets/s_tqgroupbox.h"
#include "../widgets/s_tqwidget.h"
#include "../gen/s_tablefields.h"
#include "../gen/publicclass.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QFont>
#include <QPainter>
#include <QPaintEvent>
#include <QStringListModel>

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
    // Табвиджет.
    S_ColorTabWidget *ctw = new S_ColorTabWidget;
    s_tqWidget *wdgt = new s_tqWidget(":/res/WhWallpaper.jpg");
    hlyout = new QHBoxLayout;
    // Первая вкладка "Склад"
    QList<QStringList> Places = tfl.tbvll("Склады типы размещения_сокращ");
    QStringList headers = Places.at(0);
    QStringListModel *PlacesModel = new QStringListModel;
//    PlacesModel->setStringList(PlacesNames);
    for (int i=0; i<5; i++)
    {
        hlyout = new QHBoxLayout;
//        QVBoxLayout *BoxLayout = new QVBoxLayout;
        lbl = new s_tqLabel;
        lbl->setMinimumHeight(80);
        lbl->setObjectName("place"+QString::number(i)); // метка для вывода картинки изображения места хранения
//        BoxLayout->addWidget(lbl);
        hlyout->addWidget(lbl,0);
        s_tqComboBox *cb = new s_tqComboBox;
        cb->setAData(i);
        cb->setModel(PlacesModel);
        cb->setCurrentIndex(-1);
        connect(cb,SIGNAL(textChanged(QString,s_tqComboBox*)),this,SLOT(UpdatePlacePicture(QString,s_tqComboBox*)));
//        BoxLayout->addWidget(cb);
//        hlyout->addLayout(BoxLayout);
        hlyout->addWidget(cb,1);
        s_tqSpinBox *spb = new s_tqSpinBox;
        spb->setDecimals(0);
        spb->setMinimum(0);
        spb->setMaximum(10);
        spb->setValue(0);
        spb->setObjectName("cabinetspb");
        connect(spb,SIGNAL(valueChanged(double)),this,SLOT(UpdateSmallTWWithNewQuantities()));
        hlyout->addWidget(spb,1);
        vlyout->addLayout(hlyout);
    }
    wdgt->setLayout(vlyout);
    int idx = ctw->addTab(wdgt, "Состав склада");
    ctw->tabBar()->setTabData(idx, TW_WH);
    ctw->tabBar()->tabButton(idx,QTabBar::RightSide)->hide();
    ctw->tabBar()->setCurrentIndex(idx  );
    ctw->repaint();
    //      Кнопка "добавить тип шкафа"
    //      "Типы шкафов", комбобокс
    //      "параметры шкафа"
    //      "Кол-во полок", спин
    //      групбокс "Стеллажи"
    //      Кнопка "добавить тип стеллажа"
    //      "Типы стеллажей", комбобокс
    //      "параметры стеллажа"
    //      "Кол-во рядов", спин
    //      "Кол-во полок", спин
    lyout->addWidget(ctw);
    lyout->addStretch(1);
    setLayout(lyout);
}

void Wh_Editor::UpdateSmallTWWithNewQuantities()
{
    S_ColorTabWidget *ctw = this->findChild<S_ColorTabWidget *>("smctw");
    if (ctw == 0)
    {
        WHEDDBG;
        return;
    }
    //      в каждой вкладке шкафа:
    //          "Имя шкафа", поле ввода
    //          "тип", комбобокс с типами шкафов
    //          "Количество ячеек/мешков на одной полке", спин
    //      в каждой вкладке стеллажа:
    //          "Имя стеллажа", поле ввода
    //          "тип", комбобокс с типами стеллажей
    //          "Количество ячеек/мешков на одной полке", спин
    //      в каждой вкладке коробки:
    //          "Имя коробки", поле ввода
}

void Wh_Editor::UpdatePlacePicture(QString txt, s_tqComboBox *ptr)
{
/*    QStringList PlacesNames = QStringList() << "Шкаф" << "Стеллаж" << "Коробка" << "Пакет" << "Ячейка";
    int PlacesNamesIndex = ptr->getAData().toInt();
    QString PlaceName;
    if (PlacesNamesIndex < PlacesNames.size())
        PlaceName = PlacesNames.at(PlacesNamesIndex);
    else
    {
        WHEDDBG;
        return;
    } */
    QStringList vl = tfl.valuesbyfield("Склады типы типов размещения_полн",QStringList("Картинка"),"Наименование",txt);
    if (vl.isEmpty())
        WHEDWARN;
    s_tqLabel *lbl = this->findChild<s_tqLabel *>("place"+QString::number(ptr->getAData().toInt()));
    if (lbl == 0)
    {
        WHEDDBG;
        return;
    }
    lbl->setPixmap(QPixmap(":/res/" + vl.at(0) + ".png"));
}

void Wh_Editor::AddNewWh()
{

}

void Wh_Editor::DeleteWh()
{

}

void Wh_Editor::ModifyWh(QString WhName)
{

}
