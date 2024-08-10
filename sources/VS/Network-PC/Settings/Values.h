// 2024/08/08 21:19:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Config.h"


template<class T>
struct Value
{
    Value(pchar _key, const T &_def) : key(_key), value(_def), def(_def)
    {

    }

    void SetDefault(const T &_def)
    {
        def = _def;
    }

    void Set(T _value)
    {
        value = _value;

        LoadSave(false);
    }

    T Get() const
    {
        return value;
    }

    void LoadSave(bool load)
    {
        if (load)
        {
            value = Config::Read(key, def);
        }
        else
        {
            Config::Write(key, value);
        }
    }

private:

    pchar key;
    T value;
    T def;
};


struct TimeScale : public Value<int>
{
    TimeScale(pchar _key, const int &_def) : Value(_key, _def) { }
};
