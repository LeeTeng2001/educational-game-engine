#include "CameraActor.hpp"
#include "../components/control/MoveComponent.hpp"
#include <SDL_scancode.h>
#include "../core/Renderer.hpp"
#include "../core/InputSystem.hpp"
#include "../audio/AudioSystem.hpp"
#include "../Game.hpp"

CameraActor::CameraActor(Game *game) : Actor(game) {
    mMoveComp = new MoveComponent(this);
}

void CameraActor::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);

    // Compute new camera from this actor
    Vector3 cameraPos = GetPosition();
    Vector3 target = GetPosition() + GetForward() * 100.0f;
    Vector3 up = Vector3::UnitZ;

    Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
    GetGame()->GetRenderer()->SetViewMatrix(view);
    GetGame()->GetAudioSystem()->SetListener(view);
}

void CameraActor::ActorInput(const InputState& state) {
    float forwardSpeed = 0.0f;
    float angularSpeed = 0.0f;

    // wasd movement
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_W)) {
        forwardSpeed += 300.0f;
    }
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_S)) {
        forwardSpeed -= 300.0f;
    }
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_A)) {
        angularSpeed -= Math::TwoPi;
    }
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_D)) {
        angularSpeed += Math::TwoPi;
    }

    mMoveComp->SetForwardSpeed(forwardSpeed);
    mMoveComp->SetAngularSpeed(angularSpeed);
}
