#include "game/scenes/TimerScene.hpp"
#include <memory>
#include <raykit/scene/SceneManager.hpp>
#include <game/Game.hpp>
#include <raylib.h>
#include <game/scenes/SpinningTextureScene.hpp>

#include <imgui.h>
#include <rlImGui.h>

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
    SceneManager::add_scene("SPINNING_TEXTURE", std::make_unique<SpinningTextureScene>());
    SceneManager::add_scene("TIMER_SCENE", std::make_unique<TimerScene>());

    SceneManager::change_scene("SPINNING_TEXTURE");
}

void Game::on_dispose()
{
    // Example of removing a scene from the SceneManager
    SceneManager::remove_scene("SPINNING_TEXTURE");
    SceneManager::remove_scene("TIMER_SCENE");
}

void Game::on_update(float delta)
{
    // Update your game logic here
}

void Game::on_draw()
{
    // Draw your game elements here
    ImGui::Begin("Scene Manager");
        if(ImGui::Button("Spinning Texture Example")) {
            SceneManager::change_scene("SPINNING_TEXTURE"); 
        }
        if(ImGui::Button("Timer Example")) {
            SceneManager::change_scene("TIMER_SCENE");
        }
    ImGui::End();
}

void Game::run()
{
    InitWindow(this->window_config.width, this->window_config.height, this->window_config.title.c_str());
    SetExitKey(KEY_NULL);
    InitAudioDevice();
    SetTargetFPS(this->window_config.target_fps);
    
    rlImGuiSetup(true);

    this->on_compose();

    while(!WindowShouldClose()) {
        float delta = GetFrameTime();
        SceneManager::on_update(delta);
        this->on_update(delta);

        BeginDrawing();
            ClearBackground(BLACK);

            rlImGuiBegin();
            SceneManager::on_draw();
            this->on_draw();
            rlImGuiEnd();
        EndDrawing();
    }

    this->on_dispose();

    rlImGuiShutdown();

    CloseAudioDevice();
    CloseWindow();
}
