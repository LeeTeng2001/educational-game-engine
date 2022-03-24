#pragma once

#include "../Component.hpp"
#include "../../audio/SoundEvent.hpp"
#include <vector>
#include <string>

class AudioComponent : public Component {
public:
	explicit AudioComponent(class Actor* owner, int updateOrder = 200);
	~AudioComponent();

	void Update(float deltaTime) override;
	void OnUpdateWorldTransform() override;

	SoundEvent PlayEvent(const std::string& name);
	void StopAllEvents();

private:
    // Note that we have event for 2d and 3d
	std::vector<SoundEvent> mEvents2D;
	std::vector<SoundEvent> mEvents3D;
};