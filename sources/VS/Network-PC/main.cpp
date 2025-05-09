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

//    static TimeMeterMS meter;

//    if (meter.ElapsedTime() > 500)
//    {
//        meter.Reset();
//
//        for (int i = 0; i < Measure::Count; i++)
//        {
//            Sensor::Pool::AppendMeasure(2, (uint8)i, (float)rand());
//        }
//    }

//    {
//        static TimeMeterMS meter;
//
//        if (meter.ElapsedTime() > 5000)
//        {
//            meter.Reset();
//
//            float values[Measure::Count];
//
//            for (int i = 0; i < Measure::Count; i++)
//            {
//                values[i] = (float)(std::rand() % 100);
//            }
//
//            HTTP::SendPOST(values, Measure::Count);
//        }
//    }
}
