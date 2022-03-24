#pragma once

#include "Actor.hpp"

class OrbitActor : public Actor {
public:
	explicit OrbitActor(class Game* game);

    void ActorInput(const struct InputState &state) override;

	void SetVisible(bool visible);

private:
	class OrbitCamera* mCameraComp;
	class MeshComponent* mMeshComp;
};
