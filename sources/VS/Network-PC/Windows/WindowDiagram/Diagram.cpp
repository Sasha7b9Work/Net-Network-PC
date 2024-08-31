// 2022/09/05 08:52:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Clock.h"
#include "Windows/WindowDiagram/Diagram.h"


namespace PoolDiagram
{
    static Diagram *diagrams[Measure::Count];
    static wxFrame *frame = nullptr;                // На этом фрейме рисуются диаграммы

    static void UpdateArea();
}


Diagram::Diagram(wxWindow *parent, Measure::E type) : wxPanel(parent, wxID_ANY)
{
    canvas = new Canvas(parent, type);

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


void PoolDiagram::Create(wxFrame *parent)
{
    frame = parent;

    for (int i = 0; i < Measure::Count; i++)
    {
        diagrams[i] = new Diagram(frame, (Measure::E)i);
    }
}


void PoolDiagram::Destroy()
{
    for (int i = 0; i < Measure::Count; i++)
    {
        delete diagrams[i];
        diagrams[i] = nullptr;
    }
}


void PoolDiagram::SetSizeArea(int width, int height)
{
    int dy = height / Measure::NumMeasures();
    int y = 0;

    for (int i = 0; i < Measure::Count; i++)
    {
        Diagram *diagram = diagrams[i];

        if (Measure(i).IsShown())
        {
            diagram->SetSizeArea(width, dy);
            diagram->SetPosition({ 0, y });
            y += dy;
            diagram->Refresh();
        }
        else
        {
            diagram->SetSize(0, 0);
        }
    }

    frame->Refresh();
}


void PoolDiagram::UpdateArea()
{
    Time time = Clock::CurrentTime();

    static int prev = time.sec;

    if (prev != time.sec)
    {
        prev = time.sec;

        frame->Refresh();
    }
}


void PoolDiagram::OnEventSize()
{
    wxSize size = frame->GetClientSize();

    SetSizeArea(size.x, size.y);
}
