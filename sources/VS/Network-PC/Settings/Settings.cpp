// 2022/09/28 08:32:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Diagram/Diagram.h"


namespace SET
{
    namespace DIAGRAM
    {
        TimeScale time_scale{ "time_scale", 1 };

        static void LoadSave(bool load)
        {
            time_scale.LoadSave(load);
        }
    }

    namespace NETWORK
    {
        Value<wxString> host_ip{ "host_ip", "localhost" };
        Value<int>      host_port{ "host_port", 3333 };

        static void LoadSave(bool load)
        {
            host_ip.LoadSave(load);
            host_port.LoadSave(load);
        }
    }

    namespace GUI
    {
        Value<WindowParameters> window_main{ "gui_window_main", { { 0, 0, 320, 240 }, true} };
        Value<WindowParameters> window_diagram{ "gui_window_diagram", { { 0, 240, 320, 240 }, true} };
        Value<WindowParameters> window_log{ "gui_window_log", { { 320, 0, 320, 240 }, true } };
        Value<WindowParameters> window_scpi{ "gui_window_scpi", { { 320, 240, 320, 240 }, true } };

        static void LoadSave(bool load)
        {
            if (load)
            {
                wxSize d_size = wxGetDisplaySize();
                wxSize size{ d_size.GetWidth() / 2, d_size.GetHeight() / 2 };

                window_main.SetDefault({ { 0, 0, size.GetWidth(), size.GetHeight() }, true });
                window_diagram.SetDefault({ { 0, d_size.GetHeight() / 2, size.GetWidth(), size.GetHeight() }, true });
                window_log.SetDefault({ {d_size.GetWidth() / 2, 0, size.GetWidth(), size.GetHeight() }, true });
                window_scpi.SetDefault({ {d_size.GetWidth() / 2, d_size.GetHeight() / 2, size.GetWidth(), size.GetHeight() }, true });
            }

            window_main.LoadSave(load);
            window_diagram.LoadSave(load);
            window_log.LoadSave(load);
            window_scpi.LoadSave(load);
        }
    }

    static void LoadSave(bool load)
    {
        DIAGRAM::LoadSave(load);
        NETWORK::LoadSave(load);
        GUI::LoadSave(load);
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
