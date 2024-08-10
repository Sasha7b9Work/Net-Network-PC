// 2022/09/28 08:32:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Values.h"





namespace SET
{
    void Init();

    void DeInit();

    namespace DIAGRAM
    {
        extern TimeScale time_scale;
    }

    namespace NETWORK
    {
        extern Value<wxString> host_ip;
        extern Value<int> host_port;
    }

    namespace GUI
    {
        extern Value<wxRect> window_main;
        extern Value<wxRect> window_sensors;
        extern Value<wxRect> window_log;
        extern Value<wxRect> window_scpi;
    }
}
