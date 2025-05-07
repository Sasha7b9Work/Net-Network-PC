// 2024/08/26 13:39:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Windows/MainWindow/MainWindow.h"
#include "Windows/WindowDiagram/WindowDiagram.h"
#include "Data/Sensors.h"
#include "Windows/WindowDiagram/Diagram.h"


FrameDiagram *FrameDiagram::self = nullptr;


enum
{
    CONTEXT_FIRST_MEASURE = wxID_ABOUT,     // id меню первого измерения
    CONTEXT_LAST_MEASURE = CONTEXT_FIRST_MEASURE + Measure::Count,
    CONTEXT_SHOW_ALL,
    CONTEXT_HIDE_ALL
};


void FrameDiagram::Create()
{
    self = new FrameDiagram();
}


FrameDiagram::FrameDiagram() :
    wxFrame(nullptr, wxID_ANY, _("Графики"))
{
    Show(false);

    const WindowParameters params = SET::GUI::window_diagram.Get();

    wxRect rect = params.rect;

    SetPosition({ rect.x, rect.y });
    SetSize({ rect.width, rect.height });

    Bind(wxEVT_SIZE, &FrameDiagram::OnEventSize, this);
    Bind(wxEVT_CLOSE_WINDOW, &FrameDiagram::OnEventClose, this);
    Bind(wxEVT_CONTEXT_MENU, &FrameDiagram::OnEventCreateContextMenu, this);
    Bind(wxEVT_MENU, &FrameDiagram::OnEventContextMenu, this);

    PoolDiagram::Create(this);

    Show(params.shown);

    if (params.maximized)
    {
        Maximize(true);
    }
}


FrameDiagram::~FrameDiagram()
{
    SET::GUI::window_diagram.Set({ { GetPosition().x, GetPosition().y, GetSize().x, GetSize().y }, IsShown(), IsMaximized()});

    PoolDiagram::Destroy();

    self = nullptr;
}


void FrameDiagram::UpdateArea()
{
    if (self)
    {
        PoolDiagram::UpdateArea();
    }
}


void FrameDiagram::OnEventSize(wxSizeEvent &event)
{
    PoolDiagram::OnEventSize();

    Layout();

    event.Skip();
}


void FrameDiagram::SwitchVisibility()
{
    Show(!IsShown());
}


void FrameDiagram::OnEventClose(wxCloseEvent &)
{
    Show(false);

    MainWindow::SetTitleMenu(VIEW_DIAGRAM);
}


void FrameDiagram::OnEventCreateContextMenu(wxContextMenuEvent &event)
{
    wxPoint point = event.GetPosition();

    ShowContextMenu(ScreenToClient(point));

    event.Skip();
}


void FrameDiagram::ShowContextMenu(const wxPoint &pos)
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


void FrameDiagram::OnEventContextMenu(wxCommandEvent &event)
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

    MainWindow::OnEventChangedShowingMeasures();

    event.Skip();
}
