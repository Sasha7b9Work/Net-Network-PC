// 2022/08/25 08:50:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include "Display/Windows/WindowLog.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>


namespace Log
{
    static const int SIZE_BUFFER = 1024 * 10;

    static wxString file_name{ wxGetCwd() + ".log" };

    static wxTextFile log_file{ file_name };

    static wxString GetTime();

    // Ограничивает размер файла лога, чтобы не тормозил работу приложения
    static void CutSize();

    static void WriteLine(pchar);
}


void Log::Init()
{
    wxFile::Exists(file_name) ? log_file.Open() : log_file.Create();

    CutSize();

    WindowLog::Create();

    LOG_WRITE("\n\n *** Start application in %s ***", GetTime().c_str().AsChar());
}


void Log::DeInit()
{
    WindowLog::Delete();
}


void Log::WriteLine(pchar line)
{
    log_file.AddLine(line);

    WindowLog::AddLine(line);

    CutSize();

    log_file.Write();
}


void Log::Write(char *format, ...)
{
    char line[SIZE_BUFFER];
    char *pointer = line;
    std::va_list args;
    va_start(args, format);
    std::vsprintf(pointer, format, args);
    va_end(args);

    WriteLine(line);
}


void Log::Error(char *format, ...)
{
    char line[SIZE_BUFFER];
    std::strcpy(line, "!!! ERRROR !!!");
    char *pointer = line + std::strlen(line);
    std::va_list args;
    va_start(args, format);
    std::vsprintf(pointer, format, args);
    va_end(args);

    WriteLine(line);
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


wxString Log::GetTime()
{
    wxDateTime now = wxDateTime::Now();

    return wxString::Format("%02d:%02d:%02d-%02d:%02d:%02d",
        now.GetHour(),
        now.GetMinute(),
        now.GetSecond(),
        now.GetDay(),
        now.GetMonth(),
        now.GetYear()
    );
}
