// 2024/08/08 20:35:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Config
{
    void Init();

    void DeInit();

    template<class T>
    T Read(pchar key, const T &def)
    {
        T result = def;

        config->Read(key, &result, def);

        return result;
    }

    template<class T>
    void Write(pchar key, const T &value)
    {
        config->Write(key, value);
    }

    extern wxConfigBase *config;
}
