#include "BoxComponent.hpp"
#include "../../actors/Actor.hpp"
#include "../../Game.hpp"
#include "../../core/PhysWorld.hpp"

BoxComponent::BoxComponent(Actor *owner, int updateOrder)
        : Component(owner, updateOrder) {
    mOwner->GetGame()->GetPhysWorld()->AddBox(this);
}

BoxComponent::~BoxComponent() {
    mOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
}

void BoxComponent::OnUpdateWorldTransform() {
    // Reset to object space box
    mWorldBox = mObjectBox;

    // Must apply transform in this order
    // Scale
    mWorldBox.mMin *= mOwner->GetScale();
    mWorldBox.mMax *= mOwner->GetScale();
    // Rotate (if we want to)
    if (mShouldRotate) {
        mWorldBox.Rotate(mOwner->GetRotation());
    }
    // Translate
    mWorldBox.mMin += mOwner->GetPosition();
    mWorldBox.mMax += mOwner->GetPosition();
}
