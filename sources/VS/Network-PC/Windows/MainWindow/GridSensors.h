// 2024/08/31 19:53:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class GridSensors : public wxGrid
{
public:

    GridSensors(wxWindow *, int, const wxPoint &, const wxSize &);

    // Растянуть на всю ширину
    void StretchColumns();

private:

    void OnEventSize(wxSizeEvent &);
};
