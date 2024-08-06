// 2022/08/24 14:57:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/ReceivedData.h"
#include "Data/Sensors.h"
#include "Utils/DynamicBuffer.h"
#include "Utils/Math.h"
#include "Frame.h"
#include <cstring>


namespace ReceivedData
{
    static DynamicBuffer <SIZE_MESSAGE>buffer;

    static bool FindFirstABC();

    static bool ParseCommand(char bytes[20]);
}


void ReceivedData::Append(uint8 *data, int size)
{
    buffer.Append(data, size);
}


void ReceivedData::Update()
{
    while (buffer.Size() >= 20 && FindFirstABC())
    {
        char bytes[20];

        std::memcpy(bytes, buffer.Data(), 20);

        buffer.RemoveFirst(20);

        if (!ParseCommand(bytes))
        {
            static int counter = 0;
            LOG_ERROR("Can not parse command %d", counter++);
        }
    }
}


bool ReceivedData::FindFirstABC()
{
    DynamicBuffer <20>removed;

    int removed_bytes = 0;

    while (buffer.Size() >= 3)
    {
        if (std::memcmp("ABC", buffer.Data(), 3) == 0)
        {
            return true;
        }

        uint8 byte = buffer.Data()[0];

        removed.Append(&byte, 1);

        buffer.RemoveFirst(1);

        removed_bytes++;
    }

    if (removed_bytes != 0)
    {
        char data[128] = { 0 };

        int i = 0;

        for (; i < removed.Size(); i++)
        {
            data[i] = removed.Data()[i];
        }

        data[i] = 0;

        static int counter = 0;
        LOG_ERROR("error command %d %s", counter++, data);
    }

    return false;
}


bool ReceivedData::ParseCommand(char message[20])
{
    uint8 type = message[3];

    uint id;

    std::memcpy(&id, &message[4], 4);           // offset 4

    uint hash = 0;

    std::memcpy(&hash, &message[8], 4);        // offset 12

    float value = 0.0f;

    std::memcpy(&value, &message[12], 4);       // offset 16

    if (Math::CalculateHash(&value, 4) == hash)
    {
        Sensor::Pool::AppendMeasure(id, type, value);

        return true;
    }

    return false;
}
