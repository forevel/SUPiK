#ifndef PERSDIALOG_H
#define PERSDIALOG_H

#include <QDialog>
#include <QVariant>

class PersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PersDialog(QString PersID="", int DialogType = PDT_PERS, QWidget *parent = 0);
    ~PersDialog();

    enum PersDialogTypes
    {
        PDT_TB, // диалог с данными по технике безопасности
        PDT_PERS // общий диалог сотрудников
    };

signals:
    void DialogClosed();

public slots:

private:
    void SetupUI();
    void ShowPhoto(const QString &Pers);

    int DialogType;

private slots:
    void Filter();
    void Unfilter();
    bool Fill(QString PersID);
    void ShowEBProt();
    void ShowMedProt();
    void EnterEBData();
    void ShowEBHistory();
    void EnterPBData();
    void EnterOTData();
    void EnterMedData();
    void LoadPhoto();

protected:
    void paintEvent(QPaintEvent *);


};

#endif // PERSDIALOG_H
