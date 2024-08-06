// 2022/09/05 08:52:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Diagram/Diagram.h"
#include "Display/Diagram/Canvas.h"
#include "Display/Table/Table.h"
#include "Utils/Clock.h"


Diagram::Pool *Diagram::Pool::self = nullptr;


Diagram::Diagram(wxWindow *parent, TypeMeasure::E type) : wxPanel(parent, wxID_ANY)
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


Diagram::Pool::Pool(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    for (int i = 0; i < TypeMeasure::Count; i++)
    {
        if (TypeMeasure::Exist((TypeMeasure::E)i))
        {
            pool[i] = new Diagram(this, (TypeMeasure::E)i);

            sizer->Add(pool[i]);
        }
        else
        {
            pool[i] = nullptr;
        }
    }

    SetSizer(sizer);
}


Diagram::Pool *Diagram::Pool::Create(wxWindow *parent)
{
    self = new Pool(parent);

    return self;
}


void Diagram::Pool::SetSizeArea(int width, int height)
{
    int dy = height / TypeMeasure::NumMeasures();

    for (Diagram *diagram : pool)
    {
        if (diagram)
        {
            diagram->SetSizeArea(width, dy);
        }
    }

    Refresh();
}


void Diagram::Pool::UpdateArea()
{
    Time time = Clock::CurrentTime();

    static int prev = time.sec;

    if (prev != time.sec)
    {
        prev = time.sec;

        Refresh();
    }
}


void Diagram::Pool::OnEventSize(ModeView::E mode)
{
    if (mode == ModeView::Full)
    {
        SetSizeArea(GetParent()->GetClientRect().width - Table::self->GetSize().x, GetParent()->GetClientRect().height);

        int x = Table::self->GetCreatedWidth();

        SetPosition({ x, 0 });
    }
    else if (mode == ModeView::Graph)
    {
        wxSize size = GetParent()->GetClientSize();

        SetSizeArea(size.x, size.y);
    }
}
