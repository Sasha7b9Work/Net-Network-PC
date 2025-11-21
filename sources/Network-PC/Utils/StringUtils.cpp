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


pchar SU::LeaveTheLastOnes(pchar in, int number)
{
    int size = (int)std::strlen(in);

    if (size >= number)
    {
        return in + size - number;
    }

    static char buffer[1024];

    char *pointer = buffer;

    int num_spaces = number - size;

    for (int i = 0; i < num_spaces; i++)
    {
        *pointer++ = ' ';
    }

    *pointer = '\0';

    std::strcat(buffer, in);

    return buffer;
}
