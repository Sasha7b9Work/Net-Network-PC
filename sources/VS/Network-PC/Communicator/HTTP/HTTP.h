// 2024/6/21 11:40:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Data/Sensors.h"


/*
*   Связь с сервером Кузнечикова
*/


namespace HTTP
{
    // Послать значения измерений так как они идут по порядку в TypeMeasure
    void SendPOST(uint id, float temp, float humidity, float pressure, float dew_point, float illuminate);

    void SendPOST(uint id, float illuminate);
}
