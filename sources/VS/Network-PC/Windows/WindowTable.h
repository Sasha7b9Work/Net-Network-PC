// 2024/08/08 14:46:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"

/*
*  Окно, где размещаются все обнаруженные датчики
*/


class WindowTable : public wxGrid
{
public:

    static WindowTable *self;

    static void Create(const wxSize &);

    void SetMeasure(uint id, const wxColour &color, uint8 type, float value);

    int GetCreatedWidth() const { return create_width; }

    // Растянуть на ширину width
    void StretchEntireWidth(int width);

    void OnEventSize(ModeView::E);

private:

    WindowTable(const wxSize &);

    int create_width = 0;                   // Такую ширину имеет таблица во время создания

    //     <id, num_row>
    std::map<uint, int> rows;

    void SetCellValue(int row, int col, float, const wxColour &color);
    void SetCellValue(int row, int col, int, const wxColour &color);

    // Растянуть колонки на всю ширину
    void StretchColumns();
};
