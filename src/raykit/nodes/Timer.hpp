#pragma once

#include <functional>
class Timer 
{
    public: // -- Properties -- //
        float duration;
        std::function<void()> callback;
    private: // -- Utilities -- //
        float elapsed;
        bool running;
    public:
        Timer(float seconds = 0.0f);
    public:
        void start(float duration = -1.0f, std::function<void()> callback = nullptr);
        void stop();
        void update(float delta);
    public:
        bool is_finished() const;
        bool is_running() const;
        float get_elapsed() const;
        float get_duration() const;
        float get_remaining() const;
};
