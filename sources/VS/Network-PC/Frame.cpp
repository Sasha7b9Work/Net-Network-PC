// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "Display/Diagram/Diagram.h"
#include "Display/Table/Table.h"
#include "Display/Diagram/Canvas.h"
#include "Settings.h"
#include "Controls/ConsoleSCPI.h"
#include "Communicator/Server/Server.h"


Frame *Frame::self = nullptr;


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

    TOOL_CONSOLE,
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


Frame::Frame(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title)
{
    self = this;

    SetIcon(wxICON(MAIN_ICON));

    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(FILE_QUIT, _("Выход\tAlt-X"), _("Закрыть окно программы"));
    menuBar->Append(menuFile, _("Файл"));

    wxMenu *menuSettings = new wxMenu();
    wxMenu *menuSpeed = new wxMenu();
    wxMenu *menuModeView = new wxMenu();

    menuSpeed->Append(new wxMenuItem(menuSpeed, ID_SPEED_1, "1 сек", wxEmptyString, wxITEM_RADIO));
    menuSpeed->Append(new wxMenuItem(menuSpeed, ID_SPEED_2, "2 сек", wxEmptyString, wxITEM_RADIO));
    menuSpeed->Append(new wxMenuItem(menuSpeed, ID_SPEED_5, "5 сек", wxEmptyString, wxITEM_RADIO));
    menuSpeed->Append(new wxMenuItem(menuSpeed, ID_SPEED_30, "30 сек", wxEmptyString, wxITEM_RADIO));
    menuSpeed->Append(new wxMenuItem(menuSpeed, ID_SPEED_60, "60 сек", wxEmptyString, wxITEM_RADIO));

    menuModeView->Append(new wxMenuItem(menuModeView, ID_MODE_VIEW_FULL, "Полный", wxEmptyString, wxITEM_RADIO));
    menuModeView->Append(new wxMenuItem(menuModeView, ID_MODE_VIEW_TABLE, "Таблица", wxEmptyString, wxITEM_RADIO));
    menuModeView->Append(new wxMenuItem(menuModeView, ID_MODE_VIEW_GRAPH, "График", wxEmptyString, wxITEM_RADIO));

    menuSettings->AppendSubMenu(menuSpeed, _("Скорость обновления"));
    menuSettings->AppendSubMenu(menuModeView, _("Вид"));

    //    wxMenu *menuTools = new wxMenu();
    //    menuTools->Append(TOOL_CONSOLE, _("Открыть консоль\tCtrl-K"), _("Открыть консоль"));
    //    menuTools->Append(TOOL_DATABASE, _("База данных\tCtrl-D"), _("База данных"));

    Bind(wxEVT_MENU, &Frame::OnMenuSettings, this);

    menuBar->Append(menuSettings, _("Настройки"));

    //    menuBar->Append(menuTools, _("Инструменты"));

    wxFrameBase::SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Frame::OnMenuTool, this, TOOL_CONSOLE);
    Bind(wxEVT_MENU, &Frame::OnMenuTool, this, TOOL_DATABASE);
    Bind(wxEVT_CLOSE_WINDOW, &Frame::OnCloseWindow, this);

    Bind(wxEVT_SIZE, &Frame::OnSize, this);

    Bind(wxEVT_SOCKET, &Frame::OnSocketEvent, this, SOCKET_ID);

    sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(Table::Create(this, FromDIP(wxSize((TypeMeasure::NumMeasures() + 1) * 60, 400))));

    sizer->Add(Diagram::Pool::Create(this));

    SetSizer(sizer);

    SetClientSize(1024, 600);
    wxWindowBase::SetMinClientSize({ 800, 300 });

    ConsoleSCPI::Self()->SwitchVisibility();

    SetModeView(mode_view);
}


void Frame::OnMenuSettings(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id >= ID_SPEED_1 && id <= ID_SPEED_60)
    {
        static const int scales[] = { 1, 2, 5, 30, 60 };

        Set::TimeScale::Set(scales[event.GetId() - ID_SPEED_1]);
    }
    else if (id >= ID_MODE_VIEW_FULL && id <= ID_MODE_VIEW_GRAPH)
    {
        SetModeView((ModeView::E)(id - ID_MODE_VIEW_FULL));
    }
}


void Frame::SetModeView(ModeView::E mode)
{
    mode_view = mode;

    sizer->Show(Table::self);
    sizer->Show(Diagram::Pool::self);

    if (mode_view == ModeView::Table)
    {
        sizer->Hide(Diagram::Pool::self);
    }
    else if (mode_view == ModeView::Graph)
    {
        sizer->Hide(Table::self);
    }

    Table::self->OnEventSize(mode);
    Diagram::Pool::self->OnEventSize(mode);

    Layout();
}


void Frame::OnSize(wxSizeEvent &event)
{
    Table::self->OnEventSize(mode_view);
    Diagram::Pool::self->OnEventSize(mode_view);

    Layout();

    event.Skip();
}


void Frame::OnAbout(wxCommandEvent &WXUNUSED(event))
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


void Frame::OnMenuTool(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == TOOL_CONSOLE)
    {
        ConsoleSCPI::Self()->SwitchVisibility();

        FindItemInMenuBar(TOOL_CONSOLE)->SetItemLabel(ConsoleSCPI::Self()->IsShown() ? _("Закрыть консоль") : _("Открыть консоль"));
    }
    else if (id == TOOL_DATABASE)
    {

    }
}


void Frame::OnSocketEvent(wxSocketEvent &event)
{
    ServerMeasures::CallbackOnSocketEvent(event);
}


void Frame::OnCloseWindow(wxCloseEvent &event)
{
    delete ConsoleSCPI::Self();

    event.Skip();
}


void Frame::OnWetRequestState(wxWebRequestEvent &event)
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