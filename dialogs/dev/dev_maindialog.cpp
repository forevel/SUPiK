#include "dev_maindialog.h"
#include <QAction>
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
#include "../../widgets/s_tqwidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../../gen/sftp.h"
#include "../messagebox.h"

DevMainDialog::DevMainDialog(QString DevID, QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    SetupUI();
    if (Fill(DevID))
    {
        DEVMAINWARN;
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
    QHBoxLayout *hlyout = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);

    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel("Редактор изделий");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);

//    s_tqGroupBox *gb = new s_tqGroupBox;
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("ИД изделия:");
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
    lyout->addLayout(hlyout);

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
    lyout->addLayout(hlyout);

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
    lyout->addLayout(hlyout);

    s_tqFrame *line = new s_tqFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    lyout->addWidget(line);


    setLayout(lyout);
    Refresh();
}

void DevMainDialog::Refresh()
{
/*    TreeView *tv = this->findChild<TreeView *>("mtv");
    if (tv == 0)
    {
        DEVMAINDBG;
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
    QString DevDesc = sqlc.GetValueFromTableByID(sqlc.GetDB("dev"), "devices", "description", DevID);
    if (sqlc.result)
    {
        DEVMAINWARN;
        return 1;
    }
    sftp.ChangeDir("/");
    while (sftp.Busy)
        qApp->processEvents();
    if (sftp.Error) // нет такого каталога
    {
        DEVMAINER("Невозможно перейти к корневому каталогу");
        return 1;
    }
    QString Dir = DevDesc;
    sftp.ChangeDir(Dir);
    while (sftp.Busy)
        qApp->processEvents();
    if (sftp.Error) // нет такого каталога
    {
        sftp.MakeDir(Dir);
        while (sftp.Busy)
            qApp->processEvents();
        if (sftp.Error)
        {
            DEVMAINER("Невозможно создать каталог "+DevDesc+" на ftp-сервере");
            return 1;
        }
    }
    return 0;
}

void DevMainDialog::SetClassByManuf(QVariant Class)
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("decimalle");
    if (le == 0)
    {
        DEVMAINDBG;
        return;
    }
    QString tmps = le->text();
    QStringList tmpsl = tmps.split(".");
/*    QStringList ClassDecimals = tfl.htovlc("Классификатор ЕСКД_полн","Наименование","ИД",Class.toString());
    if (ClassDecimals.isEmpty())
    {
        DEVMAINWARN;
        return;
    }
    QString ClassDecimal = ClassDecimals.at(0);
    if (tfl.result)
    {
        DEVMAINWARN;
        return;
    }
    if (ClassDecimal.isEmpty())
    {
        DEVMAINER("У данного производителя отсутствует код предприятия");
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
        DEVMAINDBG;
        return;
    }
    QString tmps = le->text();
    QStringList tmpsl = tmps.split(".");
    QStringList ManufDecimals = tfl.htovlc("Производители_полн","Код предприятия","Наименование",Manuf.toString());
    if (ManufDecimals.isEmpty())
    {
        DEVMAINWARN;
        return;
    }
    QString ManufDecimal = ManufDecimals.at(0);
    if (tfl.result)
    {
        DEVMAINWARN;
        return;
    }
    if (ManufDecimal.isEmpty())
    {
        DEVMAINER("У данного производителя отсутствует код предприятия");
        return;
    }
    tmpsl.replace(0, ManufDecimal);
    tmps=tmpsl.join(".");
    le->setText(tmps);
}
