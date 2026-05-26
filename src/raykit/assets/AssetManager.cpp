#include <raykit/assets/AssetManager.hpp>

#include <cstddef>
#include <cstdlib>
#include <raylib.h>

#include <cstdio>
#include <filesystem>
#include <fstream>

#ifdef NDEBUG
#include <physfs.h>
#endif

namespace
{
    constexpr const char* ASSETS_PROTOCOL = "assets://";
    constexpr const char* USER_PROTOCOL = "user://";

    bool starts_with(const std::string& s, const char* prefix)
    {
        return s.rfind(prefix, 0) == 0;
    }

    std::string strip_leading_slashes(const std::string& s)
    {
        size_t i = 0;
        while (i < s.size() && (s[i] == '/' || s[i] == '\\')) {
            ++i;
        }
        return s.substr(i);
    }
}

AssetManager::AssetManager()
{
    this->asset_path = "";
    this->pak_name = "";
    this->project_name = "";
    this->user_path = "";
    this->mounted = false;
}

AssetManager& AssetManager::get_singleton()
{
    static AssetManager instance;
    return instance;
}

void AssetManager::set_asset_path(const std::string& path)
{
    AssetManager& self = AssetManager::get_singleton();
    self.asset_path = path;
    if (!self.asset_path.empty() && self.asset_path.back() != '/') {
        self.asset_path.push_back('/');
    }
}

void AssetManager::set_pak_name(const std::string& name)
{
    AssetManager& self = AssetManager::get_singleton();
    self.pak_name = name;
}

void AssetManager::set_project_name(const std::string& name)
{
    AssetManager& self = AssetManager::get_singleton();
    self.project_name = name;
    self.user_path = AssetManager::compute_user_path(name);
}

std::string AssetManager::compute_user_path(const std::string& project)
{
    if (project.empty()) {
        return "";
    }

#if defined(_WIN32)
    const char* appdata = std::getenv("APPDATA");
    std::string base = appdata && *appdata ? appdata : "";
    if (base.empty()) {
        return "";
    }
    return base + "/" + project + "/";
#elif defined(__APPLE__)
    const char* home = std::getenv("HOME");
    std::string base = home && *home ? home : "";
    if (base.empty()) {
        return "";
    }
    return base + "/Library/Application Support/" + project + "/";
#else
    // Linux: XDG Base Directory spec, mirroring the per-user data location
    // used on Windows (%APPDATA%) and macOS (~/Library/Application Support).
    const char* xdg = std::getenv("XDG_DATA_HOME");
    if (xdg && *xdg) {
        return std::string(xdg) + "/" + project + "/";
    }
    const char* home = std::getenv("HOME");
    std::string base = home && *home ? home : "";
    if (base.empty()) {
        return "";
    }
    return base + "/.local/share/" + project + "/";
#endif
}

AssetManager::Resolved AssetManager::resolve(const std::string& path)
{
    AssetManager& self = AssetManager::get_singleton();
    Resolved r{};

    if (starts_with(path, USER_PROTOCOL)) {
        std::string rest = strip_leading_slashes(path.substr(std::string(USER_PROTOCOL).size()));
        r.is_user = true;
        r.virtual_path = rest;
        r.filesystem_path = self.user_path + rest;
        return r;
    }

    std::string rest = path;
    if (starts_with(rest, ASSETS_PROTOCOL)) {
        rest = strip_leading_slashes(rest.substr(std::string(ASSETS_PROTOCOL).size()));
    }
    r.is_user = false;
    r.virtual_path = rest;
    r.filesystem_path = self.asset_path + rest;
    return r;
}

void AssetManager::compose()
{
#ifdef NDEBUG
    AssetManager& self = AssetManager::get_singleton();

    if (!PHYSFS_init(nullptr)) {
        TraceLog(LOG_ERROR, "AssetManager: PHYSFS_init failed: %s",
                PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        return;
    }

    std::string pak = std::string(GetApplicationDirectory()) + self.pak_name;
    if (!PHYSFS_mount(pak.c_str(), nullptr, 1)) {
        TraceLog(LOG_ERROR, "AssetManager: failed to mount '%s': %s",
                pak.c_str(),
                PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        return;
    }

    self.mounted = true;
#else
    AssetManager& self = AssetManager::get_singleton();
    if (!std::filesystem::is_directory(self.asset_path)) {
        TraceLog(LOG_WARNING, "AssetManager: asset_path '%s' is not a directory",
                self.asset_path.c_str());
    }
#endif
}

void AssetManager::dispose()
{
#ifdef NDEBUG
    AssetManager& self = AssetManager::get_singleton();
    if (self.mounted) {
        PHYSFS_deinit();
        self.mounted = false;
    }
#endif
}

bool AssetManager::exists(const std::string& path)
{
    Resolved r = resolve(path);

    if (r.is_user) {
        AssetManager& self = AssetManager::get_singleton();
        if (self.user_path.empty()) {
            TraceLog(LOG_WARNING, "AssetManager: user:// used but project_name is not set");
            return false;
        }
        std::error_code ec;
        return std::filesystem::exists(r.filesystem_path, ec);
    }

#ifdef NDEBUG
    return PHYSFS_exists(r.virtual_path.c_str()) != 0;
#else
    std::error_code ec;
    return std::filesystem::exists(r.filesystem_path, ec);
#endif
}

std::vector<unsigned char> AssetManager::read_bytes(const std::string& path)
{
    Resolved r = resolve(path);

#ifdef NDEBUG
    if (!r.is_user) {
        PHYSFS_File* file = PHYSFS_openRead(r.virtual_path.c_str());
        if (!file) {
            TraceLog(LOG_ERROR, "AssetManager: failed to open '%s': %s",
                    path.c_str(),
                    PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
            return {};
        }

        PHYSFS_sint64 length = PHYSFS_fileLength(file);
        std::vector<unsigned char> data(length > 0 ? static_cast<size_t>(length) : 0);
        if (length > 0) {
            PHYSFS_readBytes(file, data.data(), static_cast<PHYSFS_uint64>(length));
        }
        PHYSFS_close(file);
        return data;
    }
#endif

    if (r.is_user) {
        AssetManager& self = AssetManager::get_singleton();
        if (self.user_path.empty()) {
            TraceLog(LOG_ERROR, "AssetManager: user:// read requested but project_name is not set");
            return {};
        }
    }

    std::ifstream file(r.filesystem_path, std::ios::binary | std::ios::ate);
    if (!file) {
        TraceLog(LOG_ERROR, "AssetManager: failed to open '%s'", r.filesystem_path.c_str());
        return {};
    }

    std::streamsize length = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> data(length > 0 ? static_cast<size_t>(length) : 0);
    if (length > 0) {
        file.read(reinterpret_cast<char*>(data.data()), length);
    }
    return data;
}

std::string AssetManager::read_text(const std::string& path)
{
    std::vector<unsigned char> bytes = AssetManager::read_bytes(path);
    return std::string(bytes.begin(), bytes.end());
}

bool AssetManager::write_text(const std::string& path, const std::string& contents)
{
    Resolved r = resolve(path);

    if (!r.is_user) {
#ifdef NDEBUG
        TraceLog(LOG_WARNING, "AssetManager: write_text to assets:// is a no-op in release builds (read-only)");
        return false;
#endif
    } else {
        AssetManager& self = AssetManager::get_singleton();
        if (self.user_path.empty()) {
            TraceLog(LOG_ERROR, "AssetManager: user:// write requested but project_name is not set");
            return false;
        }
    }

    std::filesystem::path full(r.filesystem_path);

    std::error_code ec;
    std::filesystem::create_directories(full.parent_path(), ec);

    std::ofstream file(full, std::ios::binary | std::ios::trunc);
    if (!file) {
        TraceLog(LOG_ERROR, "AssetManager: failed to write '%s'", full.string().c_str());
        return false;
    }
    file.write(contents.data(), static_cast<std::streamsize>(contents.size()));
    return true;
}

Texture2D AssetManager::read_texture(const std::string &path)
{
    std::vector<unsigned char> bytes = AssetManager::read_bytes(path);

    // Find the extension.
    size_t dot = path.rfind('.');
    std::string ext = "";
    if (dot != std::string::npos) {
        ext = path.substr(dot);
    }

    // Load the image from data.
    Image img = LoadImageFromMemory(ext.c_str(), bytes.data(), static_cast<int>(bytes.size()));
    Texture2D result = LoadTextureFromImage(img);
    UnloadImage(img); // <-- Clean up

    return result;
}
