// 2024/08/26 13:39:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Windows/WindowDiagram.h"
#include "Display/Diagram/Diagram.h"


WindowDiagram *WindowDiagram::self = nullptr;


void WindowDiagram::Create(const wxSize &size)
{
    self = new WindowDiagram(size);
}


WindowDiagram::WindowDiagram(const wxSize &size) :
    wxFrame(nullptr, wxID_ANY, _("Графики"), wxDefaultPosition, size)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(Diagram::Pool::Create(this));

    SetSizer(sizer);
}
