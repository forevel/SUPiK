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
#include "s_tqdatetimewidget.h"
#include "s_maskedle.h"
#include "s_tqlabel.h"
#include "s_tqtextedit.h"
#include "wd_func.h"
#include "../gen/s_sql.h"
#include "../gen/client.h"
#include "../gen/s_tablefields.h"
#include "../dialogs/gen/twocoldialog.h"
#include "../dialogs/gen/chooseitemdialog.h"
#include "../dialogs/gen/specialdialog.h"
#include "../dialogs/gen/accessdialog.h"

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
    pc.getFFfromLinks(links, ff);
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
        tfl.idtovl(links, tmpsl);
        if (tfl.result == TFRESULT_ERROR)
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
    case FD_TEXTEDIT:
    {
        s_tqTextEdit *te = new s_tqTextEdit;
        te->setObjectName("fdcte");
        te->setEnabled(false);
        s_tqPushButton *pb = new s_tqPushButton("...");
        pb->setObjectName("fdcpb");
        connect(pb, SIGNAL(clicked()), this, SLOT(pbclicked()));
        ml2->addWidget(te, 80);
        pb->setFixedSize(32, 32);
        ml2->addWidget(pb, 1);
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
    if (ff.delegate == FD_TEXTEDIT)
    {
        QDialog *dlg = new QDialog(this);
        dlg->setObjectName("tedlg");
        QVBoxLayout *vlyout = new QVBoxLayout;
        s_tqTextEdit *te = new s_tqTextEdit;
        te->setObjectName("tedit");
        QString tmps;
        WDFunc::TEData(this, "fdcte", tmps);
        vlyout->addWidget(te, 95);

        s_tqLineEdit *le = new s_tqLineEdit;
        le->setObjectName("teledit");
        // вытаскиваем имя файла из textedit-а, если таковое присутствует
        if (tmps.contains(TOKEN)) // есть составная запись
        {
            QStringList tmpsl = tmps.split(TOKEN);
            le->setText(tmpsl.at(0));
            if (tmpsl.size() > 1) // есть и текст тоже
                te->setPlainText(tmpsl.at(1));
            else // только картинка
                te->clear();
        }
        else
        {
            te->setPlainText(tmps);
            le->clear();
        }
        if (!ff.link.isEmpty()) // если есть линк на тип каталога, добавляем поле для ввода имени файла
        {
            QHBoxLayout *hlyout = new QHBoxLayout;
            s_tqLabel *lbl = new s_tqLabel("Имя файла:");
            hlyout->addWidget(lbl, 5);
            hlyout->addWidget(le, 90);
            s_tqPushButton *pb = new s_tqPushButton("...");
            connect(pb,SIGNAL(clicked(bool)),this,SLOT(ChooseTEFile()));
            hlyout->addWidget(pb, 5);
            vlyout->addLayout(hlyout);
        }
        s_tqPushButton *pb = new s_tqPushButton("Готово");
        connect(pb,SIGNAL(clicked(bool)),this,SLOT(tepbclicked()));
        vlyout->addWidget(pb, 5);
        dlg->setLayout(vlyout);
        dlg->show();
        return;
    }
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fdcle");
    if (le == 0)
        return;
    switch (ff.ftype)
    {
    case FW_ALLINK:
    {
        ChooseItemDialog *chooseDialog = new ChooseItemDialog;
        chooseDialog->SetupTable(ff.link.at(0), hdr, true); // диалог с "корневой кнопкой"
        connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
        chooseDialog->SetTvCurrentText(le->text());
        chooseDialog->exec();
        break;
    }
    case FW_LINK:
    {
        ChooseItemDialog *chooseDialog = new ChooseItemDialog;
        chooseDialog->SetupTable(ff.link.at(0), hdr);
        connect(chooseDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(accepted(QString)));
        chooseDialog->SetTvCurrentText(le->text());
        chooseDialog->exec();
        break;
    }
    case FW_DLINK:
    {
        int count = ff.link.size(); // в поле link - имена таблиц
        if (count == 0)
            break;
        TwoColDialog *dlg = new TwoColDialog(hdr);
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
        AccessDialog *dlg = new AccessDialog;
        dlg->SetupUI(le->text());
        connect(dlg, SIGNAL(acceptChanges(QString)), this, SLOT(accepted(QString)));
        dlg->exec();
        break;
    }
    case FW_DATE:
    {
        s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fdcle");
        if (le == 0)
            return;
        QDate dte;
        dte = QDate::fromString(le->text(), DATEFORMAT);
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
    case FW_DATETIME:
    {
        s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("fdcle");
        if (le == 0)
            return;
        QDateTime dte;
        dte = QDateTime::fromString(le->text(), DATETIMEFORMAT);
        s_tqDateTimeWidget *dt = new s_tqDateTimeWidget;
        QPoint pt;
        pt = le->cursor().pos();
        dt->move(pt);
        dt->SetDateTime(dte);
        connect(dt, SIGNAL(activated(QDateTime)), this, SLOT(DateTimeEditFinished(QDateTime)));
        connect(dt, SIGNAL(activated(QDateTime)), this,SLOT(DateTimeEditFinished(QDateTime)));
        dt->show();
        break;
    }
    case FW_SPECIAL:
    {
        QStringList tmpsl = QStringList() << ff.link.at(0) << ff.link.at(1);
        SpecialDialog *dlg = new SpecialDialog(tmpsl, le->text());
        connect(dlg,SIGNAL(finished(QString)),this,SLOT(accepted(QString)));
        dlg->exec();
        break;
    }
    case FW_FLLINK:
    {
        ChooseItemDialog *dlg = new ChooseItemDialog;
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
        QString filename = QFileDialog::getOpenFileName(this,"Открыть файл","",Template);
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
    PublicClass::ValueStruct vs;
    QString tmps;
    pc.getlinksfromFF(ff, tmps);
    tfl.idtov(tmps, str, vs);
    WDFunc::SetLEData(this, "fdcle", vs.Value);
    emit textchanged(QVariant(vs.Value));
}

void s_tqChooseWidget::tepbclicked()
{
    QString tmps, tmps2;
    WDFunc::TEData(this, "tedit", tmps);
    if (!ff.link.isEmpty())
    {
        WDFunc::LEData(this, "teledit", tmps2);
        // составляем из файла и текста запись
        tmps.insert(0, TOKEN);
        tmps.insert(0, tmps2);
    }
    QDialog *dlg = this->findChild<QDialog *>("tedlg");
    if (dlg != 0)
        dlg->close();
    SetValue(tmps);
}

void s_tqChooseWidget::ChooseTEFile()
{
    QString tmps;
    if (!ff.link.isEmpty())
    {
        bool ok;
        int tmpi = ff.link.at(0).toInt(&ok);
        if (ok && (tmpi < Cli->PathPrefixes.size()))
            tmps += Cli->PathPrefixes.at(tmpi);
        if (ff.link.size() > 1) // есть ещё и суффикс
        {
            tmpi = ff.link.at(1).toInt(&ok);
            if (ok && (tmpi < Cli->PathSuffixes.size()))
                tmps += Cli->PathSuffixes.at(tmpi);
        }
    }
    CurPath = pc.HomeDir+"/"+tmps;
    QFileDialog *fdlg = new QFileDialog;
    connect(fdlg,SIGNAL(directoryEntered(QString)),this,SLOT(TEDirCheck(QString)));
    connect(fdlg,SIGNAL(fileSelected(QString)),this,SLOT(accepted(QString)));
    fdlg->setAcceptMode(QFileDialog::AcceptOpen);
    fdlg->setDirectory(CurPath);
    fdlg->exec();
}

void s_tqChooseWidget::TEDirCheck(QString dir)
{
    QFileDialog *fdlg = static_cast<QFileDialog *>(sender());
    if (fdlg != 0)
    {
        if (dir != CurPath)
            fdlg->setDirectory(CurPath);
    }
    else
        DBGMSG;
}

void s_tqChooseWidget::SetValue(QVariant data)
{
    PublicClass::ValueStruct vls;
    QString datastring = data.toString();
    tfl.idtov(links, datastring, vls);
    SetData(vls);
}

void s_tqChooseWidget::SetData(PublicClass::ValueStruct data)
{
    switch (ff.delegate)
    {
    case FD_CHOOSE:
    case FD_CHOOSE_X:
    {
        WDFunc::SetLEData(this, "fdcle", data.Value);
        break;
    }
    case FD_COMBO:
    {
        WDFunc::SetCBData(this, "fdccb", data.Value);
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
            WDFunc::SetLEData(this, "lele", data.Value);
            break;
        }
        }
        break;
    }
    case FD_SPIN:
    {
        WDFunc::SetSPBData(this, "fdcsb", data.Value.toDouble());
        break;
    }
    case FD_CHECK:
    {
        WDFunc::SetChBData(this, "fdcb", (data.Value == "1"));
        break;
    }
    case FD_TEXTEDIT:
    {
        WDFunc::SetTEData(this, "fdcte", data.Value);
        break;
    }
    default:
        break;
    }
}

QString s_tqChooseWidget::Value()
{
    QString tmps;
    tfl.vtoid(Data(), tmps);
    return tmps;
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

        WDFunc::LEData(this, "fdcle", vs.Value);
        break;
    }
    case FD_COMBO:
    {
        WDFunc::CBData(this, "fdccb", vs.Value);
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
            WDFunc::LEData(this, "lele", vs.Value);
            break;
        }
        }
        break;
    }
    case FD_SPIN:
    {
        s_tqSpinBox *sb = this->findChild<s_tqSpinBox *>("fdcsb");
        if (sb != 0)
        {
            int tmpInt2 = ff.link.at(0).count("d", Qt::CaseSensitive);
            vs.Value = QString::number(sb->value(),'f',tmpInt2);
        }
        break;
    }
    case FD_CHECK:
    {
        vs.Type = VS_ICON;
        bool chbdata;
        WDFunc::ChBData(this, "fdcb", chbdata);
        vs.Value = (chbdata) ? "1" : "0";
        break;
    }
    case FD_TEXTEDIT:
    {
        WDFunc::TEData(this, "fdcte", vs.Value);
        break;
    }
    default:
        break;
    }
    return vs;
}


void s_tqChooseWidget::dateChoosed(QDate dte)
{
    accepted(dte.toString("dd-MM-yyyy"));
}

void s_tqChooseWidget::DateTimeEditFinished(QDateTime dtm)
{
    accepted(dtm.toString("dd-MM-yyyy hh:mm:ss"));
}
