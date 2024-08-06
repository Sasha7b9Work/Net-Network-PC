// 2022/08/24 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Clock.h"
#include <map>


struct TypeMeasure
{
    enum E
    {
        Temperature,    // Температура
        Pressure,       // Давление
        Humidity,       // Влажность
        DewPoint,       // Точка росы
        Velocity,       // Скорость воздуха
        Latitude,       // Широта
        Longitude,      // Долгота
        Altitude,       // Высота
        Azimuth,        // Азимут - угол от направления на север
        Illuminate,     // Освещённость
        Count
    };

    static pchar GetTitle(TypeMeasure::E);
    static pchar GetTitleEn(TypeMeasure::E);
    static pchar GetUnits(TypeMeasure::E);
    static bool Exist(TypeMeasure::E);
    // Возвращает количество присутствующих измерений
    static int NumMeasures();
    // Номер колонки для вывода
    static int NumColumn(TypeMeasure::E);
};


struct DataPoint
{
    DataPoint(float);
    DataPoint(float, const Time &);
    float value;
    Time time;
    static DataPoint null;
};


struct DataArray
{
    int Size() const { return (int)array.size(); }

    void PushBack(const DataPoint &point) { array.push_back(point); }

    DataPoint &Last() { return Size() ? *(array.end() - 1) : DataPoint::null; }

    // Рассчитывает мин/макс на from_end элементах с конца
    float Min(int from_end) const;
    float Max(int from_end) const;

    std::vector<DataPoint> array;
};


class Sensor
{
public:

    Sensor(uint _id, wxColour _color) : id(_id), color(_color) {};

    void AppendMeasure(uint8 type, float value);

    const DataArray &GetMeasures(TypeMeasure::E type) const { return measures[type]; }

    wxColour GetColor() const { return color; }

private:

    const uint id;

    const wxColour color;

    DataArray measures[TypeMeasure::Count];

public:

    struct Pool
    {
        static void AppendMeasure(uint id, uint8 type, float value);

        static const std::map<uint, Sensor> &GetPool() { return pool; };

        static int CountSensors() { return pool.size(); };

    private:

        static std::map<uint, Sensor> pool;

        static wxColour ColorForSensor();
    };
};
