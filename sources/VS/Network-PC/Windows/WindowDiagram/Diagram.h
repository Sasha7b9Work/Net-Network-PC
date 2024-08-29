// 2022/09/05 08:53:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Data/Sensors.h"
#include "Settings/Settings.h"
#include "Windows/WindowDiagram/Canvas.h"


// Здесь рисуется один график
class Diagram : public wxPanel
{
public:

    Diagram(wxWindow *parant, Measure::E);

    void SetSizeArea(int, int);

private:

    Canvas *canvas = nullptr;
};


// Здесь рисуются все графики
namespace PoolDiagram
{
    wxPanel *Create(wxWindow *);

    void Destroy();

    void SetSizeArea(int width, int height);

    void OnEventSize();
};
