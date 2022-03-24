#pragma once

#include "CameraComponent.hpp"

class FollowCamera : public CameraComponent {
public:
	explicit FollowCamera(class Actor* owner);

	void Update(float deltaTime) override;

    // Snap current damping actual camera to ideal position
	void SnapToIdeal();

    // Setter
	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }

private:
	[[nodiscard]] Vector3 ComputeCameraPos() const;

	// Actual position of camera
	Vector3 mActualPos;
	// Velocity of actual camera
	Vector3 mVelocity;

	// Horizontal follow distance
	float mHorzDist = 350;
	// Vertical follow distance
	float mVertDist = 150;
	// Target distance
	float mTargetDist = 100;
	// Spring constant (higher is more stiff)
	float mSpringConstant = 64;
};
