#include <QtWidgets>

#include "../inc/s_accessdialog.h"
#include "../inc/s_tqcheckbox.h"
#include "../inc/s_tqpushbutton.h"
#include "../inc/s_tqlabel.h"

s_accessdialog::s_accessdialog(QWidget *parent) :
    QDialog(parent)
{
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    MainLayout = new QVBoxLayout;
    CheckBoxLayout = new QGridLayout;
    setAttribute(Qt::WA_DeleteOnClose);
}

void s_accessdialog::SetupUI(long rights)
{
    int i, j, k;
    long one = 0x0001;
    QStringList msg, lblm;
    msg.clear();
    lblm.clear();
    msg << "Системные" << "По складу" << "САПР" << "ТБ" << "Сисадмин";
    lblm << "Чтение" << "Изменение" << "Удаление";
//    s_tqCheckBox *cb[15];
    s_tqLabel *MainL = new s_tqLabel;
    MainL->setText("Настройка прав доступа");
    QFont font, fontB;
    font.setPointSize(15);
    fontB.setBold(true);
    MainL->setFont(font);
    MainLayout->addWidget(MainL);
    MainLayout->setAlignment(MainL, Qt::AlignRight);
    k = 0;
    for (i = 0; i < 10; i+=2)
    {
        QFrame *line1 = new QFrame;
        line1->setFrameShape(QFrame::HLine);
        line1->setFrameShadow(QFrame::Sunken);
        line1->setMinimumHeight(2);
        line1->setMinimumWidth(80);
        s_tqLabel *lbl = new s_tqLabel;
        lbl->setText(msg.at(k));
        lbl->setFont(fontB);
        QFrame *line2 = new QFrame;
        line2->setFrameShape(QFrame::HLine);
        line2->setFrameShadow(QFrame::Sunken);
        line2->setMinimumHeight(2);
        line2->setMinimumWidth(80);
        CheckBoxLayout->addWidget(line1, i, 0, 1, 1, Qt::AlignCenter);
        CheckBoxLayout->addWidget(lbl, i, 1, 1, 1, Qt::AlignCenter);
        CheckBoxLayout->addWidget(line2, i, 2, 1, 1, Qt::AlignCenter);
        for (j = 0; j < 3; j++)
        {
            s_tqCheckBox *cb = new s_tqCheckBox;
            cb->setText(lblm[j]);
            cb->setChecked(rights&one);
            CheckBoxLayout->addWidget(cb, i+1, j, 1, 1, Qt::AlignLeft);
            one <<= 1;
        }
        k++;
    }
    MainLayout->addLayout(CheckBoxLayout);
    s_tqPushButton *pbOK = new s_tqPushButton;
    pbOK->setText("Ага");
    connect (pbOK, SIGNAL(clicked()), this, SLOT(OkPressed()));
    MainLayout->addWidget(pbOK, 0, Qt::AlignCenter);
    setLayout(MainLayout);
//    s_tqCheckBox *cbsd = new s_tqCheckBox;
//    cbsd->setChecked(rights&0x0004);
//    s_tqCheckBox *cbsm = new s_tqCheckBox;
//    cbsm->setChecked(rights&0x0002);
//    s_tqCheckBox *cbsr = new s_tqCheckBox;
//    cbsr->setChecked(rights&0x0001);
}

void s_accessdialog::OkPressed()
{
    int i, j;
    long one = 0x0001;
    long res = 0x0;
    for (i = 0; i < 10; i+=2)
    {
        for (j = 0; j < 3; j++)
        {
            if (dynamic_cast<s_tqCheckBox *>(CheckBoxLayout->itemAtPosition(i+1,j)->widget())->isChecked())
                res |= one;
            one <<= 1;
        }
    }
    emit acceptChanges(res);
    this->close();
}
