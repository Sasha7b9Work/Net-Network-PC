// 2024/6/21 11:40:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Data/Sensors.h"


/*
*   ����� � �������� �����������
*/


namespace HTTP
{
    // ������� �������� ��������� ��� ��� ��� ���� �� ������� � TypeMeasure
    void SendPOST(float *values, int count);
}
