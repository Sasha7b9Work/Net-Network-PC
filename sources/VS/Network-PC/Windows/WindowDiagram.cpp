// 2024/08/26 13:39:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Windows/WindowDiagram.h"
#include "Display/Diagram/Diagram.h"
#include "Settings/Settings.h"


WindowDiagram *WindowDiagram::self = nullptr;


void WindowDiagram::Create()
{
    self = new WindowDiagram();
}


WindowDiagram::WindowDiagram() :
    wxFrame(nullptr, wxID_ANY, _("Графики"))
{
    Bind(wxEVT_SIZE, &WindowDiagram::OnEventSize, this);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(Diagram::Pool::Create(this));

    SetSizer(sizer);

    wxRect rect = SET::GUI::window_diagram.Get().rect;

    SetPosition({ rect.x, rect.y });
    SetSize({ rect.width, rect.height });

    Show(SET::GUI::window_diagram.Get().shown);

//    Bind(wxEVT_CLOSE_WINDOW, &WindowDiagram::OnEventClose, this);
}


WindowDiagram::~WindowDiagram()
{
    SET::GUI::window_diagram.Set({ { GetPosition().x, GetPosition().y, GetSize().x, GetSize().y }, IsShown() });

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


//void WindowDiagram::OnEventClose(wxCloseEvent &event)
//{
//    SET::GUI::window_diagram.Set({ GetPosition().x, GetPosition().y, GetSize().x, GetSize().y });
//
//    event.Skip();
//}


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
