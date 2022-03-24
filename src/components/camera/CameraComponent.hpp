#pragma once

#include "../Component.hpp"
#include "../../helper/Math.hpp"

// Base class for all the cameras
class CameraComponent : public Component {
public:
	explicit CameraComponent(class Actor* owner, int updateOrder = 200);
protected:
	void SetViewMatrix(const Matrix4& view);
};
