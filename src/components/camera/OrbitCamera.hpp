#pragma once
#include "CameraComponent.hpp"

class OrbitCamera : public CameraComponent {
public:
	explicit OrbitCamera(class Actor* owner);

	void Update(float deltaTime) override;

    // Getter
	[[nodiscard]] float GetPitchSpeed() const { return mPitchSpeed; }
	[[nodiscard]] float GetYawSpeed() const { return mYawSpeed; }

    // Setter
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetYawSpeed(float speed) { mYawSpeed = speed; }

private:
	// Offset from target
	Vector3 mOffset{-400.0f, 0.0f, 300.0f};
	// Up vector of camera, needed because we can rotate 360
	Vector3 mUp = Vector3::UnitZ;
	// Rotation/sec speed of pitch
	float mPitchSpeed = 0;
	// Rotation/sec speed of yaw
	float mYawSpeed = 0;
};
