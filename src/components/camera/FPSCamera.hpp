#pragma once

#include "CameraComponent.hpp"

class FPSCamera : public CameraComponent{
public:
    explicit FPSCamera(class Actor* owner);

    void Update(float deltaTime) override;

    // Getter
    [[nodiscard]] float GetPitch() const { return mPitch; }
    [[nodiscard]] float GetPitchSpeed() const { return mPitchSpeed; }
    [[nodiscard]] float GetMaxPitch() const { return mMaxPitch; }

    // Setter
    void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
    void SetMaxPitch(float pitch) { mMaxPitch = pitch; }

private:
    // Rotation/sec speed of pitch
    float mPitchSpeed = 0;
    // Maximum pitch deviation from forward
    float mMaxPitch = Math::Pi / 3.0f;
    // Current pitch
    float mPitch = 0;
};



