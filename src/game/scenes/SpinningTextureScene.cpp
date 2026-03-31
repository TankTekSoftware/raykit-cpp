#include "raylib.h"
#include <imgui.h>
#include <game/scenes/SpinningTextureScene.hpp>

void SpinningTextureScene::on_compose()
{
    // Code to execute when entering the scene
    this->bricks_texture = LoadTexture(ASSET_PATH "/textures/bricks.png");
    this->speed = 90.0f;
}

void SpinningTextureScene::on_dispose()
{
    // Code to execute when exiting the scene
    UnloadTexture(this->bricks_texture);
}

void SpinningTextureScene::on_update(float delta)
{
    // Update your scene's objects here.
    this->rotation += this->speed * delta; // Rotate 90 degrees per second
}

void SpinningTextureScene::on_draw()
{
    // Draw your scene elements here
    DrawTextureEx(this->bricks_texture, { 400.0f, 300.0f }, this->rotation, 1.0f, WHITE);
}
