// 2022/8/4 13:37:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define TIME_MS Timer::CurrentTime()


namespace Timer
{
    uint CurrentTime();
}



// Структура для отсчёта времени
struct TimeMeterMS
{
    TimeMeterMS();

    // Установить момент отсчёта
    void Reset();

    void Pause();

    void Continue();

    // Столько миллисекунд прошло с момента вызова Reset()
    uint ElapsedTime();

private:

    uint time_reset;        // От этого времени отсчитывается ElapsedTime()
    uint time_pause;        // В этот момент поставили на паузу
};
