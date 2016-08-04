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
#include "../../widgets/s_tqcheckbox.h"
#include "../../widgets/s_tqtreeview.h"
#include "../../widgets/s_tqtableview.h"
#include "../../widgets/s_tqframe.h"
#include "../../widgets/s_tqsplitter.h"
#include "../../widgets/s_colortabwidget.h"
#include "../../widgets/s_tqwidget.h"
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

    setLayout(lyout);
    QApplication::restoreOverrideCursor();
}
