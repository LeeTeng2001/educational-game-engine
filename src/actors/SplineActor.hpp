#pragma once

#include "Actor.hpp"

class SplineActor : public Actor {
public:
    explicit SplineActor(class Game *game);

    void ActorInput(const struct InputState &state) override;
    void RestartSpline();

private:
    class SplineCamera *mCameraComp = nullptr;
};
