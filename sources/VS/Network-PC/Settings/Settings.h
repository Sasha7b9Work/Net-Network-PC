// 2022/09/28 08:32:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Values.h"


struct WindowParameters
{
    wxRect rect;
    bool   shown;
    bool   maximized = false;
};


struct Lang
{
    enum E
    {
        RU,
        EN,
        Count
    };
};


namespace SET
{
    void Init();

    void DeInit();

    namespace DIAGRAM
    {
        extern TimeScale time_scale;        // Здесь хранится индекс настройки
    }

    namespace NETWORK
    {
        extern Value<wxString> host_ip;
        extern Value<int> host_port;
    }

    namespace GUI
    {
        extern Value<WindowParameters> window_main;
        extern Value<WindowParameters> window_diagram;
        extern Value<WindowParameters> window_log;
        extern Value<WindowParameters> window_scpi;

        extern Value<int, Lang::E> lang;
    }
}
