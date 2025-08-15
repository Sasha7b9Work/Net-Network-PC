// 2022/05/05 09:30:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define LOG_WRITE(...)          Log::Write(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_ERROR(...)          Log::Error(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)


namespace Log
{
    void Init();
    void DeInit();
    void Write(char *file, int line, char *func, char *format, ...);
    void Error(char *file, int line, char *func, char *format, ...);
}
