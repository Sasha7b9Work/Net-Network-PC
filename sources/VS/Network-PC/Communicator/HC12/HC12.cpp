// 2022/04/29 13:56:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/HC12/HC12.h"
#include "Communicator/RS232/rs232.h"
#include "Data/ReceivedData.h"


namespace HC12
{
    static int opened_port = -1;

    // „итает байты из всех возможных портов. ¬озвращает количество считанных байт
    static int ReadBytes();
}


void HC12::Init(int num_port)
{
    num_port--;

    if (RS232_OpenComport(num_port, 9600, "8N1", 0) == 0)
    {
        opened_port = num_port;
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

    if (opened_port != -1)
    {
        static const int SIZE_BUFFER = 1000;

        unsigned char buffer[SIZE_BUFFER];

        int num_bytes = RS232_PollComport(opened_port, buffer, SIZE_BUFFER);

        if (num_bytes != 0)
        {
            ReceivedData::Append(buffer, num_bytes);

            readed_bytes += num_bytes;
        }
    }

    return readed_bytes;
}
