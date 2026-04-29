# raykit-cpp
A modern C++ game development starter kit built with Raylib, featuring scene management, built-in utilities, and minimal boilerplate.
> "Build. Play. Iterate."

## 🧩 Features
* ✅ Zero setup friction — clone and build
* 🎯 Cross-platform: Windows / macOS / Linux
* ⚙️ CMake build system with Ninja and CMakePresets
* 🎮 Raylib 6.0 via FetchContent
* 🎬 Scene management system with lifecycle hooks
* 🛠️ Built-in utilities: Timer node, FastNoiseLite integration
* 🪟 ImGui (rlImGui) debugging UI integrated
* 📦 Automatic asset copying in release builds
* 🎮 Optional Steamworks SDK integration
* 🗂️ Clean project structure with organized source layout
* 🔧 Compile commands export for IDE support (LSP/clangd)
  
## 📂 Project Structure
* `src/` - Source code organized by domain:
  * `game/` - Your game-specific code and scenes
  * `raykit/` - Reusable framework utilities:
    * `scene/` - Scene management system (Scene, SceneManager)
    * `nodes/` - Utility nodes (Timer, etc.)
    * `noise/` - FastNoiseLite procedural noise generation
* `assets/` - Textures, audio, and other media resources
* `vendor/` - Third-party libraries (ImGui, optional Steamworks SDK)
* `cmake/` - Platform-specific toolchain files
* `CMakePresets.json` - Build configuration presets for all platforms
* `bin/` - Output directory for built executables
* `lib/` - Output directory for static/shared libraries

## 🛠️ Build & Run Instructions
### Prerequisites
* CMake >= 3.25
* A C++20-capable compiler (GCC, Clang, or MinGW)
* Ninja build system

### Quick Start (Linux/macOS)
```bash
# Windows
.\make.bat

# macOS
make build PRESET=desktop-debug

# Linux
make build PRESET=desktop-debug
```

### Running the Game
After building, the executable will be in the `bin/` directory:
```bash
./bin/raykit-cpp
```

## 📄 Makefile Commands
```bash
make build PRESET=<preset-name>     # Configure and build using the specified CMake preset
make configure PRESET=<preset-name> # Configure project only (runs cmake --preset)
make clean PRESET=<preset-name>     # Clean build artifacts
make rebuild PRESET=<preset-name>   # Clean, configure, and rebuild
```

Available presets:
- `desktop-debug` / `desktop-release`

## 🎮 Scene System
The project includes a scene management system for organizing game states:

```cpp
// Create a scene by inheriting from Scene
class MyScene : public Scene {
    void on_compose() override { /* Initialize resources */ }
    void on_dispose() override { /* Cleanup resources */ }
    void on_update(float delta) override { /* Update logic */ }
    void on_draw() override { /* Rendering */ }
};

// Add and switch scenes
SceneManager::add_scene("MY_SCENE", std::make_unique<MyScene>());
SceneManager::change_scene("MY_SCENE");
```

## 🔧 Built-in Utilities
### Timer Node
Simple timer with callback support:
```cpp
Timer* timer = new Timer();
timer->start(5.0f, []() { 
    std::cout << "Timer finished!" << std::endl; 
});
timer->update(delta); // Call in your update loop
```

### FastNoiseLite
Procedural noise generation for terrain, textures, and more. See [FastNoiseLite.hpp](src/raykit/noise/FastNoiseLite.hpp) for usage.

## 🎮 Optional: Steamworks Integration
To enable Steamworks SDK integration:
1. Download the Steamworks SDK
2. Extract it to `vendor/steamworks/`
3. The build system will automatically detect and integrate it
4. A `USE_STEAMWORKS` preprocessor definition will be added

If the SDK is not present, the project builds without Steamworks support.

## 🐛 Debug vs Release Builds
- **Debug**: Assets loaded from source tree, debug symbols enabled, `RAYKIT_DEBUG_MODE` defined
- **Release**: Assets automatically copied to binary directory, optimizations enabled, `RAYKIT_RELEASE_MODE` defined

## 📝 License
See [LICENSE](LICENSE) for details.
