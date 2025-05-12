// 2022/08/24 14:57:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/ReceivedData.h"
#include "Data/Sensors.h"
#include "Utils/Math.h"
#include "Utils/RingBuffer.h"
#include <cstring>


namespace ReceivedData
{
    static RingBuffer <uint8, SIZE_RECEIVED_BUFFER> buffer;

    static bool ParseCommand(char bytes[SIZE_MESSAGE]);

    // ƒл€ старых устройств
    static bool ParseCommandOld(char bytes[SIZE_MESSAGE]);
}


void ReceivedData::Append(uint8 *data, int size)
{
    buffer.Append(data, size);
}


void ReceivedData::Update()
{
    if (buffer.GetElementCount() < 3)
    {
        return;
    }

    if (buffer[0] != 'A')
    {
        buffer.RemoveFirst(1);
        return;
    }

    if (buffer[1] != 'B')
    {
        buffer.RemoveFirst(2);
        return;
    }

    if (buffer[2] != 'C')
    {
        buffer.RemoveFirst(3);
        return;
    }

    if (buffer.GetElementCount() >= SIZE_MESSAGE)
    {
        char bytes[SIZE_MESSAGE];

        buffer.GetData((uint8 *)bytes, SIZE_MESSAGE);

        static int num_command = 0;
        num_command++;

        if (!ParseCommand(bytes))
        {
            static int prev_num = 0;

            LOG_ERROR("Can not parse command %d, parsed %d", num_command, num_command - prev_num);

            prev_num = num_command;

            // \todo удалить
            ParseCommandOld(bytes);     // ѕытаемс€ распарсить по старому формату передачи данных
        }
    }
}


bool ReceivedData::ParseCommand(char message[SIZE_MESSAGE])
{
    uint8 type = message[3];

    static int num_types[4] = { 0, 0, 0, 0 };

    if (type < 4)
    {
        num_types[type]++;

        LOG_WRITE("%d = %d, %d = %d, %d = %d, %d = %d", 0, num_types[0], 1, num_types[1], 2, num_types[2], 3, num_types[3]);
    }

    uint id;

    std::memcpy(&id, &message[4], 4);           // offset 4

    if (type == Measure::Temperature)
    {
        LOG_WRITE("Receive temperature from device %08X", id);
    }

    uint hash = 0;

    std::memcpy(&hash, &message[8], 4);        // offset 12

    float value = 0.0f;

    std::memcpy(&value, &message[12], 4);       // offset 16

    if (Math::CalculateHash(&value, 4) == hash)
    {
        Sensor::Pool::AppendMeasure(id, type, value);

        return true;
    }
    else
    {
        LOG_ERROR("Invalid hash for measure %u device %08X", type, id);
    }

    return false;
}


bool ReceivedData::ParseCommandOld(char message[SIZE_MESSAGE])
{
    uint8 type = message[3];

    uint id;

    std::memcpy(&id, &message[4], 4);

    if (type == Measure::Temperature)
    {
        LOG_WRITE("Receive temperature from device %08X", id);
    }

    float value = 0.0f;

    std::memcpy(&value, &message[16], 4);

    uint hash = 0;

    std::memcpy(&hash, &message[12], 4);

    if (Math::CalculateHash(&value, 4) == hash)
    {
        Sensor::Pool::AppendMeasure(id, type, value);

        return true;
    }
    else
    {
        LOG_ERROR("Invalid hash for measure %u device %08X", type, id);
    }

    return false;
}
