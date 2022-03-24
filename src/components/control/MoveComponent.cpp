#include "MoveComponent.hpp"
#include "../../actors/Actor.hpp"

MoveComponent::MoveComponent(class Actor *owner, int updateOrder)
        : Component(owner, updateOrder) {}

void MoveComponent::Update(float deltaTime) {
    // Update rotation in Quaternion
    if (!Math::NearZero(mAngularSpeed)) {
        Quaternion rot = mOwner->GetRotation();
        float angle = mAngularSpeed * deltaTime;

        // Rotate up axis only (temporary)
        Quaternion inc(Vector3::UnitZ, angle);
        // Concatenate old and new quaternion
        rot = Quaternion::Concatenate(rot, inc);
        mOwner->SetRotation(rot);
    }

    // Update position in a similar fashion
    if (!Math::NearZero(mForwardSpeed) || !Math::NearZero(mStrafeSpeed)) {
        Vector3 pos = mOwner->GetPosition();
        pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
        pos += mOwner->GetRight() * mStrafeSpeed * deltaTime;
        mOwner->SetPosition(pos);
    }
}
