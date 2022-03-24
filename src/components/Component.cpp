#include "Component.hpp"
#include "../actors/Actor.hpp"
#include "../core/InputSystem.hpp"

Component::Component(Actor *owner, int updateOrder)
        : mOwner(owner), mUpdateOrder(updateOrder) {
    // Add to actor's vector of components
    mOwner->AddComponent(this);
}

Component::~Component() {
    mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime) {

}

void Component::ProcessInput(const InputState &keyState) {

}
