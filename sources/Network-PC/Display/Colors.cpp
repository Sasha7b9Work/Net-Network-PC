// 2022/04/29 17:13:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"


wxColour colors[Color::Count] =
{
    wxColour(0, 0, 0),
    wxColour(255, 255, 255),
    wxColour(200, 200, 200)
};


Color COLOR_1(Color::_1);


void Color::SetAsCurrent()
{
    if (value == Count)
    {
        return;
    }
}
