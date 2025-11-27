#include <imgui.h>
#include <game/scenes/ExampleScene.hpp>
#include <iostream>

void ExampleScene::on_compose()
{
    // Code to execute when entering the scene
    this->bricks_texture = LoadTexture(ASSET_PATH "/textures/bricks.png");
    this->speed = 90.0f;

    this->timer = new Timer();

    this->timer->start(5.0f, [&]() { this->on_timer_finished(); });
}

void ExampleScene::on_dispose()
{
    // Code to execute when exiting the scene
    UnloadTexture(this->bricks_texture);

    delete this->timer;
}

void ExampleScene::on_update(float delta)
{
    // Update your scene logic here
    this->rotation += this->speed * delta; // Rotate 90 degrees per second
    this->timer->update(delta);
}

void ExampleScene::on_draw()
{
    // Draw your scene elements here
    DrawTextureEx(this->bricks_texture, { 400.0f, 300.0f }, this->rotation, 1.0f, WHITE);

    ImGui::Begin("Demo Debug");
        ImGui::SliderFloat("Degrees", &this->speed, 0.0f, 360.0f);        
    ImGui::End();

}

void ExampleScene::on_timer_finished()
{
    std::cout << "Timer finished!" << std::endl;
}
