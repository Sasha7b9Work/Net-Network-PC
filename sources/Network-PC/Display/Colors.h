// 2022/04/29 17:13:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Color
{
    enum E
    {
        BLACK,
        WHITE,
        _1,
        Count
    };

    E value;

    Color(E v) : value(v) {}

    void SetAsCurrent();
};


extern Color COLOR_1;
