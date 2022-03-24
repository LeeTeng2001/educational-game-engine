#include "FollowActor.hpp"
#include "../components/render/MeshComponent.hpp"
#include "../Game.hpp"
#include "../core/Renderer.hpp"
#include "../components/camera/FollowCamera.hpp"
#include "../components/control/MoveComponent.hpp"

FollowActor::FollowActor(Game *game) : Actor(game) {
    mMeshComp = new MeshComponent(this);
    mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
    SetPosition(Vector3(0.0f, 0.0f, -100.0f));

    mMoveComp = new MoveComponent(this);
    mCameraComp = new FollowCamera(this);

    mCameraComp->SnapToIdeal();  // Need to have the first snap to ideal when initialising.
}

void FollowActor::ActorInput(const InputState &state) {
    float forwardSpeed = 0.0f;
    float angularSpeed = 0.0f;

    // wasd movement
    if (state.Keyboard.GetKeyValue(SDL_SCANCODE_W)) {
        forwardSpeed += 700.0f;
    }
    if (state.Keyboard.GetKeyValue(SDL_SCANCODE_S)) {
        forwardSpeed -= 700.0f;
    }
    if (state.Keyboard.GetKeyValue(SDL_SCANCODE_A)) {
        angularSpeed -= Math::Pi;
    }
    if (state.Keyboard.GetKeyValue(SDL_SCANCODE_D)) {
        angularSpeed += Math::Pi;
    }

    mMoveComp->SetForwardSpeed(forwardSpeed);
    mMoveComp->SetAngularSpeed(angularSpeed);

    // Adjust horizontal distance of camera based on speed
    if (!Math::NearZero(forwardSpeed)) {
        mCameraComp->SetHorzDist(500.0f);
    } else {
        mCameraComp->SetHorzDist(350.0f);
    }
}

void FollowActor::SetVisible(bool visible) {
    mMeshComp->SetVisible(visible);
}
