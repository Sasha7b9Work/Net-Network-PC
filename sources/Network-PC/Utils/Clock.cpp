// 2022/09/12 14:15:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Clock.h"
#include <ctime>


using namespace std;


namespace Clock
{
    static Time current;
}


void Clock::Update()
{
    time_t now = time(0);

    tm *time = localtime(&now);

    current.sec = time->tm_sec;
    current.min = time->tm_min;
    current.hour = time->tm_hour;
}


Time &Clock::CurrentTime()
{
    return current;
}


String<> Time::ToString() const
{
    return String<>("%d:%02d", hour, min);
}


void Time::SubMin(int value)
{
    min -= value;

    if (min < 0)
    {
        min = 60 + min;

        hour--;
    }
}


const Time Time::operator-(const Time &rhs)
{
    Time result = *this;

    result.sec -= rhs.sec;

    if (result.sec < 0)
    {
        result.sec += 60;
        result.min -= 1;
    }

    result.min -= rhs.min;

    if (result.min < 0)
    {
        result.min += 60;
        result.hour -= 1;
    }

    result.hour -= rhs.hour;

    return result;
}


int Time::ToSec() const
{
    return sec + min * 60 + hour * 60 * 60;
}
