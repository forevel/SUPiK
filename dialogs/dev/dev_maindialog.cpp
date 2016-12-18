#include "dev_maindialog.h"
#include <QAction>
#include <QScrollArea>
#include <QMenu>
#include <QIcon>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include "../s_2cdialog.h"
#include "../../models/proxymodel.h"
#include "../../models/griddelegate.h"
#include "../../models/treemodel.h"
#include "../../widgets/s_tqgroupbox.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqchoosewidget.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../../widgets/treeview.h"
#include "../../widgets/s_tqframe.h"
#include "../../widgets/s_tqsplitter.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/s_tqwidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../../gen/ftp.h"
#include "../messagebox.h"

DevMainDialog::DevMainDialog(QString DevID, QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
    if (Fill(DevID))
    {
        WARNMSG("");
        return;
    }
}

DevMainDialog::~DevMainDialog()
{

}

void DevMainDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/DevWallpaper.png"));

    event->accept();
}

// Настройка интерфейса

void DevMainDialog::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    QScrollArea *SArea = new QScrollArea;
    s_tqWidget *w = new s_tqWidget;
    SArea->setStyleSheet("QScrollArea {background-color: rgba(0,0,0,0);}");
    SArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QVBoxLayout *slyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb,SIGNAL(clicked(bool)),this,SIGNAL(DialogClosed()));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb,0);
    hlyout->addStretch(100);
    slyout->addLayout(hlyout);

//    s_tqGroupBox *gb = new s_tqGroupBox;
    hlyout = new QHBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("ИД изделия:");
    hlyout->addWidget(lbl);
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setObjectName("devidle");
    le->setEnabled(false);
    hlyout->addWidget(le, 5);
    hlyout->addStretch(5);
    lbl = new s_tqLabel("Дата внесения в классификатор:");
    hlyout->addWidget(lbl);
    le = new s_tqLineEdit;
    le->setObjectName("datele");
    le->setEnabled(false);
    hlyout->addWidget(le, 5);
    hlyout->addStretch(5);
    lbl = new s_tqLabel("Внёс:");
    hlyout->addWidget(lbl);
    le = new s_tqLineEdit;
    le->setObjectName("persle");
    le->setEnabled(false);
    hlyout->addWidget(le, 10);
    slyout->addLayout(hlyout);

    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Фирма-изготовитель:");
    hlyout->addWidget(lbl);
    s_tqChooseWidget *cw = new s_tqChooseWidget(true);
    cw->Setup("2.2..Производители_сокращ.Наименование");
    cw->setObjectName("manufcw");
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(SetDecimalByManuf(QVariant)));
    hlyout->addWidget(cw, 10);
    lbl = new s_tqLabel("Класс:");
    hlyout->addWidget(lbl);
    cw = new s_tqChooseWidget(true);
    cw->Setup("2.2..Классификатор ЕСКД_сокращ.Наименование");
    cw->setObjectName("classcw");
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(SetClassByManuf(QVariant)));
    hlyout->addWidget(cw, 10);
    lbl = new s_tqLabel("Децимальный номер:");
    hlyout->addWidget(lbl);
    le = new s_tqLineEdit;
    le->setObjectName("decimalle");
    le->setEnabled(false);
    hlyout->addWidget(le, 10);
    slyout->addLayout(hlyout);

    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Наименование:");
    hlyout->addWidget(lbl);
    le = new s_tqLineEdit;
    le->setObjectName("namele");
    hlyout->addWidget(le,10);
    lbl = new s_tqLabel("Редакция:");
    hlyout->addWidget(lbl);
    s_tqComboBox *cb = new s_tqComboBox;
    cb->setObjectName("revisioncb");
    hlyout->addWidget(cb, 2);
    slyout->addLayout(hlyout);

    s_tqFrame *line = new s_tqFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    slyout->addWidget(line);

    QGridLayout *glyout = new QGridLayout;
//    s_tqGroupBox *gb = new s_tqGroupBox(255,255,51,40);
    s_tqGroupBox *gb = new s_tqGroupBox(pc.DifferentColors().at(0));
    gb->setTitle("Схемы");
    gb->setObjectName("sch");
    gb->setMinimumHeight(70);
    glyout->addWidget(gb, 0, 0, 1, 1);
    gb = new s_tqGroupBox(pc.DifferentColors().at(1));
    gb->setTitle("Чертежи");
    gb->setObjectName("chert");
    gb->setMinimumHeight(70);
    glyout->addWidget(gb, 0, 1, 1, 1);
    gb = new s_tqGroupBox(pc.DifferentColors().at(2));
    gb->setTitle("Изготовление");
    gb->setObjectName("izg");
    gb->setMinimumHeight(70);
    glyout->addWidget(gb, 0, 2, 1, 1);

    gb = new s_tqGroupBox(pc.DifferentColors().at(3));
    gb->setTitle("Проектные");
    gb->setObjectName("pr");
    gb->setMinimumHeight(70);
    glyout->addWidget(gb, 1, 0, 1, 1);
    gb = new s_tqGroupBox(pc.DifferentColors().at(4));
    gb->setTitle("Эксплуатационные");
    gb->setObjectName("eksp");
    gb->setMinimumHeight(70);
    glyout->addWidget(gb, 1, 1, 1, 1);
    gb = new s_tqGroupBox(pc.DifferentColors().at(5));
    gb->setTitle("Прогр. обесп.");
    gb->setObjectName("po");
    gb->setMinimumHeight(70);
    glyout->addWidget(gb, 1, 2, 1, 1);

    gb = new s_tqGroupBox(pc.DifferentColors().at(6));
    gb->setTitle("Модели");
    gb->setObjectName("mod");
    gb->setMinimumHeight(70);
    glyout->addWidget(gb, 2, 0, 1, 1);
/*    gb = new s_tqGroupBox;
    gb->setTitle("");
    gb->setEnabled(false);
    gb->setMinimumHeight(70);
    glyout->addWidget(gb, 2, 1, 1, 1);
    gb = new s_tqGroupBox;
    gb->setTitle("");
    gb->setEnabled(false);
    gb->setMinimumHeight(70);
    glyout->addWidget(gb, 2, 2, 1, 1); */
    slyout->addLayout(glyout);

    pb = new s_tqPushButton("История",153,255,204,40);
    connect(pb,SIGNAL(clicked()),this,SLOT(History()));
    slyout->addWidget(pb);

    pb = new s_tqPushButton("Объекты",255,102,0,40);
    connect(pb,SIGNAL(clicked()),this,SLOT(Objects()));
    slyout->addWidget(pb);

    pb = new s_tqPushButton("Командировки",255,102,102,40);
    connect(pb,SIGNAL(clicked()),this,SLOT(Trips()));
    slyout->addWidget(pb);
    w->setLayout(slyout);
    w->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Ignored);
    SArea->setWidget(w);
    lyout->addWidget(SArea);
    setLayout(lyout);
    Refresh();
}

void DevMainDialog::Refresh()
{
/*    TreeView *tv = this->findChild<TreeView *>("mtv");
    if (tv == 0)
    {
        DBGMSG;
        return;
    }
    ProxyModel *tvmodel = new ProxyModel;
    TreeModel *mdl = new TreeModel;
    tvmodel->setSourceModel(mdl);
    QItemSelectionModel *m = tv->selectionModel();
    tv->setModel(tvmodel);
    delete m;
    mdl->Setup("Изделия_сокращ");
    ResizeMainTV(); */
}

void DevMainDialog::Filter()
{

}

void DevMainDialog::Unfilter()
{

}

int DevMainDialog::Fill(QString DevID)
{
    QString DevDesc = sqlc.GetValueFromTableByID("dev", "devices", "description", DevID);
    if (sqlc.result)
    {
        WARNMSG("");
        return 1;
    }
    if (!Ftps->ChDir("/"))
    {
        ERMSG("Невозможно перейти к корневому каталогу");
        return 1;
    }
    QString Dir = DevDesc;
/*    if (!Ftps->ChDir(Dir))
    {
        if (!Ftps->MkDir(Dir))
        {
            ERMSG("Невозможно создать каталог "+DevDesc+" на ftp-сервере");
            return 1;
        }
        else
            DEVMAININFO("Каталог создан успешно");
    } */
    if (!Ftps->List())
    {
        ERMSG("Невозможно получить данные с FTP-сервера");
        return 1;
    }
    return 0;
}

void DevMainDialog::SetClassByManuf(QVariant Class)
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("decimalle");
    if (le == 0)
    {
        DBGMSG;
        return;
    }
    QString tmps = le->text();
    QStringList tmpsl = tmps.split(".");
/*    QStringList ClassDecimals = tfl.GetValuesByColumnAndField("Классификатор ЕСКД_полн","Наименование","ИД",Class.toString());
    if (ClassDecimals.isEmpty())
    {
        WARNMSG("");
        return;
    }
    QString ClassDecimal = ClassDecimals.at(0);
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    if (ClassDecimal.isEmpty())
    {
        ERMSG("У данного производителя отсутствует код предприятия");
        return;
    } */
    if (tmpsl.size()>1)
        tmpsl.replace(1, Class.toString());
    else
        tmpsl.append(Class.toString());
    tmps=tmpsl.join(".");
    le->setText(tmps);
}

void DevMainDialog::SetDecimalByManuf(QVariant Manuf)
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("decimalle");
    if (le == 0)
    {
        DBGMSG;
        return;
    }
    QString tmps = le->text();
    QStringList tmpsl = tmps.split(".");
    QStringList ManufDecimals;
    QString table = "Производители_полн";
    QString field = "Код предприятия";
    QString cmpfield = "Наименование";
    tfl.GetValuesByColumnAndField(table,field,cmpfield,Manuf.toString(), ManufDecimals);
    if (ManufDecimals.isEmpty())
    {
        WARNMSG("");
        return;
    }
    QString ManufDecimal = ManufDecimals.at(0);
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return;
    }
    if (ManufDecimal.isEmpty())
    {
        ERMSG("У данного производителя отсутствует код предприятия");
        return;
    }
    tmpsl.replace(0, ManufDecimal);
    tmps=tmpsl.join(".");
    le->setText(tmps);
}

void DevMainDialog::History()
{

}

void DevMainDialog::Objects()
{

}

void DevMainDialog::Trips()
{

}
