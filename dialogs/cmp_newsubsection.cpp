#include "cmp_newsubsection.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include "../gen/publicclass.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"
#include "../widgets/s_tqchoosewidget.h"
#include "../widgets/s_tqpushbutton.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"

cmp_newsubsection::cmp_newsubsection(int CompType, QWidget *parent) : QDialog(parent)
{
    this->CompType = CompType;
    SomethingChanged = false;
    SetupUI();
}

cmp_newsubsection::~cmp_newsubsection()
{

}

void cmp_newsubsection::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/CompWallpaper.jpg"));

    event->accept();
}

void cmp_newsubsection::SetupUI()
{
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;
    s_tqLabel *lbl = new s_tqLabel("Имя категории:");
    s_tqLineEdit *le = new s_tqLineEdit;
    le->setObjectName("name");
    connect(le,SIGNAL(textChanged(QString)),this,SLOT(SetSomethingChanged()));
    connect(le, SIGNAL(editingFinished()), this, SLOT(transliteDirName()));
    hlyout->addWidget(lbl);
    hlyout->addWidget(le, 1);
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Имя в БД:");
    le = new s_tqLineEdit;
    le->setObjectName("tblename");
    connect(le,SIGNAL(textChanged(QString)),this,SLOT(SetSomethingChanged()));
    hlyout->addWidget(lbl);
    hlyout->addWidget(le, 1);
    lyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    lbl = new s_tqLabel("Группа параметров:");
    s_tqChooseWidget *cw = new s_tqChooseWidget(true);
    cw->Setup("2.2..Наборы параметров_сокращ.Наименование");
    connect(cw,SIGNAL(textchanged(QVariant)),this,SLOT(SetSomethingChanged()));
    hlyout->addWidget(lbl);
    hlyout->addWidget(cw, 1);
    lyout->addLayout(hlyout);
    s_tqPushButton *pbOk = new s_tqPushButton("Ага");
    pbOk->setIcon(QIcon(":/res/ok.png"));
    s_tqPushButton *pbCancel = new s_tqPushButton("Неа");
    pbCancel->setIcon(QIcon(":/res/cross.png"));
    connect(pbOk,SIGNAL(clicked()),this,SLOT(Ok()));
    connect(pbCancel,SIGNAL(clicked()),this,SLOT(Cancel()));
    hlyout = new QHBoxLayout;
    hlyout->addWidget(pbOk);
    hlyout->addWidget(pbCancel);
    lyout->addLayout(hlyout);
    setLayout(lyout);
}

void cmp_newsubsection::transliteDirName()
{
    s_tqLineEdit *le1 = this->findChild<s_tqLineEdit *>("name");
    s_tqLineEdit *le2 = this->findChild<s_tqLineEdit *>("tblename");
    if ((le1 == 0) || (le2 == 0))
    {
        CMPNSDBG;
        return;
    }
    if (le2->text().isEmpty())
    {
        QString tmpString = pc.getTranslit(le1->text());
        le2->setText(tmpString);
    }
}

void cmp_newsubsection::SetSomethingChanged()
{
    SomethingChanged = true;
}

void cmp_newsubsection::Cancel()
{
    if (SomethingChanged)
    {
        if (QMessageBox::question(this,"Данные были изменены","Данные были изменены, всё равно выйти?", \
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
            this->close();
    }
}

void cmp_newsubsection::Ok()
{
    s_tqLineEdit *le1 = this->findChild<s_tqLineEdit *>("name");
    s_tqLineEdit *le2 = this->findChild<s_tqLineEdit *>("tblename");
    if ((le1 == 0) || (le2 == 0))
    {
        CMPNSDBG;
        return;
    }
    // 2. Создать таблицу в соотв. БД (например, relay в altium)
    // сначала проверим наличие такой таблицы
    QStringList sl = QStringList() << "" << "А" << "З" << "Э" << "К" << "У";
    switch(CompType)
    {
    case CTYPE_ALT:
    {
        QStringList tmpsl = sqlc.GetColumnsFromTable(sqlc.getdb("alt"), le2->text());
        if (sqlc.result); // нет такой таблицы, продолжаем
        else
        {
            CMPNSINFO("Таблица "+le2->text()+" уже имеется в БД Altium!");
            return;
        }
        // создаём таблицу
        QStringList Fields = QStringList() << "Library Ref" << "Footprint Ref" << "Sim Description" << "Sim File" << \
                                              "Sim Model Name" << "Sim Parameters" << "Manufacturer" << "PartNumber" << "Package" << "Marking" << "NominalValue" << \
                                              "NominalVoltage" << "Tolerance" << "OpTemperaturen" << "OpTemperaturem" << "Pmax" << "TC" << "Comment" << "HelpURL" << \
                                              "RevNotes" << "Discontinued" << "Description" << "Notes" << "Modify Date" << "Creator" << "prefix" << "isSMD" << \
                                              "Nominal" << "Unit" << "par4" << "par5";
        sqlc.CreateTable(sqlc.GetDB("alt"), le2->text(), Fields);
        // создаём её описание в description
        break;
    }
    }
    QString TableName = sl.at(CompType)+"Компоненты_полн";
    QString newID = tfl.insert(TableName);
    if (tfl.result)
    {
        CMPNSWARN;
        return;
    }
    QStringList fl = QStringList() << "ИД" << "Наименование" << "Описание";
    QStringList vl = QStringList() << newID << le2->text() << le1->text();
    tfl.idtois(TableName,fl,vl);
    if (tfl.result)
    {
        CMPNSWARN;
        return;
    }

    // 3. Создать категорию, если её ещё нет
    TableName = "Категории_полн";
    QStringList tmpsl = tfl.valuesbyfield(TableName,QStringList("ИД"),"Наименование",le1->text(), false);
    if (tmpsl.isEmpty()) // нет такой категории, пишем в БД
    {
        newID = tfl.insert(TableName);
        if (tfl.result)
        {
            CMPNSWARN;
            return;
        }
        QStringList fl = QStringList() << "ИД" << "Наименование" << "ИД_а";
        QStringList vl = QStringList() << newID << le2->text() << "0";
        tfl.idtois(TableName,fl,vl);
        if (tfl.result)
        {
            CMPNSWARN;
            return;
        }
    }
    // 4. Создать соответствующие записи в unitset и parameters
}
