// 2024/08/26 13:39:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Windows/WindowDiagram.h"
#include "Display/Diagram/Diagram.h"
#include "Settings/Settings.h"
#include "MainWindow.h"


WindowDiagram *WindowDiagram::self = nullptr;


enum
{
    CONTEXT_FIRST_MEASURE = wxID_ABOUT      // id меню первого измерения
};


void WindowDiagram::Create()
{
    self = new WindowDiagram();
}


WindowDiagram::WindowDiagram() :
    wxFrame(nullptr, wxID_ANY, _("Графики"))
{
    Show(false);

    Bind(wxEVT_SIZE, &WindowDiagram::OnEventSize, this);
    Bind(wxEVT_CLOSE_WINDOW, &WindowDiagram::OnEventClose, this);
    Bind(wxEVT_CONTEXT_MENU, &WindowDiagram::OnEventContextMenu, this);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(Diagram::Pool::Create(this));

    SetSizer(sizer);

    wxRect rect = SET::GUI::window_diagram.Get().rect;

    SetPosition({ rect.x, rect.y });
    SetSize({ rect.width, rect.height });

    Show(SET::GUI::window_diagram.Get().shown);

    if (SET::GUI::window_diagram.Get().maximized)
    {
        Maximize(true);
    }
}


WindowDiagram::~WindowDiagram()
{
    SET::GUI::window_diagram.Set({ { GetPosition().x, GetPosition().y, GetSize().x, GetSize().y }, IsShown(), IsMaximized()});

    delete Diagram::Pool::self;

    Diagram::Pool::self = nullptr;

    self = nullptr;
}


void WindowDiagram::OnEventSize(wxSizeEvent &event)
{
    Diagram::Pool::self->OnEventSize();

    Layout();

    event.Skip();
}


void WindowDiagram::UpdateArea()
{
    if (self && self->IsShown())
    {
        Diagram::Pool::self->UpdateArea();
    }
}


void WindowDiagram::SwitchVisibility()
{
    Show(!IsShown());
}


void WindowDiagram::OnEventClose(wxCloseEvent &)
{
    Show(false);

    MainWindow::self->SetTitleMenu(VIEW_DIAGRAM);
}


void WindowDiagram::OnEventContextMenu(wxContextMenuEvent &event)
{
    wxPoint point = event.GetPosition();

    ShowContextMenu(ScreenToClient(point));

    event.Skip();
}


void WindowDiagram::ShowContextMenu(const wxPoint &pos)
{
    wxMenu menu;

    for (int i = 0; i < TypeMeasure::Count; i++)
    {
        wxMenuItem *item = menu.Append(CONTEXT_FIRST_MEASURE + i, TypeMeasure(i).GetTitle(SET::GUI::lang.Get()));

        item->SetCheckable(true);
        item->Check(TypeMeasure(i).IsShown());
    }

    PopupMenu(&menu, pos);
}
