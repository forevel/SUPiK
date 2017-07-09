#ifndef S_STATUSBAR_H
#define S_STATUSBAR_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include "s_tqprogressbar.h"

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
    void SetStatusDisconnected();
    void SetStatusConnected();
    void SetProgressBarText(const QString &txt);
    void SetProgressBarRange(quint64 max);
    void SetProgressBarValue(quint64 value);

private slots:
    void ClearActiveServerState();

private:
    quint64 IncBytes, OutBytes;
    QTimer *ActiveTimer;
    QPixmap *Sf, *Sna, *Snp;
    s_tqProgressBar *PRB;

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // S_STATUSBAR_H
