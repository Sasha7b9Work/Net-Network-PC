// 2022/04/29 13:57:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Communicator/Communicator.h"
#include "Communicator/LAN/ServerTCP.h"
#include "Communicator/ComPort/ComPort.h"
#include "Data/ReceivedData.h"
#include "Utils/Clock.h"
#include "Display/Windows/WindowTerminal.h"
#include "Communicator/Server/Server.h"
#include "Utils/Timer.h"
#include "Communicator/HTTP/HTTP.h"
#include "Display/Windows/WindowDiagram/WindowDiagram.h"


using namespace std;


void Application::Update()
{
    Clock::Update();

    Communicator::Update();

    ReceivedData::Update();

    FrameDiagram::self->UpdateArea();

    HTTP::Update();
}
