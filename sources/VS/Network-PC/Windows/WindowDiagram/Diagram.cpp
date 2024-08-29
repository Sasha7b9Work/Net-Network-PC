// 2022/09/05 08:52:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Clock.h"
#include "Windows/WindowDiagram/Diagram.h"


PoolDiagram *PoolDiagram::self = nullptr;
Diagram *PoolDiagram::diagrams[Measure::Count];


Diagram::Diagram(wxWindow *parent, Measure::E type) : wxPanel(parent, wxID_ANY)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    canvas = new Canvas(this, type);

    sizer->Add(canvas);

    SetSizer(sizer);

    Bind(wxEVT_PAINT, &Diagram::OnPaint, this);
}


void Diagram::SetSizeArea(int width, int height)
{
    canvas->SetSizeArea(width, height);
}


PoolDiagram::PoolDiagram(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    self = this;

    for (int i = 0; i < Measure::Count; i++)
    {
        diagrams[i] = nullptr;
    }

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    SetSizer(sizer);

    {
        sizer->Clear();

        for (int i = 0; i < Measure::Count; i++)
        {
            if (diagrams[i])
            {
                delete diagrams[i];
                diagrams[i] = nullptr;
            }
        }
    }

    for (int i = 0; i < Measure::Count; i++)
    {
        if (Measure(i).IsShown())
        {
            diagrams[i] = new Diagram(self, (Measure::E)i);

            sizer->Add(diagrams[i]);
        }
        else
        {
            diagrams[i] = nullptr;
        }
    }
}


PoolDiagram *PoolDiagram::Create(wxWindow *parent)
{
    self = new PoolDiagram(parent);

    return self;
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

    self->Refresh();
}


void PoolDiagram::UpdateArea()
{
    Time time = Clock::CurrentTime();

    static int prev = time.sec;

    if (prev != time.sec)
    {
        prev = time.sec;

        self->Refresh();
    }
}


void PoolDiagram::OnEventSize()
{
    wxSize size = self->GetParent()->GetClientSize();

    SetSizeArea(size.x, size.y);
}
