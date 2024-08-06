// 2022/08/25 08:50:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include "Frame.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>


namespace Log
{
    static HANDLE handle = nullptr;

    static const int SIZE_BUFFER = 1024;
}


void Log::Init()
{
    AllocConsole();

    handle = GetStdHandle(STD_OUTPUT_HANDLE);
}


void Log::DeInit()
{
    FreeConsole();
}


void Log::Write(char *format, ...)
{
    char buffer[SIZE_BUFFER];
    char *pointer = buffer;
    std::va_list args;
    va_start(args, format);
    std::vsprintf(pointer, format, args);
    va_end(args);

    std::strcat(buffer, "\n");

    DWORD written_bytes = 0;

    WriteConsoleA(handle, buffer, std::strlen(buffer), &written_bytes, nullptr);
}


void Log::Error(char *format, ...)
{
    char buffer[SIZE_BUFFER];
    std::strcpy(buffer, "!!! ERRROR !!!");
    char *pointer = buffer + std::strlen(buffer);
    std::va_list args;
    va_start(args, format);
    std::vsprintf(pointer, format, args);
    va_end(args);

    std::strcat(buffer, "\n");

    DWORD written_bytes = 0;

    WriteConsoleA(handle, buffer, std::strlen(buffer), &written_bytes, nullptr);
}
