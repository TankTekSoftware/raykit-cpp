#pragma once

#include <string>

struct WindowConfig
{
    int width;
    int height;
    std::string title;

    int target_fps;
};

class Game
{
    private: 
        WindowConfig window_config;
    public:
        Game();
    private:
        void on_compose();
        void on_dispose();
        void on_update(float delta);
        void on_draw();
    public:
        void run();

};
