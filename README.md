# OpenGL Interactive Scene

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Technologies Used](#technologies-used)
- [Installation & Usage](#installation--usage)
  - [Prerequisites](#prerequisites)
  - [Running the Application](#running-the-application)
- [Controls](#controls)
- [Screenshots](#screenshots)

## Overview
This project is an interactive 3D scene created using OpenGL and Blender. It includes animated objects, camera manipulation, realistic fog effects, and various viewing modes, providing an engaging user experience.

## Features
- **Interactive Scene:** Users can navigate and explore the 3D environment with intuitive camera controls.
- **Animated Objects:**
  - Duck: Moves across the lake.
  - Hot Air Balloon: Flies and returns to the initial position.
- **Fog Effect:** Adjustable fog creates realistic environmental visuals.
- **Multiple Viewing Modes:** Solid, Wireframe, Polygonal, and Smooth shading.
- **Dynamic Skybox:** Immersive background that enhances scene realism.

## Technologies Used
- **OpenGL**: Graphics rendering and animation.
- **GLFW**: Window and input handling.
- **GLM**: Mathematical computations for transformations.
- **Blender**: 3D modeling of scene elements.

## Installation & Usage

### Prerequisites
- Install [OpenGL](https://www.opengl.org/)
- Install [GLFW](https://www.glfw.org/)
- Install [GLM](https://glm.g-truc.net/)
- Ensure you have a compatible C++ compiler installed (e.g., GCC, MSVC).

### Running the Application
1. Clone the repository:
```bash
git clone https://github.com/AndreeaP31/OpenGL-Interactive-Scene.git
```

2. Navigate to the project directory and compile:
```bash
cd OpenGL-Interactive-Scene
mkdir build && cd build
cmake ..
make
```

3. Run the executable:
```bash
./OpenGL-Interactive-Scene
```

## Controls

### Camera Movement:
- `W` - Move Forward
- `S` - Move Backward
- `A` - Move Left
- `D` - Move Right
- `Q` - Move Down
- `E` - Move Up

### Object Interaction:
- `F` - Lift the hot air balloon
- `B` - Reset hot air balloon position

### Camera Angle:
- Mouse Move - Rotate camera view
- Mouse Scroll - Zoom In/Out

### Viewing Modes:
- `1` - Solid Mode
- `2` - Wireframe Mode
- `3` - Polygonal Mode
- `4` - Smooth Mode

### Visual Effects:
- `C` - Toggle Fog Effect
- `P` - Toggle Particle Effects

## Screenshots
![image](https://github.com/user-attachments/assets/b8e2d708-066c-469e-ba93-bf2a7ae51cd6)
![image](https://github.com/user-attachments/assets/177e8921-803a-430f-8048-2434c7ff57ea)



