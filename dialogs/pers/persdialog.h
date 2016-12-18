#ifndef PERSDIALOG_H
#define PERSDIALOG_H

#include <QDialog>
#include <QVariant>
#include "../../gen/publicclass.h"

class PersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PersDialog(QString PersID="", int DialogType = PDT_PERS, int Mode = MODE_EDIT, QWidget *parent = 0);
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
    enum EkzTypes
    {
        EKZ_GENERAL,
        EKZ_NGENERAL
    };

    void SetupUI();

    int DialogType, Mode, EkzType;
    bool EBDataChanged;
    QString idPers, Pers, EBDate, OTDate, PBDate, MedDate, idRec, idRecEkz;
    QList<QColor> CList = QList<QColor>() << Qt::darkGreen << Qt::darkYellow << Qt::red;

private slots:
    bool Fill();
    void ShowEBProt();
    void ShowMedProt();
    void EnterEBData();
    void ShowEBHistory();
    void SetPhoto();
    void LoadPhoto();
    void Accept();
    void AcceptEBData();
    void SetEkzType(const QString &ekztype);
    void CloseDialog();

protected:
    void paintEvent(QPaintEvent *);


};

#endif // PERSDIALOG_H
