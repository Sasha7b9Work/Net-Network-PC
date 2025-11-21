// 2024/08/08 20:35:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/StringUtils.h"


struct WindowParameters;


inline wxString wxToString(const wxRect &rect)
{
    return wxString::Format("%d %d %d %d", rect.x, rect.y, rect.width, rect.height);
}


inline bool wxFromString(const wxString &s, wxRect *rect)
{
    std::vector<wxString> words;

    SU::SplitToWords(s, words);

    if (words.size() == 4)
    {
        words[0].ToInt(&rect->x);
        words[1].ToInt(&rect->y);
        words[2].ToInt(&rect->width);
        words[3].ToInt(&rect->height);

        return true;
    }

    return false;
}


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

    void Write(pchar key, const WindowParameters &);

    WindowParameters Read(pchar key, const WindowParameters &def);

    extern wxConfigBase *config;
}
