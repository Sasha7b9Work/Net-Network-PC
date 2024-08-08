// 2024/08/08 21:19:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


template<class T>
struct Value
{
    Value(pchar _key, const T &def) : key(_key), value(def)
    {

    }
    void Set(T _value)
    {
        value = _value;
    }
    T Get() const
    {
        return value;
    }
private:
    pchar key;
    T value;
};
