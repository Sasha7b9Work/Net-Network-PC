// 2024/08/31 19:53:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Windows/MainWindow/GridSensors.h"
#include "Data/Sensors.h"


GridSensors::GridSensors(wxWindow *parent, int id, const wxPoint &position, const wxSize &size) :
    wxGrid(parent, id, position, size)
{
    CreateGrid(0, 0);

    AppendCols(Measure::Count + 1);

    EnableEditing(false);

    DisableCellEditControl();

    SetRowLabelSize(0);

    SetColLabelValue(0, "ID");

    for (int meas = 0; meas < Measure::Count; meas++)
    {
        Measure measure(meas);

        int col = measure.NumColumn();

        if (col >= 0)
        {
            SetColLabelValue(col, measure.GetTitle() + "\n" + measure.GetUnits());
        }
    }

    Bind(wxEVT_SIZE, &GridSensors::OnEventSize, this);

    StretchColumns();
}


void GridSensors::StretchColumns()
{
    int width = GetSize().x;

    int size = width / (Measure::Count + 1);

    for (int i = 0; i < GetNumberCols(); i++)
    {
        SetColSize(i, size);

        width -= size;
    }

    SetColSize(GetNumberCols() - 1, width);
}


void GridSensors::OnEventSize(wxSizeEvent &)
{
    StretchColumns();
}
