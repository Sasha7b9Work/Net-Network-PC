// 2022/09/05 10:18:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include <map>


class Table : public wxGrid
{
public:

    static Table *Create(wxWindow *, const wxSize &);

    static Table *self;

    void SetMeasure(uint id, const wxColour &color, uint8 type, float value);

    int GetCreatedWidth() const { return create_width; }

    // Растянуть на ширину width
    void StretchEntireWidth(int width);

    void OnEventSize(ModeView::E);

private:

    Table(wxWindow *, const wxSize &);

    int create_width = 0;                   // Такую ширину имеет таблица во время создания

    //     <id, num_row>
    std::map<uint, int> rows;

    void SetCellValue(int row, int col, float, const wxColour &color);
    void SetCellValue(int row, int col, int, const wxColour &color);

    // Растянуть колонки на всю ширину
    void StretchColumns();
};