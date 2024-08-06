// 2023/11/10 13:55:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace ComPort
{
    bool Open();

    bool IsOpened();

    void Close();

    int Receive(void *buffer, int size);

    void Send(pchar);
}
