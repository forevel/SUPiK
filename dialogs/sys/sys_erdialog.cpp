#include <QVBoxLayout>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QApplication>
#include <QTimer>
#include <QPainter>
#include <QHeaderView>

#include "sys_erdialog.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"
#include "../../models/errorprotocolmodel.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/treeview.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqcombobox.h"

SysErDialog::SysErDialog(QWidget *parent) :
    QDialog(parent)
{
    QString ErrWss = "QWidget {background-color: "+QString(ERPROTCLR)+";}";
    setStyleSheet(ErrWss);
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);

    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel("Протокол работы СУПиК");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);
    TreeView *tv = new TreeView;
    ErrorProtocolModel *erm = new ErrorProtocolModel;
    erm->setHeaderData(0, Qt::Horizontal, "Номер",Qt::EditRole);
    erm->setHeaderData(1, Qt::Horizontal, "Дата/время",Qt::EditRole);
    erm->setHeaderData(2, Qt::Horizontal, "Модуль",Qt::EditRole);
    erm->setHeaderData(3, Qt::Horizontal, "Строка",Qt::EditRole);
    erm->setHeaderData(4, Qt::Horizontal, "Сообщение",Qt::EditRole);
    tv->setModel(erm);
    tv->resizeColumnsToContents();
    connect(erm,SIGNAL(dataChanged(QModelIndex,QModelIndex)),tv,SLOT(resizeColumnsToContents()));
    tv->horizontalHeader()->setStretchLastSection(true);
    tv->horizontalHeader()->setEnabled(false);
    tv->verticalHeader()->setVisible(false);
    tv->verticalHeader()->setDefaultSectionSize(tv->verticalHeader()->fontMetrics().height()+2);
    tv->setGridStyle(Qt::SolidLine);
    tv->setShowGrid(true);
    tv->setObjectName("ertv");
    lyout->addWidget(tv,5);
    setLayout(lyout);
    setAttribute(Qt::WA_DeleteOnClose);
}

void SysErDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/Sys2WallPaper.png"));
    e->accept();
}

void SysErDialog::InitiateDialog()
{
    QTimer *tmr = new QTimer;
    tmr->setInterval(2000);
    connect(tmr,SIGNAL(timeout()),this,SLOT(UpdateProblems()));
    InitialModelFill();
    tmr->start();
}

void SysErDialog::InitialModelFill()
{
    for (int i=0; i<pc.ermsgpool.size(); i++)
        AddRowToProt(pc.ermsgpool.at(i));
    ErMsgLastNum = pc.ermsgpool.last().ErNum;
}

void SysErDialog::NewProblemsDetected()
{
    int i;
    for (i=pc.ermsgpool.size(); i>=0; i--)
    {
        if (pc.ermsgpool.at(i-1).ErNum <= ErMsgLastNum) // дошли до отображённого элемента
            break;
    }
    while (i < pc.ermsgpool.size())
    {
        AddRowToProt(pc.ermsgpool.at(i));
        i++;
    }
    ErMsgLastNum = pc.ermsgpool.last().ErNum;
    emit ProblemsNumberUpdated();
}

void SysErDialog::AddRowToProt(PublicClass::ermsg ermsg)
{
    TreeView *tv = this->findChild<TreeView *>("ertv");
    if (tv == 0)
    {
        return;
    }
    ErrorProtocolModel *erm = static_cast<ErrorProtocolModel *>(tv->model());
    erm->AddRow(ermsg);
}

void SysErDialog::UpdateProblems()
{
    if (ErMsgLastNum < pc.ermsgpool.last().ErNum)
        NewProblemsDetected();
}
