#ifndef S_TQRADIOBUTTON_H
#define S_TQRADIOBUTTON_H

#include <QRadioButton>
#include "s_tqlabel.h"
#include <QHBoxLayout>

#define S_TQRADIOBUTTON_MARGIN  4

class s_tqRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit s_tqRadioButton(const QString &text="", QWidget *parent = 0);

//    void setText(const QString & text);
 //   QSize sizeHint() const;
  //  bool hitButton(const QPoint &pos) const;

signals:

public slots:

protected:
   // void paintEvent(QPaintEvent *e);

private:
   // QHBoxLayout *_layout;
   // QLabel *_label;
};

#endif // S_TQRADIOBUTTON_H
