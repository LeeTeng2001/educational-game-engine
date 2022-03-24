#pragma once

#include "../Component.hpp"

class TargetComponent : public Component {
public:
	explicit TargetComponent(class Actor* owner);

	~TargetComponent();
};
