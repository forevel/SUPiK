#include "cmp_maindialog.h"
#include "../widgets/s_tqtreeview.h"
#include "../widgets/s_tqtableview.h"
#include "../widgets/s_tqframe.h"
#include "../widgets/s_tqsplitter.h"
#include "../widgets/s_tqlabel.h"
#include "../models/s_ntmodel.h"
#include "../models/s_ncmodel.h"

#include <QVBoxLayout>
#include <QFont>

cmp_maindialog::cmp_maindialog(QWidget *parent) : QDialog(parent)
{

}

cmp_maindialog::~cmp_maindialog()
{

}

void cmp_maindialog::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Компоненты");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    lyout->addWidget(lbl, 0);
    lyout->setAlignment(lbl, Qt::AlignRight);
    s_tqTreeView *MainTV = new s_tqTreeView;
    s_tqTableView *SlaveTV = new s_tqTableView;
    MainTV->setObjectName("MainTV");
    SlaveTV->setObjectName("SlaveTV");
    s_tqSplitter *spl = new s_tqSplitter;
    s_tqFrame *left = new s_tqFrame;
    QVBoxLayout *leftlyout = new QVBoxLayout;
    leftlyout->addWidget(MainTV);
    left->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    left->setLineWidth(1);
    left->setLayout(leftlyout);
    spl->addWidget(left);
    s_tqFrame *right = new s_tqFrame;
    QVBoxLayout *rlyout = new QVBoxLayout;
    rlyout->addWidget(SlaveTV);
    right->setLayout(rlyout);
    right->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    right->setLineWidth(1);
    spl->addWidget(right);
    spl->setOrientation(Qt::Horizontal);
    lyout->addWidget(spl, 90);
    setLayout(lyout);
//    adjustSize();
}
