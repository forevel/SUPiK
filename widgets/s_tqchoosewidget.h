#ifndef S_TQCHOOSEWIDGET_H
#define S_TQCHOOSEWIDGET_H

#include <QWidget>
#include <QDate>
#include <QVariant>

class s_tqChooseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit s_tqChooseWidget(QWidget *parent = 0);

    void setlinks (QString links);
    void setdata (QString data);
    QVariant getAData();
    void setAData(QVariant dat);

signals:
    void datachanged();
public slots:

private:
    QVariant adata;
    typedef struct
    {
        int ftype;
        int delegate;
        int dependson;
        QStringList link;
    } fieldformat;
    s_tqChooseWidget::fieldformat ff;
    s_tqChooseWidget::fieldformat getFFfromLinks (QString links);

private slots:
    void pbclicked();
    void dateChoosed(QDate);
    void accepted(QString);
};

#endif // S_TQCHOOSEWIDGET_H
