// 2022/04/29 13:56:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"


class DiagramPool;


class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString &title);

    void OnAbout(wxCommandEvent &event);

    static MainWindow *self;

    void OnWebRequestState(wxWebRequestEvent &);

    void SetMeasure(uint id, const wxColour &color, uint8 type, float value);

    int GetCreatedWidth() const
    {
        return create_width;
    }

    // Растянуть на ширину width
    void StretchEntireWidth(int width);

    void OnEventSize();


private:

    wxToolBar *toolBar = nullptr;

    int create_width = 0;                   // Такую ширину имеет таблица во время создания

    //     <id, num_row>
    std::map<uint, int> rows;

    void SetCellValue(int row, int col, float, const wxColour &color);
    void SetCellValue(int row, int col, int, const wxColour &color);

    // Растянуть колонки на всю ширину
    void StretchColumns();

    wxGrid *grid = nullptr;             // Список датчиков

    void OnCloseWindow(wxCloseEvent &);

    void OnMenuTool(wxCommandEvent &);

    void OnMenuSettings(wxCommandEvent &);

    void OnSocketEvent(wxSocketEvent &);
};
