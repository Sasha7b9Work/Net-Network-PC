// 2024/08/08 21:19:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Values.h"


int TimeScale::SecsToPixel() const
{
    static const int scales[] = { 1, 2, 5, 30, 60, 60 * 15 };

    return scales[value];
}
