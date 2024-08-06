// 2022/8/4 13:37:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Timer.h"
#include <ctime>


uint Timer::CurrentTime()
{
    return (uint)clock();
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
