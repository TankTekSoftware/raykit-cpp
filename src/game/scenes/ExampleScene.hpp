#pragma once

#include "toolbox/Scene.hpp"
#include <raylib.h>

class ExampleScene : public Scene
{
    private:
        Texture2D bricks_texture;
        float rotation;
    public:
        void on_compose() override;
        void on_dispose() override;
        void on_update(float delta) override;
        void on_draw() override;
};
