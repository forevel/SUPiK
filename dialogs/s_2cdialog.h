#ifndef S_GENERAL2COLUMNSDIALOG_H
#define S_GENERAL2COLUMNSDIALOG_H

#include <QDialog>
#include <QByteArray>
#include <QSortFilterProxyModel>
#include "../models/s_ncmodel.h"

#define MODE_CHOOSE 1
#define MODE_EDIT   2

#define CD_ERROR    0x0200

class s_2cdialog : public QDialog
{
    Q_OBJECT
public:
    explicit s_2cdialog(QString caption, QWidget *parent = 0);
    // sl1 - надписи, sl2 - поля для заполнения, links - формат полей sl2
    void SetupFile(QString Filename, QString StringToFind, QString str);
    // tble - имя таблицы из tablefields, Mode - список выбора или редактор полей, caption - заголовок в окне, matchtext - текущее значение
    // (для MODE_CHOOSE), isQuarantine - признак для MODE_EDIT: карантинная база или нет, для спец. обработки слота accepted()
    void setup(QString tble, int Mode=MODE_CHOOSE, QString id="", QString matchtext="", bool isQuarantine=false);
    // sl - список строк для выбора, links - опционально вид для каждой ячейки, str - текущее выбранное значение
//    void setup(QStringList sl, QStringList links=QStringList(), QString str="");
    // функция добавления к существующей таблице ещё одной - для FW_DLINK
    void AddTable(QString tble);
    void sortModel();
    void SetTvCurrentText(QString str);
    bool IsQuarantine;
    int Mode,result;
    QString caption;

signals:
    void changeshasbeenMade(QString);
    void error(int,int);

public slots:

private:
    QStringList tble;
    s_ncmodel *mainmodel;
    QSortFilterProxyModel *pmainmodel;
    bool DialogIsNeedToBeResized;
    int constheight;
    void fillModelAdata();
    void setupUI();

private slots:
    void accepted(QModelIndex);
    void accepted();
    void cancelled();
    void resizemainTV(QModelIndex, QModelIndex);

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // S_GENERAL2COLUMNSDIALOG_H
