// 2022/09/12 14:15:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Clock.h"
#include <ctime>


String<> Time::ToString() const
{
    return String<>("%d:%02d", time.GetHour(), time.GetMinute());
}
