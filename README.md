# raykit-cpp
A modern C++ game development starter kit built with Raylib, featuring scene management, built-in utilities, and minimal boilerplate.
> "Build. Play. Iterate."

## Features
* Zero setup friction — clone and build
* Cross-platform: Windows / macOS / Linux
* CMake build system with Ninja and CMakePresets
* Raylib 6.0 via FetchContent
* physfs asset bundling — assets packed into a `.pck` file for release
* Scene management system with lifecycle hooks
* Asset manager with debug/release path abstraction and user data storage
* FastNoiseLite procedural noise generation
* ImGui (rlImGui) debugging UI integrated
* Optional Steamworks SDK integration
* CI/CD via GitHub Actions (Ubuntu and Windows)
* Compile commands export for IDE support (LSP/clangd)

## Project Structure
```
raykit-cpp/
├── assets/              # Textures, audio, and other media resources
├── cmake/               # Platform-specific toolchain files
├── src/
│   ├── main.cpp
│   ├── game/            # Your game-specific code and scenes
│   └── raykit/          # Reusable framework
│       ├── assets/      # AssetManager (physfs-backed asset loading & user data)
│       ├── scene/       # Scene and SceneManager
│       └── noise/       # FastNoiseLite header
├── vendor/
│   └── imgui/           # rlImGui integration
├── CMakeLists.txt
├── CMakePresets.json
├── Makefile
└── make.bat
```

## Build & Run

### Prerequisites
* CMake >= 3.25
* A C++20-capable compiler (GCC, Clang, or MinGW)
* Ninja build system

### Quick Start
```bash
# macOS / Linux
make build PRESET=desktop-debug

# Windows
.\make.bat build PRESET=desktop-debug
```

### Running the Game
The executable is placed in `bin/{platform}-{build_type}/`:
```bash
./bin/desktop-debug/raykit-cpp
```

## Makefile Commands
```bash
make build PRESET=<preset>      # Configure and build
make configure PRESET=<preset>  # Configure only
make clean PRESET=<preset>      # Clean build artifacts
make rebuild PRESET=<preset>    # Clean, configure, and rebuild
```

Available presets: `desktop-debug` / `desktop-release`

## Scene System
```cpp
class MyScene : public Scene {
    void on_compose() override { /* load resources */ }
    void on_dispose() override { /* unload resources */ }
    void on_update(float delta) override { /* update logic */ }
    void on_draw() override { /* render */ }
};

SceneManager::add_scene("MY_SCENE", std::make_unique<MyScene>());
SceneManager::change_scene("MY_SCENE");
```

`SceneManager` calls `on_dispose` on the outgoing scene and `on_compose` on the incoming scene automatically.

## Asset Manager
`AssetManager` abstracts asset loading across debug and release builds using physfs. In debug, assets are read directly from the source tree. In release, they are loaded from a bundled `raykit-cpp.pck` file (ZIP format) copied next to the executable.

```cpp
// Initialize at startup
AssetManager::set_project_name("MyGame");
AssetManager::compose();

// Load assets
Texture2D tex = AssetManager::read_texture("textures/bricks.png");
std::string data = AssetManager::read_text("data/config.json");

// User data (save files, settings)
AssetManager::write_text("user://saves/slot1.dat", contents);

// Cleanup
AssetManager::dispose();
```

**Path protocols:**
- `assets://path/file` or plain `path/file` — bundled assets
- `user://path/file` — platform user data directory:
  - Windows: `%APPDATA%/<project>/`
  - macOS: `~/Library/Application Support/<project>/`
  - Linux: `$XDG_DATA_HOME/<project>/` or `~/.local/share/<project>/`

## FastNoiseLite
Header-only procedural noise for terrain, textures, and more. See [FastNoiseLite.hpp](src/raykit/noise/FastNoiseLite.hpp) for usage.

## Optional: Steamworks Integration
1. Download the Steamworks SDK
2. Extract it to `vendor/steamworks/`
3. The build system detects it automatically and defines `USE_STEAMWORKS`

If the SDK is not present, the project builds without Steamworks support.

## Debug vs Release Builds
- **Debug**: Assets read from `assets/` in the source tree; `RAYKIT_DEBUG_MODE` defined; debug symbols enabled
- **Release**: Assets bundled into `raykit-cpp.pck` beside the executable; `RAYKIT_RELEASE_MODE` defined; optimizations enabled

## License
See [LICENSE](LICENSE) for details.
