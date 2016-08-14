#ifndef TB_MAINDIALOG_H
#define TB_MAINDIALOG_H

#define TBMWARN    WARNMSG(PublicClass::ER_TBM, __LINE__)
#define TBMDBG     DBGMSG(PublicClass::ER_TBM, __LINE__)
#define TBMINFO(a) INFOMSG(PublicClass::ER_TBM, __LINE__,a)

#define TB_PTENUM   3
#define TB_SIZNUM   2
#define TB_POMNUM   2
#define TB_PUENUM   3
#define TB_POTNUM   3
#define TB_QUESTNUM 13

#define TB_TH_PTE   0 // ПТЭ
#define TB_TH_PUE   1 // ПУЭ
#define TB_TH_POT   2 // ПОТ
#define TB_TH_SIZ   3 // средства индивид. защиты
#define TB_TH_POM   4 // первая помощь

#define MAX_MARK    10 // максимальная оценка

#include <QDialog>
#include "../../models/treemodel.h"
#include "../../widgets/s_tqwidget.h"

class tb_maindialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit tb_maindialog(QWidget *parent = 0);
    ~tb_maindialog();

signals:

public slots:

private slots:
    void GroupChoosed();
    void NextQuestion();
    void AnswerChoosed();

protected:
    void paintEvent(QPaintEvent *);

private:
    struct Questions_s
    {
        int Id;
        int RightAnswer;
        QString Comment;
    };

    struct Answers_s
    {
        int Id;
        int Answer;
        bool Good;
    };

    int TBGroup, AnswerCount, RightAnswerCount;
    QList<Questions_s> Questions;
    QList<Answers_s> Answers;

    bool PrepareQuestions();
    QList<s_tqWidget *> PrepareQuestionsByTheme(int theme, int questnum); // подготовка набора виджетов по теме theme в количестве questnum
    void SetupUI();
    void ProcessResultsAndExit();
};

#endif // tb_maindialog_H
