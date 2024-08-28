// 2022/05/05 09:30:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define LOG_WRITE(...)          Log::Write(__VA_ARGS__)
#define LOG_ERROR(...)          Log::Error(__VA_ARGS__)
#define LOG_ERROR_TRACE(...)

namespace Log
{
    void Init();
    void DeInit();
    void Write(char *format, ...);
    void Error(char *format, ...);
}
