#include "OrbitActor.hpp"
#include "../Game.hpp"
#include "../core/Renderer.hpp"
#include "../components/camera/OrbitCamera.hpp"
#include "../components/render/MeshComponent.hpp"


OrbitActor::OrbitActor(Game *game) : Actor(game) {
    game->GetInputSystem()->SetRelativeMouseMode(true);

    mMeshComp = new MeshComponent(this);
    mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
    SetPosition(Vector3(0.0f, 0.0f, -100.0f));

    mCameraComp = new OrbitCamera(this);
}

void OrbitActor::ActorInput(const InputState &state) {
    // Mouse rotation, Get relative movement from SDL
    Vector2 mousePos = state.Mouse.GetPosition();

    // Only apply rotation if right-click is held, code is similar to first person
    if (state.Mouse.GetButtonValue(SDL_BUTTON_RIGHT)) {

        // Assume mouse movement is usually between -500 and +500
        const int maxMouseSpeed = 500;
        // Rotation/sec at maximum speed
        const float maxOrbitSpeed = Math::Pi * 8;
        float yawSpeed = 0.0f;
        if (mousePos.x != 0) {
            // Convert to ~[-1.0, 1.0]
            yawSpeed = mousePos.x / maxMouseSpeed;
            // Multiply by rotation/sec
            yawSpeed *= maxOrbitSpeed;
        }
        mCameraComp->SetYawSpeed(-yawSpeed);

        // Compute pitch
        float pitchSpeed = 0.0f;
        if (mousePos.y != 0) {
            // Convert to ~[-1.0, 1.0]
            pitchSpeed = mousePos.y / maxMouseSpeed;
            pitchSpeed *= maxOrbitSpeed;
        }
        mCameraComp->SetPitchSpeed(pitchSpeed);
    }
}

void OrbitActor::SetVisible(bool visible) {
    mMeshComp->SetVisible(visible);
}
