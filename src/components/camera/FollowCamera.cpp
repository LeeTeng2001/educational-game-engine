#include "FollowCamera.hpp"
#include "../../actors/Actor.hpp"


FollowCamera::FollowCamera(Actor* owner) : CameraComponent(owner) { }

void FollowCamera::Update(float deltaTime) {
    CameraComponent::Update(deltaTime);

    // Compute dampening from spring constant
    float dampening = 2.0f * sqrtf(mSpringConstant);
    // Compute ideal position
    Vector3 idealPos = ComputeCameraPos();
    // Compute difference between actual and ideal
    Vector3 diff = mActualPos - idealPos;
    // Compute acceleration of spring
    Vector3 acel = -mSpringConstant * diff - dampening * mVelocity;
    // Update velocity
    mVelocity += acel * deltaTime;
    // Update actual camera position
    mActualPos += mVelocity * deltaTime;

    // Target is target dist in front of owning actor
    Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
    // Use actual position here, not ideal
    Matrix4 view = Matrix4::CreateLookAt(mActualPos, target,Vector3::UnitZ);
    SetViewMatrix(view);
}

void FollowCamera::SnapToIdeal() {
    mActualPos = ComputeCameraPos();
    // Zero velocity
    mVelocity = Vector3::Zero;
    // Compute target and view
    Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
    // Use actual position here, not ideal
    Matrix4 view = Matrix4::CreateLookAt(mActualPos, target,Vector3::UnitZ);
    SetViewMatrix(view);
}

Vector3 FollowCamera::ComputeCameraPos() const {
    // Set camera position behind and above owner, very easy math
    Vector3 cameraPos = mOwner->GetPosition();
    cameraPos -= mOwner->GetForward() * mHorzDist;
    cameraPos += Vector3::UnitZ * mVertDist;
    return cameraPos;
}
