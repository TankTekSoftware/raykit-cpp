#include "toolbox/SceneManager.hpp"
#include <game/Game.hpp>
#include <raylib.h>
#include <game/scenes/ExampleScene.hpp>

Game::Game()
{
    this->window_config = { 0 };
    this->window_config.width = 800;
    this->window_config.height = 600;
    this->window_config.title = "raykit-cpp - Starter Template";
    this->window_config.target_fps = 60;
}

void Game::on_compose()
{
    // Example of adding a scene to the SceneManager
    SceneManager::add_scene("EXAMPLE", std::make_unique<ExampleScene>());


    SceneManager::change_scene("EXAMPLE");
}

void Game::on_dispose()
{
    // Example of removing a scene from the SceneManager
    SceneManager::remove_scene("EXAMPLE");
}

void Game::on_update(float delta)
{
    // Update your game logic here
}

void Game::on_draw()
{
    // Draw your game elements here
}

void Game::run()
{
    InitWindow(this->window_config.width, this->window_config.height, this->window_config.title.c_str());
    InitAudioDevice();
    SetTargetFPS(this->window_config.target_fps);

    this->on_compose();

    while(!WindowShouldClose()) {
        float delta = GetFrameTime();
        SceneManager::on_update(delta);
        this->on_update(delta);

        BeginDrawing();
            ClearBackground(BLACK);
            SceneManager::on_draw();
            this->on_draw();
        EndDrawing();
    }

    this->on_dispose();

    CloseAudioDevice();
    CloseWindow();
}
