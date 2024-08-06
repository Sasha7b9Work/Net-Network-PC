// 2022/09/05 10:18:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Table/Table.h"
#include "Data/Sensors.h"
#include "Communicator/Server/Server.h"
#include "Communicator/HTTP/HTTP.h"


using namespace std;


Table *Table::self = nullptr;


Table *Table::Create(wxWindow *parent, const wxSize &size)
{
    self = new Table(parent, size);

    return self;
}


Table::Table(wxWindow *parent, const wxSize &size) :
    wxGrid(parent, wxID_ANY, { 0, 0 }, size)
{
    create_width = size.x;

    CreateGrid(0, 0);

    AppendCols(TypeMeasure::NumMeasures() + 1);

    EnableEditing(false);

    DisableCellEditControl();

    SetRowLabelSize(0);

    SetColLabelValue(0, "ID");

    for (int meas = 0; meas < TypeMeasure::Count; meas++)
    {
        int col = TypeMeasure::NumColumn((TypeMeasure::E)meas);

        if (col >= 0)
        {
            SetColLabelValue(TypeMeasure::NumColumn((TypeMeasure::E)meas), wxString(TypeMeasure::GetTitle((TypeMeasure::E)meas)) +
                wxString("\n") + wxString(TypeMeasure::GetUnits((TypeMeasure::E)meas)));
        }
    }

    StretchColumns();

//    wxScrollHelperBase::SetScrollbars(20, 20, 5, 5);
}


void Table::StretchColumns()
{
    int width = wxGrid::GetSize().x;

    int size = width / GetNumberCols();

    for (int i = 0; i < GetNumberCols() - 1; i++)
    {
        SetColSize(i, size);

        width -= size;
    }

    SetColSize(GetNumberCols() - 1, width);
}


void Table::SetMeasure(uint id, const wxColour &color, uint8 type, float value)
{
    if (id == 0)
    {
        return;
    }

    auto row = rows.find(id);

    if (row == rows.end()) 
    {
        AppendRows(1);

        rows.emplace(pair<uint, int>(id, GetNumberRows() - 1));

        SetCellValue(GetNumberRows() - 1, 0, (int)id, color);
    }

    row = rows.find(id);

    TypeMeasure::E type_meas = (TypeMeasure::E)type;

    SetCellValue(row->second, TypeMeasure::NumColumn(type_meas), (float)value, color);

    ServerMeasures::Send(id, type_meas, (float)value);

    HTTP::SendPOST(type_meas, value);
}


void Table::SetCellValue(int row, int col, float value, const wxColour &color)
{
    if (col >= 0)
    {
        SetCellTextColour(row, col, color);

        wxGrid::SetCellValue(row, col, wxString::Format("%10.2f", value));
    }
}


void Table::SetCellValue(int row, int col, int value, const wxColour &color)
{
    if (col >= 0)
    {
        SetCellTextColour(row, col, color);

        wxGrid::SetCellValue(row, col, wxString::Format("%08X", value));
    }
}


void Table::StretchEntireWidth(int width)
{
    wxSize size = GetParent()->GetClientSize();

    size.x = width;

    SetMinSize(size);
    SetMaxSize(size);

    SetSize(size);

    StretchColumns();
}


void Table::OnEventSize(ModeView::E mode)
{
    if (mode == ModeView::Full)
    {
        wxSize size = { GetSize().GetWidth(), GetParent()->GetClientSize().y };

        Table::self->SetMinClientSize(size);
        Table::self->SetClientSize(size);
        Table::self->SetSize(size);

        StretchEntireWidth(create_width);
    }
    else if (mode == ModeView::Table)
    {
        StretchEntireWidth(GetParent()->GetClientSize().x);
    }
}
