// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Windows/WindowTerminal.h"
#include "Windows/WindowLog.h"
#include "Communicator/Server/Server.h"
#include "Communicator/HTTP/HTTP.h"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/WindowDiagram/WindowDiagram.h"
#include "Windows/WindowEmulator.h"


class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);

    void OnAbout(wxCommandEvent &event);

    void OnWebRequestState(wxWebRequestEvent &);

    void SetMeasure(uint id, const wxColour &color, uint8 type, float value);

    void SetTitleMenu(int id);

    void OnEventChangedShowingMeasures();

private:

    wxToolBar *toolBar = nullptr;

    //     <id, num_row>
    std::map<uint, int> rows;

    void SetCellValue(int row, int col, float, const wxColour &color);
    void SetCellValue(int row, int col, int, const wxColour &color);

    GridSensors *grid = nullptr;             // Список датчиков

    void OnEventClose(wxCloseEvent &);

    void OnMenuView(wxCommandEvent &);

    void OnMenuSettings(wxCommandEvent &);

    void OnSocketEvent(wxSocketEvent &);
};


namespace MainWindow
{
    static MainFrame *self = nullptr;
}


MainFrame::MainFrame(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP)
{
    Show(false);

    wxSize size = FromDIP(wxSize((Measure::Count + 1) * 60, 400));

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

    Bind(wxEVT_MENU, &MainFrame::OnMenuSettings, this);

    menuBar->Append(menuSettings, _("Настройки"));

    menuBar->Append(menuView, _("Вид"));

    wxFrameBase::SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnMenuView, this, VIEW_TERMINAL);
    Bind(wxEVT_MENU, &MainFrame::OnMenuView, this, VIEW_LOG);
    Bind(wxEVT_MENU, &MainFrame::OnMenuView, this, VIEW_DIAGRAM);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnEventClose, this);

    Bind(wxEVT_SOCKET, &MainFrame::OnSocketEvent, this, SOCKET_ID);

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

    Bind(wxEVT_WEBREQUEST_STATE, &MainFrame::OnWebRequestState, this);
}


void MainFrame::SetMeasure(uint id, const wxColour &color, uint8 type, float value)
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


void MainFrame::SetCellValue(int row, int col, float value, const wxColour &color)
{
    if (col >= 0)
    {
        grid->SetCellTextColour(row, col, color);

        grid->SetCellValue(row, col, wxString::Format("%10.2f", value));
    }
}


void MainFrame::SetCellValue(int row, int col, int value, const wxColour &color)
{
    if (col >= 0)
    {
        grid->SetCellTextColour(row, col, color);

        grid->SetCellValue(row, col, wxString::Format("%08X", value));
    }
}



void MainFrame::OnMenuSettings(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id >= ID_SPEED_1 && id <= ID_SPEED_60)
    {
        SET::DIAGRAM::time_scale.Set(event.GetId() - ID_SPEED_1);
    }
}


void MainFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
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


void MainFrame::OnMenuView(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == VIEW_TERMINAL)
    {
        WindowTerminal::self->SwitchVisibility();
    }
    else if (id == VIEW_LOG)
    {
        WindowLog::SwitchVisibility();
    }
    else if (id == VIEW_DIAGRAM)
    {
        FrameDiagram::self->SwitchVisibility();
    }

    SetTitleMenu(id);
}


void MainFrame::SetTitleMenu(int id)
{
    if (id == VIEW_TERMINAL)
    {
        wxMenuItem *item = FindItemInMenuBar(VIEW_TERMINAL);

        item->SetItemLabel(WindowTerminal::self->IsShown() ? _("Скрыть терминал\tCtrl-K") : _("Показать терминла\tCtrl-K"));
    }
    else if (id == VIEW_LOG)
    {
        FindItemInMenuBar(VIEW_LOG)->SetItemLabel(WindowLog::IsShown() ? _("Скрыть лог\tCtrl-L") : _("Показать лог\tCtrl-L"));
    }
    else if (id == VIEW_DIAGRAM)
    {
        FindItemInMenuBar(VIEW_DIAGRAM)->SetItemLabel(FrameDiagram::self->IsShown() ? _("Скрыть графики\tCtrl-G") : _("Показать графики\tCtrl-G"));
    }
}


void MainFrame::OnSocketEvent(wxSocketEvent &event)
{
    ServerMeasures::CallbackOnSocketEvent(event);
}


void MainFrame::OnEventClose(wxCloseEvent &event)
{
    SET::GUI::window_main.Set({ { GetPosition().x, GetPosition().y, GetSize().x, GetSize().y }, true, IsMaximized()});

//    WindowEmulator::Delete();

    delete WindowTerminal::self;

    delete FrameDiagram::self;

    Log::DeInit();

    event.Skip();
}


void MainFrame::OnWebRequestState(wxWebRequestEvent &event)
{
    switch (event.GetState())
    {
    case wxWebRequest::State_Completed:
    {
        LOG_WRITE(">>> REQUEST COMPLETE : %s", event.GetResponse().AsString().ToStdString().c_str());
        break;
    }

    case wxWebRequest::State_Failed:
        LOG_WRITE(">>> REQUEST FAILED : %s", event.GetErrorDescription().c_str().AsChar());
        break;

    case wxWebRequest::State_Cancelled:
        LOG_WRITE(">>> REQUEST CANCELLED : %s", event.GetResponse().AsString().ToStdString().c_str());
        break;

    case wxWebRequest::State_Unauthorized:
        LOG_WRITE(">>> REQUEST Unauthorized : %s", event.GetResponse().AsString().ToStdString().c_str());
        break;

    case wxWebRequest::State_Active:
        break;

    case wxWebRequest::State_Idle:
        LOG_WRITE(">>> REQUEST IDLE : %s", event.GetResponse().AsString().ToStdString().c_str());
        break;
    }
}


void MainFrame::OnEventChangedShowingMeasures()
{
    grid->StretchColumns();
}


void MainWindow::Create()
{
    self = new MainFrame(_("Датчики"));
}


void MainWindow::Show(bool show)
{
    self->Show(show);
}


wxEvtHandler &MainWindow::GetEventHandler()
{
    return *self;
}


void MainWindow::SetTitleMenu(int id)
{
    self->SetTitleMenu(id);
}


void MainWindow::SetMeasure(uint id, const wxColour &color, uint8 type, float value)
{
    self->SetMeasure(id, color, type, value);
}


void MainWindow::OnEventChangedShowingMeasures()
{
    self->OnEventChangedShowingMeasures();
}
