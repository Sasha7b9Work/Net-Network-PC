// 2024/08/10 11:51:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/StringUtils.h"


void SU::SplitToWords(const wxString &string, std::vector<wxString> &words)
{
    words.clear();

    char buf[1024];

    std::strcpy(buf, string.c_str());

    char *token = std::strtok(buf, " ");

    while (token != nullptr)
    {
        words.push_back(token);
        token = std::strtok(nullptr, " ");
    }
}
