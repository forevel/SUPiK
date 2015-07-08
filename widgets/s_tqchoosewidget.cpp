#include <QHBoxLayout>
#include <QComboBox>
#include <QStringListModel>
#include "s_tqchoosewidget.h"
#include "s_tqlineedit.h"
#include "s_tqpushbutton.h"
#include "s_tqcalendarwidget.h"
#include "s_tqspinbox.h"
#include "s_maskedle.h"
#include "../gen/s_sql.h"
#include "../gen/s_tablefields.h"
#include "../dialogs/s_2cdialog.h"
#include "../dialogs/s_2ctdialog.h"
#include "../dialogs/s_2tdialog.h"
#include "../dialogs/s_accessdialog.h"

// универсальный класс для организации списков выбора из таблиц БД СУПиК
// правила его использования:
// 1. Создать экземпляр класса
// 2. Вызвать setup(QString links,QString hdr="") для установки типа диалогов и ссылок, а также заголовка вызываемого по кнопке диалога (для делегатов FD_CHOOSE) (подробнее о links см. РП)
// 3. Установить связь между сигналом choosed(QVariant) (выбор сделан) и требуемым слотом
// 4. Вызвать метод hide или присвоить данный виджет соответствующему свойству editor (в делегатах)

s_tqChooseWidget::s_tqChooseWidget(QWidget *parent) :
    QWidget(parent)
{
    ff.ftype = -1; // проверка на то, вызывали ли перед работой с виджетом функцию getlinks
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
}

void s_tqChooseWidget::Setup(QString links, QString hdr)
{
    ff = pc.getFFfromLinks(links);
    this->hdr = hdr;

    setStyleSheet("QWidget {background: khaki};");
    QVBoxLayout *ml = new QVBoxLayout;
    switch (ff.delegate)
    {
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        QHBoxLayout *ml2 = new QHBoxLayout;
        s_tqLineEdit *le = new s_tqLineEdit;
        s_tqPushButton *pb = new s_tqPushButton("...");
        le->setObjectName("fdcle");
        if (ff.delegate == FD_CHOOSE)
            le->setEnabled(false);
        else
            le->setEnabled(true);
        pb->setObjectName("fdcpb");
        ml->addWidget(le, 80);
        ml->addWidget(pb,0);
        connect(pb, SIGNAL(clicked()), this, SLOT(pbclicked()));
        ml2->setContentsMargins(0, 0, 0, 0);
        ml->addLayout(ml2);
        break;
    }
    case FD_COMBO:
    {
        QComboBox *cb;
        cb = new QComboBox;
        cb->setStyleSheet("QComboBox {background: khaki};");
        cb->setObjectName("fdccb");
        QStringListModel *tmpModel = new QStringListModel;
        QStringList tmpsl;
        tmpsl = tfl.idtovl(links);
        if (tfl.result)
            tmpsl.clear();
        tmpModel->setStringList(tmpsl);
        cb->setModel(tmpModel);
        ml->addWidget(cb);
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
            ml->addWidget(le);
        }
        else
        {
            s_tqLineEdit *le = new s_tqLineEdit;
            le->setObjectName("lele");
            ml->addWidget(le);
        }
        break;
    }
    case FD_SIMGRID:
    case FD_SIMPLE:
    case FD_DISABLED:
        break;
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
        ml->addWidget(sb);
        break;
    }
    default:
        break;
    }
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
        s_2ctdialog *chooseDialog = new s_2ctdialog(hdr);
        chooseDialog->setup(ff.link.at(0), true); // диалог с "корневой кнопкой"
        connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
        chooseDialog->setTvCurrentText(le->text());
        chooseDialog->exec();
        break;
    }
    case FW_LINK:
    {
        if (tfl.tableistree(ff.link.at(0))) // это дерево
        {
            s_2ctdialog *chooseDialog = new s_2ctdialog(hdr);
            chooseDialog->setup(ff.link.at(0));
            connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
            chooseDialog->setTvCurrentText(le->text());
            chooseDialog->exec();
        }
        else // это таблица
        {
            s_2cdialog *chooseDialog = new s_2cdialog(hdr);
            chooseDialog->setup(ff.link.at(0), MODE_CHOOSE, "", le->text());
            if (!chooseDialog->result)
            {
                connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
                chooseDialog->exec();
            }
        }
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
        dlg->SetTvCurrentText(le->text());
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
    default:
        break;
    }

}

void s_tqChooseWidget::accepted(QString str)
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit*>("fdcle");
    if (le == 0)
        return;
    QString tmpString = tfl.idtov(pc.getlinksfromFF(ff),str);
    le->setText(tmpString);
}

void s_tqChooseWidget::SetData(QVariant data)
{
    switch (ff.delegate)
    {
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        s_tqLineEdit *le = this->findChild<s_tqLineEdit*>("fdcle");
        if (le != 0)
            le->setText(data.toString());
        break;
    }
    case FD_COMBO:
    {
        QComboBox *cb = this->findChild<QComboBox *>("fdccb");
        if (cb != 0)
            cb->setCurrentText(data.toString());
        break;
    }
    case FD_SIMGRID:
    case FD_SIMPLE:
    case FD_DISABLED:
        break;
    case FD_LINEEDIT:
    {
        QLineEdit *le;
        if (ff.ftype == FW_MASKED)
            le = this->findChild<s_MaskedLineEdit *>("lele");
        else
            le = this->findChild<s_tqLineEdit *>("lele");
        if (le != 0)
            le->setText(data.toString());
        break;
    }
    case FD_SPIN:
    {
        s_tqSpinBox *sb = this->findChild<s_tqSpinBox *>("fdcsb");
        if (sb != 0)
            sb->setValue(data.toDouble());
        break;
    }
    default:
        break;
    }
}

QVariant s_tqChooseWidget::Data()
{
    switch (ff.delegate)
    {
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fdcle");
        if (le != 0)
            return QVariant(le->text());
        break;
    }
    case FD_COMBO:
    {
        QComboBox *cb = this->findChild<QComboBox *>("fdccb");
        if (cb != 0)
            return QVariant(cb->currentText());
        break;
    }
    case FD_SIMGRID:
    case FD_DISABLED:
    case FD_SIMPLE:
        break;
    case FD_LINEEDIT:
    {
        QLineEdit *le;
        if (ff.ftype == FW_MASKED)
            le = this->findChild<s_MaskedLineEdit *>("lele");
        else
            le = this->findChild<s_tqLineEdit *>("lele");
        if (le != 0)
            return QVariant(le->text());
        break;
    }
    case FD_SPIN:
    {
        s_tqSpinBox *sb = this->findChild<s_tqSpinBox *>("fdcsb");
        if (sb != 0)
            return QVariant(sb->value());
        break;
    }
    default:
        break;
    }
    return QVariant();
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
