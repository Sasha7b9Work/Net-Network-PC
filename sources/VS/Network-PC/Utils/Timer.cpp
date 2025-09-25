// 2022/8/4 13:37:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Timer.h"
#include <ctime>


uint Timer::CurrentTime()
{
    return (uint)clock();
}


wxString Timer::GetDateTime()
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


TimeMeterMS::TimeMeterMS()
{
    Reset();
}


void TimeMeterMS::Reset()
{
    time_reset = TIME_MS;
    time_pause = 0;
}


void TimeMeterMS::Pause()
{
    time_pause = TIME_MS;
}


void TimeMeterMS::Continue()
{
    time_reset += (TIME_MS - time_pause);
}


uint TimeMeterMS::ElapsedTime()
{
    return TIME_MS - time_reset;
}
