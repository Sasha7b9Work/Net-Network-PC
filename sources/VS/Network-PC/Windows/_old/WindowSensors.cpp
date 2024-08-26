// 2024/08/08 14:46:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Sensors.h"
#include "Communicator/Server/Server.h"
#include "Communicator/HTTP/HTTP.h"
#include "Settings/Settings.h"
#include "Windows/WindowSensors.h"


WindowSensors *WindowSensors::self = nullptr;


void WindowSensors::Create(const wxSize &size)
{
    self = new WindowSensors(size);
}


WindowSensors::WindowSensors(const wxSize &size) :
    wxFrame(nullptr, wxID_ANY, _("Датчики"), wxDefaultPosition, size)
{
    wxTopLevelWindowMSW::Show();

    wxRect rect = SET::GUI::window_sensors.Get();

    SetPosition({ rect.x, rect.y });
    SetSize({ rect.width, rect.height });
}


WindowSensors::~WindowSensors()
{
    SET::GUI::window_sensors.Set({ GetPosition().x, GetPosition().y, GetSize().x, GetSize().y });

    self = nullptr;
}
