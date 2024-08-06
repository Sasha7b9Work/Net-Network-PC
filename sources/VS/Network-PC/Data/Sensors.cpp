// 2022/08/24 14:56:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Sensors.h"
#include "Display/Table/Table.h"
#include "Display/Diagram/Diagram.h"
#include <map>
#include <vector>


using namespace std;


map<uint, Sensor> Sensor::Pool::pool;


DataPoint DataPoint::null{ 0.0f, {0, 0, 0} };


pchar TypeMeasure::GetTitle(TypeMeasure::E type)
{
    static const pchar titles[Count] =
    {
        "Температура",
        "Давление",
        "Влажность",
        "Точка росы",
        "Скорость",
        "Широта",
        "Долгота",
        "Высота",
        "Азимут",
        "Освещённость"
    };

    return titles[type];
}


pchar TypeMeasure::GetTitleEn(TypeMeasure::E type)
{
    static const pchar titles[Count] =
    {
        "Temperature",
        "Pressure",
        "Humidity",
        "Dew point",
        "Speed",
        "Latitude",
        "Longitude",
        "Height",
        "Azimuth",
        "Illuminate"
    };

    return titles[type];
}


bool TypeMeasure::Exist(TypeMeasure::E type)
{
    static const bool exists[Count] =
    {
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true
    };

    return exists[type];
}


int TypeMeasure::NumMeasures()
{
    int result = 0;

    for (int i = 0; i < Count; i++)
    {
        if (Exist((TypeMeasure::E)i))
        {
            result++;
        }
    }

    return result;
}


int TypeMeasure::NumColumn(TypeMeasure::E type)
{
    int result = 0;

    for (int i = 0; i < Count; i++)
    {
        if (type == (TypeMeasure::E)i)
        {
            if (Exist(type))
            {
                break;
            }
            else
            {
                return -1;
            }
        }

        if (Exist((TypeMeasure::E)i))
        {
            result++;
        }
    }

    return result + 1;
}


pchar TypeMeasure::GetUnits(TypeMeasure::E type)
{
    static const pchar units[Count] =
    {
        "С",
        "гПа",
        "%",
        "С",
        "м/с",
        "град",
        "град",
        "м",
        "град",
        "лк"
    };

    return units[type];
}


void Sensor::Pool::AppendMeasure(uint id, uint8 type, float value)
{
    auto sensor = pool.find(id);

    if (sensor == pool.end())
    {
        pool.emplace(pair<uint, Sensor>(id, Sensor(id, ColorForSensor())));
    }

    sensor = pool.find(id);

    if (sensor != pool.end())
    {
        sensor->second.AppendMeasure(type, value);

        Table::self->SetMeasure(id, sensor->second.GetColor(), type, value);
    }
}


wxColour Sensor::Pool::ColorForSensor()
{
    static const wxColour colors[3] =
    {
        wxColour(0, 0, 255),
        wxColour(255, 0, 0),
        wxColour(0, 255, 0)
    };

    return colors[pool.size() % 3];
}


void Sensor::AppendMeasure(uint8 type, float value)
{
    if (type < TypeMeasure::Count)
    {
        DataPoint point(value);

        if (measures[type].Size() && (measures[type].Last().time == point.time))
        {
            // В это время измерение уже получено - отбрасываем
        }
        else
        {
            measures[type].PushBack(point);
        }
    }
    else
    {
        LOG_ERROR_TRACE("Bad type measure %d", type);
    }
}


DataPoint::DataPoint(float _value) : value(_value), time(Clock::CurrentTime())
{
}


DataPoint::DataPoint(float _value, const Time &_time) : value(_value), time(_time)
{
}


float DataArray::Min(int from_end) const
{
    float result = 1e10f;

    int index = (int)array.size() - from_end;

    if (index < 0)
    {
        index = 0;
    }

    for (uint i = index; i < array.size(); i++)
    {
        if (array[i].value < result)
        {
            result = array[i].value;
        }
    }

    return result;
}


float DataArray::Max(int from_end) const
{
    float result = -1e10f;

    int index = (int)array.size() - from_end;

    if (index < 0)
    {
        index = 0;
    }

    for (uint i = index; i < array.size(); i++)
    {
        if (array[i].value > result)
        {
            result = array[i].value;
        }
    }

    return result;
}
