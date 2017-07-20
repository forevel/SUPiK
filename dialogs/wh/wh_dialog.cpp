#include <QDebug>
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
#include <QComboBox>
#include "../../widgets/s_tqcalendarwidget.h"

#include "wh_dialog.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_sql.h"
#include "../../models/s_duniversal.h"
#include "../gen/twocoldialog.h"

wh_dialog::wh_dialog(int Reason, QString id, QWidget *parent) :
    QDialog(parent)
{
    int res;
    setAttribute(Qt::WA_DeleteOnClose);
    if (Reason > R_SIZE)
    {
        WARNMSG("");
        this->close();
    }
    mainmodel = new TreeModel;
    bl1 = new QGridLayout;
    bl2 = new QGridLayout;
    mainbl = new QVBoxLayout;
    ml1 = new QHBoxLayout;
    ml2 = new QHBoxLayout;
    firstShow = true;
    mainTV = new TreeView;
    this->Reason = Reason;
    needtorefresh = false;
    ReasonTable << "Ввод остатков_полн" << "Приём на склад_полн" << "Приём на склад_полн";
    MainText << "Приходный ордер №" << "Приходный ордер №" << "Расходный ордер №";
    FlowFields.clear();
    if (res = SetupUI(id))
    {
        WARNMSG("");
        this->close();
    }
}

void wh_dialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/WhWallpaper.jpg"));

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
    int i, res;
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
    datePB->setIcon(QIcon(":/res/calend.png"));
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
    acceptPB->setIcon(QIcon(":/res/icon_zap.png"));
    s_tqPushButton *cancelPB = new s_tqPushButton;
    cancelPB->setText("Отменить");
    cancelPB->setIcon(QIcon(":/res/cross.png"));
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
    viewTNPB->setIcon(QIcon(":/res/TN.png"));
    viewTNPB->setToolTip("Просмотр отсканированной ТН");
    connect(viewTNPB, SIGNAL(clicked()), this, SLOT(viewTN()));
    s_tqLabel *reasonL = new s_tqLabel("Основание: ");;
    s_tqLineEdit *reasonLE = new s_tqLineEdit;
    reasonLE->setObjectName("Reason");
    reasonLE->setEnabled(false);
    s_tqPushButton *chooseReasonPB = new s_tqPushButton;
    chooseReasonPB->setText("...");
    connect(chooseReasonPB, SIGNAL(clicked()), this, SLOT(chooseReason()));
    s_duniversal *universalDelegate = new s_duniversal;
    connect(universalDelegate,SIGNAL(error(int,int)),this,SIGNAL(error(int,int)));
    connect(universalDelegate, SIGNAL(commitData(QWidget*)), this, SLOT(CBChanged(QWidget*)));
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
    QStringList fl = QStringList() << "header" << "links";
    QList<QStringList> lsl;
    lsl = sqlc.GetMoreValuesFromTableByFields("sup", "tablefields", fl, QStringList("tablename"), QStringList(ReasonTable.at(Reason)), "fieldsorder", true);
    if (sqlc.result)
        return 0x01;
    int delegates[DELEGNUM];
    for (i = 0; i < DELEGNUM; i++)
        delegates[i] = 0;
    for (i = 0; i < lsl.size(); i++)
    {
/*        mainmodel->setcolumnlinks(i, lsl.at(i).at(1));
        int dlg = lsl.at(i).at(1).split(".").at(0).toInt();
        if ((dlg >= 0) && (dlg < DELEGNUM))
            delegates[dlg]++;
        mainmodel->addColumn(lsl.at(i).at(0)); */
    }
    mainTV->setModel(mainmodel);
    // расчёт удельной ширины для каждого столбца
    int Ow = delegates[FD_SPIN]*W_SPIN + delegates[FD_DISABLED]*W_DIS + delegates[FD_COMBO]*W_COMBO + delegates[FD_LINEEDIT]*W_LINEEDIT + delegates[FD_CHOOSE]*W_LINKS;
    widths[FD_SPIN] = W_SPIN;
    widths[FD_DISABLED] = W_DIS;
    widths[FD_COMBO] = W_COMBO;
    widths[FD_LINEEDIT] = W_LINEEDIT;
    widths[FD_CHOOSE] = W_LINKS;
    if (Ow > W_OVERALL)
    {
        float k = Ow / W_OVERALL;
        for (i = 0; i < W_SIZE; i++)
            widths[i] /= k;
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
            return res;
    }
    else
    {
        id = QString::number(sqlc.GetNextFreeIndex("ent", "documents"), 10);
        if (res = fillNullFlow())
            return res;
    }
    ordernum->setText(id);
    mainL->setText(MainText[Reason]);
    updateDialog();
    connect(mainmodel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(resizeMainTV(QModelIndex,QModelIndex)));
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
}

void wh_dialog::resizeMainTV(QModelIndex index1, QModelIndex index2)
{
    Q_UNUSED(index1);
    Q_UNUSED(index2);
/*    float realwidths[W_SIZE];
    QStringList sl;
    if (!mainmodel->checkforEmptyRows())
        mainmodel->addRow();
    int i;
    float k = this->geometry().width()-20;
    k = k / 1000;
    for (i = 0; i < W_SIZE; i++)
        realwidths[i] = k * widths[i];
    for (i = 0; i < mainTV->horizontalHeader()->count(); i++)
    {
        int links = mainmodel->getCellType(0, i).toInt();
        mainTV->setColumnWidth(i, realwidths[links]);
    }
    needtorefresh = true; */
}

void wh_dialog::chooseConsumer()
{
    TwoColDialog *dlg = new TwoColDialog("Выбор контрагента");
    if (dlg->setup("Контрагенты_сокращ", MODE_CHOOSE, Consumer) != RESULTOK)
        return;
    connect(dlg, SIGNAL(changeshasbeenMade(QString)), this, SLOT(consumerChoosed(QString)));
    dlg->exec();
}

void wh_dialog::consumerChoosed(QString str)
{
    Consumer = str;
    updateDialog();
}

void wh_dialog::chooseSupplier()
{
    TwoColDialog *dlg = new TwoColDialog("Выбор контрагента");
    if (dlg->setup("Контрагенты_сокращ", MODE_CHOOSE, Supplier) != RESULTOK)
        return;
    connect(dlg, SIGNAL(changeshasbeenMade(QString)), this, SLOT(supplierChoosed(QString)));
    dlg->exec();
}

void wh_dialog::supplierChoosed(QString str)
{
    Supplier = str;
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
    try
    {
        QString newID;
        QString tmpString, tmpValue;
        QStringList tmpsl1, tmpsl2;
        QStringList fl, vl, tmpsl;
        s_tqLineEdit *le;
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
        if ((DocNum.isEmpty()) && (Reason == R_BUY) || (Reason == R_SELL))
        {
            QMessageBox::warning(this, "warning!", "Введите номер ТН!");
            return;
        }
        if (DocNum.isEmpty())
        {
            le = this->findChild<s_tqLineEdit *>("orderNum");
            if (le == 0)
                throw 0x24;
            DocNum = le->text();
        }

        // пишем параметры ордера в таблицу documents
        fl << "date" << "idsupplier" << "idcustomer" << "reason" << "documents" << "scanpath" << "direction" << "value" << "idpers";
        le = this->findChild<s_tqLineEdit *>("Date");
        if (le == 0)
            throw 0x21;
        vl << le->text();
        tmpString = sqlc.GetValueFromTableByField("ent", "contractors", "idcontractors", "contractors", Supplier);
        if (sqlc.result)
            throw 0x22;
        vl << tmpString;
        tmpString = sqlc.GetValueFromTableByField("ent", "contractors", "idcontractors", "contractors", Consumer);
        if (sqlc.result)
            throw 0x23;
        vl << tmpString;
        vl << QString::number(Reason) << DocNum << ScanPath;
        if ((Reason == R_BUY) || (Reason == R_IREPAIR))
            tmpString = "i";
        else if ((Reason == R_SELL) || (Reason == R_OREPAIR))
            tmpString = "o";
        else
            tmpString = "";
        vl << tmpString;
        s_tqLabel *ll = this->findChild<s_tqLabel *>("overallLE");
        if (ll == 0)
            throw 0x25;
        vl << ll->text() << QString::number(pc.idPers);
        newID = sqlc.GetValueFromTableByField("ent", "documents", "iddocuments", "documents", DocNum);
        if (!sqlc.result) // есть такая запись, значит, её надо удалить
        {
            QMessageBox tmpMB;
            tmpMB.setText("Для данного ордера в базе есть запись №" + newID + "");
            tmpMB.setInformativeText("Удалить её и создать новую?");
            QPushButton *tmpOkPB = tmpMB.addButton("Ага", QMessageBox::YesRole);
            QPushButton *tmpNoPB = tmpMB.addButton("Неа", QMessageBox::NoRole);
            tmpMB.exec();
            if (tmpMB.clickedButton() == tmpOkPB)
            {
                tmpsl1 << "deleted";
                tmpsl2 << "1";
                sqlc.UpdateValuesInTable("ent", "documents", tmpsl1, tmpsl2, "documents", newID);
                if (sqlc.result)
                    throw 0x26;
            }
            else if (tmpMB.clickedButton() == tmpNoPB)
                return;
        }
        newID = sqlc.InsertValuesToTable("ent", "documents", fl, vl); // номер нового ордера - в newID
        if (sqlc.result)
            throw 0x27;

        // пишем теперь таблицу flow
        QStringList values, headers;
        QString tble = ReasonTable.at(Reason);
        for (j = 0; j < mainmodel->columnCount(QModelIndex()); j++)
            headers << mainmodel->headerData(j, Qt::Horizontal, Qt::DisplayRole).toString();
        int whidx = headers.indexOf("Склад");
        if (whidx == -1)
            throw 0x28;
        int nkidx = headers.indexOf("Наименование");
        if (nkidx == -1)
            throw 0x29;
        for (j = 0; j < mainmodel->rowCount(QModelIndex()); j++)
        {
            values.clear();
            for (i = 0; i < headers.size(); i++)
                values << mainmodel->data(mainmodel->index(j, i), Qt::DisplayRole).toString(); // выцарапываем само значение
            tmpString = values.at(whidx);
            QString tmpString2 = values.at(nkidx);
            tmpString2 = sqlc.GetValueFromTableByField("ent", "nk", "idnk", "nk", tmpString2); // берём idnk
            tmpString = sqlc.GetValueFromTableByField("ent", "wh", "idwh", "wh", tmpString); // берём idwh
            tmpsl1 = QStringList() << "idnk" << "idwh"; // поля, по которым ищем запись idnkwh
            tmpsl2 = QStringList() << tmpString2 << tmpString;
            tmpString = sqlc.GetValueFromTableByFields("ent", "nkwh", "idnkwh", tmpsl1, tmpsl2);
            if (sqlc.result == 1) // нет такой записи
            {
                tmpString = sqlc.InsertValuesToTable("ent", "nkwh", tmpsl1, tmpsl2);
                if (sqlc.result)
                    throw 0x2b;
                TwoColDialog *dlg = new TwoColDialog("");
                if (dlg->setup("Расположение на складе_полн", MODE_EDIT, tmpString) != RESULTOK)
                    throw 0x2c;
                dlg->exec();
            }
            else if (sqlc.result > 1) // ошибка
                throw 0x2a;
            values.replace(whidx, tmpString);
//            sqlc.bytablefieldsinsert(tble, headers, values);
        }
        this->close();
    }
    catch (int res)
    {
        qDebug() << res;
    }
}

// ДОШЁЛ ПОКА ДОСЮДА

int wh_dialog::fillFlow(QString id)
{
    QString newID;
    QString tmpString, tmpValue;
    QStringList fl, vl, tmpsl;
    int i, j;
    fl << "date" << "idsupplier" << "idcustomer" << "reason" << "documents" << "scanpath" << "direction" << "idpers";
    vl = sqlc.GetValuesFromTableByField("ent", "documents", fl, "iddocuments", id);
    if (sqlc.result)
        return 11;
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("Date");
    le->setText(vl.at(0));
    tmpString = sqlc.GetValueFromTableByField("ent", "contractors", "contractors", "idcontractors", vl.at(1));
    if (sqlc.result)
        return 12;
    Supplier = tmpString;
    tmpString = sqlc.GetValueFromTableByField("ent", "contractors", "contractors", "idcontractors", vl.at(2));
    if (sqlc.result)
        return 13;
    Consumer = tmpString;
    tmpString = sqlc.GetValueFromTableByField("ent", "reasons", "reasons", "idreasons", vl.at(3));
    if (sqlc.result)
        return 14;
    DocNum = vl.at(4);
    ScanPath = vl.at(5);
    s_tqLabel *ll = this->findChild<s_tqLabel *>("Author");
    tmpString = sqlc.GetValueFromTableByID("sup", "personel", "personel", vl.at(7));
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
    QList<QStringList> tmpslsl = sqlc.GetMoreValuesFromTableByFields("ent", "flow", fl, QStringList("flow"), QStringList(id));
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
                    tmpString = sqlc.GetValueFromTableByField(tmpsl.at(1), tmpsl.at(2), tmpsl.at(3), "id"+tmpsl.at(2), tmpValue);
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
                    tmpString = sqlc.GetValueFromTableByField(tmpsl.at(3), tmpsl.at(4), tmpsl.at(4), "id"+tmpsl.at(4), tmpValue);
                    if (tmpString.isEmpty()) // нет во второй таблице такого элемента, поищем в первой
                    {
                        tmpString = sqlc.GetValueFromTableByField(tmpsl.at(1), tmpsl.at(2), tmpsl.at(2), "id"+tmpsl.at(2), tmpValue);
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
            tmpValue = sqlc.GetValueFromTableByField("ent", "nkwh", "idwh", "idnkwh", tmpValue);
            if (sqlc.result)
                return 19;
            tmpValue = sqlc.GetValueFromTableByField("ent", "wh", "wh", "idwh", tmpValue);
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
    le->setText(pc.DateTime.left(10));
    s_tqLabel *ll = this->findChild<s_tqLabel *>("Author");
    ll->setText("Автор: " + pc.Pers);
    DocNum = "";
    ScanPath = "";
//    mainmodel->addRow();
    return 0;
}

void wh_dialog::chooseDate()
{
    s_tqLineEdit *le = this->findChild<s_tqLineEdit *>("Date");
    QDate dte;
    QString tmpString = le->text();
    dte = QDate::fromString(le->text(), "dd-MM-yyyy");
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
    Q_UNUSED(wdgt);
    // таблицы periodic больше нет! Пользоваться таблицей [Валюты движения]
/*    QComboBox *cb = static_cast<QComboBox *>(wdgt);
    if (mainmodel->headerData(mainTV->currentIndex().column(), Qt::Horizontal, Qt::DisplayRole) == "Валюта")
    {
        int tmpInt = mainmodel->getHeaderPosition("Курс", Qt::Horizontal, Qt::DisplayRole);
        QString tmpString = sqlc.GetLastValueFromTableByField("ent", "periodic", "value", "periodic", \
                                                              cb->currentText());
        mainmodel->setData(mainmodel->index(mainTV->currentIndex().row(), tmpInt, QModelIndex()), QVariant(tmpString), Qt::EditRole);
    } */
}

// процедура возвращает tmpfl.at(0)=<db>.<tble> и tmpfl.at(1)=<column>

QStringList wh_dialog::getTableNFields(QString tablename, QString headers)
{
    QStringList tmpfl = QStringList() << "table" << "tablefields";
    QStringList cmpfl = QStringList() << "tablename" << "headers";
    QStringList cmpvl = QStringList() << tablename << headers;
    tmpfl = sqlc.GetValuesFromTableByFields("sup", "tablefields", tmpfl, cmpfl, cmpvl); // получили tmpfl.at(0) = <db>.<tble> и  tmpfl.at(1) = <column>, с которыми надо сравнивать tmpValue
    return tmpfl;
}
