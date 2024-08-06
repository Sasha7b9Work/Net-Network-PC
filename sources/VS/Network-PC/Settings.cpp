// 2022/09/28 08:32:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings.h"
#include "Display/Diagram/Diagram.h"


namespace Set
{
    namespace TimeScale
    {
        static int value = 1;
    }
}


void Set::TimeScale::Set(int scale)
{
    value = scale;

    Diagram::Pool::self->Refresh();
}


int Set::TimeScale::Get()
{
    return value;
}
