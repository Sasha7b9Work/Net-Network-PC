// 2024/08/08 21:19:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Config.h"


// class_cast - дополнительный необязательный тип, к которому будет приводится возвращаемое значение.
// Обычно кастуется int к перечислению
template<class T, class class_cast = T>
struct Value
{
    Value(pchar _key, const T &_def) : value(_def), key(_key), def(_def)
    {

    }

    virtual ~Value() { }

    void SetDefault(const T &_def)
    {
        def = _def;
    }

    void Set(T _value)
    {
        value = _value;

        LoadSave(false);
    }

    class_cast Get() const
    {
        return (class_cast)value;
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

protected:

    T value;

private:

    pchar key;
    T def;
};


struct TimeScale : public Value<int>
{
    TimeScale(pchar _key, const int &_def) : Value(_key, _def) { }

    int SecsToPixel() const;
};
