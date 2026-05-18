# Minesweeper

![Version](https://img.shields.io/badge/version-1.6.1-blue.svg)
![C++](https://img.shields.io/badge/C++-20-orange.svg)
![License](https://img.shields.io/badge/license-Source%20Available-green.svg)

A modern, high-performance Minesweeper implementation built with **C++20**. This project combines classic gameplay with a refined technical architecture, featuring a custom keyboard-driven navigation system and a modern UI.

## 🌟 Key Features

* **Modern Graphics Stack**: Built using **SFML 3.0** for low-level rendering and **TGUI 1.12** for a responsive user interface.
* **Keyboard-Centric Navigation**: Supports full grid navigation via `WASD` or Arrow keys, offering a faster alternative to traditional mouse-only gameplay.
* **Dynamic Mine Generation**: Implements a robust randomization logic to ensure fair and challenging board layouts.
* **First-Click Guarantee**: Optimized algorithm to ensure the first cell opened is never a mine.
* **Industrial-Grade Development**: Developed using **Visual Studio 2026** with a modular **CMake** configuration for professional project management.

## 🎮 Default Controls

| Action | Keyboard |
| :---   | :--- |
| **Move Cursor** | `W`, `A`, `S`, `D` |
| **Open Cell** | Automatic on move |
| **Switch Mode (Move/Flag)** | `Space` |
| **Pause Game** | `Esc` |

## 🛠 Tech Stack

* **Language**: C++20
* **Frameworks**: 
    * [SFML 3.0](https://www.sfml-dev.org/) (Multimedia & Input)
    * [TGUI 1.12](https://tgui.eu/) (User Interface)
* **Build System**: CMake 3.12+
* **IDE**: Visual Studio 2022+

## 🚀 Getting Started

### Prerequisites

Ensure you have the following installed and configured:
* **Visual Studio** or **VS Code** with C++20 support.
* **SFML 3.0** and **TGUI 1.12** libraries linked to your environment.

### Installation & Build

1. **Clone the repository**:
   ```bash
   git clone https://github.com/HeyheyEason/Minesweeper.git
   cd Minesweeper
   ```

2. **Generate Project Files**:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. **Build & Run**:
   Open the generated `.sln` file in Visual Studio, build the solution, and run the executable.

## 📂 Project Structure

* `src/`: Core implementation files (`.cpp`).
* `include/`: Header files (`.hpp`) defining game logic and grid states.
* `rc/`: Textures, fonts, and TGUI theme files.
* `CMakeLists.txt`: Build configuration and dependency management.

## 📝 License

This is a source-available project.  
Copyright © 2026 HeyheyEason. All rights reserved.
