set(PROJECT_ENGINE MINIMAL_ENGINE)

set(SOURCE_ACTORS_ENGINE
        actors/Actor.cpp actors/Actor.hpp
        actors/CameraActor.cpp actors/CameraActor.hpp
        actors/FPSActor.cpp actors/FPSActor.hpp
        actors/FollowActor.cpp actors/FollowActor.hpp
        actors/PlaneActor.cpp actors/PlaneActor.hpp
        actors/OrbitActor.cpp actors/OrbitActor.hpp
        actors/SplineActor.cpp actors/SplineActor.hpp
        actors/BallActor.cpp actors/BallActor.hpp
        actors/TargetActor.cpp actors/TargetActor.hpp
        )

set(SOURCE_COMPONENTS_ENGINE
        components/Component.cpp components/Component.hpp
        components/render/SpriteComponent.cpp components/render/SpriteComponent.hpp
        components/render/MeshComponent.cpp components/render/MeshComponent.hpp
        components/control/MoveComponent.cpp components/control/MoveComponent.hpp
        components/control/InputComponent.cpp components/control/InputComponent.hpp
        components/control/AudioComponent.cpp components/control/AudioComponent.hpp
        components/camera/CameraComponent.cpp components/camera/CameraComponent.hpp
        components/camera/FPSCamera.cpp components/camera/FPSCamera.hpp
        components/camera/FollowCamera.cpp components/camera/FollowCamera.hpp
        components/camera/OrbitCamera.cpp components/camera/OrbitCamera.hpp
        components/camera/SplineCamera.cpp components/camera/SplineCamera.hpp
        components/collision/BoxComponent.cpp components/collision/BoxComponent.hpp
        components/collision/BallMove.cpp components/collision/BallMove.hpp
        components/gameplay/TargetComponent.cpp components/gameplay/TargetComponent.hpp
#        CircleComponent.cpp CircleComponent.hpp
        )

set(SOURCE_CORE_ENGINE
        core/Shader.cpp core/Shader.hpp
        core/Renderer.cpp core/Renderer.hpp
        core/InputSystem.cpp core/InputSystem.hpp
        core/PhysWorld.cpp core/PhysWorld.hpp
        )

set(SOURCE_MAIN_ENGINE
        Main.cpp
        Game.cpp Game.hpp
        helper/Math.cpp helper/Math.hpp
        helper/Random.cpp helper/Random.hpp
        helper/VertexArray.cpp helper/VertexArray.hpp
        helper/Texture.cpp helper/Texture.hpp
        helper/Mesh.cpp helper/Mesh.hpp
        helper/Collision.cpp helper/Collision.hpp
        audio/AudioSystem.cpp audio/AudioSystem.hpp
        audio/SoundEvent.cpp audio/SoundEvent.hpp
        ui/Font.cpp ui/Font.hpp
        ui/UIScreen.cpp ui/UIScreen.hpp
        ui/PauseMenu.cpp ui/PauseMenu.hpp
        ui/DialogBox.cpp ui/DialogBox.hpp
        ui/HUD.cpp ui/HUD.hpp
        )


# Executables ------------------------------------------------------------
add_executable(${PROJECT_ENGINE}
        ${SOURCE_CORE_ENGINE}
        ${SOURCE_ACTORS_ENGINE}
        ${SOURCE_COMPONENTS_ENGINE}
        ${SOURCE_MAIN_ENGINE}
        ../external/src/glad.c
        ../external/src/stbInclude.cpp
        ../external/src/rapidjson_fix.cpp
        )
target_link_libraries(
        ${PROJECT_ENGINE}
        ${SDL2_LIBRARIES}
        ${FMOD_LIBRARIES}
        ${SDL2TTF_LIBRARY}
)
