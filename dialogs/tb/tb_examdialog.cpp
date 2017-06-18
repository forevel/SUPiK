#include "tb_examdialog.h"
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QGridLayout>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include <QScrollBar>
#include <QThread>
#include <stdlib.h>
#include "../../widgets/s_tqscrollarea.h"
#include "../../models/proxymodel.h"
#include "../../models/s_duniversal.h"
#include "../../widgets/s_tqgroupbox.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqradiobutton.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../../widgets/s_tqtextedit.h"
#include "../../widgets/s_tqwidget.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/s_tqlinkswidget.h"
#include "../../widgets/goodbadwidget.h"
#include "../../widgets/waitwidget.h"
#include "../../widgets/clevertimer.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../../gen/pdfout.h"
#include "../gen/messagebox.h"
#include "../../gen/client.h"

// --------------------------------------
// Конструктор
// --------------------------------------

tb_examdialog::tb_examdialog(QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    TBGroup = 0;
    ExType = EXTYPE_TEST;
    SetupUI();
}

// --------------------------------------
// Деструктор
// --------------------------------------

tb_examdialog::~tb_examdialog()
{
}

void tb_examdialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/TBWallpaper.png"));

    event->accept();
}

// Настройка интерфейса

void tb_examdialog::SetupUI()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);
    hlyout->addStretch(160);
    CleverTimer *Tmr = new CleverTimer;
    Tmr->SetDirection(CleverTimer::COUNT_DOWN);
    Tmr->Init(CleverTimer::TIMER_SIMPLIM);
    Tmr->SetBoundaries(0, TB_EX_TIME*60000); // 10 mins
    Tmr->SetFont(FONT_BOLD | FONT_ITALIC, 16);
    Tmr->SetThreshold(60000); // 60 sec
    connect(this,SIGNAL(destroyed(QObject*)),Tmr,SLOT(deleteLater()));
    connect(this,SIGNAL(StartCleverTimer()),Tmr,SLOT(Start()));
    connect(this,SIGNAL(StopCleverTimer()),Tmr,SLOT(Stop()));
    connect(Tmr,SIGNAL(Finished()),this,SLOT(ProcessResultsAndExit()));
    hlyout->addWidget(Tmr);
    hlyout->addStretch(100);
    s_tqLabel *lbl = new s_tqLabel("Экзамен ОТ и ТБ");
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    hlyout->addWidget(lbl, 0);
    hlyout->setAlignment(lbl, Qt::AlignRight);
    lyout->addLayout(hlyout);

    s_tqStackedWidget *stw = new s_tqStackedWidget;
    stw->setObjectName("groupstw");
    s_tqGroupBox *gb = new s_tqGroupBox;
    gb->setTitle("Выбор раздела");
    QVBoxLayout *vlyout = new QVBoxLayout;
    hlyout = new QHBoxLayout;
    hlyout->addStretch(100);
    lbl = new s_tqLabel("Вы хотите пройти экзамен или пока потренироваться?");
    hlyout->addWidget(lbl);
    hlyout->addStretch(20);
    QButtonGroup *gbg1 = new QButtonGroup;
    s_tqRadioButton *rb = new s_tqRadioButton;
    rb->setText("Хочу экзамен");
    connect(rb,SIGNAL(clicked(bool)),this,SLOT(ExTypeExChoosed()));
    hlyout->addWidget(rb);
    gbg1->addButton(rb);
    rb = new s_tqRadioButton;
    rb->setText("Хочу потренироваться");
    connect(rb,SIGNAL(clicked(bool)),this,SLOT(ExTypeTestChoosed()));
    hlyout->addWidget(rb);
    gbg1->addButton(rb);
    hlyout->addStretch(100);
    vlyout->addLayout(hlyout);
    hlyout = new QHBoxLayout;
    QButtonGroup *gbg2 = new QButtonGroup;
    hlyout->addStretch(150);
    lbl = new s_tqLabel("Выберите группу ТБ, по которой будете проходить тест:");
    hlyout->addWidget(lbl);
    hlyout->addStretch(20);
    rb = new s_tqRadioButton("III");
    rb->setObjectName("rb3");
    hlyout->addWidget(rb);
    gbg2->addButton(rb);
    rb = new s_tqRadioButton("IV");
    rb->setObjectName("rb4");
    hlyout->addWidget(rb);
    gbg2->addButton(rb);
    rb = new s_tqRadioButton("V");
    rb->setObjectName("rb5");
    rb->setEnabled(false);
    hlyout->addWidget(rb);
    gbg2->addButton(rb);
    hlyout->addStretch(20);
    pb = new s_tqPushButton("Далее");
    connect(pb,SIGNAL(clicked(bool)),this,SLOT(GroupChoosed()));
    hlyout->addWidget(pb);
    hlyout->addStretch(150);
    vlyout->addLayout(hlyout);
    gb->setLayout(vlyout);
    stw->addWidget(gb);
    lyout->addWidget(stw, 10);

    stw = new s_tqStackedWidget;
    stw->setObjectName("questionstw");
    gb = new s_tqGroupBox;
    gb->setObjectName("questiongb");
    gb->setTitle("Вопрос");
    s_tqScrollArea *sa = new s_tqScrollArea;
    sa->setFrameShape(QFrame::NoFrame);
    sa->setWidgetResizable(true);
    sa->setWidget(stw);
    vlyout = new QVBoxLayout;
    vlyout->addWidget(sa);
    gb->setLayout(vlyout);
//    gb->setVisible(false);
//    stw->addWidget(gb);
    lyout->addWidget(gb, 75);

    gb = new s_tqGroupBox;
    gb->setObjectName("goodbadgb");
    gb->setTitle("Результаты ответов");
    vlyout = new QVBoxLayout;
    GoodBadWidget *gbw = new GoodBadWidget;
    gbw->setObjectName("gbw");
    if (!gbw->SetItemsCount(TB_QUESTNUM))
    {
        WARNMSG("");
        return;
    }
    vlyout->addWidget(gbw);
    gb->setLayout(vlyout);
    gb->setFixedHeight(70);
    lyout->addWidget(gb);

    gb = new s_tqGroupBox;
    gb->setTitle("Комментарий");
//    gb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    vlyout = new QVBoxLayout;
    gb->setObjectName("commentgb");
    stw = new s_tqStackedWidget;
    stw->setObjectName("commentstw");
    s_tqWidget *w = new s_tqWidget;
    stw->addWidget(w);
    stw->setCurrentIndex(0);
    vlyout->addWidget(stw);
    gb->setLayout(vlyout);
    lyout->addWidget(gb, 20);

    setLayout(lyout);
    QApplication::restoreOverrideCursor();
    gbw->SetItem(0, GoodBadTableModel::GBIT_NEUTRAL); // принудительный ресайз
    gbw->Resize();
}

void tb_examdialog::ExTypeExChoosed()
{
    ExType = EXTYPE_EX;
}

void tb_examdialog::ExTypeTestChoosed()
{
    ExType = EXTYPE_TEST;
}

void tb_examdialog::GroupChoosed()
{
    WaitWidget *ww = new WaitWidget;
    ww->Start();
    ww->SetMessage("Подготовка...");
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("groupstw");
    if (stw == 0)
    {
        DBGMSG;
        ww->Stop();
        return;
    }
    s_tqRadioButton *rb3 = this->findChild<s_tqRadioButton *>("rb3");
    s_tqRadioButton *rb4 = this->findChild<s_tqRadioButton *>("rb4");
    s_tqRadioButton *rb5 = this->findChild<s_tqRadioButton *>("rb5");
    if ((rb3 == 0) || (rb4 == 0) || (rb5 == 0))
    {
        DBGMSG;
        ww->Stop();
        return;
    }
    QString tmps;
    if (rb3->isChecked())
    {
        tmps = "III";
        TBGroup = 3;
    }
    if (rb4->isChecked())
    {
        tmps = "IV";
        TBGroup = 4;
    }
    if (rb5->isChecked())
    {
        tmps = "V";
        TBGroup = 5;
    }
    if (tmps.isEmpty()) // не выбрана ни одна группа
    {
        MessageBox2::information(this,"Предупреждение","Чтобы продолжить, требуется выбрать одну из групп");
        ww->Stop();
        return;
    }
    s_tqLabel *lbl = new s_tqLabel("Тестирование на знание правил по электробезопасности, охране труда группы " + tmps);
    QFont font;
    font.setPointSize(15);
    lbl->setFont(font);
    QVBoxLayout *vlyout = new QVBoxLayout;
    s_tqWidget *w = new s_tqWidget;
    vlyout->addWidget(lbl);
    vlyout->setAlignment(lbl, Qt::AlignCenter);
    w->setLayout(vlyout);
    stw->addWidget(w);
    stw->setCurrentIndex(1);
    if (!PrepareQuestions())
    {
        WARNMSG("");
        ww->Stop();
        return;
    }
    s_tqGroupBox *questiongb = this->findChild<s_tqGroupBox *>("questiongb");
    s_tqGroupBox *goodbadgb = this->findChild<s_tqGroupBox *>("goodbadgb");
    if ((questiongb == 0) || (goodbadgb == 0))
    {
        DBGMSG;
        ww->Stop();
        return;
    }
    questiongb->setVisible(true);
    goodbadgb->setVisible(true);
    ww->Stop();
}

bool tb_examdialog::PrepareQuestions()
{
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("questionstw");
    if (stw == 0)
    {
        DBGMSG;
        return false;
    }
    stw->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    Questions.clear();
    Answers.clear();
    AnswerCount = 0; // номер текущего ответа
    RightAnswerCount = 0;
    QList<s_tqScrollArea *> tmplw = PrepareQuestionsByTheme(TB_TH_PTE, TB_PTENUM);
    while (tmplw.size() != 0)
        stw->addWidget(tmplw.takeFirst());
    tmplw = PrepareQuestionsByTheme(TB_TH_PUE, TB_PUENUM);
    while (tmplw.size() != 0)
        stw->addWidget(tmplw.takeFirst());
    tmplw = PrepareQuestionsByTheme(TB_TH_POT, TB_POTNUM);
    while (tmplw.size() != 0)
        stw->addWidget(tmplw.takeFirst());
    tmplw = PrepareQuestionsByTheme(TB_TH_SIZ, TB_SIZNUM);
    while (tmplw.size() != 0)
        stw->addWidget(tmplw.takeFirst());
    tmplw = PrepareQuestionsByTheme(TB_TH_POM, TB_POMNUM);
    while (tmplw.size() != 0)
        stw->addWidget(tmplw.takeFirst());
    stw->setCurrentIndex(0);
    emit StartCleverTimer();
    return true;
}

QList<s_tqScrollArea *> tb_examdialog::PrepareQuestionsByTheme(int theme, int questnum)
{
    QList<s_tqScrollArea *> wdgts;

    if (TBGroup == 0)
    {
        WARNMSG("");
        return wdgts;
    }
    QStringList mainfl = QStringList() << "Содержание вопроса" << "Ответ 1" << "Ответ 2" << "Ответ 3" << "Ответ 4" << "Ответ 5" << "Ответ 6" << \
                                      "Номер правильного ответа" << "Комментарий";
    QStringList cmpfl = QStringList() << "Группа ТБ" << "Раздел вопроса";
    QStringList cmpvl = QStringList() << QString::number(TBGroup) << QString::number(theme);
    QString tble = "Экзамен ТБ_полн";
    QString field = "ИД";
    QStringList idvl;
    tfl.GetValuesByColumnAndFields(tble, field, cmpfl, cmpvl, idvl);
    if (tfl.result == TFRESULT_ERROR)
    {
        WARNMSG("");
        return wdgts;
    }
    // from qt fortune server example
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime())); // инициализация генератора случайных чисел
    for (int i=0; i<questnum; ++i)
    {
        s_tqWidget *w = new s_tqWidget;
        QVBoxLayout *lyout = new QVBoxLayout;
        int idsize = idvl.size();
        if (idsize == 0) // protection against fools
            break;
        int index = qrand()%idsize; // числа от 0 по (idsize-1)
        QString AnotherID;
        if (index < idvl.size())
             AnotherID = idvl.takeAt(index);
        else
            continue;
        QStringList mainvl;
        QString table = "Экзамен ТБ_полн";
        QString field = "ИД";
        tfl.valuesbyfield(table, mainfl, field, AnotherID, mainvl);
        if ((tfl.result == TFRESULT_ERROR) || (mainvl.size() < 9))
        {
            WARNMSG("");
            return wdgts;
        }
        QString tmps = mainvl.at(0);
        s_tqLabel *lbl = new s_tqLabel(tmps); // содержание вопроса
        QFont font;
        font.setPointSize(12);
        lbl->setFont(font);
        lbl->setWordWrap(true);
        lyout->addWidget(lbl);
        for (int j=1; j<7; ++j)
        {
            if (!mainvl.at(j).isEmpty())
            {
                QHBoxLayout *hlyout = new QHBoxLayout;
                s_tqRadioButton *rb = new s_tqRadioButton(""); // ответ
                rb->setObjectName(QString::number(j));
                connect(rb,SIGNAL(clicked(bool)),this,SLOT(AnswerChoosed()));
                hlyout->addWidget(rb);
                s_tqLinksWidget *lw = new s_tqLinksWidget("9.22..0.5", mainvl.at(j)); // fw_pixte/tb/photo
                hlyout->addWidget(lw, 100);
                lyout->addLayout(hlyout);
            }
        }
        Questions_s quest;
        quest.RightAnswer = mainvl.at(7).toInt();
        quest.Comment = mainvl.at(8);
        quest.Id = AnotherID.toInt();
        Questions.append(quest);
        w->setLayout(lyout);
        s_tqScrollArea *sa = new s_tqScrollArea;
        sa->setWidgetResizable(true);
        sa->setWidget(w);
        wdgts.append(sa);
    }
    return wdgts;
}

void tb_examdialog::AnswerChoosed()
{
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("questionstw");
    if (stw == 0)
    {
        DBGMSG;
        return;
    }
    QWidget *curw = stw->currentWidget();
    curw->setEnabled(false);
    GoodBadWidget *gbw = this->findChild<GoodBadWidget *>("gbw");
    if (gbw == 0)
    {
        DBGMSG;
        return;
    }
    bool ok;
    int tmpi = sender()->objectName().toInt(&ok);
    if (!ok)
    {
        WARNMSG("");
        return;
    }
    Questions_s quest = Questions.takeFirst();
    Answers_s ans;
    ans.Answer = tmpi;
    ans.Id = quest.Id;
    if (tmpi == quest.RightAnswer)
    {
        ans.Good = true;
        gbw->SetItem(AnswerCount, GoodBadTableModel::GBIT_GOOD);
        RightAnswerCount++;
        NextQuestion();
        Answers.append(ans);
    }
    else
    {
        ans.Good = false;
        gbw->SetItem(AnswerCount, GoodBadTableModel::GBIT_BAD);
        s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("commentstw");
        if (stw == 0)
        {
            DBGMSG;
            return;
        }
        s_tqWidget *w = new s_tqWidget;
        QVBoxLayout *lyout = new QVBoxLayout;
        s_tqLabel *lbl = new s_tqLabel(quest.Comment);
        lbl->setWordWrap(true);
        lyout->addWidget(lbl);
        s_tqPushButton *pb = new s_tqPushButton("Понятно, далее");
        pb->setStyleSheet("QPushButton {background-color: #99eeee;}");
        connect(pb,SIGNAL(clicked(bool)),this,SLOT(NextQuestion()));
        lyout->addWidget(pb);
        w->setLayout(lyout);
        stw->addWidget(w);
        stw->setCurrentIndex(stw->count()-1); // устанавливаем последнедобавленный виджет в качестве текущего
        Answers.append(ans);
    }
}

void tb_examdialog::NextQuestion()
{
    ++AnswerCount;
    if (AnswerCount >= TB_QUESTNUM) // хватит вопросов, пора кончать экзамен
    {
        ProcessResultsAndExit();
        return;
    }
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("questionstw");
    if (stw == 0)
    {
        DBGMSG;
        return;
    }
    stw->setCurrentIndex(AnswerCount);
    s_tqScrollArea *sa = static_cast<s_tqScrollArea *>(stw->currentWidget());
    sa->verticalScrollBar()->setValue(sa->verticalScrollBar()->minimum());
    stw = this->findChild<s_tqStackedWidget *>("commentstw");
    if (stw == 0)
    {
        DBGMSG;
        return;
    }
    stw->setCurrentIndex(0); // "очищаем" поле комментариев
}

void tb_examdialog::ProcessResultsAndExit()
{
    emit StopCleverTimer();
    // сначала запишем в базу данных результаты
    // потом выдадим окно с результатами
    // потом закроем этот диалог
    float Mark = static_cast<float>(RightAnswerCount) / TB_QUESTNUM * MAX_MARK;
    QList<QStringList> lsl;
    QStringList sl;
    if (ExType == EXTYPE_EX)
    {
        WaitWidget *ww = new WaitWidget;
        ww->Start();
        ww->SetMessage("Запись результатов...");
        QString table = "Экзамен ТБ ответы_полн";
        for (int i=0; i<TB_QUESTNUM; ++i)
        {
            Answers_s ans;
            if (Answers.size() != 0)
                ans = Answers.takeFirst();
            else
                break;
            QString newID;
            tfl.Insert(table, newID);
            if (tfl.result == TFRESULT_ERROR)
            {
                WARNMSG("Ошибка добавления нового элемента в базу ответов");
                ww->Stop();
                return;
            }
            QStringList fl = QStringList() << "ИД" << "ИД вопроса" << "Номер ответа" << "Правильный ответ";
            QString tmps = (ans.Good) ? "1" : "0";
            QStringList vl = QStringList() << newID << QString::number(ans.Id) << QString::number(ans.Answer) << tmps;
            tfl.Update(table, fl, vl);
            if (tfl.result == TFRESULT_ERROR)
            {
                WARNMSG("Ошибка обновления очередного ответа в БД");
                ww->Stop();
                this->close();
                return;
            }
            lsl.append(vl);
        }
        ww->SetMessage("Формирование протокола...");
        // сформируем протокол в pdf
        sl = QStringList() << "ИД" << "ИД вопроса" << "Номер ответа" << "Правильный ответ";
        lsl.insert(0, sl);
        QString FullFilename = pc.HomeDir + "/tb/prot/" + pc.Pers+" " + pc.DateTime.replace(':','.') +".pdf";
        PdfOut *PdfDoc = new PdfOut(FullFilename);
        QFont font;
        font.setPointSize(15);
        PdfDoc->SetTextFont(font);
        PdfDoc->InsertText("Результаты экзамена по ОТ и ТБ "+pc.Pers);
        PdfDoc->InsertTable(lsl);
        PdfDoc->WritePdf();
        // запишем результаты в Экзам рез
        table = "Экзам рез_полн";
        QString newID;
        tfl.Insert(table, newID);
        if (tfl.result == TFRESULT_ERROR)
        {
            WARNMSG("Ошибка добавления нового элемента в базу результатов");
            ww->Stop();
            this->close();
            return;
        }
        QString Filename = pc.Pers+" " + pc.DateTime.replace(':','.') +".pdf";
        QStringList fl = QStringList() << "ИД" << "Результат" << "Раздел" << "Тип" << "Файл";
        QStringList vl = QStringList() << newID << QString::number(Mark, 'g', 2) << QString::number(TBGroup) << QString::number(ExType) << Filename;
        tfl.Update(table, fl, vl);
        if (tfl.result == TFRESULT_ERROR)
        {
            WARNMSG("Ошибка при записи результатов экзамена в БД");
            ww->Stop();
            this->close();
            return;
        }
        // отправим протокол на сервер
        ww->SetMessage("Отправка протокола на сервер...");
        if (Cli->PutFile(FullFilename, FLT_TB, FLST_PROT, Filename) != Client::CLIER_NOERROR)
            WARNMSG("Ошибка отправки файла с протоколом экзамена на сервер");
        ww->Stop();
    }
    MessageBox2::information(this, "Информация", "По результатам экзамена Вы получаете оценку\n" + QString::number(Mark, 'f', 2) + " баллов из " + \
                             QString::number(MAX_MARK) + " возможных!");
    this->close();
}
