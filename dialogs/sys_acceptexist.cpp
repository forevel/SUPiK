#include "sys_acceptexist.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>

sys_acceptexist::sys_acceptexist(QWidget *parent) :
    QDialog(parent)
{
    SetupUI();
}

void sys_acceptexist::SetupUI()
{
    MainLayout = new QVBoxLayout;
    resize(400, 251);
    setMinimumSize(QSize(400, 251));
    setMaximumSize(QSize(400, 251));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/pic/Pic/supik.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);
    setStyleSheet(QLatin1String("background-color: rgb(223, 223, 223); color: rgb(85, 85, 0);"));
    setModal(true);
    buttonBox = new QDialogButtonBox;
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    ADTV = new QTableView;
    ADTV->setFrameShape(QFrame::NoFrame);
    ADTV->setFrameShadow(QFrame::Plain);
    ADTV->setShowGrid(false);
    ADTV->setWordWrap(false);
    ADTV->setCornerButtonEnabled(false);
    ADTV->horizontalHeader()->setVisible(false);
    ADTV->horizontalHeader()->setHighlightSections(false);
    ADTV->verticalHeader()->setVisible(false);
    ADTV->verticalHeader()->setHighlightSections(false);
    SimilarL = new QLabel;
    AreyousureL = new QLabel;

    MainLayout->addWidget(SimilarL);
    MainLayout->addWidget(AreyousureL);
    MainLayout->addWidget(ADTV);
    MainLayout->addWidget(buttonBox);
    setLayout(MainLayout);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
