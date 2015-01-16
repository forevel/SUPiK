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
    s_tqLabel *dirBelongL = new s_tqLabel("Системный");
    s_tqCheckBox *dirBelongC = new s_tqCheckBox;
    dirBelongC->setObjectName("dirBelong");
    dirBelongC->setChecked(false);
    if (pc.access & 0x0002) // если есть права на изменение системных вещей
        dirBelongC->setEnabled(true);
    else
        dirBelongC->setEnabled(false);
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
    QStringList FTypes;
    FTypes << "простое поле" << "ссылку на поле" << "права доступа" << "косвенную ссылку";
    QStringListModel *FTypesModel = new QStringListModel;
    FTypesModel->setStringList(FTypes);
    for (int i = 0; i < FSIZE; i++)
    {
        s_tqLabel *par1L1 = new s_tqLabel("Поле №" + QString::number(i+1) + " :");
        par1L1->setObjectName("par1"+QString::number(i)+"L");
        s_tqLineEdit *par1LE = new s_tqLineEdit;
        par1LE->setObjectName("parf" + QString::number(i) + "LE");
        adjustFieldSize(par1LE, 15);
        s_tqLabel *par1L2 = new s_tqLabel(", представляет собой:");
        par1L2->setObjectName("par2"+QString::number(i)+"L");
        s_tqComboBox *par1CB = new s_tqComboBox;
        par1CB->setObjectName("par" + QString::number(i) + "CB");
        par1CB->setModel(FTypesModel);
        adjustFieldSize(par1CB, 20); // 20 = длина выражения "ссылку на таблицу"+3
        par1CB->setAData(i);
        connect(par1CB, SIGNAL(textChanged(QString,s_tqComboBox*)), this, SLOT(CBPressed(QString,s_tqComboBox*)));
        s_tqLabel *par1L3 = new s_tqLabel(" и содержит:");
        par1L3->setObjectName("par3"+QString::number(i)+"L");
        s_tqLineEdit *par32LE = new s_tqLineEdit;
        par32LE->setObjectName("parv2" + QString::number(i) + "LE");
        adjustFieldSize(par32LE, 20);
        s_tqLineEdit *par3LE = new s_tqLineEdit;
        par3LE->setObjectName("parv" + QString::number(i) + "LE");
        adjustFieldSize(par3LE, 10);
        par3LE->setEnabled(true);
        s_tqPushButton *par3PB = new s_tqPushButton("...");
        par3PB->setAData(i);
        par3PB->setObjectName("par" + QString::number(i) + "PB");
        adjustFieldSize(par3PB, 4);
        connect(par3PB, SIGNAL(clicked(s_tqPushButton*)), this, SLOT(FPBPressed(s_tqPushButton*)));
        QList<QWidget *> wl;
        wl << par1L1 << par1LE << par1L2 << par1CB << par1L3 << par32LE << par3LE << par3PB;
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
    updateTWFields(dirFieldNumSB->value());

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
    QStringList sl;
    int i;
    s_tqLineEdit *lef = new s_tqLineEdit;
    s_tqLineEdit *lev = new s_tqLineEdit;
    s_tqLineEdit *lel = new s_tqLineEdit;
    s_tqLineEdit *dirNameLE = new s_tqLineEdit;
    s_tqLineEdit *dirAliasLE = new s_tqLineEdit;
    s_tqLineEdit *dirAccessLE = new s_tqLineEdit;
    s_tqComboBox *cb = new s_tqComboBox;
    s_tqCheckBox *dirB = new s_tqCheckBox;
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
    dir = (dirB->isChecked())?"sup.":"ent.";
    dir += dirNameLE->text();
    QSqlDatabase db = sqlc.getdb(dir.left(3));
    QString tble = dir.right(dir.size()-4);
    if (!db.isValid())
    {
        QMessageBox::warning(this,"warning!","Невозможно открыть БД!");
        return;
    }
    else
    {
        sl = sqlc.getcolumnsfromtable(db, tble);
        if (!sl.isEmpty())
        {
            if (QMessageBox::question(this, "Справочник существует", "Перезаписать? Данные справочника будут утрачены!", QMessageBox::Yes|QMessageBox::No,\
                                  QMessageBox::No) == QMessageBox::Yes)
            {
                if (sqlc.dropTable(db, tble))
                {
                    QMessageBox::warning(this,"warning!","Ошибка удаления таблицы!");
                    return;
                }
            }
        }
    }
    sb = this->findChild<s_tqspinbox *>("dirFieldNum");
    if (sb == 0)
        WarningAndClose(15);
    sl.clear();
    for (i = 0; i < sb->value(); i++)
    {
        lef = this->findChild<s_tqLineEdit *>("parf"+QString::number(i)+"LE");
        if (lef == 0)
            WarningAndClose(16);
        sl << lef->text();
    }
//    sl << "date" << "deleted";
    if (sqlc.createTable(db, tble, sl))
    {
        QMessageBox::warning(this,"warning!","Ошибка создания таблицы!");
        return;
    }
    // теперь создадим строки 1 и 2
    QStringList vl1, vl2;
    for (i = 0; i < sl.size(); i++)
    {
        cb = this->findChild<s_tqComboBox *>("par"+QString::number(i)+"CB");
        lel = this->findChild<s_tqLineEdit *>("parv"+QString::number(i)+"LE");
        lev = this->findChild<s_tqLineEdit *>("parv2"+QString::number(i)+"LE");
        if (cb->currentIndex() == 0) // простое поле
        {
            vl2.insert(i, lev->text());
            if ((sl.at(i) == tble) || (sl.at(i) == "alias")) // столбцы с именами <tble> и alias должны всегда отображаться, значит, их надо прописать в строку 1
                vl1.insert(i, lev->text());
            else
                vl1.insert(i, lel->text());
//            else
//                vl1.insert(i, "");
        }
        else if (cb->currentIndex() == 1)  // ссылка на поле
        {
            vl2.insert(i, lev->text());
            vl1.insert(i, lel->text());
        }
        else if (cb->currentIndex() == 2) // права доступа
        {
            vl2.insert(i, lev->text());
            vl1.insert(i, "x");
        }
        else if (cb->currentIndex() == 3) // косвенная ссылка
        {
            vl2.insert(i, lev->text());
            vl1.insert(i, lel->text());
        }
    }
    sl << "idpers" << "date" << "deleted";
    vl1.insert(i, "");
    vl2.insert(i, "");
    i++;
    vl1.insert(i, "0000-00-00 00:00:00");
    vl2.insert(i, "0000-00-00 00:00:00");
    i++;
    vl1.insert(i, "0");
    vl2.insert(i, "0");
    QString newID = sqlc.insertvaluestotable(db, tble, sl, vl1, 1);
    if ((sqlc.result) || (newID != "1"))
    {
        QMessageBox::warning(this, "warning!", "Ошибка записи в строку 1");
        return;
    }
    newID = sqlc.insertvaluestotable(db, tble, sl, vl2, 2);
    if ((sqlc.result) || (newID != "2"))
    {
        QMessageBox::warning(this, "warning!", "Ошибка записи в строку 2");
        return;
    }

    db = sqlc.getdb("sup");
    tble = "dirlist";
    QStringList fl, vl;
    fl << "dirlist" << "access" << "pc" << "deleted";
    vl << dirAliasLE->text() << dirAccessLE->text() << dir << "0";
    QString tmpString = sqlc.getvaluefromtablebyfield(db, tble, "dirlist", "pc", dir);
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
        tmpString = sqlc.updatevaluesintable(db, tble, fl, vl, "pc", dir);
        if (sqlc.result)
        {
            QMessageBox::warning(this, "warning!", "Ошибка обновления каталога справочников");
            return;
        }
    }
    s_acceptdialog *dlg = new s_acceptdialog("Ура!","Записано успешно!");
    connect(dlg,SIGNAL(caccepted()), this, SLOT(close()));
    dlg->exec();
}

void dir_adddialog::updateTWFields(int fn)
{
    s_tqLabel *ll = new s_tqLabel;
    s_tqLineEdit *le = new s_tqLineEdit;
    s_tqComboBox *cb = new s_tqComboBox;
    s_tqPushButton *pb = new s_tqPushButton;
    int i = 0;
    while (i < fn)
    {
        ll = this->findChild<s_tqLabel *>("par1"+QString::number(i)+"L");
        if (ll == 0)
            WarningAndClose(21);
        ll->setVisible(true);
        ll = this->findChild<s_tqLabel *>("par2"+QString::number(i)+"L");
        if (ll == 0)
            WarningAndClose(22);
        ll->setVisible(true);
        ll = this->findChild<s_tqLabel *>("par3"+QString::number(i)+"L");
        if (ll == 0)
            WarningAndClose(23);
        ll->setVisible(true);
        le = this->findChild<s_tqLineEdit *>("parf"+QString::number(i)+"LE");
        if (le == 0)
            WarningAndClose(24);
        le->setVisible(true);
        le = this->findChild<s_tqLineEdit *>("parv2"+QString::number(i)+"LE");
        if (le == 0)
            WarningAndClose(25);
        le->setVisible(true);
        le = this->findChild<s_tqLineEdit *>("parv"+QString::number(i)+"LE");
        if (le == 0)
            WarningAndClose(26);
        le->setVisible(true);
        cb = this->findChild<s_tqComboBox *>("par"+QString::number(i)+"CB");
        if (cb == 0)
            WarningAndClose(27);
        cb->setVisible(true);
        pb = this->findChild<s_tqPushButton *>("par"+QString::number(i)+"PB");
        if (pb == 0)
            WarningAndClose(28);
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
        ll = this->findChild<s_tqLabel *>("par1"+QString::number(i)+"L");
        if (ll == 0)
            WarningAndClose(29);
        ll->setVisible(false);
        ll = this->findChild<s_tqLabel *>("par2"+QString::number(i)+"L");
        if (ll == 0)
            WarningAndClose(30);
        ll->setVisible(false);
        ll = this->findChild<s_tqLabel *>("par3"+QString::number(i)+"L");
        if (ll == 0)
            WarningAndClose(31);
        ll->setVisible(false);
        le = this->findChild<s_tqLineEdit *>("parf"+QString::number(i)+"LE");
        if (le == 0)
            WarningAndClose(32);
        le->setVisible(false);
        le = this->findChild<s_tqLineEdit *>("parv"+QString::number(i)+"LE");
        if (le == 0)
            WarningAndClose(33);
        le->setVisible(false);
        le = this->findChild<s_tqLineEdit *>("parv2"+QString::number(i)+"LE");
        if (le == 0)
            WarningAndClose(34);
        le->setVisible(false);
        cb = this->findChild<s_tqComboBox *>("par"+QString::number(i)+"CB");
        if (cb == 0)
            WarningAndClose(35);
        cb->setVisible(false);
        pb = this->findChild<s_tqPushButton *>("par"+QString::number(i)+"PB");
        if (pb == 0)
            WarningAndClose(36);
        pb->setVisible(false);
        i++;
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
    idx = ptr->getAData().toInt(0);
    s_tqspinbox *sb = new s_tqspinbox;
    sb = this->findChild<s_tqspinbox *>("dirFieldNum");
    if (sb == 0)
        WarningAndClose(41);
    s_tqComboBox *cb = new s_tqComboBox;
    cb = this->findChild<s_tqComboBox *>("par"+QString::number(idx)+"CB");
    if (cb == 0)
        WarningAndClose(42);
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("parv"+QString::number(idx)+"LE");
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

void dir_adddialog::CBPressed(QString str, s_tqComboBox *ptr)
{
    isSomethingChanged = true;
    Q_UNUSED(str);
    s_tqPushButton *pb = new s_tqPushButton;
    pb = this->findChild<s_tqPushButton *>("par"+ptr->getAData().toString()+"PB");
    s_tqLineEdit *le = new s_tqLineEdit;
    le = this->findChild<s_tqLineEdit *>("parv"+ptr->getAData().toString()+"LE");
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
    le = this->findChild<s_tqLineEdit *>("parv"+QString::number(idx)+"LE");
    s_tqComboBox *cb = new s_tqComboBox;
    cb = this->findChild<s_tqComboBox *>("par"+QString::number(idx)+"CB");
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
    le = this->findChild<s_tqLineEdit *>("parv"+QString::number(idx)+"LE");
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
    if (vl1.isEmpty())
        WarningStr("Отсутствуют описания полей справочника!");
    QStringList vl2 = sqlc.getvaluesfromtablebyid(db, tble, fl, "2");
    if (vl2.isEmpty())
        WarningStr("Отсутствуют заголовки справочника!");

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
        lef = this->findChild<s_tqLineEdit *>("parf"+QString::number(i)+"LE"); // имя поля
        if (lef == 0)
            WarningAndClose(81);
        lel = this->findChild<s_tqLineEdit *>("parv"+QString::number(i)+"LE"); // строка 1
        if (lel == 0)
            WarningAndClose(82);
        lev = this->findChild<s_tqLineEdit *>("parv2"+QString::number(i)+"LE"); // строка 2
        if (lev == 0)
            WarningAndClose(83);
        cb = this->findChild<s_tqComboBox *>("par"+QString::number(i)+"CB");
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
    s_acceptdialog *dlg = new s_acceptdialog("warning!","Ошибка dir_adddialog №"+QString::number(res));
    connect(dlg,SIGNAL(accepted()), this, SLOT(close()));
}

void dir_adddialog::WarningStr(QString str)
{
    QMessageBox::warning(this, "warning!", str);
}

void dir_adddialog::transliteFieldName()
{

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
