#include "raykit/nodes/Timer.hpp"
#include "raylib.h"
#include <game/scenes/TimerScene.hpp>

void TimerScene::on_compose()
{
    this->clear_color = { 0, 0, 0 };

    this->timer = new Timer();
    this->timer->start(5.0f, [&]() { this->on_timer_timeout(); } );
}

void TimerScene::on_dispose()
{
    delete this->timer;
}

void TimerScene::on_update(float delta)
{
    this->timer->update(delta);
}

void TimerScene::on_draw()
{
    if(this->timer->is_running()) {
        DrawText("Timer Running...", 100, 100, 48, WHITE);
    } else {
        DrawText("Timer Finished!", 100, 100, 48, WHITE);
    }
    ClearBackground(this->clear_color);
}

void TimerScene::on_timer_timeout()
{
    this->clear_color = { 255, 0, 0 };
}
