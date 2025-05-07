// 2022/09/05 08:47:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Data/Sensors.h"


/*
*   Здесь непосредственно отрисовка одного измерения
*/


class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent, Measure::E);

    ~Canvas();

    void SetSizeArea(int, int);

private:

    Measure::E type;

    void OnPaint(wxPaintEvent &);

    // Нарисовать измерения со всех датчиков
    void DrawAllSensors(wxMemoryDC &);

    // Нарисовать измерения от одного датчика
    void DrawSensor(wxMemoryDC &, const wxColour &, const DataArray &, float min, float max, float scale);

    void DrawTimeScale(wxMemoryDC &);

    void DrawTextOnBackground(wxMemoryDC &, pchar, int x, int y, int width, int height);

    // Преобразует время в координату х на графике
    int TimeToX(const Time &);
};
