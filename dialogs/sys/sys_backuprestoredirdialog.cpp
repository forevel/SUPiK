//#include <QShowEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QHeaderView>
#include <QPaintEvent>
#include <QPainter>
#include <QApplication>
#include <QtXlsx/xlsxdocument.h>
#include "sys_backuprestoredirdialog.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/treeview.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"

sys_backuprestoredirdialog::sys_backuprestoredirdialog(bool isImport, QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    this->isImport = isImport;
    SetupUI();
}

void sys_backuprestoredirdialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/Sys2Wallpaper.png"));

    event->accept();
}

void sys_backuprestoredirdialog::SetupUI()
{
    int i;
    QGridLayout *lowerLayout = new QGridLayout;
    QVBoxLayout *mainbl = new QVBoxLayout;
    QHBoxLayout *upperLayout = new QHBoxLayout;
    QHBoxLayout *midLayout = new QHBoxLayout;
    TreeView *mainTV = new TreeView;
    mainTV->setObjectName("mainTV");
    mainTV->verticalHeader()->setVisible(false);
    s_tqPushButton *ImpExpPB = new s_tqPushButton;
    ImpExpPB->setText((isImport)?"Импорт":"Экспорт");
    ImpExpPB->setIcon(QIcon((isImport)?":/res/import.png":":/res/export.png"));
    s_tqPushButton *CancelPB = new s_tqPushButton;
    CancelPB->setText("Отмена");
    CancelPB->setIcon(QIcon(":/res/cross.png"));
    s_tqPushButton *loadFilePB = new s_tqPushButton("...");
    s_tqLabel *loadFileL = new s_tqLabel("Файл:");
    s_tqLineEdit *loadFileLE = new s_tqLineEdit;
    loadFileLE->setEnabled(false);
    loadFileLE->setObjectName("loadFileLE");
    s_tqLabel *dirL = new s_tqLabel("Справочник:");
    s_tqComboBox *dirCB = new s_tqComboBox;
    QStringList tmpsl;
    if (isImport)
        tmpsl << "Номенклатура (к)" << "Компоненты Altium (к)" << "Компоненты Schemagee (к)";
    else
        tmpsl << "Номенклатура";
    dirM = new QStringListModel(tmpsl);
    dirCB->setModel(dirM);
    dirCB->setCurrentIndex(-1);
    dirCB->setObjectName("dirCB");
    upperLayout->addWidget(ImpExpPB);
    upperLayout->addWidget(CancelPB);
    upperLayout->addWidget(loadFileL);
    upperLayout->addWidget(loadFileLE, 100);
    upperLayout->addWidget(loadFilePB);
    mainbl->addLayout(upperLayout);
    midLayout->addWidget(dirL);
    midLayout->addWidget(dirCB);
    midLayout->addStretch(100);
    mainbl->addLayout(midLayout);
    if (isImport)
    {
        int j = 0;
        int k = 0;
        for (i = 0; i < MAX_COL_NUM; i++)
        {
            s_tqLabel *lbl = new s_tqLabel;
            lbl->setObjectName("fieldl"+QString::number(i));
            lbl->setVisible(false);
            lowerLayout->addWidget(lbl, j, k);
            k++;
            s_tqComboBox *cb = new s_tqComboBox;
            cb->setObjectName("fieldcb"+QString::number(i));
            cb->setCurrentIndex(-1);
            cb->setVisible(false);
            lowerLayout->addWidget(cb, j, k);
            k++;
            if (!((i+1)%4))
            {
                j++;
                k = 0;
            }
        }
        lowerLayout->setColumnStretch(1, 10);
        lowerLayout->setColumnStretch(3, 10);
        lowerLayout->setColumnStretch(5, 10);
        lowerLayout->setColumnStretch(7, 10);
        lowerLayout->setColumnStretch(9, 10);
        mainbl->addLayout(lowerLayout);
    }
    mainbl->addWidget(mainTV, 100);
    setLayout(mainbl);

    connect(ImpExpPB, SIGNAL(clicked()), this, SLOT(ImpExpPBPressed()));
    connect(CancelPB, SIGNAL(clicked()), this, SLOT(CancelPBPressed()));
    connect(loadFilePB, SIGNAL(clicked()), this, SLOT(LoadFilePBPressed()));
    connect(dirCB, SIGNAL(currentIndexChanged(QString)), this, SLOT(DirChoosed(QString)));
}

void sys_backuprestoredirdialog::ImpExpPBPressed()
{
    int i, j;
    QStringList fl, vl;
    QString db;
    QString tble, tmpString;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    s_tqComboBox *cb = this->findChild<s_tqComboBox *>("dirCB");
    if (cb->currentIndex() < 0)
    {
        QMessageBox::warning(this, "warning!", "Не выбран справочник!");
        QApplication::restoreOverrideCursor();
        return;
    }
    if (filename.isEmpty())
    {
        QMessageBox::warning(this, "warning!", "Не выбран файл!");
        QApplication::restoreOverrideCursor();
        return;
    }
    QString dirpc = sqlc.GetValueFromTableByField("sup", "dirlist", "pc", "dirlist", cb->currentText());
    vl = dirpc.split(".");
    db = vl.at(0);
    tble = vl.at(1);
    QStringList dirColumns = sqlc.GetColumnsFromTable(db, tble);
    dirColumns.removeAll("id"+tble);
    dirColumns.removeAll("date");
    dirColumns.removeAll("deleted");
    dirColumns.removeAll("idpers");
    vl = sqlc.GetValuesFromTableByID(db, tble, dirColumns, "2");
    if (isImport)
    {
        int sm[MAX_COL_NUM];
        fl.clear();
        for (i = 0; i < vl.size(); i++)
        {
            for (j = 0; j < vl.size(); j++)
            {
                s_tqLabel *le = this->findChild<s_tqLabel *>("fieldl"+QString::number(j));
                if (le->text() == vl.at(i))
                {
                    s_tqComboBox *cb = this->findChild<s_tqComboBox *>("fieldcb"+QString::number(j));
                    QString tmpString = dirColumns.at(vl.indexOf(le->text()));
                    sm[i] = cb->currentIndex();
                    if (sm[i] > 0)
                    {
                        sm[i]--;
                        fl << tmpString;
                    }
                    break;
                }
            }
        }
        // sm[str]=i: str - имя поля в таблице db.tble, i - номер колонки в mainTV, которую надо в поле писать
        for (i = 0; i < mainmodel->rowCount(QModelIndex()); i++)
        {
            vl.clear();
            for (j = 0; j < fl.size(); j++)
                vl << mainmodel->data(mainmodel->index(i, sm[j], QModelIndex()), Qt::DisplayRole).toString();
            sqlc.InsertValuesToTable(db, tble, fl, vl);
            if (sqlc.result)
            {
                QMessageBox::warning(this, "warning!", "Ошибка записи в БД "+tble+"!");
                QApplication::restoreOverrideCursor();
                return;
            }
        }
    }
    else
    {
        QXlsx::Document xlsx;
        j = 10; // 'A'
        for (i = 0; i < vl.size(); i++)
        {
            tmpString = QString::number(j, 36).toUpper(); // from 'A' to 'Z'
            tmpString += QString::number(1, 10); // формируем номер ячейки
            xlsx.write(tmpString, vl.at(i));
            j++;
        }
        QSqlQuery get_db_contents(sqlc.GetDB(db));
        tmpString = "SELECT ";
        for (i = 0; i < dirColumns.size(); i++)
            tmpString += "`" + dirColumns.at(i) + "`,";
        tmpString = tmpString.left(tmpString.size()-1); // удаляем последнюю запятую
        tmpString += " FROM `" + tble + "` WHERE `deleted`=0 ORDER BY `id"+tble+"` ASC;";
        get_db_contents.exec(tmpString);
        int rowCount = 2;
        while (get_db_contents.next())
        {
            j = 10;
            for (i = 0; i < dirColumns.size(); i++)
            {
                tmpString = QString::number(j, 36).toUpper(); // from 'A' to 'Z'
                tmpString += QString::number(rowCount, 10); // формируем номер ячейки
                xlsx.write(tmpString, get_db_contents.value(i).toString());
                j++;
            }
            rowCount++;
        }
        xlsx.saveAs(filename.toUtf8());
    }
    QApplication::restoreOverrideCursor();
}

void sys_backuprestoredirdialog::CancelPBPressed()
{
    this->close();
}

void sys_backuprestoredirdialog::LoadFilePBPressed()
{
/*    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("loadFileLE");
    if (!isImport)
    {
        filename = QFileDialog::getSaveFileName(this,"Открыть файл","","Excel files (*.xlsx)");
        le->setText(filename);
        return;
    }
    filename = QFileDialog::getOpenFileName(this,"Открыть файл","","Excel files (*.xlsx)");
    le->setText(filename);
    QXlsx::Document xlsx(filename.toUtf8());
    QString tmpString, readString;
    if (mainmodel == (void*)0) delete mainmodel;
    mainmodel = new s_ncmodel;

    int i = 1;
    int j = 10; // 'A'

    QStringList fieldValuesStringList;

    readString = "12345"; // произвольный набор, чтобы только не пустая строка

    while (readString != "")
    {
        tmpString = QString::number(j, 36).toUpper(); // from 'A' to 'Z'
        tmpString += QString::number(i, 10); // формируем номер ячейки
        readString = xlsx.read(tmpString).toString();
        if (!readString.isEmpty())
            mainmodel->addColumn(readString, "");
        j++;
    }

    int jFinish = j-1;

    fieldValuesStringList.clear();

    bool Finished = false;

    for (i = 2; !Finished; i++)
    {
        Finished = true;
        fieldValuesStringList.clear();
        for (j = 10; j < jFinish; j++)
        {
            tmpString = QString::number(j, 36).toUpper() + QString::number(i, 10);
            readString = xlsx.read(tmpString).toString();
            if (!readString.isEmpty())
                Finished = false;
            fieldValuesStringList << readString;
        }
        if (!Finished)
            mainmodel->addRow(fieldValuesStringList);
    }
    TreeView *tv = this->findChild<TreeView *>("mainTV");
    tv->setModel(mainmodel);
    tv->setColumnWidth(0, 100);
    tv->resizeColumnsToContents();
    fieldValuesStringList.clear();
    for (i = 0; i < mainmodel->columnCount(QModelIndex()); i++)
        fieldValuesStringList << mainmodel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
    fM = new QStringListModel;
    fM->setStringList(fieldValuesStringList);
    fM->insertRow(0);
    fM->setData(fM->index(0), "", Qt::EditRole);
    for (i = 0; i < MAX_COL_NUM; i++)
    {
        s_tqComboBox *cb = this->findChild<s_tqComboBox *>("fieldcb"+QString::number(i));
        cb->setModel(fM);
    } */
}

void sys_backuprestoredirdialog::DirChoosed(QString str)
{
    if (isImport)
    {
        int i;
        QString dirpc = sqlc.GetValueFromTableByField("sup", "dirlist", "pc", "dirlist", str);
        QStringList vl = dirpc.split(".");
        QString db = vl.at(0);
        QString tble = vl.at(1);
        QStringList dirColumns = sqlc.GetColumnsFromTable(db, tble);
        dirColumns.removeAll("id"+tble);
        dirColumns.removeAll("date");
        dirColumns.removeAll("deleted");
        dirColumns.removeAll("idpers");
        vl = sqlc.GetValuesFromTableByID(db, tble, dirColumns, "2");
        for (i = 0; i < MAX_COL_NUM; i++)
        {
            s_tqComboBox *cb = this->findChild<s_tqComboBox *>("fieldcb"+QString::number(i));
            s_tqLabel *le = this->findChild<s_tqLabel *>("fieldl"+QString::number(i));
            if (i<vl.size())
            {
                le->setVisible(true);
                cb->setVisible(true);
                le->setText(vl.at(i));
            }
            else
            {
                le->setVisible(false);
                cb->setVisible(false);
            }
        }
    }
}
