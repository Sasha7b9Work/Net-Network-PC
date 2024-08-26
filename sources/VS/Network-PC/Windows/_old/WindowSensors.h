// 2024/08/08 14:46:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"

/*
*  Окно, где размещаются все обнаруженные датчики
*/


class WindowSensors : public wxFrame
{
public:

    virtual ~WindowSensors();

    static WindowSensors *self;

    static void Create(const wxSize &);

private:

    WindowSensors(const wxSize &);
};
