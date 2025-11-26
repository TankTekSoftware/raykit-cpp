# raykit-cpp
A modern C++ starter kit for building cross-platform games with Raylib, CMake, and minimal boilerplate.
> "Build. Play. Iterate."

## 🧩 Features
* ✅ Zero setup friction — clone and build
* 🎯 Cross-platform: Windows / macOS / Linux
* ⚙️ CMake build system, FetchContent for Raylib
* 🗂️ Organized project structure (src / assets)
* 📦 Auto-copy of assets to build output
* 🪟 ImGui debugging ui built-in
  
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
make build windows-mingw-debug
```
Release Mode
```bash
make build windows-mingw-release
```

#### Linux (Using gcc/g++)
Debug Mode
```bash
make build linux-debug
```
Release Mode
```bash
make build linux-release
```

#### macOS
Debug Mode
```bash
make build preset macos-debug
```
Release Mode
```bash
make build macos-release
```
