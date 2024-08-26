// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Diagram/Diagram.h"
#include "Display/Diagram/Canvas.h"
#include "Settings/Settings.h"
#include "Windows/ConsoleSCPI.h"
#include "Windows/ConsoleLog.h"
#include "Communicator/Server/Server.h"
#include "Windows/WindowDiagram.h"
#include "Communicator/HTTP/HTTP.h"
#include "MainWindow.h"


MainWindow *MainWindow::self = nullptr;


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
    TOOL_DATABASE,

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


MainWindow::MainWindow(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP)
{
    wxSize size = FromDIP(wxSize((TypeMeasure::NumMeasures() + 1) * 60, 400));

    create_width = size.x;

    self = this;

    SetIcon(wxICON(MAIN_ICON));

    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(FILE_QUIT, _("Выход\tAlt-X"), _("Закрыть окно программы"));
    menuBar->Append(menuFile, _("Файл"));

    wxMenu *menuSettings = new wxMenu();
    wxMenu *menuSpeed = new wxMenu();

    menuSpeed->Append(new wxMenuItem(menuSpeed, ID_SPEED_1, "1 сек", wxEmptyString, wxITEM_RADIO));
    menuSpeed->Append(new wxMenuItem(menuSpeed, ID_SPEED_2, "2 сек", wxEmptyString, wxITEM_RADIO));
    menuSpeed->Append(new wxMenuItem(menuSpeed, ID_SPEED_5, "5 сек", wxEmptyString, wxITEM_RADIO));
    menuSpeed->Append(new wxMenuItem(menuSpeed, ID_SPEED_30, "30 сек", wxEmptyString, wxITEM_RADIO));
    menuSpeed->Append(new wxMenuItem(menuSpeed, ID_SPEED_60, "60 сек", wxEmptyString, wxITEM_RADIO));

    menuSettings->AppendSubMenu(menuSpeed, _("Скорость обновления"));

    wxMenu *menuView = new wxMenu();
    menuView->Append(VIEW_TERMINAL, _("Открыть терминал\tCtrl-K"), _("Открыть терминал"));
//    menuTools->Append(TOOL_DATABASE, _("База данных\tCtrl-D"), _("База данных"));

    Bind(wxEVT_MENU, &MainWindow::OnMenuSettings, this);

    menuBar->Append(menuSettings, _("Настройки"));

    menuBar->Append(menuView, _("Вид"));

//    menuBar->Append(menuTools, _("Инструменты"));

    wxFrameBase::SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnMenuView, this, VIEW_TERMINAL);
    Bind(wxEVT_MENU, &MainWindow::OnMenuView, this, TOOL_DATABASE);
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnCloseWindow, this);

    Bind(wxEVT_SOCKET, &MainWindow::OnSocketEvent, this, SOCKET_ID);

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

    SetClientSize(1024, 600);
    wxWindowBase::SetMinClientSize({ 800, 300 });

    ConsoleSCPI::Create();

    wxRect rect = SET::GUI::window_main.Get();

    SetPosition({ rect.x, rect.y });
    SetSize({ rect.width, rect.height });
}


void MainWindow::StretchColumns()
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


void MainWindow::SetMeasure(uint id, const wxColour &color, uint8 type, float value)
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


void MainWindow::SetCellValue(int row, int col, float value, const wxColour &color)
{
    if (col >= 0)
    {
        grid->SetCellTextColour(row, col, color);

        grid->SetCellValue(row, col, wxString::Format("%10.2f", value));
    }
}


void MainWindow::SetCellValue(int row, int col, int value, const wxColour &color)
{
    if (col >= 0)
    {
        grid->SetCellTextColour(row, col, color);

        grid->SetCellValue(row, col, wxString::Format("%08X", value));
    }
}



void MainWindow::OnMenuSettings(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id >= ID_SPEED_1 && id <= ID_SPEED_60)
    {
        SET::DIAGRAM::time_scale.Set(event.GetId() - ID_SPEED_1);
    }
}


void MainWindow::StretchEntireWidth(int width)
{
    wxSize size = GetParent()->GetClientSize();

    size.x = width;

    SetMinSize(size);
    SetMaxSize(size);

    SetSize(size);

    StretchColumns();
}


void MainWindow::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxBoxSizer *topsizer;
    wxDialog dlg(this, wxID_ANY, wxString(_("About")));

    topsizer = new wxBoxSizer(wxVERTICAL);

#if wxUSE_STATLINE
    topsizer->Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
#endif // wxUSE_STATLINE

    wxButton *bu1 = new wxButton(&dlg, wxID_OK, _("OK"));
    bu1->SetDefault();

    topsizer->Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);

    dlg.SetSizer(topsizer);
    topsizer->Fit(&dlg);

    dlg.ShowModal();
}


void MainWindow::OnMenuView(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == VIEW_TERMINAL)
    {
        ConsoleSCPI::self->SwitchVisibility();

        FindItemInMenuBar(VIEW_TERMINAL)->SetItemLabel(ConsoleSCPI::self ->IsShown() ? _("Закрыть консоль") : _("Открыть консоль"));
    }
    else if (id == TOOL_DATABASE)
    {

    }
}


void MainWindow::OnSocketEvent(wxSocketEvent &event)
{
    ServerMeasures::CallbackOnSocketEvent(event);
}


void MainWindow::OnCloseWindow(wxCloseEvent &event)
{
    SET::GUI::window_main.Set({ GetPosition().x, GetPosition().y, GetSize().x, GetSize().y });

    delete ConsoleSCPI::self;

    delete WindowDiagram::self;

    Log::DeInit();

    event.Skip();
}


void MainWindow::OnWebRequestState(wxWebRequestEvent &event)
{
    switch (event.GetState())
    {
    case wxWebRequest::State_Completed:
    {
        LOG_WRITE(">>> RESPONSE : %s", event.GetResponse().AsString().ToStdString().c_str());
        break;
    }

    case wxWebRequest::State_Failed:
        break;

    case wxWebRequest::State_Cancelled:
        break;

    case wxWebRequest::State_Unauthorized:
        break;

    case wxWebRequest::State_Active:
        break;

    case wxWebRequest::State_Idle:
        break;
    }
}


void MainWindow::OnEventSize()
{
    wxSize size = { GetSize().GetWidth(), GetParent()->GetClientSize().y };

    SetMinClientSize(size);
    SetClientSize(size);
    SetSize(size);

    StretchEntireWidth(create_width);
}
