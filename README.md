# AC Trainer

**clementi/ac_trainer** is a simple external cheat/trainer for the open-source game **Assault Cube (Version 1.3.0.2)**. It was created as an educational project based on video tutorials, primarily drawing inspiration from sources like [Guided Hacking](https://www.youtube.com/c/GuidedHacking) on YouTube.

## 📖 About the Project

This trainer is an **external** cheat. This means it does not inject into the game or interact directly with the game's internal code or functions. Instead, it runs as a completely separate process and uses Windows API functions (like `ReadProcessMemory` and `WriteProcessMemory`) for most of the heavy lifting.

For rendering and user interface, the trainer uses **ImGui** and **DirectX 11**. It creates a transparent, click-through overlay on top of the game window. This overlay is used for drawing ESP elements—like bounding boxes and health bars—and for displaying the interactive cheat menu.

## ✨ Features
* **ESP (Extra Sensory Perception)**
  * 2D Bounding Boxes around players
  * Health bars
  * Player names
  * Snaplines
* **Aimbot**
  * Automated targeting within a configurable Field of View (FOV)
  * Visual FOV circle
* **Visual Effects**
  * Floating damage numbers (RPG style)
  * Hitmarkers

## 🛠️ Prerequisites & Setup

To build and run this project, you will need:
* Windows operating system
* Visual Studio (with C++ Desktop Development workflow) or another MSVC compiler
* [CMake](https://cmake.org/) (Version 3.20+)
* [Doxygen](https://www.doxygen.nl/) (Optional, for generating documentation)

### Building the Project

We use CMake to configure and build the project. Run the following commands in your terminal:

```powershell
# 1. Clone the repository
git clone https://github.com/wclementi/ac_trainer.git
cd ac_trainer

# 2. Configure the project
cmake -B build -S .

# 3. Compile the executable
cmake --build build --config Release
```

### Generating Documentation
The project source code is heavily documented using Doxygen. If you have Doxygen installed, you can generate an HTML website containing the API documentation:

```powershell
cmake --build build --target docs
```
Open `build/html/index.html` in your web browser to view the documentation.

## 🚀 How to Run

1. Launch **Assault Cube v1.3.0.2** and join a match or start a bot game.
2. Ensure the game is running in **Windowed** or **Borderless Windowed** mode (overlays often struggle with exclusive Fullscreen).
3. Run the compiled `ac_trainer.exe` as Administrator. *(Administrator privileges are required for `ReadProcessMemory` to access the game's memory).*
4. The overlay should appear on top of your game. Press the configured menu key (usually `Insert` or `Home`) to open the ImGui configuration menu.

## 📚 Sources and Inspiration
* **[Guided Hacking](https://guidedhacking.com/) / [Guided Hacking YouTube](https://www.youtube.com/c/GuidedHacking)** - Primary source of inspiration and educational material on memory editing, pointer scanning, and game hacking fundamentals.
* **[Cazz's YouTube Channel](https://www.youtube.com/c/cazz)** - Excellent resource for game hacking tutorials, including this [External ESP & Aimbot Playlist](https://www.youtube.com/watch?v=nFHCZ4H1TQQ&list=PLZl6v3tLR5b77nOayeMFHnNIsztC3lrdJ).
* **[UnknownCheats - Other FPS Games Forum](https://www.unknowncheats.me/forum/other-fps-games/)** - General discussion, offset finding techniques, and resources for Assault Cube.
* **[ImGui (ocornut)](https://github.com/ocornut/imgui)** - Used for the graphical user interface and overlay rendering.
* **Assault Cube (v1.3.0.2)** - The open-source FPS game used as the target for this trainer.
