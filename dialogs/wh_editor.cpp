#include "wh_editor.h"
#include "../widgets/s_tqcombobox.h"
#include "../widgets/s_tqlabel.h"
#include "../widgets/s_tqlineedit.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

Wh_Editor::Wh_Editor(QWidget *parent) : QDialog(parent)
{
    SetupUI();
}

void Wh_Editor::SetupUI()
{
    // Кнопки "создать новый", "выход" и надпись "Редактор складов"
    // Комбобокс с наименованиями складов из wh
    // Табвиджет.
    // Первая вкладка "Склад". Надпись "На складе имеются:"
    //      "Шкафы", спин, "шт", "Стеллажи", спин, "шт", "Коробки(мешки)", спин, "шт"
    //      Табвиджет по кол-ву шкафов, стеллажей, коробок с именами типа "Шкаф 1", "Стеллаж 5" и т.д.
    //      в каждой вкладке шкафа:
    //          "Имя шкафа", поле ввода
    //          "тип", комбобокс с типами шкафов
    //          "Количество ячеек/мешков на одной полке", спин
    //      в каждой вкладке стеллажа:
    //          "Имя стеллажа", поле ввода
    //          "тип", комбобокс с типами стеллажей
    //          "Количество ячеек/мешков на одной полке", спин
    //      в каждой вкладке коробки:
    //          "Имя коробки", поле ввода
    // Вторая вкладка "Типы мест хранения"
    //      групбокс "Шкафы"
    //      Кнопка "добавить тип шкафа"
    //      "Типы шкафов", комбобокс
    //      "параметры шкафа"
    //      "Кол-во полок", спин
    //      групбокс "Стеллажи"
    //      Кнопка "добавить тип стеллажа"
    //      "Типы стеллажей", комбобокс
    //      "параметры стеллажа"
    //      "Кол-во рядов", спин
    //      "Кол-во полок", спин
}