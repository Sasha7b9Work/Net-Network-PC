// 2022/04/29 13:56:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/HC12/HC12.h"
#include "Communicator/RS232/rs232.h"
#include "Data/ReceivedData.h"


namespace HC12
{
    static const int NUM_PORTS = 32;

    static bool opened[NUM_PORTS];

    // „итает байты из всех возможных портов. ¬озвращает количество считанных байт
    static int ReadBytes();
}


void HC12::Init()
{
    for (int i = 0; i < NUM_PORTS; i++)
    {
        opened[i] = (RS232_OpenComport(i, 9600, "8N1", 0) == 0);
    }
}


void HC12::Update()
{
    while (ReadBytes() != 0)
    {
    }
}


int HC12::ReadBytes()
{
    int readed_bytes = 0;

    for (int i = 0; i < NUM_PORTS; i++)
    {
        if (opened[i])
        {
            unsigned char buffer[100];

            int num_bytes = RS232_PollComport(i, buffer, 16);

            if (num_bytes != 0)
            {
                ReceivedData::Append(buffer, num_bytes);

                readed_bytes += num_bytes;
            }
        }
    }

    return readed_bytes;
}
