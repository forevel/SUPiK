#include "tb_maindialog.h"
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
#include <stdlib.h>
#include "../../models/proxymodel.h"
#include "../../models/s_duniversal.h"
#include "../../widgets/s_tqgroupbox.h"
#include "../../widgets/s_tqlabel.h"
#include "../../widgets/s_tqlineedit.h"
#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqpushbutton.h"
#include "../../widgets/s_tqradiobutton.h"
#include "../../widgets/s_tqcheckbox.h"
#include "../../widgets/s_tqtableview.h"
#include "../../widgets/s_tqtextedit.h"
#include "../../widgets/s_tqwidget.h"
#include "../../widgets/s_tqstackedwidget.h"
#include "../../widgets/goodbadwidget.h"
#include "../../gen/publicclass.h"
#include "../../gen/s_tablefields.h"
#include "../messagebox.h"

// --------------------------------------
// Конструктор
// --------------------------------------

tb_maindialog::tb_maindialog(QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    TBGroup = 0;
    ExType = EXTYPE_TEST;
    SetupUI();
}

// --------------------------------------
// Деструктор
// --------------------------------------

tb_maindialog::~tb_maindialog()
{
}

void tb_maindialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/res/TBWallpaper.png"));

    event->accept();
}

// Настройка интерфейса

void tb_maindialog::SetupUI()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QVBoxLayout *lyout = new QVBoxLayout;
    QHBoxLayout *hlyout = new QHBoxLayout;

    s_tqPushButton *pb = new s_tqPushButton;
    pb->setIcon(QIcon(":/res/cross.png"));
    connect(pb, SIGNAL(clicked()), this, SLOT(close()));
    pb->setToolTip("Закрыть вкладку");
    hlyout->addWidget(pb);
    hlyout->addStretch(300);
    s_tqLabel *lbl = new s_tqLabel("ОТ и ТБ");
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
    s_tqRadioButton *rb = new s_tqRadioButton("Хочу экзамен");
    connect(rb,SIGNAL(clicked(bool)),this,SLOT(ExTypeExChoosed()));
    hlyout->addWidget(rb);
    gbg1->addButton(rb);
    rb = new s_tqRadioButton("Хочу потренироваться");
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
    lyout->addWidget(stw);

    stw = new s_tqStackedWidget;
    stw->setObjectName("questionstw");
    gb = new s_tqGroupBox;
    gb->setObjectName("questiongb");
    gb->setTitle("Вопрос");
    hlyout = new QHBoxLayout;
    hlyout->addWidget(stw);
    gb->setLayout(hlyout);
//    gb->setVisible(false);
//    stw->addWidget(gb);
    lyout->addWidget(gb);

    gb = new s_tqGroupBox;
    gb->setObjectName("goodbadgb");
    gb->setTitle("Результаты ответов");
    vlyout = new QVBoxLayout;
    GoodBadWidget *gbw = new GoodBadWidget;
    gbw->setObjectName("gbw");
    if (!gbw->SetItemsCount(TB_QUESTNUM))
    {
        TBMWARN;
        return;
    }
    vlyout->addWidget(gbw, 0);
    gb->setLayout(vlyout);
    lyout->addWidget(gb);

    gb = new s_tqGroupBox;
    gb->setTitle("Комментарий");
    vlyout = new QVBoxLayout;
    gb->setObjectName("commentgb");
    stw = new s_tqStackedWidget;
    stw->setObjectName("commentstw");
    s_tqWidget *w = new s_tqWidget;
    stw->addWidget(w);
    stw->setCurrentIndex(0);
    vlyout->addWidget(stw);
    gb->setLayout(vlyout);
    lyout->addWidget(gb);

    lyout->addStretch(300);
    setLayout(lyout);
    QApplication::restoreOverrideCursor();
    gbw->SetItem(0, GoodBadTableModel::GBIT_NEUTRAL); // принудительный ресайз
//    gbw->SetItem(0, GoodBadTableModel::GBIT_GOOD);
}

void tb_maindialog::ExTypeExChoosed()
{
    ExType = EXTYPE_EX;
}

void tb_maindialog::ExTypeTestChoosed()
{
    ExType = EXTYPE_TEST;
}

void tb_maindialog::GroupChoosed()
{
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("groupstw");
    if (stw == 0)
    {
        TBMDBG;
        return;
    }
    s_tqRadioButton *rb3 = this->findChild<s_tqRadioButton *>("rb3");
    s_tqRadioButton *rb4 = this->findChild<s_tqRadioButton *>("rb4");
    s_tqRadioButton *rb5 = this->findChild<s_tqRadioButton *>("rb5");
    if ((rb3 == 0) || (rb4 == 0) || (rb5 == 0))
    {
        TBMDBG;
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
        TBMWARN;
        return;
    }
    s_tqGroupBox *questiongb = this->findChild<s_tqGroupBox *>("questiongb");
    s_tqGroupBox *goodbadgb = this->findChild<s_tqGroupBox *>("goodbadgb");
    if ((questiongb == 0) || (goodbadgb == 0))
    {
        TBMDBG;
        return;
    }
    questiongb->setVisible(true);
    goodbadgb->setVisible(true);
}

bool tb_maindialog::PrepareQuestions()
{
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("questionstw");
    if (stw == 0)
    {
        TBMDBG;
        return false;
    }
    Questions.clear();
    Answers.clear();
    AnswerCount = 0; // номер текущего ответа
    RightAnswerCount = 0;
    QList<s_tqWidget *> tmplw = PrepareQuestionsByTheme(TB_TH_PTE, TB_PTENUM);
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
    return true;
}

QList<s_tqWidget *> tb_maindialog::PrepareQuestionsByTheme(int theme, int questnum)
{
    QList<s_tqWidget *> wdgts;

    if (TBGroup == 0)
    {
        TBMWARN;
        return wdgts;
    }
    QStringList mainfl = QStringList() << "Содержание вопроса" << "Ответ 1" << "Ответ 2" << "Ответ 3" << "Ответ 4" << "Ответ 5" << "Ответ 6" << \
                                      "Номер правильного ответа" << "Комментарий";
    QStringList cmpfl = QStringList() << "Группа ТБ" << "Раздел вопроса";
    QStringList cmpvl = QStringList() << QString::number(TBGroup) << QString::number(theme);
    QString tble = "Экзамен ТБ_полн";
    QString field = "ИД";
    QStringList idvl = tfl.HeaderByFields(tble, field, cmpfl, cmpvl);
    if (tfl.result)
    {
        TBMWARN;
        return wdgts;
    }
    qsrand(QTime::currentTime().msecsSinceStartOfDay()); // инициализация генератора случайных чисел

    for (int i=0; i<questnum; ++i)
    {
        s_tqWidget *w = new s_tqWidget;
        QVBoxLayout *lyout = new QVBoxLayout;
        int idsize = idvl.size();
        int index = qrand()%idsize; // числа от 0 по (idsize-1)
        QString AnotherID;
        if (index < idvl.size())
             AnotherID = idvl.takeAt(index);
        else
            continue;
        QStringList mainvl = tfl.valuesbyfield("Экзамен ТБ_полн", mainfl, "ИД", AnotherID);
        if ((tfl.result) || (mainvl.size() < 9))
        {
            TBMWARN;
            return wdgts;
        }
        QString tmps = AnotherID + ". " + mainvl.at(0);
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
                s_tqRadioButton *rb = new s_tqRadioButton(mainvl.at(j)); // ответ
                rb->setObjectName(QString::number(j));
                connect(rb,SIGNAL(clicked(bool)),this,SLOT(AnswerChoosed()));
                lyout->addWidget(rb);
            }
        }
        Questions_s quest;
        quest.RightAnswer = mainvl.at(7).toInt();
        quest.Comment = mainvl.at(8);
        quest.Id = AnotherID.toInt();
        Questions.append(quest);
        w->setLayout(lyout);
        wdgts.append(w);
    }
    return wdgts;
}

void tb_maindialog::AnswerChoosed()
{
    GoodBadWidget *gbw = this->findChild<GoodBadWidget *>("gbw");
    if (gbw == 0)
    {
        TBMDBG;
        return;
    }
    bool ok;
    int tmpi = sender()->objectName().toInt(&ok);
    if (!ok)
    {
        TBMWARN;
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
            TBMDBG;
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

void tb_maindialog::NextQuestion()
{
    ++AnswerCount;
    if (AnswerCount >= TB_QUESTNUM) // хватит вопросов, пора кончать экзамен
        ProcessResultsAndExit();
    s_tqStackedWidget *stw = this->findChild<s_tqStackedWidget *>("questionstw");
    if (stw == 0)
    {
        TBMDBG;
        return;
    }
    stw->setCurrentIndex(AnswerCount);
    stw = this->findChild<s_tqStackedWidget *>("commentstw");
    if (stw == 0)
    {
        TBMDBG;
        return;
    }
    stw->setCurrentIndex(0); // "очищаем" поле комментариев
}

void tb_maindialog::ProcessResultsAndExit()
{
    // сначала запишем в базу данных результаты
    // потом выдадим окно с результатами
    // потом закроем этот диалог
    if (ExType == EXTYPE_EX)
    {
        for (int i=0; i<TB_QUESTNUM; ++i)
        {
            QString newID = tfl.insert("Экзамен ТБ результаты_полн");
            if (tfl.result)
            {
                TBMWARN;
                return;
            }
            Answers_s ans;
            if (Answers.size() != 0)
                ans = Answers.takeFirst();
            else
                break;
            QStringList fl = QStringList() << "ИД" << "ИД вопроса" << "Номер ответа" << "Правильный ответ";
            QString tmps = (ans.Good) ? "1" : "0";
            QStringList vl = QStringList() << newID << QString::number(ans.Id) << QString::number(ans.Answer) << tmps;
            tfl.idtois("Экзамен ТБ результаты_полн", fl, vl);
            if (tfl.result)
            {
                TBMWARN;
                return;
            }
        }
    }
    float Mark = static_cast<float>(RightAnswerCount) / TB_QUESTNUM * MAX_MARK;
    MessageBox2::information(this, "Информация", "По результатам экзамена Вы получаете оценку\n" + QString::number(Mark, 'f', 2) + " баллов из " + \
                             QString::number(MAX_MARK) + " возможных!");
    this->close();
}
