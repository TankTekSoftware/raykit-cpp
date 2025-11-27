#pragma once

#include "raykit/nodes/Timer.hpp"
#include <raykit/scene/Scene.hpp>
#include <raylib.h>

class ExampleScene : public Scene
{
    private:
        Timer* timer;
    private:
        Texture2D bricks_texture;
        float rotation;
        float speed;
    public:
        void on_compose() override;
        void on_dispose() override;
        void on_update(float delta) override;
        void on_draw() override;
    private:
        void on_timer_finished();
};
