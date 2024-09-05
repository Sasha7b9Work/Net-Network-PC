// 2022/04/29 13:56:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "Windows/MainWindow/GridSensors.h"


class DiagramPool;


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

    ID_MODE_VIEW_FULL,
    ID_MODE_VIEW_TABLE,
    ID_MODE_VIEW_GRAPH
};


class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);

    void OnAbout(wxCommandEvent &event);

    static MainFrame *self;

    void OnWebRequestState(wxWebRequestEvent &);

    void SetMeasure(uint id, const wxColour &color, uint8 type, float value);

    void SetTitleMenu(int id);

    void OnEventChangedShowingMeasures();

private:

    wxToolBar *toolBar = nullptr;

    //     <id, num_row>
    std::map<uint, int> rows;

    void SetCellValue(int row, int col, float, const wxColour &color);
    void SetCellValue(int row, int col, int, const wxColour &color);

    GridSensors *grid = nullptr;             // Список датчиков

    void OnEventClose(wxCloseEvent &);

    void OnMenuView(wxCommandEvent &);

    void OnMenuSettings(wxCommandEvent &);

    void OnSocketEvent(wxSocketEvent &);
};
