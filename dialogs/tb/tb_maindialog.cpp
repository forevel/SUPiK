#include "tb_maindialog.h"
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
#include "../../models/proxymodel.h"
#include "../../models/s_duniversal.h"
#include "../../widgets/s_tqgroupbox.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqradiobutton.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../../widgets/s_tqtableview.h"
#include "../../widgets/s_tqframe.h"
#include "../../widgets/s_tqwidget.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/goodbadwidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../messagebox.h"

// --------------------------------------
// Конструктор
// --------------------------------------

tb_maindialog::tb_maindialog(QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
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
    s_tqLabel *lbl = new s_tqLabel("ОТ и ТБ");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);

    s_tqGroupBox *gb = new s_tqGroupBox;
    hlyout = new QHBoxLayout;
    hlyout->addStretch(150);
    lbl = new s_tqLabel("Выберите группу ТБ, по которой будете проходить тест:");
    hlyout->addWidget(lbl);
    hlyout->addStretch(20);
    s_tqRadioButton *rb = new s_tqRadioButton("III");
    hlyout->addWidget(rb);
    rb = new s_tqRadioButton("IV");
    hlyout->addWidget(rb);
    rb = new s_tqRadioButton("V");
    rb->setEnabled(false);
    hlyout->addWidget(rb);
    hlyout->addStretch(20);
    pb = new s_tqPushButton("Далее");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(GroupChoosed()));
    hlyout->addWidget(pb);
    hlyout->addStretch(150);
    gb->setLayout(hlyout);
    lyout->addWidget(gb);

    s_tqStackedWidget *stw = new s_tqStackedWidget;
    gb = new s_tqGroupBox;
    gb->setTitle("Вопрос");
    hlyout = new QHBoxLayout;
    stw->setObjectName("questionstw");
    gb->setObjectName("questiongb");
    hlyout->addWidget(stw);
    gb->setLayout(hlyout);
    gb->setVisible(false);
    lyout->addWidget(gb);

    hlyout = new QHBoxLayout;
    gb = new s_tqGroupBox;
    gb->setTitle("Результаты ответов");
    GoodBadWidget *gbw = new GoodBadWidget;
    gbw->setObjectName("gbw");
    if (!gbw->SetItemsCount(20))
    {
        TBMWARN;
        return;
    }
    gbw->SetItem(0, GoodBadTableModel::GBIT_NEUTRAL);
    hlyout->addWidget(gbw, 10, Qt::AlignVCenter | Qt::AlignCenter);
    gb->setLayout(hlyout);
    lyout->addWidget(gb);

    lyout->addStretch(300);
    setLayout(lyout);
    QApplication::restoreOverrideCursor();
}

void tb_maindialog::GroupChoosed()
{
    GoodBadWidget *gbw = this->findChild<GoodBadWidget *>("gbw");
    if (gbw == 0)
    {
        TBMDBG;
        return;
    }
    gbw->SetItem(0, GoodBadTableModel::GBIT_GOOD);
}
