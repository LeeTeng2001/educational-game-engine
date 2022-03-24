#pragma once
#include "Actor.hpp"

class FollowActor : public Actor {
public:
	explicit FollowActor(class Game* game);

    void ActorInput(const struct InputState &state) override;
	void SetVisible(bool visible);

private:
	class MoveComponent* mMoveComp;
	class FollowCamera* mCameraComp;
	class MeshComponent* mMeshComp;
};
