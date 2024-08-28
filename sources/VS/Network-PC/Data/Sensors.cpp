// 2022/08/24 14:56:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Sensors.h"
#include "Display/Diagram/Diagram.h"
#include "MainWindow.h"
#include <map>
#include <vector>


using namespace std;


map<uint, Sensor> Sensor::Pool::pool;


DataPoint DataPoint::null{ 0.0f, {0, 0, 0} };


bool TypeMeasure::is_shown[Count] =
{
    true,
    true,
    true,
    true,
    false,
    true,
    true,
    true,
    true,
    true
};


pchar TypeMeasure::GetTitle() const
{
    static const pchar titles[Count][Lang::Count] =
    {
        { "Температура",  "Temperature" },
        { "Давление",     "Pressure" },
        { "Влажность",    "Humidity" },
        { "Точка росы",   "Dew point" },
        { "Скорость",     "Speed" },
        { "Широта",       "Latitude" },
        { "Долгота",      "Longitude" },
        { "Высота",       "Height" },
        { "Азимут",       "Azimuth" },
        { "Освещённость", "Illuminate" }
    };

    return titles[type][SET::GUI::lang.Get()];
}


bool TypeMeasure::IsShown() const
{
    return is_shown[type];
}


void TypeMeasure::SetShown(bool shown)
{
    is_shown[type] = shown;
}


int TypeMeasure::NumMeasures()
{
    int result = 0;

    for (int i = 0; i < Count; i++)
    {
        if (TypeMeasure(i).IsShown())
        {
            result++;
        }
    }

    return result;
}


int TypeMeasure::NumColumn() const
{
    int result = 0;

    for (int i = 0; i < Count; i++)
    {
        if (type == (TypeMeasure::E)i)
        {
            if (IsShown())
            {
                break;
            }
            else
            {
                return -1;
            }
        }

        if (TypeMeasure(i).IsShown())
        {
            result++;
        }
    }

    return result + 1;
}


pchar TypeMeasure::GetUnits() const
{
    static const pchar units[Count][Lang::Count] =
    {
        { "С",    "С"    },
        { "гПа",  "гПа"  },
        { "%",    "%"    },
        { "С",    "С"    },
        { "м/с",  "м/с"  },
        { "град", "град" },
        { "град", "град" },
        { "м",    "м"    },
        { "град", "град" },
        { "лк",   "лк"   }
    };

    return units[type][SET::GUI::lang.Get()];
}


void Sensor::Pool::AppendMeasure(uint id, uint8 type, float value)
{
    auto sensor = pool.find(id);

    if (sensor == pool.end())
    {
        pool.emplace(pair<uint, Sensor>(id, Sensor(id, Pool::ColorForSensor())));
    }

    sensor = pool.find(id);

    if (sensor != pool.end())
    {
        sensor->second.AppendMeasure(type, value);

        MainWindow::self->SetMeasure(id, sensor->second.GetColor(), type, value);
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
