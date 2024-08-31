// 2024/08/31 19:53:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Windows/MainWindow/GridSensors.h"
#include "Data/Sensors.h"


GridSensors::GridSensors(wxWindow *parent, int id, const wxPoint &position, const wxSize &size) :
    wxGrid(parent, id, position, size)
{
    CreateGrid(0, 0);

    AppendCols(Measure::NumMeasures() + 1);

    EnableEditing(false);

    DisableCellEditControl();

    SetRowLabelSize(0);

    SetColLabelValue(0, "ID");

    for (int meas = 0; meas < Measure::Count; meas++)
    {
        int col = Measure(meas).NumColumn();

        if (col >= 0)
        {
            Measure measure(meas);

            SetColLabelValue(measure.NumColumn(), measure.GetTitle() + "\n" + measure.GetUnits());
        }
    }

    Bind(wxEVT_SIZE, &GridSensors::OnEventSize, this);
}


void GridSensors::StretchColumns()
{
    int width = GetSize().x;

    int size = width / GetNumberCols();

    for (int i = 0; i < GetNumberCols() - 1; i++)
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
