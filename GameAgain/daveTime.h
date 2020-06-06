#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using namespace std::chrono;

class Timer
{
private:
    double lastTime;
public:
    void init();
    double getTime();
    double getLastTime();
    float getElapsedTime();
};
#endif