// 2022/08/25 08:50:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include "Display/Windows/WindowLog.h"
#include "Utils/StringUtils.h"
#include "Utils/Timer.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>


namespace Log
{
    static const int SIZE_BUFFER = 1024 * 10;
    static int counter = 0;

    static wxString file_name{ wxGetCwd() + "\\Network-PC.log" };

    static wxTextFile log_file{ file_name };

    // Ограничивает размер файла лога, чтобы не тормозил работу приложения
    static void CutSize();

    static void WriteLine(pchar);
}


void Log::Init()
{
    wxFile::Exists(file_name) ? log_file.Open() : log_file.Create();

    CutSize();

    WindowLog::Create();

    LOG_WRITE("                   *** Start application in %s ***", Timer::GetDateTime().c_str().AsChar());
}


void Log::DeInit()
{
    LOG_WRITE("                   *** Exit application in %s ***\n", Timer::GetDateTime().c_str().AsChar());

    WindowLog::Delete();
}


void Log::WriteLine(pchar line)
{
    log_file.AddLine(line);

    WindowLog::AddLine(line);

    CutSize();

    log_file.Write();
}


void Log::Write(char *file, int line, char *func, char *format, ...)
{
    char message[SIZE_BUFFER];
    char *pointer = message;
    std::va_list args;
    va_start(args, format);
    std::vsprintf(pointer, format, args);
    va_end(args);

    char place[SIZE_BUFFER];
    std::sprintf(place, "%s:%s:%3d", file, func, line);

    char log_message[SIZE_BUFFER];
    std::sprintf(log_message, "%3d:%s: %s : %s", ++counter, Timer::GetDateTime().c_str().AsChar(), SU::LeaveTheLastOnes(place, 30), message);

    WriteLine(log_message);
}


void Log::Error(char *file, int line, char *func, char *format, ...)
{
    char message[SIZE_BUFFER];
    std::strcpy(message, "!!! ERRROR !!!");
    char *pointer = message + std::strlen(message);
    std::va_list args;
    va_start(args, format);
    std::vsprintf(pointer, format, args);
    va_end(args);

    char place[SIZE_BUFFER];
    std::sprintf(place, "%s:%s:%3d", file, func, line);

    char log_message[SIZE_BUFFER];
    std::sprintf(log_message, "%3d : %s : !!! ERROR !!! %s", ++counter, SU::LeaveTheLastOnes(place, 30), message);

    WriteLine(log_message);
}


void Log::CutSize()
{
    if (log_file.GetLineCount() > 10000)
    {
        while (log_file.GetLineCount() > 9500)
        {
            log_file.RemoveLine(0);
        }
    }
}
