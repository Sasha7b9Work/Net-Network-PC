// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Windows/WindowTerminal.h"
#include "Windows/WindowLog.h"
#include "Communicator/Server/Server.h"
#include "Communicator/HTTP/HTTP.h"
#include "MainWindow.h"
#include "Windows/WindowDiagram/WindowDiagram.h"


MainWindow *MainWindow::self = nullptr;


MainWindow::MainWindow(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP)
{
    Show(false);

    wxSize size = FromDIP(wxSize((Measure::NumMeasures() + 1) * 60, 400));

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
    menuView->Append(VIEW_TERMINAL, _(" "), _(" "));
    menuView->Append(VIEW_LOG, _(" "), _(" "));
    menuView->Append(VIEW_DIAGRAM, _(" "), _(" "));

    Bind(wxEVT_MENU, &MainWindow::OnMenuSettings, this);

    menuBar->Append(menuSettings, _("Настройки"));

    menuBar->Append(menuView, _("Вид"));

    wxFrameBase::SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnMenuView, this, VIEW_TERMINAL);
    Bind(wxEVT_MENU, &MainWindow::OnMenuView, this, VIEW_LOG);
    Bind(wxEVT_MENU, &MainWindow::OnMenuView, this, VIEW_DIAGRAM);
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnEventClose, this);

    Bind(wxEVT_SOCKET, &MainWindow::OnSocketEvent, this, SOCKET_ID);

    grid = new GridSensors(this, wxID_ANY, { 0, 0 }, size);

    SetClientSize(1024, 600);
    wxWindowBase::SetMinClientSize({ 400, 200 });

    wxRect rect = SET::GUI::window_main.Get().rect;

    if (SET::GUI::window_main.Get().maximized)
    {
        Maximize(true);
    }

    SetPosition({ rect.x, rect.y });
    SetSize({ rect.width, rect.height });

    SetTitleMenu(VIEW_TERMINAL);
    SetTitleMenu(VIEW_LOG);
    SetTitleMenu(VIEW_DIAGRAM);

    Show(true);
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

    Measure::E type_meas = (Measure::E)type;

    SetCellValue(row->second, Measure(type).NumColumn(), (float)value, color);

    ServerMeasures::Send(id, type_meas, (float)value);
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

    grid->StretchColumns();
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
        WindowTerminal::self->SwitchVisibility();
    }
    else if (id == VIEW_LOG)
    {
        WindowLog::self->SwitchVisibility();
    }
    else if (id == VIEW_DIAGRAM)
    {
        WindowDiagram::self->SwitchVisibility();
    }

    SetTitleMenu(id);
}


void MainWindow::SetTitleMenu(int id)
{
    if (id == VIEW_TERMINAL)
    {
        wxMenuItem *item = FindItemInMenuBar(VIEW_TERMINAL);

        item->SetItemLabel(WindowTerminal::self->IsShown() ? _("Скрыть терминал\tCtrl-K") : _("Показать терминла\tCtrl-K"));
    }
    else if (id == VIEW_LOG)
    {
        FindItemInMenuBar(VIEW_LOG)->SetItemLabel(WindowLog::self->IsShown() ? _("Скрыть лог\tCtrl-L") : _("Показать лог\tCtrl-L"));
    }
    else if (id == VIEW_DIAGRAM)
    {
        FindItemInMenuBar(VIEW_DIAGRAM)->SetItemLabel(WindowDiagram::self->IsShown() ? _("Скрыть графики\tCtrl-G") : _("Показать графики\tCtrl-G"));
    }
}


void MainWindow::OnSocketEvent(wxSocketEvent &event)
{
    ServerMeasures::CallbackOnSocketEvent(event);
}


void MainWindow::OnEventClose(wxCloseEvent &event)
{
    SET::GUI::window_main.Set({ { GetPosition().x, GetPosition().y, GetSize().x, GetSize().y }, true, IsMaximized()});

    delete WindowTerminal::self;

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
