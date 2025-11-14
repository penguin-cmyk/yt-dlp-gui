# YT-DLP Downloader UI

A modern, user-friendly GUI wrapper for [yt-dlp](https://github.com/yt-dlp/yt-dlp) built with C++ and ImGui.

![YT-DLP UI Screenshot](https://raw.githubusercontent.com/penguin-cmyk/penguin-cmyk/refs/heads/main/yt-dlp-png.png)

## Overview

This project aims to make using yt-dlp easier by providing a clean graphical interface that eliminates the need to memorize complex command-line arguments. Instead of wrestling with CLI syntax, simply interact with an intuitive UI that handles all the technical details for you.

**Note:** This is a wrapper for the yt-dlp CLI executable, not a direct library integration.

## Features

- 🎨 Clean, modern interface with the "Comfy" theme
- 📁 Built-in file dialog for easy path selection
- ⚡ Simple and essential download options
- 🖥️ Native Windows application

## Prerequisites

Before running the application, you need to download yt-dlp:

1. Visit the [yt-dlp releases page](https://github.com/yt-dlp/yt-dlp/releases)
2. Download the Windows executable (`yt-dlp.exe`)
3. Place it in an accessible location (or add to your PATH)

## Building from Source

### Requirements

- MinGW (GCC for Windows)
- CMake 4.0 or higher
- C++20 compatible compiler

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/penguin-cmyk/yt-dlp-gui
cd yt-dlp-gui

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build . --config Release
```

## Technologies Used

- **[ImGui](https://github.com/ocornut/imgui)** - Immediate mode GUI framework
- **[GLFW](https://www.glfw.org/)** - Window and input handling
- **[ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog)** - Native file dialog integration
- **MinGW** - Windows compiler toolchain
- **CMake** - Build system

## Credits

- **Theme**: "Comfy" by Giuseppe, from [ImThemes](https://github.com/Patitotective/ImThemes)
- **File Dialog**: [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog) by Aiekick
- **GUI Framework**: [Dear ImGui](https://github.com/ocornut/imgui) by Omar Cornut

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.