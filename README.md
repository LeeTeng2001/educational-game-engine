<div align="center">

# Edu Engine

Edu engine is a **minimal game engine**, intended to serve as an educational engine to showcase some essential design choices adopted by major engines like Unreal and Unity.

Designs including input system, audio system, AI state machine, render pipeline and more.

[Features](#features) •
[Building](#building) •
[Configuration](#configuration) •
[Integrations](#third-party-integrations)

</div>

## Features

Introduction

## Building

### *Step 1: Install dependencies*
| Application | Description                                           | 
|-------------|-------------------------------------------------------|
| [FMOD]      | 3D spatial audio support                              |
| [SDL2]      | Window manager, friendly & robust API                 |
| [SDL-ttf]   | Font support (easy baking from ttf to OpenGL texture) |
| [rapidjson] | Fast JSON file reader                                 |

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
