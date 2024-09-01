// 2022/09/05 08:52:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Clock.h"
#include "Windows/WindowDiagram/Diagram.h"


namespace PoolDiagram
{
    static Canvas *diagrams[Measure::Count];
    static wxFrame *frame = nullptr;                // На этом фрейме рисуются диаграммы
}


void PoolDiagram::Create(wxFrame *parent)
{
    frame = parent;

    for (int i = 0; i < Measure::Count; i++)
    {
        diagrams[i] = new Canvas(frame, (Measure::E)i);
    }
}


void PoolDiagram::Destroy()
{
    for (int i = 0; i < Measure::Count; i++)
    {
        delete diagrams[i];
        diagrams[i] = nullptr;
    }

    frame = nullptr;
}


void PoolDiagram::SetSizeArea(int width, int height)
{
    if (Measure::CountShowingMeasures() == 0)
    {
        for (int i = 0; i < Measure::Count; i++)
        {
            diagrams[i]->Show(false);
        }
    }
    else
    {
        int dy = height / Measure::CountShowingMeasures();
        int y = 0;

        for (int i = 0; i < Measure::Count; i++)
        {
            Canvas *diagram = diagrams[i];

            if (Measure(i).IsShown())
            {
                diagram->Show(true);
                diagram->SetSizeArea(width, dy);
                diagram->SetPosition({ 0, y });
                y += dy;
                diagram->Refresh();
            }
            else
            {
                diagram->Show(false);
            }
        }
    }

    frame->Refresh();
}


void PoolDiagram::UpdateArea()
{
    if (!frame)
    {
        return;
    }

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
