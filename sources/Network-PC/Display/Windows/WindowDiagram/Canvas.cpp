// 2022/09/05 08:47:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Sensors.h"
#include "Utils/Clock.h"
#include "Settings/Settings.h"
#include "Display/Windows/WindowDiagram/Canvas.h"


using namespace std;


Canvas::Canvas(wxWindow *parent, Measure::E _type) : wxPanel(parent, wxID_ANY),
    type(_type)
{
    Bind(wxEVT_PAINT, &Canvas::OnPaint, this);

    SetMinClientSize({ 100, 100 });
    SetClientSize(100, 100);
}


Canvas::~Canvas()
{
    Unbind(wxEVT_PAINT, &Canvas::OnPaint, this);
}


void Canvas::OnPaint(wxPaintEvent &)
{
    wxSize size = GetClientSize();

    if (size.x && size.y)
    {
        wxBitmap bitmap(size);
        wxMemoryDC memDC;

        memDC.SelectObject(bitmap);

        size = memDC.GetSize();

        memDC.SetBrush(*wxWHITE_BRUSH);
        memDC.SetPen(wxPen(wxColor(0, 0, 0)));

        memDC.DrawRectangle(GetClientRect());

        DrawTimeScale(memDC);

        DrawAllSensors(memDC);

        memDC.SetPen(wxPen(wxColor(0, 0, 0)));

        memDC.DrawText(Measure(type).GetTitle(), 1, 0);

        memDC.SelectObject(wxNullBitmap);

        wxPaintDC dc(this);

        dc.DrawBitmap(bitmap, 0, 0);

        size = dc.GetSize();

        size = bitmap.GetSize();

        Update();
    }
}


void Canvas::DrawTimeScale(wxMemoryDC &dc)
{
    int dx = 60;

    wxDateTime time = wxDateTime::Now();

    int x = GetClientSize().GetWidth() - time.GetSecond();

    time.SetSecond(0);

    int y = GetClientSize().GetHeight();

    dc.SetPen(wxPen(wxColor(200, 200, 200)));

    while (x > 0)
    {
        dc.DrawLine(x, 0, x, y);

        dc.DrawText(Time(time).ToString().c_str(), { x + 1, y - 15 });

        time = time.Subtract(wxTimeSpan::Seconds(SET::DIAGRAM::time_scale.SecsToPixel() * dx));

        x -= dx; 
    }
}


void Canvas::SetSizeArea(int width, int height)
{
    SetMinClientSize({ width, height } );
    SetClientSize({ width, height });
}


void Canvas::DrawAllSensors(wxMemoryDC &dc)
{
    Sensor *sensor = Sensor::Pool::First();

    if (!sensor)
    {
        return;
    }

    float min = 1e20f;
    float max = -1e20f;
    float scale = 1e4f;

    while (sensor)
    {
        const DataArray &measures = sensor->GetMeasures(type);

        if (measures.Size() > 1)
        {
            int width = GetClientSize().GetWidth();
            int height = GetClientSize().GetHeight();

            wxDateTime from_end = SET::DIAGRAM::time_scale.GetTimeSecondsBack(wxDateTime::Now(), width);

            float min_value = measures.Min(from_end);
            float max_value = measures.Max(from_end);

            if (fabsf(min_value - max_value) < 0.0001f)
            {
                sensor = Sensor::Pool::Next(sensor);

                continue;
            }

            if (min_value < min)
            {
                min = min_value;
            }

            if (max_value > max)
            {
                max = max_value;
            }

            float sc = (float)height / (max - min);

            if (sc < scale)
            {
                scale = sc;
            }
        }

        sensor = Sensor::Pool::Next(sensor);
    }

    sensor = Sensor::Pool::First();

    while (sensor)
    {
        const DataArray &measures = sensor->GetMeasures(type);

        if (measures.Size() > 1)
        {
            DrawSensor(dc, sensor->GetColor(), measures, min, max, scale);
        }

        sensor = Sensor::Pool::Next(sensor);
    }
}


void Canvas::DrawSensor(wxMemoryDC &dc, const wxColour &color, const DataArray &array, float min, float max, float scale)
{
    dc.SetPen(wxPen(color));

    if (fabsf(min - max) < 0.0001f)
    {
        return;
    }

    int width = GetClientSize().GetWidth();
    int height = GetClientSize().GetHeight();

    auto point = array.array.end() - 1;

    do
    {
        uint dx_end = SET::DIAGRAM::time_scale.GetDeltaPixels(wxDateTime::Now(), point->time);
        uint dx_start = SET::DIAGRAM::time_scale.GetDeltaPixels(wxDateTime::Now(), (point - 1)->time);

        int x_end = width - dx_end;
        int x_start = width - dx_start;

        int y_end = height - (int)((point->value - min) * scale);
        int y_start = height - (int)(((point - 1)->value - min) * scale);

        dc.DrawLine({ x_start, y_start }, { x_end, y_end });

        --point;

    } while (point > array.array.begin());

    dc.SetPen(wxPen(wxColor(0, 0, 0)));

    DrawTextOnBackground(dc, wxString::Format("%10.2f", max).c_str(), width - 50, 0, 50, 15);
    DrawTextOnBackground(dc, wxString::Format("%10.2f", min).c_str(), width - 50, height - 15, 50, 15);
}


void Canvas::DrawTextOnBackground(wxMemoryDC &dc, pchar text, int x, int y, int width, int height)
{
    dc.DrawRectangle({ x, y }, { width, height });
    dc.DrawText(text, { x + 1, y + 1 });
}
