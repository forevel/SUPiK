#ifndef S_TQDATETIMEWIDGET_H
#define S_TQDATETIMEWIDGET_H

#include <QWidget>
#include <QDateTime>

class s_tqDateTimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit s_tqDateTimeWidget(QWidget *parent=0);

    void SetDateTime(const QDateTime &dtm);

signals:
    void activated(QDateTime);

private slots:
    void FinishedEditing();
};

#endif // S_TQDATETIMEWIDGET_H
