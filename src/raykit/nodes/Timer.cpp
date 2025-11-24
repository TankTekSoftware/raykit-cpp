#include <functional>
#include <raykit/nodes/Timer.hpp>

Timer::Timer(float seconds)
{
    this->duration = seconds;
    this->elapsed = 0.0f;
    this->running = false;
}

void Timer::start(float duration, std::function<void()> callback)
{
    if(duration > 0.0f) {
        this->duration = duration;
    }

    if(callback) {
        this->callback = callback;
    }

    this->elapsed = 0.0f;
    this->running = true;
}

void Timer::stop()
{
    this->running = false;
}

void Timer::update(float delta)
{
    if(this->running) {
        this->elapsed += delta;
        if(this->elapsed >= this->duration) {
            this->running = false;
            if(this->callback) {
                this->callback();
            }
        }
    }
}

bool Timer::is_finished() const
{
    return (!this->running && this->elapsed >= this->duration);
}

bool Timer::is_running() const
{
    return this->running;
}

float Timer::get_elapsed() const
{
    return this->elapsed;
}

float Timer::get_duration() const
{
    return this->duration;
}

float Timer::get_remaining() const
{
    return (this->duration - this->elapsed);
}














