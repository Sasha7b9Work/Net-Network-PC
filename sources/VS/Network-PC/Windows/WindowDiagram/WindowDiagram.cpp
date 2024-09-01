// 2024/08/26 13:39:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/WindowDiagram/WindowDiagram.h"
#include "Windows/WindowDiagram/Diagram.h"


WindowDiagram *WindowDiagram::self = nullptr;


enum
{
    CONTEXT_FIRST_MEASURE = wxID_ABOUT,     // id меню первого измерения
    CONTEXT_LAST_MEASURE = CONTEXT_FIRST_MEASURE + Measure::Count,
    CONTEXT_SHOW_ALL,
    CONTEXT_HIDE_ALL
};


void WindowDiagram::Create()
{
    self = new WindowDiagram();
}


WindowDiagram::WindowDiagram() :
    wxFrame(nullptr, wxID_ANY, _("Графики"))
{
    Show(false);

    wxRect rect = SET::GUI::window_diagram.Get().rect;

    SetPosition({ rect.x, rect.y });
    SetSize({ rect.width, rect.height });

    Bind(wxEVT_SIZE, &WindowDiagram::OnEventSize, this);
    Bind(wxEVT_CLOSE_WINDOW, &WindowDiagram::OnEventClose, this);
    Bind(wxEVT_CONTEXT_MENU, &WindowDiagram::OnEventCreateContextMenu, this);
    Bind(wxEVT_MENU, &WindowDiagram::OnEventContextMenu, this);

    PoolDiagram::Create(this);

    Show(SET::GUI::window_diagram.Get().shown);

    if (SET::GUI::window_diagram.Get().maximized)
    {
        Maximize(true);
    }
}


WindowDiagram::~WindowDiagram()
{
    SET::GUI::window_diagram.Set({ { GetPosition().x, GetPosition().y, GetSize().x, GetSize().y }, IsShown(), IsMaximized()});

    PoolDiagram::Destroy();

    self = nullptr;
}


void WindowDiagram::Delete()
{
    delete self;
}


void WindowDiagram::UpdateArea()
{
    PoolDiagram::UpdateArea();
}


void WindowDiagram::OnEventSize(wxSizeEvent &event)
{
    PoolDiagram::OnEventSize();

    Layout();

    event.Skip();
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


void WindowDiagram::OnEventCreateContextMenu(wxContextMenuEvent &event)
{
    wxPoint point = event.GetPosition();

    ShowContextMenu(ScreenToClient(point));

    event.Skip();
}


void WindowDiagram::ShowContextMenu(const wxPoint &pos)
{
    wxMenu menu;

    for (int i = 0; i < Measure::Count; i++)
    {
        wxMenuItem *item = menu.Append(CONTEXT_FIRST_MEASURE + i, Measure(i).GetTitle());

        item->SetCheckable(true);
        item->Check(Measure(i).IsShown());
    }

    menu.Append(CONTEXT_SHOW_ALL, _("Показать все"));
    menu.Append(CONTEXT_HIDE_ALL, _("Скрыть все"));

    PopupMenu(&menu, pos);
}


void WindowDiagram::OnEventContextMenu(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id < CONTEXT_LAST_MEASURE)
    {
        Measure(id - CONTEXT_FIRST_MEASURE).SetShown(event.IsChecked());
    }
    else if (id == CONTEXT_SHOW_ALL)
    {
        for (int i = 0; i < Measure::Count; i++)
        {
            Measure(i).SetShown(true);
        }
    }
    else if (id == CONTEXT_HIDE_ALL)
    {
        for (int i = 0; i < Measure::Count; i++)
        {
            Measure(i).SetShown(false);
        }
    }

    PoolDiagram::OnEventSize();

    MainWindow::self->OnEventChangedShowingMeasures();

    event.Skip();
}
