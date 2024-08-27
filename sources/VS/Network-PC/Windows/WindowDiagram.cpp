// 2024/08/26 13:39:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Windows/WindowDiagram.h"
#include "Display/Diagram/Diagram.h"
#include "Settings/Settings.h"
#include "MainWindow.h"


WindowDiagram *WindowDiagram::self = nullptr;


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
