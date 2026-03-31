#pragma once

#include "raykit/nodes/Timer.hpp"
#include "raykit/scene/Scene.hpp"
#include "raylib.h"

class TimerScene : public Scene
{
    private:
        Timer* timer;
        Color clear_color;
    public:
        void on_compose() override;
        void on_dispose() override;
        void on_update(float delta) override;
        void on_draw() override;
    private:
        void on_timer_timeout();
};
