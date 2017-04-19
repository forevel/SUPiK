#ifndef S_STATUSBAR_H
#define S_STATUSBAR_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>

#define SB_MAXHEIGHT    16

class s_StatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit s_StatusBar(QWidget *parent = 0);
    ~s_StatusBar();

    quint64 IncomeBytes();
    quint64 OutgoingBytes();
    void SetServerStatusOffline();
    void SetServerStatusOnline();

signals:

public slots:
    void UpdateIncomeBytes(quint64 bytes);
    void UpdateOutgoingBytes(quint64 bytes);
    void SetRetrCounter(int value);
    void DisableRetrCounter();

private slots:
    void ClearActiveServerState();

private:
    quint64 IncBytes, OutBytes;
    QTimer *ActiveTimer;
    QPixmap *Sf, *Sna, *Snp;

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // S_STATUSBAR_H
