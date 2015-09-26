#ifndef WH_EDITOR_H
#define WH_EDITOR_H

#include <QDialog>

class Wh_Editor : public QDialog
{
    Q_OBJECT
public:
    explicit Wh_Editor(QWidget *parent=0);

private:
    void SetupUI();

};

#endif // WH_EDITOR_H
