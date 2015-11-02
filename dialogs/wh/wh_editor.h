#ifndef WH_EDITOR_H
#define WH_EDITOR_H

#include <QDialog>
#include <QStack>
#include <QPixmap>
#include <QVBoxLayout>

#include "../../widgets/s_tqcombobox.h"
#include "../../widgets/s_tqlabel.h"
#include "../../models/whplacestreemodel.h"

#define WHEDWARN    WARNMSG(PublicClass::ER_WHED, __LINE__)
#define WHEDDBG     DBGMSG(PublicClass::ER_WHED, __LINE__)
#define WHEDINFO(a) INFOMSG(PublicClass::ER_WHED,__LINE__,a)

#define MAXROWS 2
#define MAXCOLS 6

#define MAXPLACES   999999

class Wh_Editor : public QDialog
{
    Q_OBJECT
public:
    explicit Wh_Editor(QWidget *parent=0);

signals:
    void CloseAllWidgets();

private:
    QStack<int> IDs;
    int Wh;
    WhPlacesTreeModel *WhModel;
    int CurID;
    bool SomethingChanged;
    struct IDProperties
    {
        QString Picture;
        QString ChoosePlaceString;
        int PlaceType;
        QString PlacePrefix;
        int Rows;
        int Columns;
    };
    IDProperties CurIDProperties;

    void SetupUI();
    void UpdateWhComboBox();
    void SetCells(QWidget *w);
    void BuildWorkspace(int ID, bool IsWarehouse); // отобразить рабочее поле (размещения внутри размещения с данным ID). IsWarehouse - признак "корня"
    void UpdatePlace();
    void Disband(int ID); // расформирование единицы размещения
    QStringList NameAndPicture(int ID); // вытащить картинку по ИД размещения
    void ClearLayout (QLayout *lyout);
    bool CheckPriorities (QString PlaceName);

private slots:
    void AddNewPlace();
    void ChangePlace(QVariant PlaceName);
    void GoToPlace();
    void GoBack();
    void WriteAndClose();
    void CancelAndClose();
    void ChangeWh(QString);
    void AddNewWh();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // WH_EDITOR_H
