// 2022/04/29 13:56:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "Windows/MainWindow/GridSensors.h"


enum
{
    FILE_QUIT = wxID_HIGHEST + 1,

    TOOL_OPEN,
    TOOL_SAVE,
    TOOL_NEW,

    TOOL_UNDO,
    TOOL_REDO,

    TOOL_VIEW_BRIEF,        // Сокращённый вид отображения
    TOOL_VIEW_FULL,         // Полный вид отображения

    VIEW_TERMINAL,          // Открыть/закрыть терминал
    VIEW_LOG,               // Открыть/закрыть лог
    VIEW_DIAGRAM,           // Открыть/зыкрыть графики

    MEAS_PRESSURE,          // Давление
    MEAS_ILLUMINATION,      // Освещённость
    MEAS_HUMIDITY,          // Влажность
    MEAS_VELOCITY,          // Скорость
    MEAS_TEMPERATURE,       // Температура

    ID_SPEED_1,
    ID_SPEED_2,
    ID_SPEED_5,
    ID_SPEED_30,
    ID_SPEED_60,
    ID_SPEED_15min,

    ID_MODE_VIEW_FULL,
    ID_MODE_VIEW_TABLE,
    ID_MODE_VIEW_GRAPH
};


namespace MainWindow
{
    void Create();

    void Show(bool = true);

    wxEvtHandler &GetEventHandler();

    void SetTitleMenu(int id);

    void SetMeasure(uint id, const wxColour &color, uint8 type, float value);

    void OnEventChangedShowingMeasures();
}
