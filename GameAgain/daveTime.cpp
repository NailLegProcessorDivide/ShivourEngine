#include "daveTime.h"

double Timer::getTime()
{
    auto now = high_resolution_clock::now();
    return time_point_cast<microseconds>(now).time_since_epoch().count() / 1000000.0;
}

void Timer::init()
{
    lastTime = getTime();
}

double Timer::getLastTime()
{
    return lastTime;
}

float Timer::getElapsedTime()
{
    double now = getTime();
    float duration = (float)(now - lastTime);
    lastTime = now;
    return duration;
}