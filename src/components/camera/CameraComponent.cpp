#include "CameraComponent.hpp"
#include "../../actors/Actor.hpp"
#include "../../core/Renderer.hpp"
#include "../../Game.hpp"
#include "../../audio/AudioSystem.hpp"

CameraComponent::CameraComponent(Actor *owner, int updateOrder)
        : Component(owner, updateOrder) {
}

void CameraComponent::SetViewMatrix(const Matrix4 &view) {
    // Pass view matrix to renderer and audio system
    Game *game = mOwner->GetGame();
    game->GetRenderer()->SetViewMatrix(view);
    game->GetAudioSystem()->SetListener(view);
}
