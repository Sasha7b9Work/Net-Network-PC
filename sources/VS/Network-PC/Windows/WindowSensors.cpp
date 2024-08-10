// 2024/08/08 14:46:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Sensors.h"
#include "Communicator/Server/Server.h"
#include "Communicator/HTTP/HTTP.h"
#include "Settings/Settings.h"
#include "Windows/WindowSensors.h"


WindowSensors *WindowSensors::self = nullptr;


void WindowSensors::Create(const wxSize &size)
{
    self = new WindowSensors(size);
}


WindowSensors::WindowSensors(const wxSize &size) :
    wxFrame(nullptr, wxID_ANY, _("Датчики"), wxDefaultPosition, size)
{
    create_width = size.x;

    grid = new wxGrid(this, wxID_ANY, { 0, 0 }, size);

    grid->CreateGrid(0, 0);

    grid->AppendCols(TypeMeasure::NumMeasures() + 1);

    grid->EnableEditing(false);

    grid->DisableCellEditControl();

    grid->SetRowLabelSize(0);

    grid->SetColLabelValue(0, "ID");

    for (int meas = 0; meas < TypeMeasure::Count; meas++)
    {
        int col = TypeMeasure::NumColumn((TypeMeasure::E)meas);

        if (col >= 0)
        {
            grid->SetColLabelValue(TypeMeasure::NumColumn((TypeMeasure::E)meas), wxString(TypeMeasure::GetTitle((TypeMeasure::E)meas)) +
                wxString("\n") + wxString(TypeMeasure::GetUnits((TypeMeasure::E)meas)));
        }
    }

    StretchColumns();

    wxTopLevelWindowMSW::Show();

    wxRect rect = SET::GUI::window_sensors.Get();

    SetPosition({ rect.x, rect.y });
    SetSize({ rect.width, rect.height });
}


WindowSensors::~WindowSensors()
{
    SET::GUI::window_sensors.Set({ GetPosition().x, GetPosition().y, GetSize().x, GetSize().y });
}


void WindowSensors::StretchColumns()
{
    int width = grid->GetSize().x;

    int size = width / grid->GetNumberCols();

    for (int i = 0; i < grid->GetNumberCols() - 1; i++)
    {
        grid->SetColSize(i, size);

        width -= size;
    }

    grid->SetColSize(grid->GetNumberCols() - 1, width);
}


void WindowSensors::SetMeasure(uint id, const wxColour &color, uint8 type, float value)
{
    if (id == 0)
    {
        return;
    }

    auto row = rows.find(id);

    if (row == rows.end())
    {
        grid->AppendRows(1);

        rows.emplace(std::pair<uint, int>(id, grid->GetNumberRows() - 1));

        SetCellValue(grid->GetNumberRows() - 1, 0, (int)id, color);
    }

    row = rows.find(id);

    TypeMeasure::E type_meas = (TypeMeasure::E)type;

    SetCellValue(row->second, TypeMeasure::NumColumn(type_meas), (float)value, color);

    ServerMeasures::Send(id, type_meas, (float)value);

    HTTP::SendPOST(type_meas, value);
}


void WindowSensors::SetCellValue(int row, int col, float value, const wxColour &color)
{
    if (col >= 0)
    {
        grid->SetCellTextColour(row, col, color);

        grid->SetCellValue(row, col, wxString::Format("%10.2f", value));
    }
}


void WindowSensors::SetCellValue(int row, int col, int value, const wxColour &color)
{
    if (col >= 0)
    {
        grid->SetCellTextColour(row, col, color);

        grid->SetCellValue(row, col, wxString::Format("%08X", value));
    }
}


void WindowSensors::StretchEntireWidth(int width)
{
    wxSize size = GetParent()->GetClientSize();

    size.x = width;

    SetMinSize(size);
    SetMaxSize(size);

    SetSize(size);

    StretchColumns();
}


void WindowSensors::OnEventSize()
{
    wxSize size = { GetSize().GetWidth(), GetParent()->GetClientSize().y };

    SetMinClientSize(size);
    SetClientSize(size);
    SetSize(size);

    StretchEntireWidth(create_width);
}
