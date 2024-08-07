// 2022/08/25 08:50:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include "Frame.h"
#include "Controls/ConsoleLog.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>


namespace Log
{
    static const int SIZE_BUFFER = 1024;
}


void Log::Init()
{
    ConsoleLog::Create();
}


void Log::DeInit()
{
    delete ConsoleLog::self;
}


void Log::Write(char *format, ...)
{
    char buffer[SIZE_BUFFER];
    char *pointer = buffer;
    std::va_list args;
    va_start(args, format);
    std::vsprintf(pointer, format, args);
    va_end(args);

    ConsoleLog::self->AddLine(buffer);
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

    ConsoleLog::self->AddLine(buffer);
}
