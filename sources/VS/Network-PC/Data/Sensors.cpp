// 2022/08/24 14:56:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Sensors.h"
#include "Display/Windows/MainWindow/MainWindow.h"
#include "Communicator/HTTP/HTTP.h"
#include "Utils/Timer.h"
#include <map>
#include <vector>


using namespace std;


// Для статистики принятых данных
namespace StatServer
{
    static std::map<std::pair<uint, uint8>, int> counters;

    static void Append(uint id, uint8 type);

    static void Log();
}


vector<Sensor> Sensor::Pool::pool;


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
    StatServer::Append(id, type);

    if (type >= Measure::Count)
    {
        LOG_ERROR("Unknown measure type %u", type);

        return;
    }

    Sensor *sensor = Find(id);

    if (!sensor)                                                   // Создаём новый сенсор, если сообщения от такого ещё не приходили
    {
        Append(Sensor( id, Pool::ColorForSensor()));
    }

    sensor = Find(id);

    if (sensor)
    {
        sensor->AppendMeasure(type, value);                              // И добавляем в него измерение

        MainWindow::SetMeasure(id, sensor->GetColor(), type, value);
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

        if (measures[type].Size() && (point.time == measures[type].Last().time))
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
        LOG_ERROR("Bad type measure %d", type);
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


void StatServer::Append(uint id, uint8 type)
{
    int counter = counters[std::make_pair(id, type)];

    counter++;

    counters[std::make_pair(id, type)] = counter;

    static TimeMeterMS meter;

    if (meter.ElapsedTime() > 5000)
    {
        meter.Reset();

        Log();
    }
}


void StatServer::Log()
{
    for (const auto &pair : counters)
    {
        std::pair<uint, uint8> key = pair.first;

        int counter = pair.second;

        LOG_WRITE("%8X:%12s    %d", key.first, Measure(key.second).GetTitle().c_str().AsChar(), counter);
    }
}


Sensor *Sensor::Pool::First()
{
    if (pool.size())
    {
        return &pool[0];
    }

    return nullptr;
}


Sensor *Sensor::Pool::Next(Sensor *prev)
{
    for (uint i = 0; i < pool.size(); i++)
    {
        if (&pool[i] == prev && i != pool.size() - 1)
        {
            return &pool[i + 1];
        }
    }

    return nullptr;
}


Sensor *Sensor::Pool::Find(uint id)
{
    for (uint i = 0; i < pool.size(); i++)
    {
        if (pool[i].id == id)
        {
            return &pool[i];
        }
    }

    return nullptr;
}


void Sensor::Pool::Append(const Sensor &sensor)
{
    if (pool.size() == 0)
    {
        pool.reserve(1024);
    }

    for (uint i = 0; i < pool.size(); i++)
    {
        if (pool[i].id == sensor.id)
        {
            LOG_ERROR("Sensor with id %u already exist", sensor.id);
            return;
        }
    }

    pool.push_back(sensor);
}
