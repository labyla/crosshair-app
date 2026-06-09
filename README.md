# Crosshair App

"Crosshair App" is a Windows desktop utility for drawing a customizable crosshair over a selected application window. It uses C++, GLFW, OpenGL, GLAD, and Dear ImGui.

https://github.com/user-attachments/assets/9ad9c47f-1221-40cc-b3d2-95e705353ada

[Watch demo on YouTube](https://www.youtube.com/watch?v=m2e3-FFyac0)

## Features

- Select a visible window from the application list.
- Draw a crosshair over the selected foreground window.
- Adjust crosshair length, height, width, color, and position.
- Keep the crosshair centered or move it manually with X/Y sliders.

## Requirements

This project is intended for Windows.

Install the following tools:

- CMake 3.20 or newer
- Visual Studio 2022 with the **Desktop development with C++** workload
- Windows SDK, installed through Visual Studio
- Git, if you want to clone the repository from the command line

The rendering dependencies are already vendored in `project/third_party`:

- Dear ImGui
- GLFW
- GLAD
- KHR platform headers

You do not need to install those libraries globally.

## Installing Dependencies

### 1. Install Visual Studio

Install Visual Studio 2022 Community, Professional, or Build Tools.

During installation, enable:

- **Desktop development with C++**
- **MSVC v143 C++ build tools**
- **Windows 10 SDK** or **Windows 11 SDK**
- **C++ CMake tools for Windows**

You can also install Visual Studio Community with `winget`:

```powershell
winget install --id Microsoft.VisualStudio.2022.Community -e
```

After installation, open Visual Studio Installer and confirm that the C++ desktop workload is enabled.

### 2. Install CMake

Download CMake from the official website or install it with `winget`:

```powershell
winget install --id Kitware.CMake -e
```

Verify the installation:

```powershell
cmake --version
```

### 3. Install Git

If Git is not installed yet, install it with:

```powershell
winget install --id Git.Git -e
```

Verify the installation:

```powershell
git --version
```

## Getting the Source Code

Clone the repository:

```powershell
git clone https://github.com/labyla/crosshair-app.git
cd crosshair-app
```

If you already downloaded the project, open a terminal in the repository root.

## Building

Run the commands from the repository root.

Create the build directory and generate the project files. This step only needs to be done once:

```powershell
cmake -S project -B build
```

Build the Release configuration:

```powershell
cmake --build build --config Release
```

The executable will be created at:

```text
build/app/Release/crosshair-app.exe
```

## Running

Start the application:

```powershell
.\build\app\Release\crosshair-app.exe
```

In the app, select a target window from the list, then adjust the crosshair settings in the control panel.

## Project Layout

```text
assets/                  Demo media used by this README
project/
  app/                   Application source code and runtime resources
  third_party/           Vendored GLFW, GLAD, KHR, and Dear ImGui files
build/                   Generated CMake build directory
```

## Future Improvements

The current implementation draws the crosshair by repeatedly iterating over pixels and changing them according to the selected settings. This works for a simple prototype, but it is not the best rendering model for a real overlay application.

The project could be improved in two main ways:

- Render the crosshair through a transparent overlay window, similar to the approach used in `xbox-gamepad-overlay`.
- Prepare the crosshair as a reusable renderable object whenever the app starts or the settings change, instead of rebuilding it every frame. In practice, this would work like generating a small crosshair image or texture once, then placing that prepared object on the screen.

This would make the rendering logic cleaner, reduce repeated per-pixel work, improve performance and move the project closer to a proper overlay architecture.

## Notes

- The app uses Win32 APIs, so it is Windows-only.
- The `build/` directory is generated and should not be committed.
- If CMake cannot find a compiler, run the commands from **Developer PowerShell for VS 2022** or check that Visual Studio C++ tools are installed.
