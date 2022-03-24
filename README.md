<div align="center">

# Edu Engine

Edu engine is a **minimal game engine** intended to serve as an educational engine to showcase some of the essential design choices adopted by top players like Unreal and Unity.

Designs including input system, audio system, physics, rendering and more.

[Features](#features) •
[Building](#building) •
[Reading materials](#reading-materials)

</div>

## Features

- At the top level, it's a classic ECS driven engine. You don't need to worry about freeing memories when dynamically allocating new components and actors. ECS is a good tradeoff between scalability, data redundancy and developoment efficiency. Although industry seems to be exploring the potential of integrating data oriented design in their game engine, ECS is still a good classic game engine design to learn.

- A fully featured abstracted InputSystem that support multiple input states, input states consist of common devices like keyboard, mouse and controller. On top of that, it's easy to extend the input state to fit your need.

- Stack based UI system.

- Audio system that support 3D spatial audio

- Multiple classic camera controllers like FPS, Orbit movement, TPS, Spline path.

- Forward rendering pipeline, Blinn–Phong reflection model.

- support multiple ttf fonts for text, a relatively simple json based localisation feature.

- Common 3D collision physics, continuous collision detection and models like AABB, Sphere, Plane, Capsule, Polygon, LineSegment, Raycast.

- 3D math library (of course it can't be competed against glm).

  

## Building

### *Step 1: Install dependencies*
| Application | Description                                                  |
| ----------- | ------------------------------------------------------------ |
| [FMOD]      | Support for 3D spatial audio                                 |
| [SDL2]      | Window manager, it has a friendly & robust API               |
| [SDL-ttf]   | Font support (baking process is easy from ttf to OpenGL texture) |
| [rapidjson] | Fastest JSON file reader in cpp                              |

[FMOD]: https://www.fmod.com/
[SDL2]: https://www.libsdl.org/
[SDL-ttf]: https://github.com/libsdl-org/SDL_ttf
[rapidjson]: https://rapidjson.org/


### *Step 2: Configure project*
- In src/Game.cpp, you should change the `std::string Game::PROJECT_BASE` to point to your src folder.
- You probably want to specify your FMOD dynamic lib if you're not on Mac, the configuration should be modified in CMakeList.txt `file(GLOB FMOD_LIBRARIES "path-to-lib"")`

### *Step 3: Build project*

- Build project using cmake

  

## Things that can be improved
- **Render**: Different lighting models, deferred rendering algorithm.

- **Audio**: Support for multiple filters.

- **Control**: Support for multiple controllers, hot reloading, key-mapping files.

- Mini animation system.

  

## Reading materials

- [Game engine architecture](https://www.gameenginebook.com/)
- [Best OpenGL reference](https://learnopengl.com/)
