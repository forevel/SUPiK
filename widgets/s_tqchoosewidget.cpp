#include <QCoreApplication>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStringListModel>
#include "s_tqchoosewidget.h"
#include "s_tqlineedit.h"
#include "s_tqpushbutton.h"
#include "s_tqcalendarwidget.h"
#include "s_tqspinbox.h"
#include "s_tqcombobox.h"
#include "s_tqcheckbox.h"
#include "s_maskedle.h"
#include "s_tqlabel.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"
#include "../dialogs/s_2cdialog.h"
#include "../dialogs/chooseitemdialog.h"
#include "../dialogs/s_2tdialog.h"
#include "../dialogs/s_accessdialog.h"

// универсальный класс для организации списков выбора из таблиц БД СУПиК
// правила его использования:
// 1. Создать экземпляр класса
// 2. Вызвать setup(QString links,QString hdr="") для установки типа диалогов и ссылок, а также заголовка вызываемого по кнопке диалога (для делегатов FD_CHOOSE) (подробнее о links см. РП)
// 3. Установить связь между сигналом choosed(QVariant) (выбор сделан) и требуемым слотом
// 4. Вызвать метод hide или присвоить данный виджет соответствующему свойству editor (в делегатах)

s_tqChooseWidget::s_tqChooseWidget(bool Transparent, QWidget *parent) :
    QWidget(parent)
{
    this->Transparent = Transparent;
    ff.ftype = -1; // проверка на то, вызывали ли перед работой с виджетом функцию getlinks
    setAttribute(Qt::WA_DeleteOnClose);
}

void s_tqChooseWidget::Setup(QString links, QString hdr)
{
    this->links = links;
    ff = pc.getFFfromLinks(links);
    this->hdr = hdr;

    QVBoxLayout *ml = new QVBoxLayout;
    ml->setContentsMargins(0, 0, 0, 0);
    QWidget *wdgt = new QWidget;
    if (Transparent)
        wdgt->setStyleSheet("QWidget {background-color: rgba(0,0,0,0);}");
    else
        wdgt->setStyleSheet("QWidget {background: khaki};");
    wdgt->setContentsMargins(0,0,0,0);
    QHBoxLayout *ml2 = new QHBoxLayout;
    ml2->setContentsMargins(0,0,0,0);
    switch (ff.delegate)
    {
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        s_tqLineEdit *le = new s_tqLineEdit;
        s_tqPushButton *pb = new s_tqPushButton("...");
        le->setObjectName("fdcle");
        if (ff.delegate == FD_CHOOSE)
            le->setEnabled(false);
        else
            le->setEnabled(true);
        pb->setObjectName("fdcpb");
        connect(pb, SIGNAL(clicked()), this, SLOT(pbclicked()));
        ml2->addWidget(le, 80);
        ml2->addWidget(pb, 1);
        break;
    }
    case FD_COMBO:
    {
        s_tqComboBox *cb = new s_tqComboBox;
        cb->setStyleSheet("QComboBox {background: khaki};");
        cb->setObjectName("fdccb");
        QStringListModel *tmpModel = new QStringListModel;
        QStringList tmpsl;
        tmpsl = tfl.idtovl(links);
        if (tfl.result)
            tmpsl.clear();
        tmpModel->setStringList(tmpsl);
        cb->setModel(tmpModel);
        ml2->addWidget(cb);
        break;
    }
    case FD_LINEEDIT:
    {
        if (ff.ftype == FW_MASKED)
        {
            QString tmpString = "";
            for (int i = 0; i < ff.link.size(); i++)
                tmpString += ff.link.at(i)+"."; // возвращение строке regexp первоначального вида, "побитого" при getFFfromlinks
            tmpString = tmpString.left(tmpString.size()-1);
            s_MaskedLineEdit *le = new s_MaskedLineEdit(tmpString);
            le->setObjectName("lele");
            ml2->addWidget(le);
        }
        else
        {
            s_tqLineEdit *le = new s_tqLineEdit;
            le->setObjectName("lele");
            ml2->addWidget(le);
        }
        break;
    }
    case FD_SPIN:
    {
        int tmpInt = ff.link.at(0).count("n", Qt::CaseSensitive);
        QString tmpString;
        tmpString.fill('9', tmpInt);
        int tmpInt2 = ff.link.at(0).count("d", Qt::CaseSensitive);
        QString tmpString2;
        tmpString2.fill('9', tmpInt2);
        tmpString += "." + tmpString2;
        s_tqSpinBox *sb = new s_tqSpinBox;
        sb->setStyleSheet("QSpinBox {background: khaki};");
        sb->setObjectName("fdcsb");
        sb->setMinimum(0);
        sb->setDecimals(tmpInt2);
        sb->setMaximum(tmpString.toDouble());
        ml2->addWidget(sb);
        break;
    }
    case FD_CHECK:
    {
        s_tqCheckBox *cb = new s_tqCheckBox;
        cb->setObjectName("fdcb");
        ml2->addWidget(cb);
        break;
    }
    default:
        break;
    }
    wdgt->setLayout(ml2);
    ml->addWidget(wdgt);
    setLayout(ml);
}

void s_tqChooseWidget::pbclicked()
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fdcle");
    if (le == 0)
        return;
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        ChooseItemDialog *chooseDialog = new ChooseItemDialog(ff.link.at(0), hdr, true); // диалог с "корневой кнопкой"
        connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
        chooseDialog->setTvCurrentText(le->text());
        chooseDialog->exec();
        break;
    }
    case FW_LINK:
    {
        ChooseItemDialog *chooseDialog = new ChooseItemDialog(ff.link.at(0), hdr);
        connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
        chooseDialog->setTvCurrentText(le->text());
        chooseDialog->exec();
        break;
    }
    case FW_DLINK:
    {
        int count = ff.link.size(); // в поле link - имена таблиц
        if (count == 0)
            break;
        s_2cdialog *dlg = new s_2cdialog(hdr);
        dlg->setup(ff.link.at(0),MODE_CHOOSE,"");
        for (int i = 1; i < count; i++)
            dlg->AddTable(ff.link.at(i));
        connect(dlg,SIGNAL(changeshasbeenMade(QString)),this,SLOT(accepted(QString)));
//        dlg->SetTvCurrentText(le->text());
        dlg->exec();
        break;
    }
    case FW_RIGHTS:
    {
        // вызов диалога редактирования прав доступа
        s_accessdialog *dlg = new s_accessdialog;
        dlg->SetupUI(le->text());
        connect(dlg, SIGNAL(acceptChanges(QString)), this, SLOT(accepted(QString)));
        dlg->exec();
        break;
    }
    case FW_DATE:
    {
        s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fdcle");
        QDate dte;
        dte = QDate::fromString(le->text(), "dd/MM/yyyy");
        s_tqCalendarWidget *calWdgt = new s_tqCalendarWidget;
        QPoint pt;
        pt = le->cursor().pos();
        calWdgt->move(pt);
        calWdgt->setSelectedDate(dte);
        connect(calWdgt, SIGNAL(activated(QDate)), this, SLOT(dateChoosed(QDate)));
        connect(calWdgt, SIGNAL(activated(QDate)), calWdgt, SLOT(close()));
        calWdgt->show();
        break;
    }
    case FW_SPECIAL:
    {
        s_2tdialog *dlg = new s_2tdialog;
        QStringList tmpsl = QStringList() << ff.link.at(0) << ff.link.at(1);
        dlg->Setup(tmpsl, le->text());
        connect(dlg,SIGNAL(finished(QString)),this,SLOT(accepted(QString)));
        dlg->exec();
        break;
    }
    case FW_FLLINK:
    {
        s_2cdialog *dlg = new s_2cdialog("");
        dlg->SetupFile(ff.link.at(0)+"."+ff.link.at(1),ff.link.at(2),le->text()); // ff.link.at(0) - имя файла, (1) - расширение, (2) - StringToFind
        connect(dlg,SIGNAL(changeshasbeenMade(QString)),this,SLOT(accepted(QString)));
        dlg->exec();
        break;
    }
    case FW_FLINK: // диалог выбора файлов
    {
        QString Template;
        for (int i=0; i<ff.link.size(); i++)
        {
            QString tmps = ff.link.at(i);
            tmps.replace("_",".");
            Template += tmps;
        }
        if (Template.isEmpty())
            return;
        QString filename = QFileDialog::getOpenFileName(this,"Открыть файл","",Template,0,QFileDialog::DontUseNativeDialog);
        accepted(filename);
        break;
    }
    case FW_ILINK: // диалог выбора каталога
    {
        QString dirname = QFileDialog::getExistingDirectory(this,"Выбрать каталог","",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        accepted(dirname);
        break;
    }
    default:
        break;
    }
}

void s_tqChooseWidget::accepted(QString str)
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit*>("fdcle");
    if (le == 0)
        return;
    PublicClass::ValueStruct vs = tfl.idtov(pc.getlinksfromFF(ff),str);
    le->setText(vs.Value);
    emit textchanged(QVariant(vs.Value));
}

void s_tqChooseWidget::SetValue(QVariant data)
{
    SetData(tfl.idtov(links, data.toString()));
}

void s_tqChooseWidget::SetData(PublicClass::ValueStruct data)
{
    switch (ff.delegate)
    {
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        s_tqLineEdit *le = this->findChild<s_tqLineEdit*>("fdcle");
        if (le != 0)
            le->setText(data.Value);
        break;
    }
    case FD_COMBO:
    {
        s_tqComboBox *cb = this->findChild<s_tqComboBox *>("fdccb");
        if (cb != 0)
            cb->setCurrentText(data.Value);
        break;
    }
    case FD_LINEEDIT:
    {
        switch(ff.ftype)
        {
        case FW_MASKED:
        {
            s_MaskedLineEdit *le = this->findChild<s_MaskedLineEdit *>("lele");
            if (le != 0)
                le->setText(data.Value);
            break;
        }
        default:
        {
            s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("lele");
            if (le != 0)
                le->setText(data.Value);
            break;
        }
        }
        break;
    }
    case FD_SPIN:
    {
        s_tqSpinBox *sb = this->findChild<s_tqSpinBox *>("fdcsb");
        if (sb != 0)
            sb->setValue(data.Value.toDouble());
        break;
    }
    case FD_CHECK:
    {
        s_tqCheckBox *cb = this->findChild<s_tqCheckBox *>("fdcb");
        if (cb != 0)
        {
            if (data.Value == ":/res/ok.png")
                cb->setChecked(true);
            else
                cb->setChecked(false);
        }
        break;
    }
    default:
        break;
    }
}

QString s_tqChooseWidget::Value()
{
    return tfl.vtoid(Data());
}

PublicClass::ValueStruct s_tqChooseWidget::Data()
{
    PublicClass::ValueStruct vs;
    vs.Type = VS_STRING;
    vs.Value = "";
    vs.Links = links;
    switch (ff.delegate)
    {
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fdcle");
        if (le != 0)
            vs.Value = le->text();
        break;
    }
    case FD_COMBO:
    {
        s_tqComboBox *cb = this->findChild<s_tqComboBox *>("fdccb");
        if (cb != 0)
            vs.Value = cb->currentText();
        break;
    }
    case FD_LINEEDIT:
    {
        switch(ff.ftype)
        {
        case FW_MASKED:
        {
            s_MaskedLineEdit *le = this->findChild<s_MaskedLineEdit *>("lele");
            if (le != 0)
                vs.Value = le->text();
            break;
        }
        default:
        {
            s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("lele");
            if (le != 0)
                vs.Value = le->text();
            break;
        }
        }
        break;
    }
    case FD_SPIN:
    {
        s_tqSpinBox *sb = this->findChild<s_tqSpinBox *>("fdcsb");
        if (sb != 0)
            vs.Value = QString::number(sb->value(),'f',6);
        break;
    }
    case FD_CHECK:
    {
        vs.Type = VS_ICON;
        s_tqCheckBox *cb = this->findChild<s_tqCheckBox *>("fdcb");
        if (cb != 0)
        {
            if (cb->isChecked())
                vs.Value = ":/res/ok.png";
            else
                vs.Value = ":/res/cross.png";
        }
        break;
    }
    default:
        break;
    }
    return vs;
}


void s_tqChooseWidget::dateChoosed(QDate dte)
{
    accepted(dte.toString("dd/MM/yyyy"));
}

void s_tqChooseWidget::setAData(QVariant dat)
{
    this->adata = dat;
}

QVariant s_tqChooseWidget::getAData()
{
    return this->adata;
}
