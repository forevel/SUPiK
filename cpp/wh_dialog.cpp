#include <QShowEvent>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QApplication>
#include <QSizePolicy>
#include <QFileDialog>
#include <QHeaderView>
#include "../inc/s_tqcalendarwidget.h"

#include "../inc/wh_dialog.h"
#include "../inc/s_tqlabel.h"
#include "../inc/s_tqlineedit.h"
#include "../inc/s_tqpushbutton.h"
#include "../inc/s_tqtableview.h"
#include "../inc/publicclass.h"
#include "../inc/s_sql.h"
#include "../inc/s_duniversal.h"
#include "../inc/s_tablefilterdialog.h"
#include "../inc/s_sqlfieldsdialog.h"

wh_dialog::wh_dialog(bool isIncoming, QString id, QWidget *parent) :
    QDialog(parent)
{
    int res;
    setAttribute(Qt::WA_DeleteOnClose);
    mainmodel = new s_whitemmodel;
    bl1 = new QGridLayout;
    bl2 = new QGridLayout;
    mainbl = new QVBoxLayout;
    ml1 = new QHBoxLayout;
    ml2 = new QHBoxLayout;
    firstShow = true;
    mainTV = new s_tqTableView(true);
    this->isIncoming = isIncoming;
    needtorefresh = false;
    FlowFields.clear();
    if (res = SetupUI(id))
    {
        QMessageBox::warning(this,"warning!","Ошибка №" + QString::number(res)); // коды ошибок следующие:
        // 1 - ошибка считывания полей flowfields
        // 2 - ошибка получения столбцов из таблицы documents
        // 11 - не найден документ с заданным ИД
        // 12 - не найден поставщик в справочнике контрагентов при заполнении ордера
        // 13 - не найден получатель в справочнике контрагентов при заполнении ордера
        // 14 - не найдено основание в справочнике оснований
        // 15 - не найден создавший ордер в справочнике персонала
        // 16 - проблема с получением данных из таблицы flow
        // 17 - ошибка получения данных по таблице flowfields по типу 4 (FW_ALLINK)
        // 18 - ошибка получения данных по таблице flowfields по типу 3 (FW_DLINK)
        // 19 - не найдено расположение элемента на складе
        this->close();
    }
}

void wh_dialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/pic/Pic/WhWallpaper.jpg"));

    if (needtorefresh)
    {
        s_tqLabel *ole = new s_tqLabel;
        ole = this->findChild<s_tqLabel *>("overallLE");
        int sumnum;
        int i;
        for (i = 0; i < mainTV->horizontalHeader()->count(); i++)
        {
            if (mainmodel->headerData(i, Qt::Horizontal, Qt::DisplayRole) == "Стоимость")
            {
                sumnum = i;
                break;
            }
            sumnum = 0;
        }
        float summ = 0;
        for (i = 0; i < mainmodel->rowCount(); i++)
            summ += mainmodel->data(mainmodel->index(i, sumnum), Qt::DisplayRole).toFloat();
        ole->setText(QString::number(summ, 'f', 2));
        needtorefresh = false;
    }
    e->accept();
}

void wh_dialog::showEvent(QShowEvent *e)
{
    if (firstShow)
        firstShow = false;
    resizeMainTV(QModelIndex(), QModelIndex());
    e->accept();
}

int wh_dialog::SetupUI(QString id)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    int res;
    s_tqLabel *mainL = new s_tqLabel;
    QFont font;
    font.setPointSize(15);
    mainL->setFont(font);
    s_tqLineEdit *ordernum = new s_tqLineEdit;
    ordernum->setObjectName("orderNum");
    ordernum->setEnabled(false);
    s_tqLabel *dateL = new s_tqLabel;
    dateL->setText(" от ");
    s_tqLineEdit *dateLE = new s_tqLineEdit;
    dateLE->setObjectName("Date");
    dateLE->setEnabled(false);
    s_tqPushButton *datePB = new s_tqPushButton;
    datePB->setIcon(QIcon(":/pic/Pic/calend.png"));
    if (pc.access & 0x07)
    {
        connect(datePB, SIGNAL(clicked()), this, SLOT(chooseDate()));
        datePB->setEnabled(true);
    }
    else
        datePB->setEnabled(false);
    ml1->addWidget(mainL, 0);
    ml1->addWidget(ordernum);
    ml1->addWidget(dateL, 0);
    ml1->addWidget(dateLE);
    ml1->addWidget(datePB, 0);
    mainbl->addLayout(ml1);
    QHBoxLayout *authorLayout = new QHBoxLayout;
    s_tqPushButton *acceptPB = new s_tqPushButton;
    acceptPB->setText("Записать и закрыть");
    acceptPB->setIcon(QIcon(":/pic/Pic/icon_zap.png"));
    s_tqPushButton *cancelPB = new s_tqPushButton;
    cancelPB->setText("Отменить");
    cancelPB->setIcon(QIcon(":/pic/Pic/cross.png"));
    s_tqLabel *author = new s_tqLabel;
    author->setObjectName("Author");
    connect(acceptPB, SIGNAL(clicked()), this, SLOT(acceptandclose()));
    connect(cancelPB, SIGNAL(clicked()), this, SLOT(cancelled()));
    authorLayout->addWidget(acceptPB);
    authorLayout->addWidget(cancelPB);
    authorLayout->addStretch(300);
    authorLayout->addWidget(author, 0, Qt::AlignRight);
    mainbl->addLayout(authorLayout);
    s_tqLabel *supplierL = new s_tqLabel;
    supplierL->setText("Поставщик: ");
    s_tqLineEdit *supplierLE = new s_tqLineEdit;
    supplierLE->setObjectName("Supplier");
    supplierLE->setEnabled(false);
    s_tqPushButton *chooseSupplierPB = new s_tqPushButton;
    chooseSupplierPB->setText("...");
    connect(chooseSupplierPB, SIGNAL(clicked()), this, SLOT(chooseSupplier()));
    s_tqLabel *consumerL = new s_tqLabel;
    consumerL->setText("Получатель: ");
    s_tqLineEdit *consumerLE = new s_tqLineEdit;
    consumerLE->setObjectName("Consumer");
    consumerLE->setEnabled(false);
    s_tqPushButton *chooseConsumerPB = new s_tqPushButton;
    chooseConsumerPB->setText("...");
    connect(chooseConsumerPB, SIGNAL(clicked()), this, SLOT(chooseConsumer()));
    bl1->addWidget(supplierL, 0, 0, Qt::AlignLeft);
    bl1->addWidget(supplierLE, 0, 1);
    bl1->addWidget(chooseSupplierPB, 0, 2, Qt::AlignCenter);
    bl1->addWidget(consumerL, 1, 0, Qt::AlignLeft);
    bl1->addWidget(consumerLE, 1, 1);
    bl1->addWidget(chooseConsumerPB, 1, 2, Qt::AlignCenter);
    bl1->setColumnStretch(0, 0);
    bl1->setColumnStretch(1, 10);
    bl1->setColumnStretch(2, 0);
    ml2->addLayout(bl1);
    s_tqLabel *docNumL = new s_tqLabel("№ тов. накладной: ");
    s_tqLineEdit *docNumLE = new s_tqLineEdit;
    docNumLE->setObjectName("TNNum");
    docNumLE->setEnabled(true);
    s_tqLabel *TNNumL = new s_tqLabel("Путь: ");
    s_tqLineEdit *TNFileLE = new s_tqLineEdit;
    TNFileLE->setObjectName("TNFile");
    TNFileLE->setEnabled(false);
    s_tqPushButton *loadTNPB = new s_tqPushButton;
    loadTNPB->setText("...");
    loadTNPB->setToolTip("Указать путь к отсканированной ТН");
    connect(loadTNPB, SIGNAL(clicked()), this, SLOT(chooseTN()));
    s_tqPushButton *viewTNPB = new s_tqPushButton;
    viewTNPB->setIcon(QIcon(":/pic/Pic/TN.png"));
    viewTNPB->setToolTip("Просмотр отсканированной ТН");
    connect(viewTNPB, SIGNAL(clicked()), this, SLOT(viewTN()));
    s_tqLabel *reasonL = new s_tqLabel("Основание: ");;
    s_tqLineEdit *reasonLE = new s_tqLineEdit;
    reasonLE->setObjectName("Reason");
    reasonLE->setEnabled(false);
    s_tqPushButton *chooseReasonPB = new s_tqPushButton;
    chooseReasonPB->setText("...");
    connect(chooseReasonPB, SIGNAL(clicked()), this, SLOT(chooseReason()));
    bl2->addWidget(docNumL, 0, 0, Qt::AlignLeft);
    bl2->addWidget(docNumLE, 0, 1);
    bl2->addWidget(TNNumL, 0, 2);
    bl2->addWidget(TNFileLE, 0, 3);
    bl2->addWidget(loadTNPB, 0, 4, Qt::AlignCenter);
    bl2->addWidget(viewTNPB, 0, 5, Qt::AlignCenter);
    bl2->addWidget(reasonL, 1, 0, Qt::AlignLeft);
    bl2->addWidget(reasonLE, 1, 1, 1, 4);
    bl2->addWidget(chooseReasonPB, 1, 5, Qt::AlignCenter);
    bl2->setColumnStretch(0, 0);
    bl2->setColumnStretch(1, 10);
    bl2->setColumnStretch(2, 0);
    bl2->setColumnStretch(3, 10);
    bl2->setColumnStretch(4, 0);
    bl2->setColumnStretch(5, 0);
    ml2->addLayout(bl2);
    mainbl->addLayout(ml2);

    // формирование модели и заполнение таблицы
    QStringList sl, dlg;
    QSqlQuery get_flowfields(pc.sup);
    get_flowfields.exec("SELECT `flowfields`,`flowheaders`,`links`,`delegate` FROM `flowfields` WHERE `idflowfields`>2 ORDER BY `flowfieldsasc` ASC;");
    if (!get_flowfields.isActive())
        return 1;
    s_duniversal *universalDelegate = new s_duniversal;
    connect(universalDelegate, SIGNAL(commitData(QWidget*)), this, SLOT(CBChanged(QWidget*)));
    while (get_flowfields.next())
    {
        QString tmpString = get_flowfields.value(2).toString().split(".").at(0); // выцарапываем описание действия
        FTypes << tmpString.toInt();
        FlowFields << get_flowfields.value(0).toString();
        mainmodel->addColumn(get_flowfields.value(1).toString(), get_flowfields.value(2).toString());
        dlg << get_flowfields.value(3).toString();
    }
    mainTV->setModel(mainmodel);
    int scount = 0;
    int dcount = 0;
    int ccount = 0;
    int mcount = 0;
    int ecount = 0;
    for (int i = 0; i < dlg.size(); i++)
    {
        mainTV->setItemDelegateForColumn(i, universalDelegate);
        if (dlg.at(i) == "d")
        {
            dcount++;
        }
        if (dlg.at(i) == "e")
        {
            ecount++;
        }
        if (dlg.at(i) == "s")
        {
            scount++;
        }
        if (dlg.at(i) == "c")
        {
            ccount++;
        }
        if (dlg.at(i) == "m")
        {
            mcount++;
        }
        CTypes.insert(i, dlg.at(i));
    }
    // расчёт удельной ширины для каждого столбца
    int Ow = scount*SPIN + dcount*DIS + ccount*CB + mcount*MASK;
    if (Ow < (OVERALL-EDIT))
    {
        sw = SPIN;
        dw = DIS;
        cw = CB;
        mw = MASK;
        ew = (OVERALL-Ow) / ecount;
    }
    else
    {
        float k = Ow / (OVERALL-EDIT);
        sw = SPIN/k;
        dw = DIS/k;
        cw = CB/k;
        mw = MASK/k;
        ew = EDIT/ecount;
    }
    mainTV->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mainTV->verticalHeader()->setVisible(false);
    mainbl->addWidget(mainTV);

    s_tqLabel *overallL = new s_tqLabel("Итого: ");
    overallL->setObjectName("overallL");
    s_tqLabel *overallLE = new s_tqLabel;
    QFont fontB;
    fontB.setBold(true);
    overallLE->setFont(fontB);
    overallLE->setObjectName("overallLE");
    QHBoxLayout *overallLayout = new QHBoxLayout;
    overallLayout->addStretch(90);
    overallLayout->addWidget(overallL);
    overallLayout->addWidget(overallLE, 5);
    mainbl->addLayout(overallLayout);
    setLayout(mainbl);

    if (id != "") // если документ для редактирования
    {
        if (res = fillFlow(id))
        {
            QApplication::restoreOverrideCursor();
            return res;
        }
    }
    else
    {
        id = QString::number(sqlc.getnextfreeindex(pc.ent, "documents"), 10);
        if (res = fillNullFlow())
        {
            QApplication::restoreOverrideCursor();
            return res;
        }
    }
    ordernum->setText(id);
    if (isIncoming)
        mainL->setText("Приходный ордер №");
    else
        mainL->setText("Расходный ордер №");
    updateDialog();
    connect(mainmodel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(resizeMainTV(QModelIndex,QModelIndex)));
    QApplication::restoreOverrideCursor();
    return 0;
}

void wh_dialog::updateDialog()
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("Supplier");
    le->setText(Supplier);
    le = this->findChild<s_tqLineEdit *>("Consumer");
    le->setText(Consumer);
    le = this->findChild<s_tqLineEdit *>("TNNum");
    le->setText(DocNum);
    le = this->findChild<s_tqLineEdit *>("Reason");
    le->setText(Reason);
}

void wh_dialog::resizeMainTV(QModelIndex index1, QModelIndex index2)
{
    Q_UNUSED(index1);
    Q_UNUSED(index2);
    QStringList sl;
    if (!mainmodel->checkforEmptyRows())
    {
        for (int i = 0; i < mainmodel->columnCount(QModelIndex()); i++)
            sl << "";
        mainmodel->addRow(sl);
    }
    int i;
    float k = this->geometry().width()-20;
    k = k / 1000;
    float s1 = sw*k;
    float s2 = dw*k;
    float s3 = cw*k;
    float s4 = mw*k;
    float s5 = ew*k;
    for (i = 0; i < mainTV->horizontalHeader()->count(); i++)
    {
        if (CTypes.at(i) == "s") mainTV->setColumnWidth(i, s1);
        if (CTypes.at(i) == "d") mainTV->setColumnWidth(i, s2);
        if (CTypes.at(i) == "c") mainTV->setColumnWidth(i, s3);
        if (CTypes.at(i) == "m") mainTV->setColumnWidth(i, s4);
        if (CTypes.at(i) == "e") mainTV->setColumnWidth(i, s5);
    }
    needtorefresh = true;
}

void wh_dialog::chooseConsumer()
{
    QSqlDatabase cdb(pc.ent);
    QString ctble = "contractors";
    s_tablefilterdialog *chooseDialog = new s_tablefilterdialog;
    QStringList tmpStringList;
    tmpStringList = sqlc.getvaluesfromtablebycolumn(cdb, ctble, ctble);
    chooseDialog->SetupUI(tmpStringList, Consumer);
    connect(chooseDialog, SIGNAL(accepted(QString)), this, SLOT(consumerChoosed(QString)));
    chooseDialog->exec();
}

void wh_dialog::consumerChoosed(QString str)
{
    Consumer = str;
    updateDialog();
}

void wh_dialog::chooseSupplier()
{
    QSqlDatabase cdb(pc.ent);
    QString ctble = "contractors";
    s_tablefilterdialog *chooseDialog = new s_tablefilterdialog;
    QStringList tmpStringList;
    tmpStringList = sqlc.getvaluesfromtablebycolumn(cdb, ctble, ctble);
    chooseDialog->SetupUI(tmpStringList, Supplier);
    connect(chooseDialog, SIGNAL(accepted(QString)), this, SLOT(supplierChoosed(QString)));
    chooseDialog->exec();
}

void wh_dialog::supplierChoosed(QString str)
{
    Supplier = str;
    updateDialog();
}

void wh_dialog::chooseReason()
{
    QSqlDatabase cdb(pc.ent);
    QString ctble = "reasons";
    s_tablefilterdialog *chooseDialog = new s_tablefilterdialog;
    QStringList tmpStringList;
    tmpStringList = sqlc.getvaluesfromtablebycolumn(cdb, ctble, ctble);
    chooseDialog->SetupUI(tmpStringList, Reason);
    connect(chooseDialog, SIGNAL(accepted(QString)), this, SLOT(reasonChoosed(QString)));
    chooseDialog->exec();
}

void wh_dialog::reasonChoosed(QString str)
{
    Reason = str;
    updateDialog();
}

void wh_dialog::chooseTN()
{
    QString tmpString;
    if ((tmpString = QFileDialog::getOpenFileName(this, "Открыть файл с ТН", pc.PathToSup+"//TN", "PDF Files (*.pdf)")) != "")
    {
        DocNum = tmpString;
        updateDialog();
    }
}

void wh_dialog::resizeEvent(QResizeEvent *e)
{
    resizeMainTV(QModelIndex(), QModelIndex());
    e->accept();
}

void wh_dialog::viewTN()
{
}

void wh_dialog::cancelled()
{
    if (SomethingChanged)
    {
        QMessageBox tmpMB;
        tmpMB.setText("Были внесены изменения");
        tmpMB.setInformativeText("Записать их?");
        QPushButton *tmpOkPB = tmpMB.addButton("Ага", QMessageBox::YesRole);
        QPushButton *tmpNoPB = tmpMB.addButton("Неа", QMessageBox::NoRole);
        QPushButton *tmpCancelPB = tmpMB.addButton("Отмена", QMessageBox::RejectRole);
        tmpMB.exec();
        if (tmpMB.clickedButton() == tmpOkPB)
            acceptandclose();
        else if (tmpMB.clickedButton() == tmpNoPB)
            this->close();
        else if (tmpMB.clickedButton() == tmpCancelPB)
            return;
    }
    this->close();
}

void wh_dialog::acceptandclose()
{
    QString newID;
    QString tmpString, tmpValue;
    QStringList fl, vl, tmpsl;
    int i, j;
    // проверка введённых данных
    if (Supplier.isEmpty())
    {
        QMessageBox::warning(this, "warning!", "Не заполнено поле \"Поставщик\"!");
        return;
    }
    if (Consumer.isEmpty())
    {
        QMessageBox::warning(this, "warning!", "Не заполнено поле \"Получатель\"!");
        return;
    }
    if (Reason.isEmpty())
    {
        QMessageBox::warning(this, "warning!", "Должно быть выбрано основание прихода/расхода!");
        return;
    }
    if ((DocNum.isEmpty()) && (Reason == "Изготовление") || (Reason == "Закупка"))
    {
        QMessageBox::warning(this, "warning!", "Введите номер ТН!");
        return;
    }
    if ((isIncoming) && (Reason != "Закупка") && (Reason != "Ввод остатков") && (Reason != "Приём в ремонт"))
    {
        QMessageBox::warning(this, "warning!", "Основание не соответствует направлению движения!");
        return;
    }
    if ((!isIncoming) && (Reason != "Изготовление") && (Reason != "Из ремонта"))
    {
        QMessageBox::warning(this, "warning!", "Основание не соответствует направлению движения!");
        return;
    }
    fl << "date" << "idsupplier" << "idcustomer" << "reason" << "documents" << "scanpath" << "direction" << "value" << "idpers";
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("Date");
    vl << le->text();
    tmpString = sqlc.getvaluefromtablebyfield(pc.ent, "contractors", "idcontractors", "contractors", Supplier);
    vl << tmpString;
    tmpString = sqlc.getvaluefromtablebyfield(pc.ent, "contractors", "idcontractors", "contractors", Consumer);
    vl << tmpString;
    tmpString = sqlc.getvaluefromtablebyfield(pc.ent, "reasons", "idreasons", "reasons", Reason);
    if (DocNum.isEmpty())
    {
        le = this->findChild<s_tqLineEdit *>("orderNum");
        DocNum = le->text();
    }
    vl << tmpString << DocNum << ScanPath;
    tmpString = (isIncoming) ? "i" : "o";
    vl << tmpString;
    s_tqLabel *ll = this->findChild<s_tqLabel *>("overallLE");
    vl << ll->text() << QString::number(pc.idPers);
    newID = sqlc.insertvaluestotable(pc.ent, "documents", fl, vl); // номер нового ордера - в newID
    if (sqlc.result)
    {
        QMessageBox::warning(this, "warning!", "Ошибка добавления ордера в БД!");
        return;
    }
    for (j = 0; j < mainmodel->rowCount(QModelIndex())-1; j++)
    {
        fl.clear();
        vl.clear();
        fl << "flow";
        vl << newID;
        for (i = 0; i < FlowFields.size(); i++)
        {
            if (FlowFields.at(i) != "-")
            {
                tmpString = mainmodel->data(mainmodel->index(0, i, QModelIndex()), Qt::UserRole).toString(); // выцарапываем links.at(i)
                tmpsl = tmpString.split(".");
                tmpValue = mainmodel->data(mainmodel->index(j, i), Qt::DisplayRole).toString(); // выцарапываем само значение
                switch (FTypes.at(i))
                {
                case FW_ALLINK:
                {
                    fl << "id"+tmpsl.at(2);
                    tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb(tmpsl.at(1)), tmpsl.at(2), "id"+tmpsl.at(3), tmpsl.at(3), tmpValue);
                    if (tmpString.isEmpty())
                    {
                        QMessageBox::warning(this, "warning!", "Ошибка в таблице flowfields по полю" + FlowFields.at(i));
                        return;
                    }
                    vl << tmpString;
                    break;
                }
                case FW_AUTONUM:
                case FW_EQUAT:
                case FW_NUMBER:
                case FW_MASKED:
                default: // если тип поля неопределён (как для, например, spinboxdelegate), то по умолчанию пишем его в поле
                {
                    fl << FlowFields.at(i);
                    vl << tmpValue;
                    break;
                }
                case FW_DLINK:
                {
                    tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb(tmpsl.at(3)), tmpsl.at(4), "id"+tmpsl.at(4), tmpsl.at(4), tmpValue);
                    if (tmpString.isEmpty()) // нет во второй таблице такого элемента, поищем в первой
                    {
                        tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb(tmpsl.at(1)), tmpsl.at(2), "id"+tmpsl.at(2), tmpsl.at(2), tmpValue);
                        if (tmpString.isEmpty()) // нет такого значения ни в одной из таблиц
                        {
                            QMessageBox::warning(this, "warning!", "Нет такого элемента номенклатуры, как в строке № " + QString::number(j+1) + "!");
                            return;
                        }
                        else // во второй таблице он всё-таки нашёлся
                        {
                            fl << "id"+tmpsl.at(2);
                            vl << tmpString;
                        }
                    }
                    else
                    {
                        fl << "id"+tmpsl.at(4);
                        vl << tmpString;
                    }
                    break;
                }
                case FW_LINK:
                {
                    fl << "id"+tmpsl.at(2);
                    tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb(tmpsl.at(1)), tmpsl.at(2), "id"+tmpsl.at(2), tmpsl.at(3), tmpValue);
                    if (tmpString.isEmpty())
                    {
                        QMessageBox::warning(this, "warning!", "Ошибка в таблице flowfields по полю" + FlowFields.at(i));
                        return;
                    }
                    vl << tmpString;
                    break;
                }
                case FW_MAXLINK:
                {
                    fl << FlowFields.at(i);
                    bool ok;
                    int tmpInt = tmpsl.at(4).toInt(&ok);
                    if (ok) // в поз. 4 находится число => ссылка на ячейку таблицы ордера
                    {
                        tmpString = sqlc.getlastvaluefromtablebyfield(sqlc.getdb(tmpsl.at(1)), tmpsl.at(2), "id"+tmpsl.at(2), tmpsl.at(3),\
                                                                  mainmodel->data(mainmodel->index(j, tmpInt), Qt::DisplayRole).toString());
                        if (tmpString.isEmpty())
                        {
                            QMessageBox::warning(this, "warning!", "Ошибка в таблице flowfields по полю" + FlowFields.at(i));
                            return;
                        }
                    }
                    else // в поз. 4 находится имя поля из таблицы tmpsl.at(1).tmpsl.at(2).
                    {
                        tmpString = sqlc.getlastvaluefromtablebyfield(sqlc.getdb(tmpsl.at(1)), tmpsl.at(2), "id"+tmpsl.at(2), tmpsl.at(3),\
                                                                  tmpsl.at(4));
                        if (tmpString.isEmpty())
                        {
                            QMessageBox::warning(this, "warning!", "Ошибка в таблице flowfields по полю" + FlowFields.at(i));
                            return;
                        }
                    }
                    vl << tmpString;
                    break;
                }
                }
            }
        }
        // fl ~ "idwh"
        // vl ~ <idwh>
        QSqlQuery get_whs(pc.ent);
        int whidx = fl.indexOf("idwh"); // в таблице flowfields может и не быть поля links=ent.wh.wh
        if (whidx != -1)
        {
            tmpString = "SELECT `idnkwh` FROM `nkwh` WHERE `idnk`=\""+vl.at(fl.indexOf("idnk"))+"\""
                    " AND `idwh`=\""+vl.at(whidx)+"\";";
            get_whs.exec(tmpString);
            get_whs.next();
            if (get_whs.isValid()) // есть такое расположение компонентов на складе
                vl.replace(whidx, get_whs.value(0).toString()); // пишем индекс расположения во flow
            else // нет записи в nkwh о такой позиции на таком складе => необх. задать её местоположение
            {
                QStringList tmpfl, tmpvl;
                tmpfl << "nkwh"; // для совместимости вызовов к таблицам
                tmpfl << "idnk" << "idwh" << "rack" << "box" << "cell" << "date" << "deleted";
                tmpvl << vl.at(fl.indexOf("idnk")); // для совместимости вызовов к таблицам
                tmpvl << vl.at(fl.indexOf("idnk")) << \
                      vl.at(whidx) << "" << "" << "" << pc.DateTime << "0";
                tmpString = sqlc.insertvaluestotable(pc.ent, "nkwh", tmpfl, tmpvl);
                if (!sqlc.result)
                {
                    s_sqlfieldsdialog *tmpDialog = new s_sqlfieldsdialog;
                    if (!tmpDialog->SetupUI(pc.ent, "nkwh", tmpString, "Добавление расположения"))
                    {
                        vl.replace(whidx, tmpString);
                        tmpDialog->exec();
                    }
                    else
                    {
                        QMessageBox::warning(this, "warning!", "Ошибка при создании диалога!");
                        return;
                    }
                }
                else
                {
                    QMessageBox::warning(this, "warning!", "Ошибка добавления в таблицу nkwh!");
                    return;
                }
            }
            fl.replace(fl.indexOf("idwh"), "idnkwh");
        }
        sqlc.insertvaluestotable(pc.ent, "flow", fl, vl);
        if (sqlc.result)
        {
            QMessageBox::warning(this, "warning!", "Ошибка добавления данных в таблицу flow!");
            return;
        }
    }
    this->close();
}

int wh_dialog::fillFlow(QString id)
{
    QString newID;
    QString tmpString, tmpValue;
    QStringList fl, vl, tmpsl;
    int i, j;
    fl << "date" << "idsupplier" << "idcustomer" << "reason" << "documents" << "scanpath" << "direction" << "idpers";
    vl = sqlc.getvaluesfromtablebyfield(pc.ent, "documents", fl, "iddocuments", id);
    if (sqlc.result)
        return 11;
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("Date");
    le->setText(vl.at(0));
    tmpString = sqlc.getvaluefromtablebyfield(pc.ent, "contractors", "contractors", "idcontractors", vl.at(1));
    if (sqlc.result)
        return 12;
    Supplier = tmpString;
    tmpString = sqlc.getvaluefromtablebyfield(pc.ent, "contractors", "contractors", "idcontractors", vl.at(2));
    if (sqlc.result)
        return 13;
    Consumer = tmpString;
    tmpString = sqlc.getvaluefromtablebyfield(pc.ent, "reasons", "reasons", "idreasons", vl.at(3));
    if (sqlc.result)
        return 14;
    Reason = tmpString;
    DocNum = vl.at(4);
    ScanPath = vl.at(5);
    isIncoming = (vl.at(6)=="i")?true:false;
    s_tqLabel *ll = this->findChild<s_tqLabel *>("Author");
    tmpString = sqlc.getvaluefromtablebyid(pc.sup, "personel", "personel", vl.at(7));
    if (sqlc.result)
        return 15;
    ll->setText("Автор: " + tmpString);
    for (i = 0; i < mainmodel->columnCount(QModelIndex()); i++)
        tmpsl << "";
    fl.clear();
    for (i = 0; i < FlowFields.size(); i++)
    {
        if (FlowFields.at(i) != "-")
        {
            tmpString = mainmodel->data(mainmodel->index(0, i, QModelIndex()), Qt::UserRole).toString(); // выцарапываем links.at(i)
            tmpsl = tmpString.split(".");
            switch (FTypes.at(i))
            {
            case FW_ALLINK:
            case FW_LINK:
            case FW_MAXLINK:
            {
                fl << "id"+tmpsl.at(2);
                break;
            }
            case FW_DLINK:
            {
                fl << "id"+tmpsl.at(2) << "id"+tmpsl.at(4);
                break;
            }
            case FW_AUTONUM:
            case FW_EQUAT:
            case FW_NUMBER:
            case FW_MASKED:
            default:
            {
                fl << FlowFields.at(i);
            }
            }
        }
    }
    QList<QStringList> tmpslsl = sqlc.getmorevaluesfromtablebyfield(pc.ent, "flow", fl, "flow", id);
    if (sqlc.result)
        return 16;
    for (j = 0; j < mainmodel->rowCount(QModelIndex())-1; j++)
    {
        for (i = 0; i < FlowFields.size(); i++)
        {
            tmpString = mainmodel->data(mainmodel->index(0, i, QModelIndex()), Qt::UserRole).toString(); // выцарапываем links.at(i)
            tmpsl = tmpString.split(".");
            if (FlowFields.at(i) != "-")
            {
                tmpValue = tmpslsl.at(i).at(j);
                switch (FTypes.at(i))
                {
                case FW_ALLINK:
                case FW_LINK:
                case FW_MAXLINK:
                {
                    tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb(tmpsl.at(1)), tmpsl.at(2), tmpsl.at(3), "id"+tmpsl.at(2), tmpValue);
                    if (sqlc.result)
                        return 17;
                    mainmodel->setData(mainmodel->index(j, i, QModelIndex()), QVariant(tmpString), Qt::EditRole);
                    break;
                }
                case FW_NUMBER:
                {
                    mainmodel->setData(mainmodel->index(j, i, QModelIndex()), QVariant(tmpsl.at(1)), Qt::EditRole);
                }
                case FW_AUTONUM:
                case FW_EQUAT:
                case FW_MASKED:
                default: // если тип поля неопределён (как для, например, spinboxdelegate), то по умолчанию пишем его в поле
                {
                    mainmodel->setData(mainmodel->index(j, i, QModelIndex()), QVariant(tmpValue), Qt::EditRole);
                    break;
                }
                case FW_DLINK:
                {
                    tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb(tmpsl.at(3)), tmpsl.at(4), tmpsl.at(4), "id"+tmpsl.at(4), tmpValue);
                    if (tmpString.isEmpty()) // нет во второй таблице такого элемента, поищем в первой
                    {
                        tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb(tmpsl.at(1)), tmpsl.at(2), tmpsl.at(2), "id"+tmpsl.at(2), tmpValue);
                        if (tmpString.isEmpty()) // нет такого значения ни в одной из таблиц
                            return 18;
                    }
                    mainmodel->setData(mainmodel->index(j, i, QModelIndex()), QVariant(tmpString), Qt::EditRole);
                    break;
                }
                }
            }
        }
        int whidx = fl.indexOf("idwh"); // в таблице flowfields может и не быть поля links=ent.wh.wh
        if (whidx != -1)
        {
            tmpValue = tmpslsl.at(whidx).at(j); // выгрызаем ИД записи в nkwh
            tmpValue = sqlc.getvaluefromtablebyfield(pc.ent, "nkwh", "idwh", "idnkwh", tmpValue);
            if (sqlc.result)
                return 19;
            tmpValue = sqlc.getvaluefromtablebyfield(pc.ent, "wh", "wh", "idwh", tmpValue);
            if (sqlc.result)
                return 20;
            mainmodel->setData(mainmodel->index(j, whidx, QModelIndex()), QVariant(tmpValue), Qt::EditRole);
        }
    }
    return 0;
}

int wh_dialog::fillNullFlow()
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("Date");
    le->setText(pc.Date);
    s_tqLabel *ll = this->findChild<s_tqLabel *>("Author");
    ll->setText("Автор: " + pc.Pers);
    DocNum = "";
    Reason = "";
    ScanPath = "";
    QStringList sl;
    int i;
    sl.clear();
    for (i = 0; i < mainmodel->columnCount(QModelIndex()); i++)
        sl << "";
    mainmodel->addRow(sl);
    return 0;
}

void wh_dialog::chooseDate()
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("Date");
    QDate dte;
    QString tmpString = le->text();
    dte = QDate::fromString(le->text(), "dd/MM/yyyy");
    s_tqCalendarWidget *calWdgt = new s_tqCalendarWidget;
    QPoint pt;
    pt = this->cursor().pos();
    calWdgt->move(pt);
    calWdgt->setSelectedDate(dte);
    connect(calWdgt, SIGNAL(activated(QDate)), this, SLOT(dateChoosed(QDate)));
    calWdgt->show();
}

void wh_dialog::dateChoosed(QDate dte)
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("Date");
    le->setText(dte.toString("dd/MM/yyyy"));
}

void wh_dialog::CBChanged(QWidget *wdgt)
{
    QComboBox *cb = static_cast<QComboBox *>(wdgt);
    if (mainmodel->headerData(mainTV->currentIndex().column(), Qt::Horizontal, Qt::DisplayRole) == "Валюта")
    {
        int tmpInt = mainmodel->getHeaderPosition("Курс", Qt::Horizontal, Qt::DisplayRole);
        QString tmpString = sqlc.getlastvaluefromtablebyfield(pc.ent, "periodic", "value", "periodic", \
                                                              cb->currentText());
        mainmodel->setData(mainmodel->index(mainTV->currentIndex().row(), tmpInt, QModelIndex()), tmpString, Qt::EditRole);
    }
}
