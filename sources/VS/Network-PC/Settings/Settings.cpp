// 2022/09/28 08:32:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Diagram/Diagram.h"


namespace SET
{
    static void LoadSave(bool load);

    namespace DIAGRAM
    {
        static void LoadSave(bool load);

        Value<int> time_scale{ "time_scale", 1 };
    }

    namespace NETWORK
    {
        static void LoadSave(bool load);

        Value<wxString> host_ip{ "host_ip", "localhost" };
        Value<int>      host_port{ "host_port", 3333 };
    }
}


void SET::Init()
{
    LoadSave(true);
}


void SET::DeInit()
{
    LoadSave(false);
}


void SET::LoadSave(bool load)
{
    DIAGRAM::LoadSave(load);
    NETWORK::LoadSave(load);
}


void SET::DIAGRAM::LoadSave(bool load)
{
    time_scale.LoadSave(load);
}


void SET::NETWORK::LoadSave(bool load)
{
    host_ip.LoadSave(load);
    host_port.LoadSave(load);
}
