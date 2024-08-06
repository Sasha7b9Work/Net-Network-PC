// 2024/04/29 08:36:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Data/Sensors.h"


// —юда будем посылать измерени€
namespace ServerMeasures
{
    void DeInit();

    void Update();

    void Send(uint id, TypeMeasure::E, float value);

    void CallbackOnSocketEvent(wxSocketEvent &);
}
