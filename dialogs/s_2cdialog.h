#ifndef S_GENERAL2COLUMNSDIALOG_H
#define S_GENERAL2COLUMNSDIALOG_H

#include <QDialog>
#include <QByteArray>
#include <QSortFilterProxyModel>
//#include "../models/s_ncmodel.h"
#include "../models/editmodel.h"

#define CD_ERROR    0x0200

#define CD2WARN WARNMSG(PublicClass::ER_2CDLG, __LINE__)
#define CD2DBG  DBGMSG(PublicClass::ER_2CDLG, __LINE__)
#define CD2INFO(a)  INFOMSG(PublicClass::ER_2CDLG, __LINE__, a)

enum s_2cdialog_modes
{
    MODE_CHOOSE,
    MODE_EDIT,
    MODE_EDITNEW,
    MODE_FILE,
    MODE_CHOOSE_RAW,
    MODE_EDIT_RAW,
    MODE_EDITNEW_RAW
};

class s_2cdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_2cdialog(QString caption, QWidget *parent = 0);
    // sl1 - надписи, sl2 - поля для заполнения, links - формат полей sl2
    void SetupFile(QString Filename, QString StringToFind, QString str);
    // tble - имя таблицы из tablefields, Mode - список выбора или редактор полей, caption - заголовок в окне, matchtext - текущее значение
    // (для MODE_CHOOSE), isQuarantine - признак для MODE_EDIT: карантинная база или нет, для спец. обработки слота accepted()
    void setup(QString tble, int Mode=MODE_CHOOSE, QString id="", bool isQuarantine=false);
    // sl - список строк для выбора, links - опционально вид для каждой ячейки, str - текущее выбранное значение
    // void setup(QStringList sl, QStringList links=QStringList(), QString str="");
    void SetupRaw(QString db, QString tble, int Mode=MODE_CHOOSE_RAW, QString id=""); // построение диалога по таблицам SQL
    // функция добавления к существующей таблице ещё одной - для FW_DLINK
    void AddTable(QString tble);
    bool IsQuarantine;
    int Mode,result;
    QString caption;

signals:
    void changeshasbeenMade(QString);

public slots:

private:
    QStringList tble;
    QString Db, Id;
    EditModel *MainModel;
    bool Cancelled;
    int constheight;
    void setupUI();
    void Update();
    void FillHeaderData();

private slots:
    void accepted(QModelIndex);
    void accepted();
    void cancelled();
    void resizemainTV(QModelIndex, QModelIndex);
    void AddItem();

protected:
    void paintEvent(QPaintEvent *e);
    void closeEvent(QCloseEvent *e);
};

#endif // S_GENERAL2COLUMNSDIALOG_H
