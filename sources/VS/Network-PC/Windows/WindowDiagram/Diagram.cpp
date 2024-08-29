// 2022/09/05 08:52:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Clock.h"
#include "Windows/WindowDiagram/Diagram.h"


namespace PoolDiagram
{
    static Diagram *diagrams[Measure::Count];
    static wxPanel *panel = nullptr;

    static void UpdateArea();
}


Diagram::Diagram(wxWindow *parent, Measure::E type) : wxPanel(parent, wxID_ANY)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    canvas = new Canvas(this, type);

    sizer->Add(canvas);

    SetSizer(sizer);

    Bind(wxEVT_PAINT, &Diagram::OnPaint, this);
}


Diagram::~Diagram()
{
    Unbind(wxEVT_PAINT, &Diagram::OnPaint, this);

    delete canvas;
}


void Diagram::SetSizeArea(int width, int height)
{
    canvas->SetSizeArea(width, height);
}


wxPanel *PoolDiagram::Create(wxWindow *parent)
{
    panel = new wxPanel(parent, wxID_ANY);

    for (int i = 0; i < Measure::Count; i++)
    {
        diagrams[i] = nullptr;
    }

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    panel->SetSizer(sizer);

    for (int i = 0; i < Measure::Count; i++)
    {
        if (Measure(i).IsShown())
        {
            diagrams[i] = new Diagram(panel, (Measure::E)i);

            sizer->Add(diagrams[i]);
        }
        else
        {
            diagrams[i] = nullptr;
        }
    }

    return panel;
}


void PoolDiagram::Destroy()
{
    for (int i = 0; i < Measure::Count; i++)
    {
        if (diagrams[i])
        {
            delete diagrams[i];
            diagrams[i] = nullptr;
        }
    }

    delete panel;

    panel = nullptr;
}


void PoolDiagram::SetSizeArea(int width, int height)
{
    int dy = height / Measure::NumMeasures();

    for (Diagram *diagram : diagrams)
    {
        if (diagram)
        {
            diagram->SetSizeArea(width, dy);
        }
    }

    panel->Refresh();
}


void PoolDiagram::UpdateArea()
{
    Time time = Clock::CurrentTime();

    static int prev = time.sec;

    if (prev != time.sec)
    {
        prev = time.sec;

        if (panel)
        {
            panel->Refresh();
        }
    }
}


void PoolDiagram::OnEventSize()
{
    wxSize size = panel->GetParent()->GetClientSize();

    SetSizeArea(size.x, size.y);
}
