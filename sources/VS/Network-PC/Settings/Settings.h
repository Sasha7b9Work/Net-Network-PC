﻿// 2022/09/28 08:32:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Values.h"
#include "Data/Sensors.h"


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
        extern TimeScale time_scale;                        // Здесь хранится индекс настройки
        extern Value<bool> measure_vis[Measure::Count];
    }

    // Коммуникации
    namespace COM
    {
        extern Value<int>       port_hc12;                  // К этому порту подключён hc12, получается
    }

    namespace NETWORK
    {
        extern Value<wxString>  host_ip;
        extern Value<int>       host_port;
        extern Value<int>       time_http;                  // Через столько минут засылаем в сервер
        extern Value<bool>      send_to_http;               // Посылать ли на сервер HTTP
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
