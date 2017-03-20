#ifndef TB_EXAMDIALOG_H
#define TB_EXAMDIALOG_H

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

#define MAX_MARK    5 // максимальная оценка

#define TB_EX_TIME  10 // minutes to finish exam

#include <QDialog>
#include "../../models/treemodel.h"
#include "../../widgets/s_tqwidget.h"

class tb_examdialog : public QDialog
{
    Q_OBJECT
    
    enum ExTypes
    {
        EXTYPE_EX,
        EXTYPE_TEST
    };

public:
    explicit tb_examdialog(QWidget *parent = 0);
    ~tb_examdialog();

signals:
    void StartCleverTimer();
    void StopCleverTimer();

public slots:

private slots:
    void ExTypeTestChoosed();
    void ExTypeExChoosed();
    void GroupChoosed();
    void NextQuestion();
    void AnswerChoosed();
    void ProcessResultsAndExit();

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

    int TBGroup, AnswerCount, RightAnswerCount, ExType;
    QList<Questions_s> Questions;
    QList<Answers_s> Answers;

    bool PrepareQuestions();
    QList<s_tqWidget *> PrepareQuestionsByTheme(int theme, int questnum); // подготовка набора виджетов по теме theme в количестве questnum
    void SetupUI();
};

#endif // tb_examdialog_H
