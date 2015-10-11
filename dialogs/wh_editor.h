#ifndef WH_EDITOR_H
#define WH_EDITOR_H

#include <QDialog>
#include <QStack>
#include <QPixmap>
#include <QVBoxLayout>

#include "../widgets/s_tqcombobox.h"
#include "../widgets/s_tqlabel.h"

#define WHEDWARN WARNMSG(PublicClass::ER_WHED, __LINE__)
#define WHEDDBG  DBGMSG(PublicClass::ER_WHED, __LINE__)

#define MAXROWS 2
#define MAXCOLS 6

class Wh_Editor : public QDialog
{
    Q_OBJECT
public:
    explicit Wh_Editor(QWidget *parent=0);

signals:
    void CloseAllWidgets();

private:
    QStack<int> IDs;
    QStack<int> Columns;
    QStack<int> Rows;

    void SetupUI();
    bool UpdatePlacePicture(s_tqLabel *lbl); // обновление картинки в выбранной позиции. Возвращает false, если картинка "пусто" и нет такого размещения
    void SetCells(QVBoxLayout *lyout);
    void SetRootWidget();
    void SetChildWidget(s_tqLabel *celllbl);
    void CheckIndexes();

private slots:
    void AddNewWh();
    void DeleteWh();
    void UpdatePlace();
    void UpdatePicture(QVariant value);
    void WriteAndClose();
    void ModifyWh(QString);
    void PopIDs();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // WH_EDITOR_H
