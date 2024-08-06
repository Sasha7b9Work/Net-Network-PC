// 2022/05/04 14:41:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace ServerTCP
{
    void Init(uint16 port);

    bool Opened();

    void Close();

    void Update();
};
