#include "SplineActor.hpp"
#include "../Game.hpp"
#include "../core/Renderer.hpp"
#include "../components/render/MeshComponent.hpp"
#include "../components/camera/SplineCamera.hpp"

SplineActor::SplineActor(Game *game) : Actor(game) {
    // auto* mc = new MeshComponent(this);
    // mc->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
    // SetPosition(Vector3(0.0f, 0.0f, -100.0f));

    mCameraComp = new SplineCamera(this);

    // Create a spline
    Spline path;
    path.mControlPoints.emplace_back(Vector3::Zero);
    for (int i = 0; i < 5; i++) {
        if (i % 2 == 0) {
            path.mControlPoints.emplace_back(Vector3(300.0f * (i + 1), 300.0f, 300.0f));
        } else {
            path.mControlPoints.emplace_back(Vector3(300.0f * (i + 1), 0.0f, 0.0f));
        }
    }

    mCameraComp->SetSpline(path);
    mCameraComp->SetPaused(false);
}

void SplineActor::ActorInput(const InputState &state) {
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_P) == EPressed) {
        RestartSpline();
    }
}

void SplineActor::RestartSpline() {
    mCameraComp->Restart();
}
