#include "../inc/dir_adddialog.h"
#include <QTabWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QHBoxLayout>
#include <QStringListModel>
#include <QFontMetrics>
#include <QMessageBox>
#include <QInputDialog>

#include "../inc/publicclass.h"
#include "../inc/s_tqlabel.h"
#include "../inc/s_tqcheckbox.h"
#include "../inc/s_tqspinbox.h"
#include "../inc/s_accessdialog.h"
#include "../inc/s_sql.h"
#include "../inc/s_tablefilterdialog.h"
#include "../inc/s_acceptdialog.h"

dir_adddialog::dir_adddialog(bool update, QString dir, QWidget *parent) :
    QDialog(parent)
{
    result = 0;
    idx = 0;
    setMinimumWidth(300);
    setStyleSheet("QDialog {background-color: rgba(255,255,153);}");
    setAttribute(Qt::WA_DeleteOnClose);
    isSomethingChanged = false;
    this->dir = dir;
    upd = update;
    setupUI();
}

void dir_adddialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *mainPBLayout = new QHBoxLayout;
    s_tqPushButton *WriteAndClosePB = new s_tqPushButton;
    WriteAndClosePB->setIcon(QIcon(":/pic/Pic/icon_zap.png"));
    s_tqPushButton *CancelAndClosePB = new s_tqPushButton;
    CancelAndClosePB->setIcon(QIcon(":/pic/Pic/cross.png"));
    QTabWidget *mainTW = new QTabWidget;
    QDialog *dlg1 = new QDialog;
    QDialog *dlg2 = new QDialog;
    dlg2->setObjectName("dlg2");
    QDialog *dlg3 = new QDialog;
    dlg3->setObjectName("dlg3");
    QGridLayout *dlg1Layout = new QGridLayout;
    QGridLayout *dlg2Layout = new QGridLayout;
    QGridLayout *dlg3Layout = new QGridLayout;
    s_tqLabel *dirAliasL = new s_tqLabel("Имя справочника");
    s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
    dirAliasLE->setObjectName("dirAlias");
    s_tqLabel *dirNameL = new s_tqLabel("Имя таблицы справочника (необязательно)");
    s_tqLineEdit *dirNameLE = new s_tqLineEdit;
    dirNameLE->setObjectName("dirName");
    s_tqLabel *dirFieldNumL = new s_tqLabel("Количество полей");
    s_tqspinbox *dirFieldNumSB = new s_tqspinbox;
    dirFieldNumSB->setObjectName("dirFieldNum");
    dirFieldNumSB->setValue(1);
    dirFieldNumSB->setMinimum(1);
    dirFieldNumSB->setMaximum(FSIZE);
    s_tqLabel *dirBelongL = new s_tqLabel("Тип справочника");
    s_tqComboBox *dirBelongCB = new s_tqComboBox;
    dirBelongCB->setObjectName("dirBelong");
    QStringListModel tmpSLM = new QStringListModel;
    QStringList tmpSL = "" << "Основной";
    if (pc.access & 0x0082) // САПРовские права
        tmpSL << "Altium" << "Schemagee" << "Solidworks" << "Устройства" << "Конструктивы";
    if (pc.access & 0x0402) // ГИ права
        tmpSL << "ОТ и ТБ";
    if (pc.access & 0x2002) // Сисадминские права
        tmpSL << "СисАдмин";
    if (pc.access & 0x0002)
        tmpSL << "Системный";
    dirBelongAliases.clear();
    dirBelongAliases.insert("Основной","ent");
    dirBelongAliases.insert("Altium","alt");
    dirBelongAliases.insert("Schemagee","sch");
    dirBelongAliases.insert("Solidworks","sol");
    dirBelongAliases.insert("Устройства","dev");
    dirBelongAliases.insert("Конструктивы","con");
    dirBelongAliases.insert("ОТ и ТБ","otb");
    dirBelongAliases.insert("СисАдмин","sys");
    dirBelongAliases.insert("Системный","sup");
    dirBelongCB->setEnabled(true);
    s_tqLabel *dirAccessL = new s_tqLabel("Права доступа");
    s_tqLineEdit *dirAccessLE = new s_tqLineEdit;
    dirAccessLE->setObjectName("dirAccess");
    s_tqPushButton *dirAccessPB = new s_tqPushButton("...");
    if (pc.access & 0x0002) // если есть права на изменение системных вещей
        dirAccessPB->setEnabled(true);
    else
        dirAccessPB->setEnabled(false);
    s_tqLabel *mainL = new s_tqLabel("Редактор справочников");
    QFont font;
    font.setPointSize(15);
    mainL->setFont(font);

    mainPBLayout->addWidget(WriteAndClosePB);
    mainPBLayout->addWidget(CancelAndClosePB);
    mainPBLayout->addStretch(300);
    mainLayout->addWidget(mainL, 0, Qt::AlignLeft);
    mainLayout->addLayout(mainPBLayout);
    dlg1Layout->addWidget(dirAliasL, 0, 0);
    dlg1Layout->addWidget(dirAliasLE, 0, 1, 1, 2);
    connect(dirAliasLE, SIGNAL(editingFinished()), this, SLOT(transliteDirName()));
    dlg1Layout->addWidget(dirFieldNumL, 1, 0);
    dlg1Layout->addWidget(dirFieldNumSB, 1, 1, 1, 2);
    dlg1Layout->addWidget(dirBelongL, 2, 0);
    dlg1Layout->addWidget(dirBelongC, 2, 1, 1, 2);
    dlg1Layout->addWidget(dirAccessL, 3, 0);
    dlg1Layout->addWidget(dirAccessLE, 3, 1);
    dlg1Layout->addWidget(dirAccessPB, 3, 2);
    dlg1Layout->addWidget(dirNameL, 4, 0);
    dlg1Layout->addWidget(dirNameLE, 4, 1, 1, 2);
    dlg1Layout->setColumnStretch(0, 10);
    dlg1Layout->setColumnStretch(1, 90);
    dlg1Layout->setColumnStretch(2, 0);
    QWidget *wdgt = new QWidget;
    QVBoxLayout *wdgtl = new QVBoxLayout;
    wdgtl->addStretch(100);
    dlg1Layout->addWidget(wdgt, 5, 0, 1, 3);
    dlg1->setLayout(dlg1Layout);
    mainTW->addTab(dlg1, "Основные");
    for (int i = 0; i < FSIZE; i++)
    {
        s_tqLabel *hdrL = new s_tqLabel("Поле №" + QString::number(i+1) + " :");
        hdrL->setObjectName("hdr"+QString::number(i)+"L");
        s_tqLineEdit *nameLE = new s_tqLineEdit;
        nameLE->setObjectName("name" + QString::number(i) + "LE");
        nameLE->setAData(i);
        adjustFieldSize(nameLE, 20);
        s_tqLabel *bodyL1 = new s_tqLabel(",сист. имя:");
        bodyL1->setObjectName("body"+QString::number(i)+"L1");
        s_tqLineEdit *fieldLE = new s_tqLineEdit;
        fieldLE->setObjectName("field" + QString::number(i) + "LE");
        adjustFieldSize(fieldLE, 15);
        s_tqLabel *bodyL2 = new s_tqLabel(", содержит:");
        bodyL2->setObjectName("body"+QString::number(i)+"L2");
        s_tqLineEdit *valueLE = new s_tqLineEdit;
        valueLE->setObjectName("value" + QString::number(i) + "LE");
        adjustFieldSize(valueLE, 10);
        valueLE->setEnabled(false);
        s_tqPushButton *valueconstPB = new s_tqPushButton("Конструктор");
        valueconstPB->setAData(i);
        valueconstPB->setObjectName("valueconst" + QString::number(i) + "PB");
        adjustFieldSize(valueconstPB, 4);
        connect(valueconstPB, SIGNAL(clicked(s_tqPushButton*)), this, SLOT(FPBPressed(s_tqPushButton*)));
        connect(nameLE,SIGNAL(textChanged(QString,s_tqLineEdit*)),this,SLOT(transliteFieldName(QString,s_tqLineEdit*)));
        QList<QWidget *> wl;
        wl << hdrL << nameLE << bodyL1 << fieldLE << bodyL2 << valueLE << valueconstPB;
        if (i > FSIZE2)
            addLineToDlg(wl, *dlg3Layout, i-FSIZE2);
        else
            addLineToDlg(wl, *dlg2Layout, i);
    }
    dlg2Layout->setColumnStretch(1, 10);
    dlg3Layout->setColumnStretch(1, 10);
    dlg2Layout->setColumnStretch(5, 10);
    dlg3Layout->setColumnStretch(5, 10);
    dlg2->setLayout(dlg2Layout);
    dlg3->setLayout(dlg3Layout);
    mainTW->addTab(dlg2, "Поля 1");
    mainTW->addTab(dlg3, "Поля 2");

    mainTW->setObjectName("mainTW");
    mainLayout->addWidget(mainTW, 95);
    setLayout(mainLayout);
    setMinimumHeight(height()); // меньше не даём изменять, ибо некрасиво
    setMinimumWidth(width()+50);
    if (upd)
        fillFields();
    int res = updateTWFields(dirFieldNumSB->value());
    if (res)
    {
        result = 1;
        return;
    }
    connect(dirAccessPB, SIGNAL(clicked()), this, SLOT(setAccessRights()));
    connect(WriteAndClosePB, SIGNAL(clicked()), this, SLOT(WriteAndClose()));
    connect(CancelAndClosePB, SIGNAL(clicked()), this, SLOT(CancelAndClose()));
    connect(dirFieldNumSB, SIGNAL(valueChanged(int)), this, SLOT(updateTWFields(int)));
}

void dir_adddialog::CancelAndClose()
{
    this->close();
}

void dir_adddialog::WriteAndClose()
{
    int i;
    s_tqLineEdit *lefield = new s_tqLineEdit;
    s_tqLineEdit *levalue = new s_tqLineEdit;
    s_tqLineEdit *lename = new s_tqLineEdit;
    s_tqLineEdit *dirNameLE = new s_tqLineEdit;
    s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
    s_tqLineEdit *dirAccessLE = new s_tqLineEdit;
    s_tqComboBox *dirB = new s_tqComboBox;
    s_tqspinbox *sb = new s_tqspinbox;
    dirNameLE = this->findChild<s_tqLineEdit *>("dirName");
    if (dirNameLE == 0)
        WarningAndClose(11);
    dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
    if (dirAliasLE == 0)
        WarningAndClose(12);
    dirAccessLE = this->findChild<s_tqLineEdit *>("dirAccess");
    if (dirAccessLE == 0)
        WarningAndClose(13);
    dirB = this->findChild<s_tqCheckBox *>("dirBelong");
    if (dirB == 0)
        WarningAndClose(14);
    QString tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb("sup"), "tablefields", "headers", "tablename", dirAliasLE->text()+"_полная");
    if (sqlc.result == 2) // ошибка открытия таблицы
    {
        QMessageBox::warning(this,"warning!","Невозможно открыть БД tablefields!");
        return;
    }
    else if (!sqlc.result) // есть запись про таблицу
    {
        if (QMessageBox::question(this, "Данные справочника существуют", "Перезаписать?", QMessageBox::Yes|QMessageBox::No,\
                              QMessageBox::No) == QMessageBox::No)
            return;
    }
    sb = this->findChild<s_tqspinbox *>("dirFieldNum");
    if (sb == 0)
        WarningAndClose(15);
    QSqlDatabase db = sqlc.getdb("sup");
    QStringList fl, vl;
    QStringList cmpfl, cmpvl;
    cmpfl << "tablename" << "tablefields";
    QString tble = (dirB->isChecked()) ? "sup" : "ent";
    fl << "tablefields" << "table" << "keyfield" << "headers" << "links" << "fieldsorder" << "tablename" << \
          "date" << "deleted" << "idpers";
    for (i = 0; i < sb->value(); i++)
    {
        levalue = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
        lename = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
        lefield = this->findChild<s_tqLineEdit *>("field"+QString::number(i)+"LE");
        vl.clear();
        cmpvl.clear();
        tmpString = (lename->text() == "ИД") ? "v" : "";
        vl << lefield->text() << tble+"."+dirNameLE->text() << tmpString << lename->text() << levalue->text();
        tmpString = QString("%1").arg(i, 2, 10, QChar('0'));
        vl << tmpString << dirAliasLE->text()+"_полная" << pc.DateTime << "0" << QString::number(pc.idPers);
        cmpvl << dirAliasLE->text()+"_полная" << lefield->text();
        QString id = sqlc.getvaluefromtablebyfields(sqlc.getdb("sup"), "tablefields", "idtablefields", cmpfl, cmpvl);
        if (sqlc.result == 1) // нет такой записи
            sqlc.insertvaluestotable(sqlc.getdb("sup"), "tablefields", fl, vl);
        else
            sqlc.updatevaluesintable(sqlc.getdb("sup"), "tablefields", fl, vl, "idtablefields", id);
        if (sqlc.result)
            WarningAndClose(17);
    }
    tble = "dirlist";
    fl.clear();
    vl.clear();
    fl << "dirlist" << "access" << "deleted";
    vl << dirAliasLE->text() << dirAccessLE->text() << "0";
    tmpString = sqlc.getvaluefromtablebyfield(db, tble, "dirlist", "dirlist", dirAliasLE->text());
    if (tmpString.isEmpty())
    {
        tmpString = sqlc.insertvaluestotable(db, tble, fl, vl);
        if (sqlc.result)
        {
            QMessageBox::warning(this, "warning!", "Ошибка обновления каталога справочников");
            return;
        }
    }
    else
    {
        tmpString = sqlc.updatevaluesintable(db, tble, fl, vl, "dirlist", dirAliasLE->text());
        if (sqlc.result)
        {
            QMessageBox::warning(this, "warning!", "Ошибка обновления каталога справочников");
            return;
        }
    }
    s_acceptdialog *dlg = new s_acceptdialog;
    dlg->setupUI("Ура!","Записано успешно!");
    connect(dlg,SIGNAL(destroyed()), this, SLOT(close()));
    dlg->exec();
}

int dir_adddialog::updateTWFields(int fn)
{
    try
    {
        s_tqLabel *ll = new s_tqLabel;
        s_tqLineEdit *le = new s_tqLineEdit;
        s_tqComboBox *cb = new s_tqComboBox;
        s_tqPushButton *pb = new s_tqPushButton;
        int i = 0;
        while (i < fn)
        {
            ll = this->findChild<s_tqLabel *>("hdr"+QString::number(i)+"L");
            if (ll == 0)
                throw (0x21);
            ll->setVisible(true);
            ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L1");
            if (ll == 0)
                throw(0x22);
            ll->setVisible(true);
            ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L2");
            if (ll == 0)
                throw(0x23);
            ll->setVisible(true);
            le = this->findChild<s_tqLineEdit *>("field"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x24);
            le->setVisible(true);
            le = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x25);
            le->setVisible(true);
            le = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x26);
            le->setVisible(true);
            pb = this->findChild<s_tqPushButton *>("valueconst"+QString::number(i)+"PB");
            if (pb == 0)
                throw(0x28);
            pb->setVisible(true);
            if (cb->currentIndex() == 0)
            {
                le->setEnabled(true);
                pb->setEnabled(false);
            }
            else
            {
                le->setEnabled(true);
                pb->setEnabled(true);
            }
            i++;
        }
        while (i < FSIZE)
        {
            ll = this->findChild<s_tqLabel *>("hdr"+QString::number(i)+"L");
            if (ll == 0)
                throw(0x29);
            ll->setVisible(false);
            ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L1");
            if (ll == 0)
                throw(0x2a);
            ll->setVisible(false);
            ll = this->findChild<s_tqLabel *>("body"+QString::number(i)+"L2");
            if (ll == 0)
                throw(0x2b);
            ll->setVisible(false);
            le = this->findChild<s_tqLineEdit *>("field"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x2c);
            le->setVisible(false);
            le = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x2d);
            le->setVisible(false);
            le = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE");
            if (le == 0)
                throw(0x2e);
            le->setVisible(false);
            pb = this->findChild<s_tqPushButton *>("valueconst"+QString::number(i)+"PB");
            if (pb == 0)
                throw(0x2f);
            pb->setVisible(false);
            i++;
        }
        return 0;
    }
    catch (int res)
    {
        WarningAndClose(res);
        return res;
    }
}

void dir_adddialog::setAccessRights()
{
    s_accessdialog *accessdialog = new s_accessdialog;
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("dirAccess");
    accessdialog->SetupUI(le->text().toLongLong(0, 16));
    connect(accessdialog, SIGNAL(acceptChanges(long)), this, SLOT(acceptAccess(long)));
    accessdialog->exec();
}

// обработка подтверждения в диалоге accessrights

void dir_adddialog::acceptAccess(long rights)
{
    isSomethingChanged = true;
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("dirAccess");
    le->setText(QString::number(rights, 16));
}

void dir_adddialog::addLineToDlg(QList<QWidget *> wl, QGridLayout &lyt, int row)
{
    for (int i = 0; i < wl.size(); i++)
        lyt.addWidget(wl.at(i), row, i);
}

void dir_adddialog::FPBPressed(s_tqPushButton *ptr)
{
    try
    {
        idx = ptr->getAData().toInt(0);
        s_tqspinbox *sb = new s_tqspinbox;
        sb = this->findChild<s_tqspinbox *>("dirFieldNum");
        if (sb == 0)
            WarningAndClose(41);
        s_tqComboBox *cb = new s_tqComboBox;
        cb = this->findChild<s_tqComboBox *>("ltype"+QString::number(idx)+"CB");
        if (cb == 0)
            WarningAndClose(42);
        s_tqLineEdit *le = new s_tqLineEdit;
        le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
        if (le == 0)
            WarningAndClose(43);
        switch (cb->currentIndex())
        {
        case 0:
        {
            break;
        }
        case 1: // ссылка на поле
        {
            sqlc.prepareslsfortree(sl1, sl2);
            if (sl1.isEmpty())
                WarningAndClose(44);
            s_tablefilterdialog *tabledialog = new s_tablefilterdialog;
            QString tmpString = le->text().split(".").last();
            tabledialog->SetupUI(sl1, sl2, tmpString);
            connect(tabledialog, SIGNAL(accepted(QString)), this, SLOT(updateFLE(QString)));
            tabledialog->exec();
            break;
        }
        case 2: // права доступа
        {
            s_accessdialog *accessdialog = new s_accessdialog;
            accessdialog->SetupUI(le->text().toLongLong(0, 16));
            connect(accessdialog, SIGNAL(acceptChanges(long)), this, SLOT(updateALE(long)));
            accessdialog->exec();
            break;
        }
        case 3: // косвенная ссылка
        {
            QSqlDatabase db = sqlc.getdb(dir.left(3));
            if (!db.isValid())
                WarningAndClose(45);
            QString tble = dir.right(dir.size()-4);
            QStringList fl = sqlc.getcolumnsfromtable(db, tble);
            s_tablefilterdialog *tabledialog = new s_tablefilterdialog;
            QString tmpString = le->text().split(".").last();
            tabledialog->SetupUI(fl, tmpString);
            connect(tabledialog, SIGNAL(accepted(QString)), this, SLOT(updateFLE(QString)));
            tabledialog->exec();
            break;
        }
        default:
            break;
        }
        updateTWFields(sb->value());
    }
    catch (int res)
    {
        WarningAndClose(res);
    }
}

void dir_adddialog::CBPressed(QString str, s_tqComboBox *ptr)
{
    isSomethingChanged = true;
    Q_UNUSED(str);
    s_tqPushButton *pb = new s_tqPushButton;
    pb = this->findChild<s_tqPushButton *>("valueconst"+ptr->getAData().toString()+"PB");
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("value"+ptr->getAData().toString()+"LE");
    if (pb == 0)
        WarningAndClose(51);
    if (ptr->currentIndex() == 0)
    {
        le->setEnabled(true);
        pb->setEnabled(false);
    }
    else
    {
        le->setEnabled(true);
        pb->setEnabled(true);
    }
}

void dir_adddialog::adjustFieldSize(QWidget *wdgt, int widthInChar)
{
    QFontMetrics fm(wdgt->font());
    wdgt->setMinimumWidth(fm.width(QLatin1Char('x'))*widthInChar + 2*2);
}

void dir_adddialog::updateFLE(QString str)
{
    isSomethingChanged = true;
    int i, j;
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    s_tqComboBox *cb = new s_tqComboBox;
    cb = this->findChild<s_tqComboBox *>("ltype"+QString::number(idx)+"CB");
    if (cb == 0)
        WarningAndClose(61);
    if (le == 0)
        WarningAndClose(62);
    switch (cb->currentIndex())
    {
    case 1:
    {
        i = sl2.indexOf(str);
        if (i != -1)
        {
            j = 0;
            while (i > 0)
            {
                if (sl2.at(i--) == "")
                    j++;
            }
            le->setText("pc."+sl1.at(j).left(3)+"."+str); // компонуем имя таблицы в формат <имя_БД>.<имя_таблицы>
        }
        break;
    }
    case 3:
    {
        le->setText("."+str);
    }
    default:
        break;
    }
}

void dir_adddialog::updateALE(long rights)
{
    isSomethingChanged = true;
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("value"+QString::number(idx)+"LE");
    if (le == 0)
        WarningAndClose(71);
    le->setText(QString::number(rights, 16));
}

void dir_adddialog::fillFields()
{
    s_tqLineEdit *lef = new s_tqLineEdit;
    s_tqLineEdit *lel = new s_tqLineEdit;
    s_tqLineEdit *lev = new s_tqLineEdit;
    s_tqComboBox *cb = new s_tqComboBox;
    QSqlDatabase db = sqlc.getdb(dir.left(3));
    if (!db.isValid())
        WarningAndClose(72);
    QString tble = dir.right(dir.size()-4);
    QStringList fl = sqlc.getcolumnsfromtable(db, tble);
    if (fl.isEmpty())
        WarningAndClose(73);
    fl.removeAll("id"+tble); // поле с ИД пишется независимо
    fl.removeAll("idpers"); // поле idpers пишется независимо
    fl.removeAll("date"); // поле date пишется независимо
    fl.removeAll("deleted"); // поле deleted пишется независимо
    QStringList vl1 = sqlc.getvaluesfromtablebyid(db, tble, fl, "1");
//    if (vl1.isEmpty())
//        WarningStr("Отсутствуют описания полей справочника!");
    QStringList vl2 = sqlc.getvaluesfromtablebyid(db, tble, fl, "2");
//    if (vl2.isEmpty())
//        WarningStr("Отсутствуют заголовки справочника!");

    s_tqLineEdit *dirNameLE = new s_tqLineEdit;
    dirNameLE = this->findChild<s_tqLineEdit *>("dirName");
    if (dirNameLE == 0)
        WarningAndClose(74);
    s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
    dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
    if (dirAliasLE == 0)
        WarningAndClose(75);
    s_tqCheckBox *dirBelongC = new s_tqCheckBox;
    dirBelongC = this->findChild<s_tqCheckBox *>("dirBelong");
    if (dirBelongC == 0)
        WarningAndClose(76);
    s_tqLineEdit *dirAccessLE = new s_tqLineEdit;
    dirAccessLE = this->findChild<s_tqLineEdit *>("dirAccess");
    if (dirAccessLE == 0)
        WarningAndClose(77);
    s_tqspinbox *sb = new s_tqspinbox;
    sb = this->findChild<s_tqspinbox *>("dirFieldNum");
    if (sb == 0)
        WarningAndClose(78);
    dirNameLE->setText(dir.right(dir.size()-4));
    QString tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb("sup"),"dirlist","dirlist","pc",dir);
    if (tmpString.isEmpty())
        WarningAndClose(79);
    dirAliasLE->setText(tmpString);
    if (dir.left(3) == "ent")
        dirBelongC->setChecked(false);
    else
        dirBelongC->setChecked(true);
    tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb("sup"),"dirlist","access","pc",dir);
    if (tmpString.isEmpty())
        WarningAndClose(80);
    dirAccessLE->setText(tmpString);
    sb->setValue(fl.size());
    for (int i = 0; i < fl.size(); i++)
    {
        if (i > 16)
            break;
        lef = this->findChild<s_tqLineEdit *>("field"+QString::number(i)+"LE"); // имя поля
        if (lef == 0)
            WarningAndClose(81);
        lel = this->findChild<s_tqLineEdit *>("value"+QString::number(i)+"LE"); // строка 1
        if (lel == 0)
            WarningAndClose(82);
        lev = this->findChild<s_tqLineEdit *>("name"+QString::number(i)+"LE"); // строка 2
        if (lev == 0)
            WarningAndClose(83);
        cb = this->findChild<s_tqComboBox *>("ltype"+QString::number(i)+"CB");
        if (cb == 0)
            WarningAndClose(84);
        lef->setText(fl.at(i));
        if (i < vl1.size())
        {
            if (vl1.at(i)=="")
                cb->setCurrentIndex(0);
            else if (vl1.at(i)=="x")
                cb->setCurrentIndex(2);
            else if (vl1.at(i).left(3)=="pc.")
                cb->setCurrentIndex(1);
            else if (vl1.at(i).left(1) == ".")
                cb->setCurrentIndex(3);
            else
                cb->setCurrentIndex(0);
            lel->setText(vl1.at(i));
            lev->setText(vl2.at(i));
        }
        else
        {
            lel->setText("");
            lev->setText("");
        }
    }
}

void dir_adddialog::WarningAndClose(int res)
{
    int ernum = module_number + res;
    s_acceptdialog *dlg = new s_acceptdialog;
    dlg->setupUI("warning!","Ошибка №"+QString::number(ernum, 16));
    connect(dlg,SIGNAL(destroyed()), this, SLOT(close()));
    dlg->exec();
    this->close();
}

void dir_adddialog::transliteFieldName(QString str, s_tqLineEdit *ptr)
{
    isSomethingChanged = true;
    Q_UNUSED(str);
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("name"+ptr->getAData().toString()+"LE");
    s_tqLineEdit *le2 = new s_tqLineEdit;
    le2 = this->findChild<s_tqLineEdit *>("value"+ptr->getAData().toString()+"LE");
    if (le == 0)
        WarningAndClose(101);
    if (le2 == 0)
        WarningAndClose(102);
    if (le2->text().isEmpty())
        le2->setText(pc.getTranslit(le->text()));
}

void dir_adddialog::transliteDirName()
{
    s_tqLineEdit *dirNameLE = new s_tqLineEdit;
    s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
    dirNameLE = this->findChild<s_tqLineEdit *>("dirName");
    if (dirNameLE == 0)
        WarningAndClose(91);
    if (dirNameLE->text().isEmpty())
    {
        dirAliasLE = this->findChild<s_tqLineEdit *>("dirAlias");
        if (dirAliasLE == 0)
            WarningAndClose(92);
        QString tmpString = pc.getTranslit(dirAliasLE->text());
        dirNameLE->setText(tmpString);
    }
}
