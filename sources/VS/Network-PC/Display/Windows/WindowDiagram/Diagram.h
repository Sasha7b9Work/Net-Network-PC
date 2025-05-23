﻿// 2022/09/05 08:53:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Data/Sensors.h"
#include "Settings/Settings.h"


// Здесь рисуются все графики
namespace PoolDiagram
{
    void Create(wxFrame *);

    void Destroy();

    void SetSizeArea(int width, int height);

    void OnEventSize();

    void UpdateArea();
};
