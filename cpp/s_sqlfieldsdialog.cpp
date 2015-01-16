#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSqlQuery>
#include <QPointer>

#include "../inc/s_sqlfieldsdialog.h"
#include "../inc/publicclass.h"
#include "../inc/s_sql.h"

s_sqlfieldsdialog::s_sqlfieldsdialog(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet("QDialog {background-color: rgba(204,204,153);}");
    //setWindowOpacity(0.9);
    GridLayout = new QGridLayout;
    setLayout(GridLayout);
    connect (this, SIGNAL(rejected()), this, SLOT(CancelPBPressed()));
}

// заполнение диалога из таблицы SQL. Если id=0, то необходимо создать новый элемент. В hdr - заголовок диалога

int s_sqlfieldsdialog::SetupUI(QSqlDatabase db, QString tble, QString id, QString hdr)
{
    QString tmpString;
    bool isEnabled;
    int i, idindex;

    setAttribute(Qt::WA_DeleteOnClose);
    this->db = db;
    this->tble = tble;
    this->id = id;

    sl = sqlc.getcolumnsfromtable(db, tble);
    if (sl.isEmpty())
        return 1; // ошибка обращения к таблице (нет таблицы)

// убираем лишние имена столбцов
    sl.removeAll("date");
    sl.removeAll("idpers");
    sl.removeAll("deleted");

// если нет поля а-ля idsyslist, значит, таблица неполноценная, и работать с ней нельзя (невозможно получить запись №1 о соответствии полей таблицы их реальным наименованиям)
    if (sl.contains("id" + tble));
    else
        return 2; // таблица не содержит столбца ИД

    if (id != "0") // если требуется изменить элемент
    {
        vl = sqlc.getvaluesfromtablebyid(db, tble, sl, id);
        if (vl.isEmpty())
            return 3; // проблемы с получением данных по нужному id
    }
    else
    {
        for (i = 0; i < sl.size(); i++)
            vl << "";
        vl.replace(sl.indexOf("id"+tble), QString::number(sqlc.getnextfreeindex(db, tble)));
    }

    // собираем данные по полям
    fl = sqlc.getvaluesfromtablebyid(db, tble, sl, "2");
    if (fl.isEmpty())
        return 4; // проблемы с получением данных по наименованиям полей
    ll = sqlc.getvaluesfromtablebyid(db, tble, sl, "1"); // получаем ссылки на другие таблицы
    if (ll.isEmpty())
        return 5; // проблемы с получением данных

    // в fl - наименования полей
    // в ll - ссылки на другие таблицы
    // в vl - значения полей

    idindex = fl.indexOf("2");
    fl.operator [](idindex) = "ИД";

    OkPB = new s_tqPushButton;
    CancelPB = new s_tqPushButton;
    OkPB->setText("Ага");
    CancelPB->setText("Неа");
    connect (OkPB, SIGNAL(clicked()), this, SLOT(OkPBPressed()));
    connect (CancelPB, SIGNAL(clicked()), this, SLOT(CancelPBPressed()));
    s_tqLabel *MainL = new s_tqLabel;
    MainL->setText(hdr);
    QFont font;
    font.setPointSize(15);
    MainL->setFont(font);

    GridLayout->addWidget(MainL, 0, 0, 1, 3);
    GridLayout->setAlignment(MainL, Qt::AlignRight);
    for (i = 0; i < ll.size(); i++)
    {
        tmpString = ll.at(i);
        if (tmpString.left(1) == "x")
            ftypes.insert(i, FT_RIGHTS);
        else if (tmpString.left(1) == "m")
            ftypes.insert(i, FT_METHOD);
        else if (tmpString.left(1) == ".")
            ftypes.insert(i, FT_INDIRECT);
        else if (tmpString.left(3) == "pc.")
        {
            if (tmpString.size() < 4)
                ftypes.insert(i, FT_EMLINK);
            else if (tmpString.size() < 8)
                ftypes.insert(i, FT_TBLINK);
            else
                ftypes.insert(i, FT_LINK);
        }
        else
            ftypes.insert(i, FT_PLAIN);
        switch (ftypes.at(i))
        {
        case FT_PLAIN:
        {
            fdb.insert(i, "");
            ftble.insert(i, "");
            isEnabled = (i == idindex) ? false : true; // ИД менять нельзя
            addLineEdit(fl.at(i), vl.at(i), i, *GridLayout, i+1, isEnabled);
            continue;
        }
        case FT_TBLINK:
        {
            fdb.insert(i, tmpString.mid(3, 3));
            ftble.insert(i, "");
            break;
        }
        case FT_LINK:
        {
            fdb.insert(i, tmpString.mid(3, 3));
            ftble.insert(i, tmpString.right(tmpString.size()-7));
            break;
        }
        case FT_METHOD:
        case FT_RIGHTS:
        case FT_EMLINK:
        {
            fdb.insert(i, "");
            ftble.insert(i, "");
            break;
        }
        case FT_INDIRECT:
        {
            // сначала берём ссылку из текущей таблицы
            tmpString = tmpString.right(tmpString.size()-1); // убираем точку, получаем имя столбца в текущей таблице
            int idx = sl.indexOf(tmpString); // позиция столбца, на который ссылается текущий столбец
            tmpString = ll.at(idx); // получаем ссылку по ссылочному столбцу
            QString tmpdb = tmpString.mid(3, 3);
            QString tmptble = tmpString.right(tmpString.size()-7);
            tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb(tmpdb), tmptble, sl.at(i).right(sl.at(i).size()-2), "id"+tmptble, vl.at(idx)); // взяли имя таблицы в БД <sl.at(i).right(sl.at(i).size()-2)>
/*            if (tmpString.isEmpty())
            {
                res = 50;
                return res;
            } // ссылка на компонент вполне может быть пустой */
            fdb.insert(i, sl.at(i).right(sl.at(i).size()-2)); // взяли три буквы БД из наименования столбца (idalt-id=alt)
            ftble.insert(i, tmpString);
            break;
        }
        default:
            break;
        }
        isEnabled = true;
        int res = addPushButtonWithDialog(vl.at(i), i, *GridLayout);
        if (res) return res;
    }
    GridLayout->addWidget(OkPB, i+1, 0);
    GridLayout->addWidget(CancelPB, i+1, 1);
    GridLayout->setAlignment(OkPB, Qt::AlignHCenter);
    GridLayout->setAlignment(CancelPB, Qt::AlignHCenter);
    GridLayout->setColumnStretch(0, 40);
    GridLayout->setColumnStretch(1, 55);
    GridLayout->setColumnStretch(2, 5);
    setLayout(GridLayout);
    setModal(true);
    return 0;
}

void s_sqlfieldsdialog::addLineEdit(QString lbl, QString ledt, int idx, QGridLayout &lyout, int pos, bool isEnabled)
{
    s_tqLabel *tmpLabel = new s_tqLabel(lbl);
    tmpLabel->adjustSize();
    lyout.addWidget(tmpLabel, pos, 0);
    s_tqLineEdit *tmpLineEdit = new s_tqLineEdit(ledt);
    tmpLineEdit->adjustSize();
    tmpLineEdit->setEnabled(isEnabled);
    tmpLineEdit->setAData(idx);
    connect (tmpLineEdit, SIGNAL(textChanged(QString, s_tqLineEdit *)), this, SLOT(LineEditChanged(QString, s_tqLineEdit *)));
    lyout.addWidget(tmpLineEdit, pos, 1, 1, 2);
}

int s_sqlfieldsdialog::addPushButtonWithDialog(QString ledt, int idx, QGridLayout &lyout)
{
    int pos = idx+1;
    int res;
    QString tmpString;
    tmpString = fl.at(idx);
    s_tqLabel *tmpLabel = new s_tqLabel(tmpString);
    tmpLabel->adjustSize();
    lyout.addWidget(tmpLabel, pos, 0);
    switch (ftypes.at(idx))
    {
    case FT_METHOD:
    case FT_RIGHTS:
    case FT_EMLINK:
    case FT_TBLINK:
    {
        tmpString = ledt;
        break;
    }
    case FT_LINK:
    case FT_INDIRECT:
    {
        res = sqlc.getfullpathtochild(sqlc.getdb(fdb.at(idx)), ftble.at(idx), ledt, tmpString);
        if (res) return res;
        break;
    }
    default:
        break;
    }
    s_tqLineEdit *tmpLineEdit = new s_tqLineEdit(tmpString);
    tmpLineEdit->adjustSize();
    tmpLineEdit->setEnabled(false);
    tmpLineEdit->setAData(idx);
    lyout.addWidget(tmpLineEdit, pos, 1);
    s_tqPushButton *tmpPushButton = new s_tqPushButton(QString("..."));
    tmpPushButton->setAData(idx);
    connect (tmpPushButton, SIGNAL(clicked(s_tqPushButton*)), this, SLOT(PushButtonPressed(s_tqPushButton*)));
    lyout.addWidget(tmpPushButton, pos, 2);
    return 0;
}

/*int s_sqlfieldsdialog::addComboBox(QString lbl, QString ledt, int idx, QGridLayout &lyout, int pos, bool isEnabled)
{
    int j;
    QString tmpString;
    QStringList tmpsl1, tmpsl2;
    int res = getLinkedTable (lbl.mid(3, lbl.size()), tmpString, tmpsl1, tmpsl2);
    if (res) return res;
    QStringList tmpsl = sqlc.getvaluesfromtablebyid(db, tble, sl, "1");
    tmpString = tmpsl.at(fl.indexOf(lbl));
    s_tqLabel *tmpLabel = new s_tqLabel(tmpString);
    s_tqComboBox *tmpCombobox = new s_tqComboBox;
    for (j = 0; j < tmpsl1.size(); j++)
        tmpCombobox->addItem(tmpsl2.at(j), tmpsl1.at(j));
    tmpCombobox->setCurrentIndex(tmpCombobox->findData(ledt));
    tmpCombobox->setEnabled(isEnabled);
    tmpCombobox->setAData(idx);
    connect (tmpCombobox, SIGNAL(textChanged(QString,s_tqComboBox*)), this, SLOT(ComboBoxChanged(QString,s_tqComboBox*)));
    lyout.addWidget(tmpLabel, pos, 0);
    lyout.addWidget(tmpCombobox, pos, 1);
    return 0;
}*/

void s_sqlfieldsdialog::OkPBPressed()
{
    if (isSomethingChanged)
    {
        id = QString::number(id.toInt(0));
        int ididx = sl.indexOf("id"+tble);
        sl.removeAt(ididx);
        vl.removeAt(ididx);
        sl.append("idpers");
        vl.append(QString::number(pc.idPers));
        if (id != "0")
        {
            if (int res = sqlc.updatevaluesintable(db, tble, sl, vl, "id" + tble, id))
                QMessageBox::information(this,"Ошибка при изменении!",\
                                     QString::number(res),\
                                     QMessageBox::Ok, QMessageBox::NoButton);
            else
                QMessageBox::information(this,"Success!",\
                                     "Обновление успешно",\
                                     QMessageBox::Ok, QMessageBox::NoButton);
        }
        else
        {
            if (tble == "nk") tble = "qnk";
            sqlc.insertvaluestotable(db, tble, sl, vl);
            if (sqlc.result)
                QMessageBox::information(this,"Ошибка при добавлении!",\
                                     QString::number(sqlc.result),\
                                     QMessageBox::Ok, QMessageBox::NoButton);
            else
                QMessageBox::information(this,"Success!",\
                                     "Добавление успешно",\
                                     QMessageBox::Ok, QMessageBox::NoButton);
        }
        emit newidaccepted(id.toInt(0));
        emit accepted();
    }
    this->close();
}

void s_sqlfieldsdialog::CancelPBPressed()
{
    if (isSomethingChanged)
    {
        QMessageBox tmpMB;
        tmpMB.setText("Были внесены изменения");
        tmpMB.setInformativeText("Записать их?");
        QPushButton *tmpOkPB = tmpMB.addButton("Ага", QMessageBox::AcceptRole);
        QPushButton *tmpCancelPB = tmpMB.addButton("Неа", QMessageBox::RejectRole);
        tmpMB.exec();
        if (tmpMB.clickedButton() == tmpOkPB)
            OkPBPressed();
        else if (tmpMB.clickedButton() == tmpCancelPB)
            this->close();
    }
    this->close();
}

/*void s_sqlfieldsdialog::ComboBoxChanged(QString str, s_tqComboBox *ptr)
{
    vl.replace(ptr->getAData().toInt(), str);
    isSomethingChanged = true;
} */

// обработчик нажатия на кнопку выбора элемента из дерева, на которое ссылается idalias элемента
// в ptr - указатель на кнопку, ptr.adata = индекс (номер строки) в GridLayout
// в sl.at(ptr.adata) находится имя поля с idalias

void s_sqlfieldsdialog::PushButtonPressed(s_tqPushButton *ptr)
{
    QString tmpString;
    idx = ptr->getAData().toInt(0);
    switch (ftypes.at(idx))
    {
    case FT_METHOD: // обработка процедур supik-а
    {
        sl1.clear();
        sl1.append(pc.supikprocs);
        tabledialog = new s_tablefilterdialog;
        tabledialog->SetupUI(sl1, vl.at(idx));
        connect(tabledialog, SIGNAL(accepted(QString)), this, SLOT(acceptChanges(QString)));
        tabledialog->exec();
        break;
    }
    case FT_RIGHTS: // обработка прав доступа
    {
        accessdialog = new s_accessdialog;
        accessdialog->SetupUI(vl.at(idx).toLongLong(0, 16));
        connect(accessdialog, SIGNAL(acceptChanges(long)), this, SLOT(acceptChanges(long)));
        accessdialog->exec();
        break;
    }
    case FT_EMLINK:
    {
        tmpString = vl.at(idx).right(vl.at(idx).size()-4); // выцарапываем наименование таблицы
        sqlc.prepareslsfortree(sl1, sl2);
        tabledialog = new s_tablefilterdialog;
        tabledialog->SetupUI(sl1, sl2, tmpString);
        connect(tabledialog, SIGNAL(accepted(QString)), this, SLOT(acceptChanges(QString)));
        tabledialog->exec();
        break;
    }
    case FT_LINK:
    {
        vl.replace(idx, QString("%1").arg(vl.at(idx).toInt(0), 7, 10, QChar('0')));
        sqlTreeDialog = new s_sqltreedialog;
        sqlTreeDialog->SetupUI(sqlc.getdb(fdb.at(idx)), ftble.at(idx), vl.at(idx));
        connect(sqlTreeDialog, SIGNAL(changeshasbeenMade(QString)), this, SLOT(acceptChanges(QString)));
        sqlTreeDialog->exec();
        break;
    }
    case FT_INDIRECT:
    {
        tabledialog = new s_tablefilterdialog;
        QList<QStringList> tmpsl;
        sl1 = sqlc.getvaluesfromtablebycolumn(sqlc.getdb(fdb.at(idx)), ftble.at(idx), "id"+ftble.at(idx));
        tmpsl.append(sl1);
        sl1 = sqlc.getvaluesfromtablebycolumn(sqlc.getdb(fdb.at(idx)), ftble.at(idx), ftble.at(idx));
        tmpsl.append(sl1);
        tabledialog->SetupUI(tmpsl, vl.at(idx));
        connect(tabledialog, SIGNAL(accepted(QString)), this, SLOT(acceptChanges(QString)));
        tabledialog->exec();
        break;
    }
    case FT_TBLINK:
    {
        tabledialog = new s_tablefilterdialog;
        QList<QStringList> tmpsl;
        sl1 = sqlc.gettablesfromdb(sqlc.getdb(fdb.at(idx)));
        tmpsl.append(sl1);
        tabledialog->SetupUI(tmpsl, vl.at(idx));
        connect(tabledialog, SIGNAL(accepted(QString)), this, SLOT(acceptChanges(QString)));
        tabledialog->exec();
        break;
    }
    default:
        break;
    }
}

// обработка подтверждения в диалоге accessrights

void s_sqlfieldsdialog::acceptChanges(long rights)
{
    isSomethingChanged = true;
    vl.replace(idx, QString::number(rights, 16));
    dynamic_cast<s_tqLineEdit *>(GridLayout->itemAtPosition(idx+1, 1)->widget())->setText(vl.at(idx));
}

// обработка подтверждения в диалоге sqltree - запись нового пути по ссылке в соотв. поле

void s_sqlfieldsdialog::acceptChanges(QString str)
{
    QString tmpString;
    int i, j;

    isSomethingChanged = true;
    switch (ftypes.at(idx))
    {
    case FT_METHOD:
    case FT_TBLINK:
    {
        vl.replace(idx, str);
        dynamic_cast<s_tqLineEdit *>(GridLayout->itemAtPosition(idx+1, 1)->widget())->setText(vl.at(idx));
        break;
    }
    case FT_EMLINK:
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
            tmpString = sl1.at(j).left(3)+"."+str; // компонуем имя таблицы в формат <имя_БД>.<имя_таблицы>
            vl.replace(idx, tmpString);
            dynamic_cast<s_tqLineEdit *>(GridLayout->itemAtPosition(idx+1, 1)->widget())->setText(vl.at(idx));
        }
        break;
    }
    case FT_LINK:
    case FT_INDIRECT:
    {
        str = QString::number(str.toInt(0)); // убрать незначащие нули
        vl.replace(idx, str);
        int res = sqlc.getfullpathtochild(sqlc.getdb(fdb.at(idx)), ftble.at(idx), vl.at(idx), tmpString);
        if (res) return;
        dynamic_cast<s_tqLineEdit *>(GridLayout->itemAtPosition(idx+1, 1)->widget())->setText(tmpString);
        int tmpInt = ll.indexOf("."+sl.at(idx)); // затрагивает ли изменение данного поля какие-то другие столбцы, которые на это поле ссылаются?
        while (tmpInt != -1) // да, затрагивает
        {
            QString tmpString = ll.at(idx); // получаем ссылку по ссылочному столбцу
            QString tmpdb = tmpString.mid(3, 3);
            QString tmptble = tmpString.right(tmpString.size()-7);
            tmpString = sqlc.getvaluefromtablebyfield(sqlc.getdb(tmpdb), tmptble, sl.at(tmpInt).right(sl.at(tmpInt).size()-2), "id"+tmptble, str);
            if (tmpString.isEmpty())
                return;
            fdb.replace(tmpInt, sl.at(idx).right(sl.at(idx).size()-2)); // взяли три буквы БД из наименования столбца (idalt-id=alt)
            ftble.replace(tmpInt, tmpString);
            vl.replace(tmpInt, "0"); // во избежание недоразумений обнуляем индекс элемента
            tmpInt = ll.indexOf("."+sl.at(idx), idx+1); // ищем следующее совпадение
        }
        break;
    }
    default:
        break;
    }
}

void s_sqlfieldsdialog::LineEditChanged(QString str, s_tqLineEdit *ptr)
{
    vl.replace(ptr->getAData().toInt(), str);
    isSomethingChanged = true;
}

void s_sqlfieldsdialog::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void s_sqlfieldsdialog::showEvent(QShowEvent *event)
{
    isSomethingChanged = false;
    event->accept();
}
