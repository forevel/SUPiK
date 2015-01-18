#include "../inc/s_acceptdialog.h"

#include <QVBoxLayout>
#include <QFont>
#include "../inc/s_tqlabel.h"
#include "../inc/s_tqpushbutton.h"

s_acceptdialog::s_acceptdialog(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

void s_acceptdialog::setupUI(QString title, QString str)
{
    QFont font;
    font.setPointSize(15);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    s_tqLabel *lbl1 = new s_tqLabel(title);
    s_tqLabel *lbl2 = new s_tqLabel(str);
    s_tqPushButton *pb = new s_tqPushButton ("Ага");
    lbl1->setFont(font);
    mainLayout->addWidget(lbl1, 0, Qt::AlignCenter);
    mainLayout->addWidget(lbl2, 0, Qt::AlignCenter);
    mainLayout->addWidget(pb, 0, Qt::AlignCenter);
    connect(pb, SIGNAL(clicked()), this, SLOT(OkPressed()));
    setLayout(mainLayout);
}

void s_acceptdialog::OkPressed()
{
    this->close();
}
