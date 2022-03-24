#include "../../actors/Actor.hpp"
#include "../../Game.hpp"
#include "TargetComponent.hpp"
#include "../../ui/HUD.hpp"

TargetComponent::TargetComponent(Actor *owner) : Component(owner) {
    mOwner->GetGame()->GetHUD()->AddTargetComponent(this);
}

TargetComponent::~TargetComponent() {
    mOwner->GetGame()->GetHUD()->RemoveTargetComponent(this);
}
