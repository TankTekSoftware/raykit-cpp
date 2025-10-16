# raykit-cpp
A modern C++ starter kit for building cross-platform games with Raylib, CMake, and minimal boilerplate.
> "Build. Play. Iterate."

## 🧩 Features
* ✅ Zero setup friction — clone and build
* 🎯 Cross-platform: Windows / macOS / Linux
* ⚙️ CMake build system, FetchContent for Raylib
* 🗂️ Organized project structure (src / include / assets)
* 📦 Auto-copy of assets to build output
* 🪟 ImGui built-in.
  
## 📂 Project Structure
* `src/` - your `.cpp` and `.hpp` code
* `assets/` - media, textures, etc.
* `vendor/` - Third party code libraries.
* `CMakePresets.json` - convenient presets for local development

## 🛠️ Build & Run Instructions
### Prerequisites
* CMake ≥ 3.25
* A C++20–capable compiler
* Ninja Build System

### Building Locally 
#### Windows (WSL with MinGW)
Debug Mode
```bash
cmake --preset windows-mingw-debug
cmake --build --preset windows-mingw-debug
```
Release Mode
```bash
cmake --preset windows-mingw-release
cmake --build --preset windows-mingw-release
```

#### Linux (Using gcc/g++)
Debug Mode
```bash
cmake --preset linux-debug
cmake --build --preset linux-debug
```
Release Mode
```bash
cmake --preset linux-release
cmake --build --preset linux-release
```
