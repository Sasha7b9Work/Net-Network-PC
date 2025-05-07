// 2024/08/31 19:53:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Windows/MainWindow/GridSensors.h"
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

    int size = width / (Measure::CountShowingMeasures() + 1) - 1;

    for (int i = 0; i < GetNumberCols(); i++)
    {
        if (i == 0)
        {
            SetColSize(i, size);

            width -= size;
        }
        else if (Measure(i - 1).IsShown())
        {
            SetColSize(i, size);

            width -= size;
        }
        else
        {
            SetColSize(i, 0);
        }
    }

    for (int i = GetNumberCols() - 1; i > 0; i--)
    {
        if (GetColSize(i) != 0)
        {
            SetColSize(i, GetColSize(i) + width);
            break;
        }
    }
}


void GridSensors::OnEventSize(wxSizeEvent &)
{
    StretchColumns();
}
