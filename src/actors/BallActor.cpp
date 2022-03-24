#include "BallActor.hpp"
#include "../Game.hpp"
#include "../core/Renderer.hpp"
#include "../components/render/MeshComponent.hpp"
#include "../helper/Mesh.hpp"
#include "../components/collision/BallMove.hpp"
#include "../components/control/AudioComponent.hpp"

BallActor::BallActor(Game *game) : Actor(game) {
    //SetScale(10.0f);

    // Render
    auto *mc = new MeshComponent(this);
    Mesh *mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh");
    mc->SetMesh(mesh);

    // Attach movement
    mMyMove = new BallMove(this);
    mMyMove->SetForwardSpeed(1500.0f);

    // Audio
    mAudioComp = new AudioComponent(this);
}

void BallActor::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);

    mLifeSpan -= deltaTime;
    if (mLifeSpan < 0.0f) {
        SetState(EDead);
    }
}

void BallActor::SetPlayer(Actor *player) {
    mMyMove->SetPlayer(player);
}

void BallActor::HitTarget() {
    mAudioComp->PlayEvent("event:/Ding");
}
