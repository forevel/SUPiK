#ifndef S_TQCHOOSEWIDGET_H
#define S_TQCHOOSEWIDGET_H

#include <QWidget>
#include <QDate>
#include <QVariant>
#include "../gen/publicclass.h"

class s_tqChooseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit s_tqChooseWidget(bool Transparent=false, QWidget *parent = 0);

    void Setup(QString links, QString hdr="");
    void SetData (QVariant data);
    QVariant Data();
    QVariant getAData();
    void setAData(QVariant dat);
    void SetValue(QVariant data); // установить значение поля ввода в зависимости от links
    QVariant Value(); // выдать значение в соответствии с содержимым поля ввода и links

    int result;
signals:
    void textchanged(QVariant);
    void error(int,int);

public slots:

private:
    QVariant adata;
    QString hdr, links;
    PublicClass::fieldformat ff;
    bool Transparent;
    QWidget *parent;

private slots:
    void pbclicked();
    void dateChoosed(QDate);
    void accepted(QString);
};

#endif // S_TQCHOOSEWIDGET_H
