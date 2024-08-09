// 2022/09/28 08:32:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Diagram/Diagram.h"


namespace SET
{
    namespace DIAGRAM
    {
        Value<int> time_scale{ "time_scale", 1 };
    }

    namespace NETWORK
    {
        Value<wxString> host_ip{ "host_ip", "localhost" };
        Value<int>      host_port{ "host_port", 3333 };
    }
}


void SET::Init()
{

}


void SET::DeInit()
{

}
