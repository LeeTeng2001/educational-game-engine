#pragma once

#include "Actor.hpp"

class CameraActor : public Actor {
public:
    explicit CameraActor(class Game *game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const struct InputState &state) override;

private:
    class MoveComponent *mMoveComp;
};
