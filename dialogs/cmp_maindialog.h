#ifndef CMP_MAINDIALOG_H
#define CMP_MAINDIALOG_H

#include <QDialog>
#include <QPaintEvent>

#define CTYPE_ALT   1
#define CTYPE_SCH   2
#define CTYPE_SOL   3
#define CTYPE_CON   4
#define CTYPE_DEV   5

class cmp_maindialog : public QDialog
{
    Q_OBJECT

public:
    explicit cmp_maindialog(QWidget *parent = 0);
    ~cmp_maindialog();
    void SetupUI(int CompType, int CompTable, int CompID); // подготовка диалога к отображению. 1 = тип компонента (Altium,Schemagee,...),
                                                            // 2 = номер таблицы компонента по таблице description, 3 = ИД компонента по таблице
    void SetID(); // принудительная установка ИД компонента для создания на базе существующего (сначала заполняем диалог по существующему, потом меняем ИД)

public slots:

signals:
    void error(int,int);

private:
    QString CompDb, CompTble, CompId, CompType;
    bool Changed, ChangeEnabled;
    int RevNotes;
    bool PartNumberCreatorEnabled;
    QList <QMetaObject::Connection *> handles;

    void SetAltDialog();
    void FillAltDialog(QStringList);
    QStringList GetAltData();
    void SetCWData(QString cwname, QVariant data);
    QString CWData(QString cwname);
    void SetLEData(QString lename, QVariant data);
    QString LEData(QString lename);
    void SetChBData(QString chbname, QVariant data);
    QString ChBData(QString chbname);
    void SetCBData(QString cbname, QVariant data);
    QString CBData(QString cbname);
    void SetUnitsAndPars();
    void ClearHandles();
    void SetPrefixesVisible(bool isVisible);
    void ConnectPartNumberCreatorLE(QString lename); // подключение функции изменения значения PartNumber от сигнала изменения поля ввода
    void ConnectPartNumberCreatorCB(QString cbname); // подключение функции изменения значения PartNumber от сигнала изменения комбобокса

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void AddManuf();
    void WriteAndClose();
    void CancelAndClose();
    void SomethingChanged();
    void emiterror(int er1,int er2);
    void EnablePartNumberCreator(QVariant Manufacturer);
    void PartNumberCreator();
    void SetParLE(int ParNum, QString ParValue);
    QString ParLE(int ParNum);
};

#endif // CMP_MAINDIALOG_H
