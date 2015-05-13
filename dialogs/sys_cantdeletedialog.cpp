#include "sys_cantdeletedialog.h"
#include <QIcon>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>

sys_cantdeletedialog::sys_cantdeletedialog(QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    DTV = new QTableView;
    SetupUI();
}

void sys_cantdeletedialog::SetupUI()
{
    resize (400,300);

    QIcon Icon1;
    Icon1.addFile(QString::fromUtf8(":/res/supik.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(Icon1);
    setStyleSheet("background-color: rgb(204, 204, 204);");

    MainLayout = new QVBoxLayout;

    QLabel *CDL = new QLabel("Невозможно удалить");
    QLabel *CDLL = new QLabel("Следующие записи зависят от текущей:");
    QPushButton *OkPB = new QPushButton("Ага");
    MainLayout->addWidget(CDL);
    MainLayout->setAlignment(CDL, Qt::AlignRight);
    MainLayout->addWidget(CDLL);
    MainLayout->addWidget(DTV, 100);
    MainLayout->addWidget(OkPB);

    setLayout(MainLayout);
    connect(OkPB, SIGNAL(clicked()), this, SLOT(OkPressed()));
}

void sys_cantdeletedialog::OkPressed()
{
    this->close();
}
