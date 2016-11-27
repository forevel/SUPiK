#include "tb_maindialog.h"
#include "../../models/treemodel.h"
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QGridLayout>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include <QThread>
#include <stdlib.h>
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
#include "../../widgets/s_tqtextedit.h"
#include "../../widgets/waitwidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../../gen/pdfout.h"
#include "../messagebox.h"
#include "../../gen/client.h"

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



    lyout->addStretch(300);
    setLayout(lyout);
    QApplication::restoreOverrideCursor();
    gbw->SetItem(0, GoodBadTableModel::GBIT_NEUTRAL); // принудительный ресайз
}
