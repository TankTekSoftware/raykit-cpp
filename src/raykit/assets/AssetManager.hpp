#pragma once

#include <string>
#include <vector>
#include <raylib.h>

class AssetManager
{
    private:
        struct Resolved
        {
            std::string virtual_path;     // Path relative to mount/asset root (assets only)
            std::string filesystem_path;  // Full disk path (user, or assets in debug)
            bool is_user;
        };
    private:
        std::string asset_path;
        std::string pak_name;
        std::string project_name;
        std::string user_path;
        bool mounted;
    private:
        AssetManager();
    public:
        static AssetManager& get_singleton();
    public:
        static void compose();
        static void dispose();
    public:
        static void set_asset_path(const std::string& path);
        static void set_pak_name(const std::string& name);
        static void set_project_name(const std::string& name);
    public:
        static bool exists(const std::string& path);
        static std::vector<unsigned char> read_bytes(const std::string& path);
        static std::string read_text(const std::string& path);
        static Texture2D read_texture(const std::string& path);
        static Sound read_sound(const std::string& path);
    public:
        static bool write_text(const std::string& path, const std::string& contents);
    private:
        static Resolved resolve(const std::string& path);
        static std::string compute_user_path(const std::string& project);
};
