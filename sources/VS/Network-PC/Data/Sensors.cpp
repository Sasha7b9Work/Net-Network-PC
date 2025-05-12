// 2022/08/24 14:56:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Sensors.h"
#include "Display/Windows/MainWindow/MainWindow.h"
#include "Communicator/HTTP/HTTP.h"
#include "Utils/Timer.h"
#include <map>
#include <vector>


using namespace std;


map<uint, Sensor> Sensor::Pool::pool;


DataPoint DataPoint::null{ 0.0f, {0, 0, 0} };


wxString Measure::GetTitle() const
{
    static const pchar titles[Measure::Count][Lang::Count] =
    {
        { "Температура",       "Temperature" },
        { "Давление",          "Pressure" },
        { "Влажность",         "Humidity" },
        { "Точка росы",        "Dew point" },
        { "Скорость",          "Speed" },
        { "Широта",            "Latitude" },
        { "Долгота",           "Longitude" },
        { "Высота",            "Height" },
        { "Азимут",            "Azimuth" },
        { "Освещённость",      "Illuminate" },
        { "Дистанция",         "Distance" },
        { "Поворот",           "Rotate" },
        { "Поворот абс",       "Rotate abs" },
        { "Скорость поворота", "Rotate speed" }
    };

    return titles[type][SET::GUI::lang.Get()];
}


bool Measure::IsShown() const
{
    return SET::DIAGRAM::measure_vis[type].Get();
}


void Measure::SetShown(bool shown)
{
    SET::DIAGRAM::measure_vis[type].Set(shown);
}


int Measure::CountShowingMeasures()
{
    int result = 0;

    for (int i = 0; i < Count; i++)
    {
        if (Measure(i).IsShown())
        {
            result++;
        }
    }

    return result;
}


int Measure::NumColumn() const
{
    return type + 1;
}


wxString Measure::GetUnits() const
{
    static const pchar units[Measure::Count][Lang::Count] =
    {
        { "С",      "С"    },
        { "гПа",    "гПа"  },
        { "%",      "%"    },
        { "С",      "С"    },
        { "м/с",    "м/с"  },
        { "град",   "град" },
        { "град",   "град" },
        { "м",      "м"    },
        { "град",   "град" },
        { "лк",     "лк",  },
        { "м",      "m"    },
        { "град",   "град" },
        { "град",   "град" },
        { "град/с", "град/с"}
    };

    return units[type][SET::GUI::lang.Get()];
}


void Sensor::Pool::AppendMeasure(uint id, uint8 type, float value)
{
    static float values[Measure::Count] =
    {
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f
    };

    if (type == Measure::Temperature)
    {
        LOG_WRITE("Receive temperature %f from device %08X", value, id);
    }

    if (type >= Measure::Count)
    {
        LOG_ERROR("Unknown measure type %u", type);

        return;
    }

    if (id == 0xD5E0B863)
    {
        values[type] = value;
    }

    auto sensor = pool.find(id);

    if (sensor == pool.end())
    {
        pool.try_emplace(id, Sensor( id, Pool::ColorForSensor() ));
    }

    sensor = pool.find(id);

    if (sensor != pool.end())
    {
        sensor->second.AppendMeasure(type, value);

        MainWindow::SetMeasure(id, sensor->second.GetColor(), type, value);
    }

    static TimeMeterMS meter;
    static bool first = true;

    if (values[Measure::Temperature] != 0)
    {
        if ((meter.ElapsedTime() > (uint)(1000 * 60 * SET::NETWORK::time_http.Get())) || (first && meter.ElapsedTime() > 5000))
        {
            first = false;

            meter.Reset();

            if (id == 0xD5E0B863)
            {
                HTTP::SendPOST(101, values[Measure::Temperature], values[Measure::Humidity], values[Measure::Pressure], values[Measure::DewPoint], values[Measure::Illuminate]);
            }
            else
            {
                LOG_WRITE("Not device");
            }
        }
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
    if (type < Measure::Count)
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
