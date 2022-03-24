#pragma once

#include "Actor.hpp"

class BallActor : public Actor {
public:
    explicit BallActor(class Game *game);

    void UpdateActor(float deltaTime) override;

    void SetPlayer(Actor *player);
    void HitTarget();

private:
    class AudioComponent *mAudioComp;
    class BallMove *mMyMove;
    float mLifeSpan = 2;
};
