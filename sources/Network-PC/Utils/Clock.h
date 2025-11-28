// 2022/09/12 14:12:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"


class Time
{
public:

    Time(const wxDateTime &_time) : time(_time) { }

    String<> ToString() const;

private:

    wxDateTime time;
};
